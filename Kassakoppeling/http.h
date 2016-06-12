/*#ifndef HTTP_H
#define HTTP_H

#include <string>

using std::string;

class Http
{
public:
  Http();
  // Gets web page content from the given url,
  // The document gets passed to the dataOut reference
  //Returns true if the GET request succeeded
  bool get(const string& url, const string& credentials, string& dataOut);
  // POST to server
  // Returns 1 if curl could not be initialized, returns 2 if curl_easy_perform() failed, returns 100 - 511 as http status code and returns 0 as undefined behaviour
  int post(const string& url, const string& credentials, const char data[]);
  //PUT request to server
  //Returns true if the PUT request succeeded
  bool put(const string& url, const string& credentials, const char data[]); 
};
#endif*/