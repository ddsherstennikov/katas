// my_next_permutation.cpp
// get all permutations of string without STL support
//===================================================

#include <iostream>
#include <string>
#include <algorithm>
#include <functional>

using namespace std;

// min element on condition that it is strictly > barrier
template<class ForwardIterator>
  ForwardIterator min_element_above_barrier ( ForwardIterator first, ForwardIterator last, ForwardIterator barrier )
{
  if (first==last) return ( *last > *barrier ) ? last : last + 1;
  
  ForwardIterator smallest = find_if( first, last, bind2nd( greater<int>(), *barrier ) );

  while (++first!=last)
    if (*first<*smallest && *first>*barrier)
      smallest=first;
  
  return smallest;
}

template<class BiIter> bool my_next_permutation ( BiIter first, BiIter last )
{
  BiIter it = last;
  
  for(; it > first && *(it-1) >= *it; --it );
  
  if ( it == first )
    return false;
  
  BiIter lhs = it-1;
  BiIter min = min_element_above_barrier( it, last, lhs );
    
  iter_swap( lhs, min );  
  sort( it, last );
  
  return true;
}

int main(int argc, char** argv) 
{
  string s = "abcd";
  
  do 
    cout << s << endl;  
  while ( my_next_permutation(s.begin(), s.end()) );  
  
  return 0;
}

