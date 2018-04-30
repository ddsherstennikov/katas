#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>


namespace sdd
{
	template< class RandomIt, class URBG >
	void shuffle(RandomIt first, RandomIt last, URBG&& g)
	{
		auto l = last - first;
	
		for (auto i = 0; i != l; i++)
		{ 
			unsigned int n = g();
			auto delta = (i + (n % l)) % l;
			auto it = first + i;
			auto jt = first + delta;

			if (it != jt)
				std::iter_swap(it, jt);
		}
	}
}

int main()
{
	std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	std::random_device rd;
	std::mt19937 g(rd());
	sdd::shuffle(v.begin(), v.end(), g);
	
	std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << "\n";
}