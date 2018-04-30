#include <iostream>
#include <vector>

#include "HivePtr.h"

using namespace std;

class A
{
public:
  A() { cout << "# class A ctor" << endl; v = new vector<char>( 1024*1024*1024, 'X' ); v->at(1024*1024*1024-1) = 'Y'; }
  ~A() { cout << "# class A dtor" << endl; delete v; }

private:
  vector<char>* v;
};

int main()
{
  A* pa = new A();
  HivePtr<A*> hpa( pa );

  cout << "# origin: " << pa << endl;
  cout << "# stored: " << hpa.get() << endl;

  HivePtr<A*> hpa2( hpa );

  { HivePtr<A*> hpa3( hpa ); }

  // wait
  //cin.ignore(1);

  return 0;
}