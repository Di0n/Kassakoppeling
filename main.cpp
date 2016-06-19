/* Dion van der Linden 
 * Gemoro B.V.
 * dion@gemoro.nl
 * 2016 
*/
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <curl/curl.h>
#include <mutex>
#include <fstream>
#include <dirent.h>
#include <limits.h>

#include "serial_reader.h"
#include "config_file.h"
#include "supermarket.h"
#include "utils.h"
#include "errorhandler.h"
#include "product.h"
#include "logger.h"
#include "task.h"
#include "webclient.h"

using std::string;
using std::cout;
using std::endl;
using std::vector;


const int ONESECOND(1000000);
const string PIIDPATH = "/home/pi/pi_id";

Supermarket supermarket;
SerialReader sr;
vector<Task> tasks;


string nodeUrl;
string serverLogPath;
string ftpCredentials;
string restCallCredentials;
string programPath;

// Declare functions
int run();
bool initialize(const string& argv0);
void kill_old_proc();
void clear_log_files();
void send_logs();


/* Program Start */
int main(int argc, char **argv) 
{
  if (!initialize()) // If initializing fails don't run the program.
  {
    Logger::log_error("Failed to initialize Kassakoppeling!");
    return EXIT_FAILURE; 
  }
  return run();
}

//Program keeps running here
int run()
{
  Logger::log_info("Succesfully initialized Kassakoppeling!");
  
  int returnCode(EXIT_SUCCESS);
  
  while (1)
  {
    usleep(ONESECOND / 10); 										//Give the CPU the chance to process other programs
   
    if (ErrorHandler::terminate()) 
    {
      returnCode = EXIT_FAILURE;
      Logger::log_info("ErrorHandler::terminate() reached treshold.\nError in: 'main.cpp'");
      break;
    } 
    
    for (Task & t : tasks) t.run_pending(); 
       
    string stream;
    if (!sr.fetch_stream_string(stream))
    {     
      ErrorHandler::report_crit_error(); 								// If the serial reader gives an error we want to terminate the application and start it up again
      continue;
    }
    
    if (stream.empty()) continue; 									// If there is no data in the stream do nothing

    string receipt = Utils::trim_copy(stream); 								// We want to get rid of leading and trailing whitespaces
    Utils::to_upper(receipt);
    
    if (!supermarket.check_for_discounts(receipt)) continue; 						// check if there are products on the receipt with a discount sign
    
    const int res = supermarket.check_for_active_discounts(); 						// Check for active discounts in the database
    
    if (res == 0)
    {
      cout << "There are no active discounts for supermarket: " << supermarket.id << " at this time!" << endl;
      continue;
    }
    else if (res == 2)
    {
      cout << "Failed to retrieve active products for supermarket: " << supermarket.id << endl;
      ErrorHandler::report_error();
      continue;
    }
    else
      cout << "Found active product(s) for supermarket: " << supermarket.id << endl;

    vector<Product> receiptDiscountProducts = supermarket.get_discount_products_receipt(receipt); 	// Get the products with a discount from the receipt
    
    if (receiptDiscountProducts.empty()) continue; 							// It shouldn't be empty at this point, but check to be sure
    
    cout << "\nReceipt Products:\n" << endl;
    
    for (auto& p : receiptDiscountProducts)
    {
      cout << p.get_name() << ' ';
      cout << p.get_price() << endl;
    }
    
    cout << '\n';

    vector<ActiveProduct> activeDiscountProducts;
    
    if (!supermarket.get_active_products(activeDiscountProducts)) 					// Put the active products from the supermarket in a vector
    {
      cout << "Failed to retrieve active products!" << endl;
      ErrorHandler::report_error();
      continue;
    }
    
    if (activeDiscountProducts.empty()) continue;
    
    cout << endl;
    
    std::vector<ActiveProduct> matchedActiveProducts = 
    supermarket.get_matched_active_products(receiptDiscountProducts, activeDiscountProducts); 		// Matches the receipt products with the active products and returns successful matches
    
    if (matchedActiveProducts.empty()) 
    {
      cout << "No products matched each other!" << endl;
      continue;  
    }
    
    for (auto& ap : matchedActiveProducts)
    {
      ap.remove_active_product(nodeUrl, restCallCredentials); 						// Remove the product from the database 
    }

    cout << endl;
  }
    
  sr.close_com_port();
  curl_global_cleanup();
  
  Logger::log_info("Terminating Kassakoppeling...");
  
  return returnCode;
}

