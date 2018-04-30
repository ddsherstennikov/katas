// basic_string_ex.cpp
// basic string exercises: reverse, count words, output permutations
//==================================================================

#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>
#include <utility>

using namespace std;

int main(int argc, char** argv) 
{
  // reverse string
  string s1;
  cin >> s1; 
  std::reverse( s1.begin(), s1.end() );
  cout << s1 << endl;
  
  // 2) count words in text
  string text, s2;
  getline ( cin, text );
  istringstream ss( move(text) );
  map<string, int> dict;
  while ( ss >> s2 )
    ++dict[s2];
  for( tuple<string,int> p : dict )
    cout << get<0>(p) << " " << get<1>(p) << endl;
  
  // 3) output all hardcoded string permutations once
  string s3= "abcd";
  do 
    cout << s3 << endl;  
  while (( next_permutation(s3.begin(), s3.end()) ));

  return 0;
}

