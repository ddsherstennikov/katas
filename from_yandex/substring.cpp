#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>



// e.g.: max_unique_symbols_substr("fgjkdfghlkfpgkcobok cvojgkifd jbd") == 9
int max_unique_symbols_substr(std::string& s)
{
	std::vector<int> pos(26, -1);
	int maxlen = 0;
	int begin = 0;

	for (int i = 0; i < s.length(); i++)
	{
		int idx = s[i] - 'a';

		if (pos[idx] >= begin)
		{
			maxlen = std::max(maxlen, i - begin);
			begin = pos[idx] + 1;
		}
		
		pos[idx] = i;
	}

	return maxlen;
}



int main()
{
	std::string s;

	std::cin >> s;

	std::cout << max_unique_symbols_substr(s);

	return 0;
}