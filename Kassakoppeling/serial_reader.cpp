//LINUX WINDOWS PORT CHART
/*
 * port nr. Linux 	windows
  0 	ttyS0 	COM1
  1 	ttyS1 	COM2
  2 	ttyS2 	COM3
  3 	ttyS3 	COM4
  4 	ttyS4 	COM5
  5 	ttyS5 	COM6
  6 	ttyS6 	COM7
  7 	ttyS7 	COM8
  8 	ttyS8 	COM9
  9 	ttyS9 	COM10
  10 	ttyS10 	COM11
  11 	ttyS11 	COM12
  12 	ttyS12 	COM13
  13 	ttyS13 	COM14
  14 	ttyS14 	COM15
  15 	ttyS15 	COM16
  16 	ttyUSB0 	n.a.
  17 	ttyUSB1 	n.a.
  18 	ttyUSB2 	n.a.
  19 	ttyUSB3 	n.a.
  20 	ttyUSB4 	n.a.
  21 	ttyUSB5 	n.a.
  22 	ttyAMA0 	n.a.
  23 	ttyAMA1 	n.a.
  24 	ttyACM0 	n.a.
  25 	ttyACM1 	n.a.
  26 	rfcomm0 	n.a.
  27 	rfcomm1 	n.a.
  28 	ircomm0 	n.a.
  29 	ircomm1 	n.a.
  30 	cuau0 	n.a.
  31 	cuau1 	n.a.
  32 	cuau2 	n.a.
  33 	cuau3 	n.a.
  34 	cuaU0 	n.a.
  35 	cuaU1 	n.a.
  36 	cuaU2 	n.a.
  37 	cuaU3 	n.a.
  Linux 	windows
  50 	n.a.
  75 	n.a.
  110 	110
  134 	n.a.
  150 	n.a.
  200 	n.a.
  300 	300
  600 	600
  1200 	1200
  1800 	n.a.
  2400 	2400
  4800 	4800
  9600 	9600
  19200 	19200
  38400 	38400
  57600 	57600
  115200 	115200
  230400 	128000
  460800 	256000
  500000 	500000
  576000 	n.a.
  921600 	n.a.
  1000000 	1000000
  1152000 	n.a.
  1500000 	n.a.
  2000000 	n.a.
  2500000 	n.a.
  3000000 	n.a.
  3500000 	n.a.
  4000000 	n.a.
  Mode
  8N1
  8O1
  8E1
  8N2
  8O2
  8E2
  7N1
  7O1
  7E1
  7N2
  7O2
  7E2
  6N1
  6O1
  6E1
  6N2
  6O2
  6E2
  5N1
  5O1
  5E1
  5N2
  5O2
  5E2
 */

#include "serial_reader.h"
#include "rs232.h"
#include "logger.h"
#include <iostream>

using std::cout;
using std::endl;

SerialReader::SerialReader()
{
  this->cport = 0;
  this->bdrate = 0;
  this->mode = "8N1"; // most common
  this->timeOut = 100000;
}

SerialReader::SerialReader(const int cport, const unsigned int bdrate, const string& mode, const int timeOut)
{
  this->cport = cport;
  this->bdrate = bdrate;
  this->mode = mode;
  this->timeOut = timeOut;
}

bool SerialReader::open_com_port()
{ 
  return (RS232_OpenComport(cport, bdrate, mode.c_str())) != 1 ? true : false;
}

void SerialReader::close_com_port()
{
  RS232_CloseComport(cport);
}

bool SerialReader::fetch_stream_string(string& streamOut)
{ 
  string streamString = "";
  
  while (1)
  {
    unsigned char buffer[4096];
    int stream(0);
    
    try 
    {
      stream = RS232_PollComport(cport, buffer, 4095);
    }
    catch (...)
    {
      Logger::log_error("RS232_PollComport() failed to poll com port!\nError in: 'serial_reader.cpp'");
      return false;
    }
    
    if (stream > 0)
    {
      cout << "Received bytes: " << stream << endl;
      buffer[stream] = 0; // NULL terminated
      
      for (int i = 0; i < stream; i++)
      {
	if (buffer[i] < 32)
	{
	  buffer[i] = '^';
	}
      }
      
      if (buffer != NULL) streamString += (char *)buffer;
    }
    else
      break;
    
    usleep(timeOut);
  }
  
  streamOut = streamString;
  return true;
}