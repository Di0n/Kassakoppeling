#include <curl/curl.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

#include "product.h"
//#include "http.h"
#include "webclient.h"
#include "logger.h"

using std::cout;
using std::endl;

//---------------------------//
//Product Class		     //
//---------------------------//

Product::Product() {}

Product::Product(const string& name, const double price)
{
  this->name = name;
  this->price = price;
}

string Product::get_name()
{
  return this->name;
}

double Product::get_price()
{
  return this->price;
}



//---------------------------//
// Active Product class	     //
//---------------------------//


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

bool ActiveProduct::remove_active_product(string url, const string& restCallCredentials)
{
  if (url[url.size()-1] != '/')
    url += '/';
  
  url += nid;
  //const string credentials = "restcaller:restcaller";
  const string jsonString = "{\"field_quantitycurrent\":{\"und\":[{\"value\":\""+std::to_string((this->quantity-1))+"\"}]}}"; // "{\"field_quantitycurrent\":{\"und\":[{\"value\":\"%d\"}]}}"; 
  
  WebClient::HTTP http;

  for (int i = 0; i < 2; i++)
  {
    bool success = http.put(url, restCallCredentials, jsonString.c_str());
    
    if (!success) 
    {
      usleep(1000000); // sleep 1 second
      continue;
    }

    Logger::log_info("Succesffully removed product with nid: "+nid);
    
    return true;
  }

  Logger::log_error("Failed to remove active product: "+nid+"\nError in: 'product.cpp'");
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
}