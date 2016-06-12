/*#include "utility.h"

#include <stdio.h>
#include <time.h>
#include <ctime>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <sstream>
//#include <boost/algorithm/string.hpp>

using std::string;


bool is_int(const string& str)
{
  if(str.empty() || ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+'))) return false;

   char * p;
   strtol(str.c_str(), &p, 10);

   return (*p == 0);
}

void to_upper(string& str)
{
  for (char & c : str) c = toupper(c);
}

string to_upper_copy(string str)
{
  to_upper(str);
  return str;
}

void to_lower(string& str)
{
  for (char & c : str) c = tolower(c);
}

string to_lower_copy(string str)
{
  to_lower(str);
  return str;
}

void trim_left(string& str)
{
  for (string::size_type i = 0; i < str.length(); ++i)
  {
    if (isspace(str[i])) str.erase(str.begin()+i);
    else break;
  }
}

string trim_left_copy(string str)
{
  trim_left(str);
  return str;
}

void trim_right(string& str)
{
  for (string::size_type i = str.length(); i > 0; ++i)
  {
    if (isspace(str[i])) str.erase(str.begin()+i);
    else break;
  }
}

string trim_right_copy(string str)
{
  trim_right(str);
  return str;
}

void trim(string& str)
{
  auto wsFront=std::find_if_not(str.begin(),str.end(),[](int c){return std::isspace(c);});
  auto wsBack=std::find_if_not(str.rbegin(),str.rend(),[](int c){return std::isspace(c);}).base();
  str = (wsBack<=wsFront ? std::string() : std::string(wsFront,wsBack));
}

string trim_copy(const string& str)
{
  auto wsFront=std::find_if_not(str.begin(),str.end(),[](int c){return std::isspace(c);});
  auto wsBack=std::find_if_not(str.rbegin(),str.rend(),[](int c){return std::isspace(c);}).base();
  return (wsBack<=wsFront ? std::string() : std::string(wsFront,wsBack));
}

void strip(string& str, const char c)
{
  for (string::size_type i = 0; i < str.size(); ++i)
  {
    if (str[i] == c)
    {
      str.erase(str.begin()+i);
    }
  }
}

string strip_copy(string str, const char& c)
{
  strip(str, c);
  return str;
}

bool starts_with(const string& str, const string& startsWith)
{
  return startsWith.size() <= str.size() && str.compare(0, startsWith.size(), startsWith) == 0;
}

string double_to_string(const double d)
{
  std::ostringstream oss;
  oss << d;
  return oss.str();
}

string get_out_frm_cmd(string cmd)
{
  std::cout.flush();
  string data;
  try
  {
  
   FILE * stream;
   const int max_buffer = 256;
   char buffer[max_buffer];
   cmd.append(" 2>&1");
   
   stream = popen(cmd.c_str(), "r");
   
   if (stream)
   {
     while (!feof(stream))
       
     if (fgets(buffer, max_buffer, stream) != NULL) 
       data.append(buffer);
     
     pclose(stream);
   }
   
  }
  catch (...)
  {
    // TODO log error
    data = "ERROR";
  }
  
  return data;
}

int levenshtein_distance(const std::string &s1, const std::string &s2)
{
	// To change the type this function manipulates and returns, change
	// the return type and the types of the two variables below.
	int s1len = s1.size();
	int s2len = s2.size();
	
	auto column_start = (decltype(s1len))1;
	
	auto column = new decltype(s1len)[s1len + 1];
	std::iota(column + column_start, column + s1len + 1, column_start);
	
	for (auto x = column_start; x <= s2len; x++) {
		column[0] = x;
		auto last_diagonal = x - column_start;
		for (auto y = column_start; y <= s1len; y++) {
			auto old_diagonal = column[y];
			auto possibilities = {
				column[y] + 1,
				column[y - 1] + 1,
				last_diagonal + (s1[y - 1] == s2[x - 1]? 0 : 1)
			};
			column[y] = std::min(possibilities);
			last_diagonal = old_diagonal;
		}
	}
	auto result = column[s1len];
	delete[] column;
	return result;
}

/*
string strip(const string& str, const char* c)
{
  string s = str;
  boost::algorithm::erase_all(s, c);
  return s;
}
string trim(const string& str)
{
  string s = str;
  boost::algorithm::trim(s);
  return s;
}

string trim_left(const string& str)
{
  string s = str;
  boost::algorithm::trim_left(s);
  return s;
}

string trim_right(const string& str)
{
  string s = str;
  boost::algorithm::trim_right(s);
  return s;
}

void to_upper(string &str)
{
  boost::to_upper(str);
}
*/