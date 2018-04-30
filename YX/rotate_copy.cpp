#include <algorithm>
#include <vector>
#include <iostream>

template< class ForwardIt, class OutputIt >
OutputIt rotate_copy(ForwardIt first, ForwardIt n_first, ForwardIt last, OutputIt d_first)
{
	for (ForwardIt it = n_first; it != last; it++, d_first++)
		*d_first = *it;
	for (ForwardIt it = first; it != n_first; it++, d_first++)
		*d_first = *it;
}

int main()
{
	std::vector<int> src = { 1, 2, 3, 4, 5 };
	auto pivot = std::find(src.begin(), src.end(), 3);
	std::vector<int> dest(src.size());

	std::rotate_copy(src.begin(), pivot, src.end(), dest.begin());

	for (const auto &i : dest) {
		std::cout << i << ' ';
	}
	std::cout << '\n';
}
