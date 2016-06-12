/*#pragma once

#include <string>

using std::string;

//String to uppercase
void to_upper(std::string &str);
//string to lowercase
void to_lower(std::string &str);
//trim the string (removes trailing whitespaces)
void trim(std::string &str);
//Removes all the occurences of const char c
void strip(std::string &str, const char c);
//Calculates the levenshtein_distance
int levenshtein_distance(const std::string &s1, const std::string &s2);
//Returns whether the string is convertible to an int
bool is_int(const std::string &str);
//Returns whether the string starts with the given string
bool starts_with(const string& str, const string& startsWith);
//Gets the output from the terminal/console
string get_out_frm_cmd(string cmd);
//String to uppercase copy
string to_upper_copy(string str);
//String to lowercase copy
string to_lower_copy(string str);
//Trim copy
string trim_copy(const string& str);
//Strip copy
string strip_copy(string str, const char& c);
//Converts a double-precision floating-point to a string
string double_to_string(const double d);/