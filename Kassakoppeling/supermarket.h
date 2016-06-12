//This class will be edited for almost every supermarket because not all supermarkets do things the same way
// I am aware not everything in here is the 'OOP' way but in this case I think it's justified because this should be the only class that needs to be modified for a new supermarket 

#ifndef __SUPERMARKET_H__
#define __SUPERMARKET_H__

#include "supermarket_base.h"


class Supermarket : public SupermarketBase
{
public:
  virtual ~Supermarket(){}
  
  //Returns whether there are discounts on the receipt
  virtual bool check_for_discounts(const std::string& receipt);
  //Gets the discounts from the receipt
  virtual std::vector<Product> get_discount_products_receipt(const std::string& receipt);
  //Gets the active products whom are a successfull match
  //1st param is a vector with the receipt products
  //2nd param is a vector with the active products
  virtual std::vector<ActiveProduct> get_matched_active_products(const std::vector<Product>& products, const std::vector<ActiveProduct>& activeProducts);
private:
  //These are the symbols which are placed on the receipt to identify if there are discounts on the receipt
  //Add more discount codes if needed
  const char k35 = '*';
  const char k50 = '#';
};
#endif