#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>


namespace sdd
{
	template<class ForwardIt>
	ForwardIt rotate(ForwardIt first, ForwardIt n_first, ForwardIt last)
	{
		ForwardIt next = n_first;

		while (first != next)
		{
			std::swap(*first++, *next++);
			if (next == last)
				next = n_first;
			if (first == n_first)
				n_first = next;
		}

		return next;
	}
}





int main()
{
	std::vector<int> v{ 2, 4, 2, 0, 5, 10, 7, 3, 7, 1 };


	std::cout << "before sort:      ";
	for (int n : v)
		std::cout << n << ' ';
	std::cout << '\n';

	// insertion sort
	for (auto i = v.begin(); i != v.end(); ++i) {
		sdd::rotate(std::upper_bound(v.begin(), i, *i), i, i + 1);
	}

	std::cout << "after sort:       ";
	for (int n : v)
		std::cout << n << ' ';
	std::cout << '\n';

	// simple rotation to the left
	sdd::rotate(v.begin(), v.begin() + 1, v.end());

	std::cout << "simple rotate left  : ";
	for (int n : v)
		std::cout << n << ' ';
	std::cout << '\n';

	// simple rotation to the right
	sdd::rotate(v.rbegin(), v.rbegin() + 1, v.rend());

	std::cout << "simple rotate right : ";
	for (int n : v)
		std::cout << n << ' ';
	std::cout << '\n';
}
