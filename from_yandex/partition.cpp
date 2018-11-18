#include "stdafx.h"

#include <vector>
#include <iostream>
#include <algorithm>


/*
Reorders the elements in the range [first, last) in such a way that all elements for which
the predicate p returns true precede the elements for which predicate p returns false.
Relative order of the elements is not preserved.
*/
namespace sdd
{
	template<class ForwardIt, class UnaryPredicate>
	ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPredicate p)
	{
		ForwardIt cur = first;
		size_t delta = last - first - 1;

		while (delta != 0)
		{
			if (p(*cur) == false)
				std::swap(*cur, *(cur + delta));
			else
				cur++;
			delta--;
		}

		return cur;
	}
}



int main()
{
	std::vector<int> v = { -1, 5, -7, 2, 4, 6, -17, 12, 14, 0 };
	auto p = [](int k)->bool {return k > 0; };

	auto it = sdd::partition(v.begin(), v.end(), p);

	for (auto e : v)
		std::cout << e << " ";

	std::cout << std::endl << *it;

	return 0;
}

