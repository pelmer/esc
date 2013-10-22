#ifndef STRING_H
#define STRING_H

#include <memory>
#include <cstdlib>
#include <iostream>
#include <cstring>

#define LOG(message) std::cout << message << '\n';

class String
{
  char* s_; // null-terminated
public:
  String()
    : s_(nullptr)
  {
    LOG("String()");
  }
  String(char const* s)
  {
    LOG("String(char const*)");
    size_t size = strlen(s) + 1;
    s_ = new char[size];
    strcpy(s_, s);
  }
  ~String()
  {
    LOG("~String()");
    delete [] s_;
  }
  String(String const& other)
  {
    LOG("String(String const&)");
    size_t size = strlen(other.s_) + 1;
    s_ = new char[size];
    strcpy(s_, other.s_);
  }
  String(String&& other)
  : s_(other.s_)
  {
    LOG("String(String&&)");
    other.s_ = nullptr;
  }
  char const* c_str() const
  {
    return s_;
  }
};

inline std::ostream& operator<<(std::ostream& os, String const& s)
{
  return os << s.c_str();
}

#endif

