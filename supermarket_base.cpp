#include "supermarket_base.h"
#include "utils.h"
#include "webclient.h"
#include "logger.h"
#include "config_file.h"

#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <memory>


using std::string;
using std::cout;
using std::endl;


SupermarketBase::SupermarketBase()
{
  this->id = 0;
  this->getActiveDiscountsFullUrl = "http://agptest.gemoro.nl/rest/getactivediscountsfull?";
}

SupermarketBase::SupermarketBase(const int id, const string& getActiveDiscountsFullUrl)
{
  this->id = id;
  this->getActiveDiscountsFullUrl = getActiveDiscountsFullUrl;
}

int SupermarketBase::check_for_active_discounts()
{
  const string url = getActiveDiscountsFullUrl + "supermarketid=" + std::to_string(id);
  
  
  WebClient::HTTP http;
  
  for (int count = 0; count < 2; count++)
  { 
      string httpData;
      bool success = http.get(url, restCallCredentials, httpData);
      
      if (!success) 
      {
	usleep(500000); // sleep 500 ms
	continue;
      }
      
      Utils::trim(httpData);
      
      if (httpData == "[]" || httpData.empty())
	return 0;
      
      return 1;   
  }
  return 2;
}


bool get_json_value(const string& url, const string& credentials, Json::Value& jsonOut)
{  
  WebClient::HTTP http;
    
  for (int count = 0; count < 2; count++)
  {
    string httpData;

    bool success = http.get(url, credentials, httpData);
    
    if (!success) 
    {
      usleep(500000); // sleep 500 ms
      continue;
    }
  
    Json::Reader jsonReader;
    Json::Value jsonData;
  
    if (jsonReader.parse(httpData, jsonData)) 
    {
      Logger::log_info("Succesfully parsed JSON data!");
      jsonOut = jsonData;
      return true;
    }  
    else 
    {
      Logger::log_error("Could not parse HTTP data to JSON\nError in: 'supermarket_base.cpp'");
      return false;
    }
  }
  return false;
}

bool SupermarketBase::get_active_products(std::vector<ActiveProduct>& outActiveProduct)
{
  const string url = getActiveDiscountsFullUrl + "supermarketid="+ std::to_string(id); // "http://agptest.gemoro.nl/rest/getactivediscountsfull?supermarketid="+std::to_string(id);
  
  Json::Value jsonValue;
  
  if (!get_json_value(url, restCallCredentials, jsonValue)) return false;
  
  const string jsonValueStr = Utils::trim_copy(jsonValue.toStyledString());
  
  if (jsonValue.empty())
  {
    Logger::log_error("JsonValue is empty!\nError in: 'supermarket_base.cpp'");
    return false;
  }
  
  cout << jsonValueStr << endl;
  
  std::vector<ActiveProduct> activeProduct;
  
  for (Json::ValueIterator i = jsonValue.begin(); i != jsonValue.end(); ++i)
  {
    // Json::Value jv = *i;
    const string nodeTitle = (*i).get("node_title", "").asString();
    const double oldPrice = std::stod((*i).get("oldPrice", "0").asString());
    const int quantityCurrent = std::stoi((*i).get("quantityCurrent", "0").asString());
    const string nid = (*i).get("nid", "0").asString();
    const string systemName = Utils::strip_copy((Utils::trim_copy((*i).get("systemName", "").toStyledString())), '"');
    const string eancode = (*i).get("prod_ean", "").asString();
    cout << "nt: " << nodeTitle << "  nid: " << nid << "  systemName: " << systemName << "  eancode: " << eancode; 
    ActiveProduct ap = ActiveProduct(nodeTitle, oldPrice, quantityCurrent, nid, systemName, eancode);
    activeProduct.push_back(ap);
  }
  
  outActiveProduct = activeProduct;
  return true;
}