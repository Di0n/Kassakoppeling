#include "datetime.h"
#include <ctime>
#include <time.h>

DateTime DateTime::today()
{
  DateTime dt;
  
  time_t t = time(0);
  struct tm *now = localtime(&t);
  
  dt.seconds = now->tm_sec;
  dt.minutes = now->tm_min;
  dt.hours = now->tm_hour;
  dt.day = now->tm_mday; 
  dt.month = now->tm_mon + 1;
  dt.year = now->tm_year + 1900;
  dt.wday = now->tm_wday;
  
  return dt;
}

string DateTime::to_string()
{
  return std::to_string(this->hours) + ":" + std::to_string(this->minutes) + ":" + std::to_string(this->seconds) + " " + std::to_string(this->day) + "-" + std::to_string(this->month) + "-" + std::to_string(this->year);
}


string DateTime::get_time_string()
{
  return std::to_string(this->hours) + ':' + std::to_string(this->minutes) + ':' + std::to_string(this->seconds);
}


string DateTime::get_date_string()
{
  return std::to_string(this->day) + '-' + std::to_string(this->month) + '-' + std::to_string(this->year);
}