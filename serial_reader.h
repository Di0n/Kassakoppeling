#ifndef __SERIAL_READER_H__
#define __SERIAL_READER_H__

#include <string>

using std::string;
class SerialReader
{
public:
  SerialReader();
  SerialReader(const int cport, const unsigned int bdrate, const string& mode, const int timeOut);
  
  //Opens the comport, returns true if succeeded
  bool open_com_port();
  //Fetches string data on the comport, returns false if there was an error, returns an empty string if there is currently no data being transmitted
  bool fetch_stream_string(string& stringOut);
  //Closes the com port
  void close_com_port();
  
  //COM port
  int cport;
  //COM port string equilivant example: port nr 16 = ttyUSB0
  string cportStr;
  //Data Parity Stop notation (8N1, 7E1 etc) 
  string mode;
  //Baudrate
  unsigned int bdrate;
  //PollComPort time out
  uint timeOut;
};
#endif