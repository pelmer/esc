// dear emacs, this is -*- c++ -*-
#ifndef EXCEPTION_LIBERR_H
#define EXCEPTION_LIBERR_H 1

#include <stdexcept>

class Error : public std::exception
{
  int m_err_code;
public:
  Error();
  explicit Error(int error_code);
  virtual ~Error() throw();

  int error_code() const;
};

#endif // !EXCEPTION_LIBERR_H
