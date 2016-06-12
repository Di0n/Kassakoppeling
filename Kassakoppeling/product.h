#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include <string>

using std::string;

class Product
{
public:
  Product();
  Product(const string& name, const double price);
  
  //Gets the product name
  string get_name();
  //Gets the product price
  double get_price();
private:
  string name;
  double price;
};
//#endif

//#ifndef ACTIVE_PRODUCT_H
//#define ACTIVE_PRODUCT_H

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
  bool remove_active_product(string url, const string& restCallCredentials);
private:
  int quantity;  
  string nid;
  string systemName;
  string eancode;
};
#endif