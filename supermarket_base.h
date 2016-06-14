#ifndef __SUPERMARKET_BASE_H__
#define __SUPERMARKET_BASE_H__

#include "product.h"

#include <vector>
#include <string>

class SupermarketBase
{
public:
  //Constructors / Deconstructor
  SupermarketBase() {};
  SupermarketBase(const int id, const string& getActiveDiscountsFullUrl, const string& restCallCredentials);
  virtual ~SupermarketBase(){}
  
  //Gets the active products from the supermarket
  //outActiveProduct returns the active products
  //Returns true if it succeeded to get the active products
  bool get_active_products(std::vector<ActiveProduct>& outActiveProduct);
  
  //Returns 0 if there are no active discounts for the supermarket 
  //Returns 1 if there are active discounts
  //Returns 2 if there was an error
  int check_for_active_discounts();
  
  //Returns whether there are discounts on the receipt
  virtual bool check_for_discounts(const std::string& receipt)=0;
  
  //Gets the discounts from the receipt
  virtual std::vector<Product> get_discount_products_receipt(const std::string& receipt)=0;
  
  //Gets the active products whom are a successfull match
  //1st param is a vector with the receipt products
  //2nd param is a vector with the active products
  virtual std::vector<ActiveProduct> get_matched_active_products(const std::vector<Product>& products, const std::vector<ActiveProduct>& activeProducts)=0;
  
  int id;
  string getActiveDiscountsFullUrl;
  string restCallCredentials;
};
#endif
