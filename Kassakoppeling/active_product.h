/*#pragma once
#include "product.h"

#include <string>

class ActiveProduct : public Product
{
public:
  ActiveProduct();
  ActiveProduct(const string& name, const double price, const int quantity, const string& nid, const string& systemName, const string& eancode);
  
  //Gets the current quantity of the product 
  int get_quantity();
  //Gets the node id of the product
  string get_nid();
  //Gets the system name of the product
  string get_system_name();
  //Gets the eancode of the product
  string get_eancode();
  
  //Removes the active product from the back-end
  bool remove_active_product(string url);
private:
  int quantity;  
  string nid;
  string systemName;
  string eancode;
};

*/