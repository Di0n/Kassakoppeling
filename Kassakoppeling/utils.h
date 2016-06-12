#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>

using std::string;

namespace Utils
{
  //String to uppercase
  void to_upper(string& s);
  //String to lowercase
  void to_lower(string& s);
  //Trim string (removes trailing whitespaces)
  void trim(string& s);
  //Strips the specified characters from the string
  void strip(string& s, const char c);
  
  //Levenshtein distance algorithm
  int levenshtein_distance(const string& s, const string& s2);
  
  //Returns true if the given string is convertible to an int
  bool is_int(const string& s);
  //Returns true if the string starts with the 'startsWith' string
  bool starts_with(const string& s, const string& startsWith);
  //Returns true if the string ends with the 'endsWith' string
  bool ends_with(const string& s, const string& endsWith);
  // Returns true if the file file_exists
  // Returns false if the file does not exist or if it failed to check
  bool file_exists(const string& path);
  // Copy pastes a file
  // Returns true if it succeeded
  // Returns false if it did not succeed
  bool copy_paste_file(const string& fileToCopy, const string& pasteAs);
  
  //Gets output from the terminal
  string get_output_from_terminal(string cmd);
  //Same as to_upper() but returns a copy
  string to_upper_copy(string s);
  //Same as to_lower() but returns a copy
  string to_lower_copy(string s);
  //Same as trim() but returns a copy
  string trim_copy(const string& s);
  //Same as strip() but returns a copy
  string strip_copy(string s, const char c);
  //Converts a double to string
  string double_to_string(const double d);
}
#endif