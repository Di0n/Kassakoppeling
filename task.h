/*
 * Kijken of er een taak is die uitgevoerd moet worden.
 * Een taak registreren 
 */

#ifndef __TASK_H__
#define __TASK_H__

#include <vector>
#include <string>
#include <functional>

#include "datetime.h"

using std::string;

class Task
{
public:
  enum Day
  {
    Everyday = 7,
    Monday = 1,
    Tuesday = 2,
    Wednesday = 3,
    Thursday = 4,
    Friday = 5,
    Saturday = 6,
    Sunday = 0
  };
  
  Task(const int execDay, const string& hhmm, const std::function<void(void)>& function);
  
  // Runs pending tasks if there are any
  void run_pending(); 
private:
  int wday;
  int hour;
  int minute;
  int execDay;
  
  //Stores a reference to the function
  std::function<void(void)> function;
};
#endif