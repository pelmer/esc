// compile with: c++ -std=c++11 raw_string.cc

#include <iostream>

int main()
{
  std::cout << "\\\\\""; // prints \\" (3 characters)
  std::cout << R"(\\")"; // idem, but easier

  std::cout << "\n";     // prints a newline
  std::cout << R"(\n)";  // prints \n (2 characters)

  // how to print )" (2 characters)
  //  std::cout << R"()")";     // error
  std::cout << R"*+()")*+"; // ok

  std::cout << std::endl;
}