bool initialize() 
{
  Logger::log_info("Loading Kassakoppeling...");
  
<<<<<<< HEAD
  char result[PATH_MAX];
  size_t count = readlink("/proc/self/exe", result, PATH_MAX);
  string temp = string(result, (count > 0) ? count : 0);
  programPath = temp.substr(0, temp.find_last_of("/\\") + 1);
  
=======
  programPath = argv0 + '/';
>>>>>>> origin/master
  
  CURLcode res;
  res = curl_global_init(CURL_GLOBAL_ALL);
  
  if (res != 0)
  {
    Logger::log_error("curl_global_init() failed to set up!\nError in file: 'main.cpp'");
    return false;
  }
  
  string pI_ID;
  
  std::ifstream file(PIIDPATH.c_str());
  if (file)
  {
    std::getline(file, pI_ID);
  }
  else 
    return false;
  
  ConfigFile config(programPath + "config.conf"); 
  
  string _cport = config.Value("PI", "Port");
  
  sr.bdrate = config.Value("PI", "Baudrate");
  sr.mode = (string)config.Value("PI", "Mode");
  sr.timeOut = ((uint)config.Value("PI", "SerialReaderTimeOut") * 1000); 				// default 100000 = 100 ms
  
  supermarket.id = config.Value("SUPERMARKET", "ID");
  supermarket.getActiveDiscountsFullUrl = (string)config.Value("SERVER", "GetActiveDiscounts_URL");
  
  nodeUrl = (string)config.Value("SERVER", "Node_URL");
  
  serverLogPath = (string)config.Value("SERVER", "ServerLogPath");
  
  ftpCredentials = (string)config.Value("SERVER", "FTPCredentials");
  restCallCredentials = supermarket.restCallCredentials = (string)config.Value("SERVER", "RestCallCredentials");
  

  if (supermarket.getActiveDiscountsFullUrl[supermarket.getActiveDiscountsFullUrl.size()-1] != '?')
    supermarket.getActiveDiscountsFullUrl += '?';
  
  if (serverLogPath[serverLogPath.size()-1] != '/')
    serverLogPath += '/';
  serverLogPath += pI_ID;
  
  if (nodeUrl[nodeUrl.size()-1] != '/')
    nodeUrl += '/';
  
  
  tasks.reserve(5);
  tasks.push_back(Task(config.Value("TASK", "ClearLogFiles_Day"), (string)config.Value("TASK", "ClearLogFiles_Time"), clear_log_files));
  tasks.push_back(Task(Task::Everyday, (string)config.Value("TASK", "SendLogs_1_Time"), send_logs)); 	// 10:00, 14:00, 18:00, 23:00
  tasks.push_back(Task(Task::Everyday, (string)config.Value("TASK", "SendLogs_2_Time"), send_logs)); 
  tasks.push_back(Task(Task::Everyday, (string)config.Value("TASK", "SendLogs_3_Time"), send_logs));
  tasks.push_back(Task(Task::Everyday, (string)config.Value("TASK", "SendLogs_4_Time"), send_logs));
  
  
  if (_cport.empty() || sr.bdrate == 0 || sr.mode.empty() || supermarket.id == 0)
    return false;
  
  
  const std::map<std::string, int> ports= 
    {
      {"ttyS0", 0},
      {"ttyS1", 1},
      {"ttyS2", 2},
      {"ttyS3", 3},
      {"ttyS4", 4},
      {"ttyS5", 5},
      {"ttyS6", 6},
      {"ttyS7", 7},
      {"ttyS8", 8},
      {"ttyS9", 9},
      {"ttyS10", 10},
      {"ttyS11", 11},
      {"ttyS12", 12},
      {"ttyS13", 13},
      {"ttyS14", 14},
      {"ttyS15", 15},
      {"ttyUSB0", 16},
      {"ttyUSB1", 17},
      {"ttyUSB2", 18},
      {"ttyUSB3", 19},
      {"ttyUSB4", 20},
      {"ttyUSB5", 21},
      {"ttyAMA0", 22},
      {"ttyAMA1", 23},
      {"ttyACM0", 24},
      {"ttyACM1", 25},
      {"rfcomm0", 26},
      {"rfcomm1", 27},
      {"ircomm0", 28},
      {"ircomm1", 29},
      {"cuau0", 30},
      {"cuau1", 31},
      {"cuau2", 32},
      {"cuau3", 33},
      {"cuaU0", 34},
      {"cuaU1", 35},
      {"cuaU2", 36},
      {"cuaU3", 37}
   }; 
   
  if (Utils::is_int(_cport))
  {
    sr.cport = std::stoi(_cport);
    
    for (auto it = ports.begin(); it != ports.end(); ++it)
    {
      if (it->second == sr.cport)
      {
	sr.cportStr = it->first;
      }
    }
  }
  else
  {
    sr.cportStr = _cport;

    for (auto it = ports.begin(); it != ports.end(); ++it)
    {
      if (it->first == _cport)
      {
	sr.cport = it->second;
      }
    }
  }
  
  
  Logger::log_info("Com port: "+sr.cportStr + "\nBaudrate: " + std::to_string(sr.bdrate) + "\nMode: "+sr.mode);
  Logger::log_info("Supermarket ID: "+std::to_string(supermarket.id));;

  
  int attempts(0);
  while (!(sr.open_com_port()) && attempts < 3)
  {
    Logger::log_error("Failed to open com port: "+sr.cportStr + "\nAttempts: "+std::to_string(attempts+1) + "\nTrying again...\nError in: 'main.cpp'");
    
    kill_old_proc(); 
    
    attempts++;
    
    usleep(ONESECOND); 
  }
  
  return (attempts >= 3) ? false : true;
}


