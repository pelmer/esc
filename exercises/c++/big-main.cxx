#include "big.hxx"
#include <iostream>
#include <algorithm>

int main()
{
  std::cout << "== init ==\n";
  Big x,y;
  Big a;

  std::cout << "== makebig ==\n";
  a = makeBig();

  std::cout << "== create b ==\n";
  Big b(x+y);

  std::cout << "== create c ==\n";
  Big c = x+y;

  std::cout << "== assign to a\n";
  a = x+y;

  std::cout << "== munge into a\n";
  a = munge(x+y);
  
  std::cout << "== swap...\n";
  std::swap(x,y);

  std::cout << "=== done ===\n";
  return 0;
}
