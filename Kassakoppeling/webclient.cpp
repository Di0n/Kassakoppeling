#include "webclient.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <algorithm>
#include <memory>
#include <string.h>
#include <iostream>
#include <sys/stat.h>

using std::cout;
using std::endl;


// *** HTTP *** //

bool WebClient::HTTP::get(const string& url, const string& credentials, string& dataOut)
{
  CURL *curl = curl_easy_init();
  CURLcode cCode;
  
  bool success(false);
  int returnCode(0);
  string httpDocument = "";
  
  if (curl)
  {
    std::unique_ptr<string> httpData(new string());
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    //curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4); // Only try ipv4
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);  
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); 							// Follow url redirects
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, credentials.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);  						// Write everything to the callback function
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get()); 							// Write get data to httpData
#ifdef DEBUG
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); 								// DEBUG
#endif
    
    cCode = curl_easy_perform(curl); 										// Start connecting to the server and receiving data
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &returnCode);
    
    if (returnCode == 200 && cCode == CURLE_OK)
    {
      Logger::log_info("GET request successful! Returncode: "+std::to_string(returnCode));
      httpDocument = *httpData.get();
      success = true;
    }
    else if(cCode != CURLE_OK)
      Logger::log_error("curl_easy_perform(*handle) failed! cCode != CURLE_OK\nIn file: 'webclient.cpp'");
    
    else
      Logger::log_error("HTTP::get() returned response code: " + std::to_string(returnCode) + "\nError in: 'webclient.cpp'");
    
    curl_easy_cleanup(curl); 											// free memory
  }
  else
    Logger::log_error("Curl failed to initialize\n Error in: 'webclient.cpp'");
  
  dataOut = httpDocument;
  
  return success;
}

bool WebClient::HTTP::put(const string& url, const string& credentials, const char data[])
{
  CURL *curl = curl_easy_init();
  CURLcode res;
  
  bool success(false);
  int responseCode(0);
  
  if (curl)
  {
    struct WriteData wd;
    wd.readPtr = data;
    wd.sizeLeft = (long)strlen(data);
  
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, credentials.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, &wd);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
#ifdef DEBUG
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); 								// DEBUG
#endif
    
  #ifdef USE_CHUNKED
      {
	headers = curl_slist_append(chunk, "Transfer-Encoding: chunked");
	
	res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      }
  #else
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, wd.sizeLeft);
  #endif
	
  #ifdef DISABLE_EXPECT
      {
	headers = curl_slist_append(chunk, "Expect:");
	res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      }
  #endif

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);  

    if (responseCode == 200 && res == CURLE_OK)
    {
      Logger::log_info("PUT request successful!\nReturncode: "+std::to_string(responseCode));
      success = true;
    }
    else if (res != CURLE_OK)
      Logger::log_error("curl_easy_perform(*handle) failed! cCode != CURLE_OK\nIn file: 'webclient.cpp'");
    
    else
      Logger::log_error("HTTP::put() returned response code: " + std::to_string(responseCode)+ "\nIn file: 'webclient.cpp'");

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
  }
  else 
    Logger::log_error("Curl failed to initialize\nError in 'webclient.cpp'");
  
  return success;
}
    
size_t WebClient::HTTP::write_callback(const char* ptr, size_t size, size_t nmemb, string* out)
{
  const size_t totalBytes(size * nmemb);
  out->append(ptr, totalBytes);
  return totalBytes;
}

size_t WebClient::HTTP::read_callback(void* ptr, size_t size, size_t nmemb, void* userp)
{
  struct WriteData *wd = (struct WriteData *)userp;
 
  if(size*nmemb < 1)
    return 0;
 
  if(wd->sizeLeft) {
    *(char *)ptr = wd->readPtr[0]; 										// copy one single byte 
    wd->readPtr++;                 										// advance pointer 
    wd->sizeLeft--;                										// less data left 
    return 1;                        										// we return 1 byte at a time! 
  }
 
  return 0;                          										// no more data left to deliver
}

// *** FTP *** //
bool WebClient::FTP::upload_file(const string& filePath, string destinationUrl, const string& credentials, const string& uploadAs)
{
  string fileName;
  
  if (uploadAs == "")
  {
    size_t found = filePath.find_last_of("/\\");

    if (found == string::npos)
    {
      Logger::log_error(filePath + " is an invalid path!\nError in: 'webclient.cpp'");
      return false;
    }
    fileName = filePath.substr(found+1);
  }
  else
    fileName = uploadAs;
  
  if (destinationUrl[destinationUrl.size() - 1] != '/')
    destinationUrl += '/';
  
  const string buffer = "RNFR " + fileName;

  struct stat FileInfo;
  
  if (stat(filePath.c_str(), &FileInfo))
  {
    Logger::log_error("Can't open selected file: "+filePath+"\nError in: 'webclient.cpp'");
    return false;
  }
  
  curl_off_t fsize = (curl_off_t)FileInfo.st_size;
  
  cout << "Local file size: " << fsize << " bytes." << endl;
  
  FILE *hdSrc = fopen(filePath.c_str(), "rb");
  
  if (hdSrc == NULL)
  {
    Logger::log_error("Failed to open " + filePath + ", fopen() returned a nullptr!\nError in: 'webclient.cpp'");
    return false;
  }
  
  CURL *curl = curl_easy_init();
  CURLcode res;
  
  
  bool status(false);
  
  if (curl)
  {
    struct curl_slist *headerlist = NULL;
    
    headerlist = curl_slist_append(headerlist, buffer.c_str());
    
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);    
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    
    const string fileDestination = destinationUrl + fileName;
    
    curl_easy_setopt(curl, CURLOPT_URL, fileDestination.c_str());    
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600L);   
    curl_easy_setopt(curl, CURLOPT_USERPWD, credentials.c_str());   
    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);   
    curl_easy_setopt(curl, CURLOPT_READDATA, hdSrc);    
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);
    
#ifdef DEBUG
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); 								// DEBUG   
#endif
    
    if ((long)fsize > 2147483648) // 2gb
      curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fsize);
    else
      curl_easy_setopt(curl, CURLOPT_INFILESIZE, (long)fsize);

    res = curl_easy_perform(curl);

    if (res == CURLE_OK)
    {
      Logger::log_info("FTP file upload succeeded!\nUploaded file: "+fileName);
      status = true;
    }
    else
      Logger::log_error("Failed to upload file! curl_easy_perform(*handle) failed\nError in: 'webclient.cpp'");
    
    curl_slist_free_all(headerlist);
    
    curl_easy_cleanup(curl);
  }
  else
    Logger::log_error("curl_easy_init() failed!\nError in: 'webclient.cpp'");
  
  fclose(hdSrc);
  
  return status;
}

size_t WebClient::FTP::read_callback(void* ptr, size_t size, size_t nmemb, void* stream)
{
  curl_off_t nread;

  size_t retcode = fread(ptr, size, nmemb, (FILE*)stream);
 
  //if (nmemb != retcode) error
  nread = (curl_off_t)retcode;

  cout << "Read " << nread << " bytes from file" << endl;
  
  return retcode;
}