#include "exception-liberr.h"

#include <stdio.h>

Error::Error() :
  std::exception(),
  m_err_code(0)
{}

Error::Error(int error_code) :
  std::exception(),
  m_err_code(error_code)
{
  printf("** exception [error-code:%i] thrown!\n", m_err_code);
}

Error::~Error() throw ()
{}

int
Error::error_code() const
{
  return m_err_code;
}


