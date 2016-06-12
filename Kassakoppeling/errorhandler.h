#ifndef __ERROR_HANDLER_H__
#define __ERROR_HANDLER_H__

#include <ctime>
#include <time.h>

//ErrorHandler to make it possible to safely terminate the application from any class
namespace ErrorHandler
{
  extern time_t lastError;
  extern int nrOfErrors, nrOfCritErrors;
  
  //Reports a critical error to the ErrorHandler
  void report_crit_error();
  //Reports an error to the ErrorHandler
  void report_error();
  //Returns true if the error treshold has been reached otherwise returns false
  bool terminate();
}
#endif