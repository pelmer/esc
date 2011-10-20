// Compile: c++ -g -W -Wall -Werror -ansi -pedantic -o vs-build vs-build.cc -ldl -lpthread
#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>

int main(int, char **)
{
  std::vector<std::string> result;
  std::ifstream file("/usr/share/dict/words");
  std::copy(std::istream_iterator<std::string>(file),
	    std::istream_iterator<std::string>(),
	    std::back_inserter(result));
  return result.size();
}
