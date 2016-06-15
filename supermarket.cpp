#include "supermarket.h"
#include "logger.h"
#include "utils.h"
#include <stdlib.h>
#include <algorithm>
#include <iostream>

using std::string;
using std::cout;
using std::endl;


bool Supermarket::check_for_discounts(const string& receipt)
{
  if (!(receipt.find(k35) != string::npos || receipt.find(k50) != string::npos)) return false;
  
  
  for (int i = 0; i < receipt.length(); i++)
  {
    if (receipt[i] == k35 || receipt[i] == k50)
    {
      if (isdigit(receipt[i-1])) return true;   // If the character in front of the discount sign is a number then found = true
    }
  }

  return false;
}

std::vector<Product> Supermarket::get_discount_products_receipt(const string& receipt)
{
  std::vector<Product> discountProducts;

  for (int i = 0; i < receipt.length(); i++)
  {
    if (receipt[i] == k35 || receipt[i] == k50)
    { 
      if (!isdigit(receipt[i-1])) continue; // check if there is a number next to the discount sign to make sure this is the correct product
      char c = receipt[i];
      int j = i;
      
      while (receipt[j] != ' ') j--;
      
      const int end = j;
      string _pPrice;
      
      for (; receipt[j] != c; j++) _pPrice += receipt[j];
	
      std::replace(_pPrice.begin(), _pPrice.end(), ',', '.');

      Utils::trim(_pPrice);

      double pPrice = std::stod(_pPrice.c_str());
      
      string pName;

      while (receipt[j] != '^') j--;
      
      j++;
	
      for (; j < end; j++) pName += receipt[j];
      
      Utils::trim(pName);
      
      Product product = Product(pName, pPrice);
      discountProducts.push_back(product);
    }
  } 
  return discountProducts;
}

double compare_product_names(string str1, string str2)
{ 
  Utils::strip(str1, ' '); // might be other whitespace symbols http://www.cplusplus.com/reference/cctype/isspace/
  Utils::strip(str2, ' '); // same goes for this
  
  Utils::trim(str1);
  Utils::trim(str2);

  Utils::to_upper(str1);
  Utils::to_upper(str2);

  
  if (str1 == str2) return 100;
  
  const string prefix = "AH";
  
  if (Utils::starts_with(str1, prefix) && !Utils::starts_with(str2, prefix))
    str1.erase(0, prefix.size());
  if (Utils::starts_with(str2, prefix) && !Utils::starts_with(str1, prefix))
    str2.erase(0, prefix.size());
  
  int ld = Utils::levenshtein_distance(str1, str2);
  
  double biggestNumber = std::max(str1.length(), str2.length());
   
  return (biggestNumber - ld) / biggestNumber * 100;
}

double compare_product_prices(const double product1, const double product2) 
{
  return product1 > product2 ? product1 - product2 : product2 - product1;
}

std::vector< ActiveProduct > Supermarket::get_matched_active_products(const std::vector< Product >& products, const std::vector< ActiveProduct >& activeProducts)
{
  std::vector<ActiveProduct> matchedProducts;
  
  //ProductLogger productLogger;
  Logger productLogger;
  
  for (std::vector<Product>::const_iterator i = products.begin(); i != products.end(); ++i)
  {
    Product product = *i;
    ActiveProduct highestMatchedProduct;
    
    double bNM = 0, bPM = 0;

    for (std::vector<ActiveProduct>::const_iterator j = activeProducts.begin(); j != activeProducts.end(); ++j)
    {
      ActiveProduct activeProduct = *j;
      
      int nameMargin;
      const double priceMargin = 0.05;
      bool match = false;
      
      string activeProdName;
      
      cout << "Name: " << activeProduct.get_name() << endl;
      cout << "System Name: " << activeProduct.get_system_name() << endl;
      
      if (activeProduct.get_system_name() == "[]" || activeProduct.get_system_name().empty())
      {
	activeProdName = activeProduct.get_name(); 
	nameMargin = 75;
      }
      else
      {
	activeProdName = activeProduct.get_system_name(); 
	nameMargin = 100;
      }
      
      bool weightProduct = Utils::starts_with(activeProduct.get_eancode(), "2"); // if ean code starts with 2, it's a weight product
      
      if (weightProduct)
      {
	nameMargin = 70;
      }
      double nameMatch = compare_product_names(product.get_name(), activeProdName);
      double priceDiff = compare_product_prices(product.get_price(), activeProduct.get_price());
      
      if (!weightProduct)
      {
	if (nameMatch >= nameMargin && priceDiff <= priceMargin)
	{
	  if (nameMatch > bNM || (nameMatch == bNM && priceDiff < bPM))
	  {
	    highestMatchedProduct = activeProduct;
	    bNM = nameMatch;
	    bPM = priceDiff;
	    match = true;
	  }
	}
      }
      else
      {
	if (nameMatch >= nameMargin && nameMatch >= bNM)
	{
	  double priceDiffHNM = bNM != 0 ? compare_product_prices(product.get_price(), highestMatchedProduct.get_price()) : 0; // get price difference between the current product and the heighest matchd product
	
	  cout << "Name match: " << nameMatch << "\nbNM: " << bNM << "\nProduct: " << activeProdName << endl;
	  
	  if (nameMatch > bNM || (nameMatch == bNM && priceDiff < priceDiffHNM))
	  {
	    highestMatchedProduct = activeProduct;
	    bNM = nameMatch;
	    bPM = priceDiff;
	    match = true;
	  }
	}
      } 

      productLogger.log_product_info(product, activeProduct, match, nameMatch);
      cout << endl;
    }
    
    if (bNM != 0)
    {
      matchedProducts.push_back(highestMatchedProduct);
    }
  }
  return matchedProducts;
}
