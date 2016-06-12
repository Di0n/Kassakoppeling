#include "task.h"
#include "datetime.h"
#include <iostream>
#include <thread>

Task::Task(const int execDay, const string& hhmm, const std::function< void (void)>& function)
{
  wday = execDay;
  
  for (string::size_type i = 0; i < hhmm.size(); ++i)
  {
    if (hhmm[i] == ':')
    {
      hour = std::stoi(hhmm.substr(0, i));
      minute = std::stoi(hhmm.substr(i+1, hhmm.length()));
    }
  }
  
  this->function = function;
}

void Task::run_pending()
{
  DateTime dt = DateTime::today();
  if (dt.wday == execDay)
    return;
  
  if ((dt.wday == this->wday || this->wday == 7) && dt.hours == hour && dt.minutes == minute)
  {
    std::thread(this->function).detach();
    execDay = dt.wday;
  }
}