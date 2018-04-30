#include <iostream>
#include <utility>
#include <tuple>

#include "complejo.h"

using namespace std;

typedef double _CT;

int main()
{

  Complejo<_CT> c;
  cout << c.to_string() << endl;

  std::pair<_CT, _CT> p(21.0, 53.0);
  cout << Complejo<_CT>(p).to_string() << endl;

  auto c2 = c + p;
  cout << c2.to_string() << endl;
  cout << c2.mod() << endl;
  cout << c2.arg() << endl;

  return 0;
}