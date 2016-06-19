#include "logger.h"
#include "utils.h"

#include <fstream>
//#include <dirent.h>
#include <vector>
#include <map>
#include <iostream>

using std::ofstream;

bool is_file_empty(const string& path);

Logger::Logger()
{

}

Logger::~Logger()
{
  if (file != NULL)
  {
    fclose(file);
    file = NULL;
  }
}
void Logger::log_info(const string& s)
{
  LogPath lp;
  log(lp.infoLogFile, s);
}

void Logger::log_error(const string& s)
{
  LogPath lp;
  log(lp.errorLogFile, s);
}

void Logger::log(const string& path, const string& text)
{
  std::cout << text << std::endl;
  
  ofstream f(path.c_str(), ofstream::app);
  if (f)
    f << (DateTime::today().to_string() + ": " + text).c_str() << "\n\n";
  else
    std::cout << "Failed to write to logfile: " << path << std::endl;
}

void Logger::log_product_info(Product product, ActiveProduct activeProduct, const bool match, const double matchPercentage)
{
  if (file == NULL)
  {
    LogPath lp;
    file = fopen(lp.productLogFile.c_str(), "ab");
    
    includeFirstRow = is_file_empty(lp.productLogFile);  
  }
  
  if (file != NULL)
  {
    fputs((format_product_info(product, activeProduct, match, matchPercentage) + '\n').c_str(), file);
  }
}

string Logger::format_product_info(Product& product, ActiveProduct& activeProduct, const bool match, const double matchPercentage)
{
  const string matchPerctStr = Utils::double_to_string(matchPercentage);//Utils::double_to_string(matchPercentage);
  
  const string pPrice = Utils::double_to_string(product.get_price());//Utils::double_to_string(product.get_price());  
  const string apPrice = Utils::double_to_string(activeProduct.get_price());//Utils::double_to_string(activeProduct.get_price());

  const string matchStr = match ? "MATCH" : "NO MATCH";
  
  const string activeProductName = (activeProduct.get_system_name() == "[]" || activeProduct.get_system_name().empty()) ? activeProduct.get_name() : activeProduct.get_system_name();
  
  DateTime dt = DateTime::today();
  const string date = dt.get_date_string();
  const string time = dt.get_time_string();
  
  const string firstRow = "DATE,TIME,MATCH,MATCH PERCENTAGE,RECEIPT PRODUCTNAME,ACTIVE PRODUCTNAME,RECEIPT PRODUCTPRICE,ACTIVE PRODUCT PRICE\n";
  const string format = date+','+time+','+matchStr+','+matchPerctStr+','+product.get_name()+','+activeProductName+','+pPrice+','+apPrice;
 
  if (includeFirstRow)
  {
    includeFirstRow = false;
    std::cout << firstRow + format;
    return firstRow + format;
  }
  return format;
}


bool is_file_empty(const string& path)
{ 
   std::ifstream stream(path.c_str(), std::ios::ate); 
   if (!stream) return false;
   if (stream.tellg() == 0) return true; 
   return false;
}