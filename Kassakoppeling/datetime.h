#ifndef __DATETIME_H__
#define __DATETIME_H__

#include <string>

using std::string;

struct DateTime
{
  int seconds, minutes,
  hours, day, month, year, wday;
  
  //Returns a DateTime struct with the current date & time
  static DateTime today();
  //Returns a string with a formatted date/time
  string to_string();
  //Get the current time in string format
  string get_time_string();
  //Get the current date in string format
  string get_date_string();
};
#endif
