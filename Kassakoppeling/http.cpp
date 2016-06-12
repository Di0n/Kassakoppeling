/*#include "http.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <algorithm>
#include <memory>
#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

Http::Http() {}

size_t write_callback(const char* ptr, size_t size, size_t nmemb, string* out)
{
  const size_t totalBytes(size * nmemb);
  out->append(ptr, totalBytes);
  return totalBytes;
}

bool Http::get(const string& url, const string& credentials, string& dataOut)
{
  CURL *curl = curl_easy_init();
  CURLcode cCode;
  
  bool success(false);
  int returnCode(0);
  string httpDocument = "";
  
  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    //curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4); // Only try ipv4
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);  
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow url redirects
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, credentials.c_str());
    
    
    std::unique_ptr<string> httpData(new string());
    
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);  // Write everything to the callback function
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get()); // Write get data to httpData
    
    cCode = curl_easy_perform(curl); // Start connecting to the server and receiving data
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &returnCode);
    
    curl_easy_cleanup(curl); // free memory
    
    if (returnCode == 200 && cCode == CURLE_OK)
    {
      Logger::log(Logger::INFO, "GET request successful!\nReturncode: "+std::to_string(returnCode));
      httpDocument = *httpData.get();
      success = true;
    }
    else if(cCode != CURLE_OK)
      Logger::log(Logger::ERROR, "Error: curl_easy_perform(*handle) failed! cCode != CURLE_OK\nIn file: http.cpp");
    
    else
      Logger::log(Logger::ERROR, "Error: Http::get() returned response code: " + std::to_string(returnCode) + "\nIn file: http.cpp");
  }
  else
    Logger::log(Logger::ERROR, "Error: Curl failed to initialize\nIn file: http.cpp");
  
  curl_global_cleanup();
  
  dataOut = httpDocument;
  
  return success;
}

struct WriteThis
{
  const char *readPtr;
  long sizeLeft;
};

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct WriteThis *wT = (struct WriteThis *)userp;
 
  if(size*nmemb < 1)
    return 0;
 
  if(wT->sizeLeft) {
    *(char *)ptr = wT->readPtr[0]; // copy one single byte 
    wT->readPtr++;                  // advance pointer 
    wT->sizeLeft--;                 // less data left 
    return 1;                        // we return 1 byte at a time!  
  }
 
  return 0;                          // no more data left to deliver 
}

bool Http::put(const string& url, const string& credentials, const char data[])
{
  CURL *curl = curl_easy_init();
  CURLcode res;
  
  bool success(false);
  int responseCode(0);
  
  if (curl)
  {
    struct WriteThis wT;
    wT.readPtr = data;
    wT.sizeLeft = (long)strlen(data);
  
    struct curl_slist *headers;
    headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); //http://agptest.gemoro.nl/rest/node/155531
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, credentials.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, &wT);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
  #ifdef USE_CHUNKED
      {
	headers = curl_slist_append(chunk, "Transfer-Encoding: chunked");
	
	res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      }
  #else
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, wT.sizeLeft);
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
      Logger::log(Logger::INFO, "PUT request successful!\nReturncode: "+std::to_string(responseCode));
      success = true;
    }
    else if (res != CURLE_OK)
      Logger::log(Logger::ERROR, "Error: curl_easy_perform(*handle) failed! cCode != CURLE_OK\nIn file: http.cpp");
    
    else
      Logger::log(Logger::ERROR, "Error: Http::put() returned response code: " + std::to_string(responseCode) + "\nIn file: http.cpp");
    
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
  }
  else 
    Logger::log(Logger::ERROR, "Error: Curl failed to initialize\nIn file: http.cpp");
  
  curl_global_cleanup();
  return success;
}

int Http::post(const string& url, const string& credentials, const char data[]) // untested
{
  CURL *curl;
  CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
  
  if (res != CURLE_OK)
  {
    std::cout << "Error: curl_global_init() failed to initialize!\nError in http.cpp" << std::endl;
    return 1;
  }
  
  struct WriteThis wT;
  
  wT.readPtr = data;
  wT.sizeLeft = (long)strlen(data);
  
  curl = curl_easy_init();
  
  int returnCode(0);
  
  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4); // Alleen ipv4 proberen
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10); 
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Volg redirects
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_USERPWD, credentials.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, &wT);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    
#ifdef USE_CHUNKED
    {
      struct curl_slist *chunk = NULL;
      
      chunk = curl_slist_append(chunk, "Transfer-Encoding: chunked");
      
      res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    }
#else
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, wT.sizeLeft);
#endif
      
#ifdef DISABLE_EXPECT
    {
      struct curl_slist *chunk = NULL;
	
      chunk = curl_slist_append(chunk, "Expect:");
      res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    }
#endif

      res = curl_easy_perform(curl);
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &returnCode);
      
      if (res != CURLE_OK)
      {
	std::cout << "Error: curl_easy_perform()!\nError in: http.cpp" << std::endl;
	returnCode = 2;
      }
      
      curl_easy_cleanup(curl);
      //curl_slist_free_all();
  }
  else 
  {
    std::cout << "Curl failed to initialize!\nError in: http.cpp" << std::endl;
    returnCode = 1;
  }
  curl_global_cleanup();
  return returnCode;
}*/