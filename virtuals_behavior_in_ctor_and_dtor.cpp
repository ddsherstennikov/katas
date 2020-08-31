#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Base
{
public:
    virtual void f1() { cout << "Base1" << endl; }
    virtual void f2() { cout << "Base2" << endl; }
};

class Derived : public Base
{
public:
    Derived() { f1(); f2(); }
    ~Derived() { f1(); f2(); }

    virtual void f1() override { cout << "Derived1" << endl; }
    virtual void f2() override { cout << "Derived2" << endl; }
};

class MoreDerived : public Derived
{
public:
    virtual void f1() { cout << "MoreDerived1" << endl; }
    virtual void f2() { cout << "MoreDerived2" << endl; }
};


int main()
{
    MoreDerived md;
}
