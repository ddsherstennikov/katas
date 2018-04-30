// swaps_cpp03.cpp
// different kinds of swap in C++03
//=================================

#include <cstdlib>
#include <iostream>

using std::cout;

void valSwap( int* a, int* b )
{
  int t = *b;
  *b = *a;
  *a = t;  
}

void refSwap( int& a, int& b )
{
  int t = b;
  b = a;
  a = t;  
}

void ptrSwap( int*& a /*0x28fedc*/, int*& b /*0x28fed8*/ )
{
  int* t = b;
  b = a;
  a = t;  
}

int main(int argc, char** argv) 
{
  int one = 1, two = 2;
  
  /*
  cout << "one : " << one << "\n" << "&one : " << &one << "\n";
  cout << "two : " << two << "\n" << "&two : " << &two << "\n";  
  
  valSwap(one,two);
  
  cout << "one : " << one << "\n" << "&one : " << &one << "\n";
  cout << "two : " << two << "\n" << "&two : " << &two << "\n"; 
  */
  
  // can`t init non-const refs with rvalues
  int *pOne = &one, *pTwo = &two;  
  
  cout << "pOne : " << pOne << "\n" << "*pOne : " << *pOne << "\n";
  cout << "pTwo : " << pTwo << "\n" << "*pTwo : " << *pTwo << "\n";  
  
  ptrSwap( pOne, pTwo );
  
  cout << "pOne : " << pOne << "\n" << "*pOne : " << *pOne << "\n";
  cout << "pTwo : " << pTwo << "\n" << "*pTwo : " << *pTwo << "\n";  
   
  return 0;
}

