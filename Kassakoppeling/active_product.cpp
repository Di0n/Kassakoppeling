/*#include "active_product.h"
#include "http.h"
//#include "utility.h"
#include "logger.h"

#include <curl/curl.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

ActiveProduct::ActiveProduct() : Product()
{
}

ActiveProduct::ActiveProduct(const string& name, const double price, const int quantity, const string& nid, const string& systemName, const string& eancode) : Product(name, price)
{
  this->quantity = quantity;
  this->nid = nid;
  this->systemName = systemName;
  this->eancode = eancode;
}

bool ActiveProduct::remove_active_product(string url)
{
  //const string url = "http://agptest.gemoro.nl/rest/node/"+nid;
  url += nid; 
  const string credentials = "restcaller:restcaller";
  const string jsonString = "{\"field_quantitycurrent\":{\"und\":[{\"value\":\""+std::to_string((this->quantity-1))+"\"}]}}"; // "{\"field_quantitycurrent\":{\"und\":[{\"value\":\"%d\"}]}}"; 
  
  
  Http http;
  //int response = http.put(url, credentials, buffer);
  for (int i = 0; i < 2; i++)
  {
    bool success = http.put(url, credentials, jsonString.c_str());
    
    if (!success) 
    {
      usleep(1000000); // sleep 1 second
      continue;
    }

    Logger::log(Logger::INFO, "Successfully removed product with nid: "+nid);
    
    return true;
  }

  Logger::log(Logger::ERROR, "Error: Failed to remove active product: "+nid+"\nIn file: active_product.cpp");
  return false;
}

int ActiveProduct::get_quantity()
{
  return this->quantity;
}

string ActiveProduct::get_nid()
{
  return this->nid;
}

string ActiveProduct::get_system_name()
{
  return this->systemName;
}

string ActiveProduct::get_eancode()
{
  return this->eancode;
}*/