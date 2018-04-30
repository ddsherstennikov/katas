#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main()
{
  std::vector<int> v;
  std::string line;
  int i;

  getline(cin, line, '\n');
  stringstream parse(line);
  while ( parse >> i )
    v.push_back(i);

  for (auto ii : v)
    cout << ii << " ";

  return 0;
}