//Kill the old process if there is one running
void kill_old_proc()
{
  const string path = "/dev/" + sr.cportStr;
  
  string output = Utils::get_output_from_terminal("fuser " + path); 
  
  std::cout << output << std::endl;
  
  if (!output.empty() || output != "ERROR")
  {
    string procList = output;
    procList.erase(0, path.size()+1);

    Utils::trim(procList);  
    
    for (int i = 0; i <= procList.size(); i++)
    {
      if (isspace(procList[i]) || i == procList.size())
      {
	string proc = procList.substr(0, i);
	
	procList.erase(0, i);
	
	Utils::trim(procList);
	
	Logger::log_info("Killing process: "+proc+"...");
	
	string cmd = "kill "+proc;
	
	std::cout.flush(); 										// Flush buffer before making a call to system
	
	system(cmd.c_str());
	
	Logger::log_info("Killed process: "+proc);
	
	i = 0;
	
	usleep(ONESECOND/2);	
      }
    }
   }
}

inline void clear_log_files()
{
  //system(("cd "+PROGRAMPATH+"Logs && find . -name \"*.csv\" -type f -delete && find . -name \"*.log\" -type f -delete").c_str());
  
  Logger::LogPath lp;
  
  const std::array<string, 3> dirs=
  {
    lp.baseInfoLogPath,
    lp.baseErrorLogPath,
    lp.baseProductLogPath
  };
  
  for (auto & d : dirs)
  {
    DIR* dir;
    struct dirent *ent;
    if ((dir = opendir(d.c_str())) != NULL)
    {
      while ((ent = readdir(dir)) != NULL)
      {
	if (!Utils::starts_with(ent->d_name, ".") && (Utils::ends_with(ent->d_name, ".log") || Utils::ends_with(ent->d_name, ".csv")))
	{
	  const string fileName = d + ent->d_name;
	  
	  if (remove(fileName.c_str()) != 0)
	    cout << "Failed to delete file: " << fileName << std::endl;
	  else
	    cout << "Succesfully deleted file: " << fileName << std::endl;
	}
      }
      closedir(dir);
    }
  }
}

inline void delete_temp_logs();

void send_logs()
{
  static std::mutex mtx;
  
  mtx.lock();

  Logger::LogPath lp;
  
  const std::map<string, string> logs=
  {
    { lp.errorLogFile, serverLogPath + "/Errors/"},
    { lp.infoLogFile, serverLogPath + "/Info/"},
    { lp.productLogFile, serverLogPath + "/Products/"}
  };
  
  WebClient::FTP ftp;
  
  for (auto itr = logs.begin(); itr != logs.end(); ++itr)
  {
    if (Utils::file_exists(itr->first))
    {
      const string logPath = itr->first + ".copy";
      if (!Utils::copy_paste_file(itr->first, logPath))
	break;
      
      size_t found = itr->first.find_last_of("/\\");
      
      if (found != string::npos)
      {
	const string fileName = itr->first.substr(found+1);
	
	for (int i = 0; i < 2; i++)
	{
	  if (!ftp.upload_file(logPath, itr->second, ftpCredentials, fileName))
	  {
	    cout << "Failed to upload file to ftp server!" << endl;
	    usleep(ONESECOND / 2);
	    continue;
	  }
	  break;
	}  
      }
      else
	cout << "Invalid filepath: " << itr->first << endl;
      
      delete_temp_logs();
    }
  }
  mtx.unlock();
}

inline void delete_temp_logs()
{
  Logger::LogPath lp;
  system(("cd "+lp.baseLogPath+" && find . -name \"*.copy\" -type f -delete").c_str());
}
