#include "utils.h"

#include <stdio.h>
#include <time.h>
#include <ctime>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <sstream>
#include <memory>
#include <sys/stat.h>
#include <fstream>

using std::string;


void Utils::to_upper(string& s)
{
  for (char & c : s) c = toupper(c);
}

void Utils::to_lower(string& s)
{
  for (char & c : s) c = tolower(c);
}

void Utils::trim(string& s)
{
  auto wsFront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
  auto wsBack=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
  s = (wsBack<=wsFront ? std::string() : std::string(wsFront,wsBack));
}

void Utils::strip(string& s, const char c)
{
  for (string::size_type i = 0; i < s.size(); ++i)
  {
    if (s[i] == c)
    {
      s.erase(s.begin()+i);
    }
  }
}

int Utils::levenshtein_distance(const string& s, const string& s2)
{
  int s1len = s.size();
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
			  last_diagonal + (s[y - 1] == s2[x - 1]? 0 : 1)
		  };
		  column[y] = std::min(possibilities);
		  last_diagonal = old_diagonal;
	  }
  }
  auto result = column[s1len];
  delete[] column;
  return result;
}

bool Utils::is_int(const string& s)
{
  if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

   char * p;
   strtol(s.c_str(), &p, 10);

   return (*p == 0);
}

bool Utils::starts_with(const string& s, const string& startsWith)
{
  return startsWith.size() <= s.size() && s.compare(0, startsWith.size(), startsWith) == 0;
}

bool Utils::ends_with(const string& s, const string& endsWith)
{
  return endsWith.size() <= s.size() && s.compare(s.size() - endsWith.size(), endsWith.size(), endsWith) == 0;
}

bool Utils::file_exists(const string& path)
{
  struct stat fileInfo;
  return (stat(path.c_str(), &fileInfo) == 0) ? true : false;
}

bool Utils::copy_paste_file(const string& fileToCopy, const string& pasteAs)
{
  std::ifstream src(fileToCopy.c_str(), std::ios::binary);
  std::ofstream dest(pasteAs.c_str(), std::ios::binary);
  
  if (src != NULL && dest != NULL)
  {
    dest << src.rdbuf();
    return true;
  }
  return false;
}

string Utils::get_output_from_terminal(string cmd)
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
    data = "ERROR";
  }
  
  return data;
}

string Utils::to_upper_copy(string s)
{
  to_upper(s);
  return s;
}

string Utils::to_lower_copy(string s)
{
  to_lower(s);
  return s;
}

string Utils::trim_copy(const string& s)
{
  auto wsFront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
  auto wsBack=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
  return (wsBack<=wsFront ? std::string() : std::string(wsFront,wsBack));
}

string Utils::strip_copy(string s, const char c)
{
  strip(s, c);
  return s;
}

string Utils::double_to_string(const double d)
{
  std::ostringstream oss;
  oss << d;
  return oss.str();
}