//#ifndef CHAMELEON_H__
//#define CHAMELEON_H__
#ifndef __CONFIG_FILE_H__
#define __CONFIG_FILE_H__

#include <string>

class Chameleon {
public:
  Chameleon() {};
  explicit Chameleon(const std::string&);
  explicit Chameleon(double);
  explicit Chameleon(const char*);

  Chameleon(const Chameleon&);
  Chameleon& operator=(Chameleon const&);

  Chameleon& operator=(double);
  Chameleon& operator=(std::string const&);

public:
  operator std::string() const;
  operator double     () const;
private:
  std::string value_;
};

#include <map>

class ConfigFile {
  std::map<std::string,Chameleon> content_;

public:
  ConfigFile(std::string const& configFile);

  Chameleon const& Value(std::string const& section, std::string const& entry) const;

  Chameleon const& Value(std::string const& section, std::string const& entry, double value);
  Chameleon const& Value(std::string const& section, std::string const& entry, std::string const& value);
};
#endif