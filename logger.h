#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <stdio.h>

#include "product.h"
#include "datetime.h"
// Bad log class, remake this or replace this (needs to be thread safe)

using std::string;

class Logger
{
public:
  Logger();
  ~Logger();
  
  static void log_error(const string& s);
  static void log_info(const string& s);
  void log_product_info(Product product, ActiveProduct activeProduct, const bool match, const double matchPercentage);

  struct LogPath
  {
    const string baseLogPath = "/home/pi/KassakoppelingCPP/Build/Logs/";
    
    const string baseErrorLogPath = baseLogPath + "Errors/";
    const string baseInfoLogPath = baseLogPath + "Info/";
    const string baseProductLogPath = baseLogPath + "Products/";
    
    const string errorLogFile = baseErrorLogPath + "errors_" + DateTime::today().get_date_string() + ".log";
    const string infoLogFile = baseInfoLogPath + "info_" + DateTime::today().get_date_string() + ".log";
    const string productLogFile = baseProductLogPath + "products_" + DateTime::today().get_date_string() + ".csv";
  };
private:
  FILE* file=NULL;
  bool includeFirstRow;
  
  string format_product_info(Product& product, ActiveProduct& activeProduct, const bool match, const double matchPercentage);
  static void log(const string& path, const string& text);
};
#endif