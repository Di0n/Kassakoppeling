#ifndef __WEBCLIENT_H__
#define __WEBCLIENT_H__

#include <string>

using::std::string;


namespace WebClient
{
  class HTTP
  {
  public: 
    //Gets web page content from the given url,
    //The document gets passed to the dataOut reference
    //Returns true if the GET request succeeded
    bool get(const string& url, const string& credentials, string& dataOut);
    //PUT request to server
    //Returns true if the PUT request succeeded
    bool put(const string& url, const string& credentials, const char data[]); 
    bool post() {}; // not implemented/needed atm
  private:
    static size_t write_callback(const char* ptr, size_t size, size_t nmemb, string* out); //TODO maybe not static 
    static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp);
    struct WriteData
    {
      const char *readPtr;
      long sizeLeft;
    };
  };
  
  class FTP
  {
  public:
    //Uploads a file to the FTP server
    //filePath is the location of the file you want to upload
    //destinationUrl is the location where you want to store the file on the FTP server
    //credentials are the credentials needed to upload to the server
    //uploadAs is an optional parameter
    bool upload_file(const string& filePath, string destinationUrl, const string& credentials, const string& uploadAs = "");
    bool download_file() {}; // not implemented/needed
  private:
    static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);
  };
  
  class SMTP
  {
  public:
  private:
  };
}

#endif