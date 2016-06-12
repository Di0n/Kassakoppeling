#include "errorhandler.h"



namespace ErrorHandler 
{
  time_t lastError;
  int nrOfCritErrors = 0, nrOfErrors = 0;

  void report_crit_error()
  {
    nrOfCritErrors++;
    lastError = std::time(0);
  }
  
  void report_error()
  {
    nrOfErrors++;
    lastError = std::time(0);
  }
  
  bool terminate()
  {  
    time_t now = std::time(0);
    double diff = std::difftime(now, lastError);
    if (nrOfCritErrors >= 1 || nrOfErrors >= 5) // treshold
    {
      return true;
    }
    nrOfErrors = (diff >= 3600) ? 0 : nrOfErrors; // 1 uur
    return false;
  }
}