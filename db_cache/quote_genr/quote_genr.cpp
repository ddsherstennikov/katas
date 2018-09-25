#include <vector>
#include <fstream>
#include <random>
#include <map>

#include "quote_genr/quote_genr.h"



std::string random_str(const size_t len)
{
	std::string s;

	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		char c = alphanum[rand() % (sizeof(alphanum) - 1)];
		s.push_back(c);
	}

	return s;
}

size_t random_num(const size_t range)
{
	return range * (rand() / (RAND_MAX + 1.0));
}

size_t random_num(const size_t lo, const size_t hi)
{
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(lo, hi); // define the range

	return distr(eng); // generate numbers
}



void genr_io(std::ofstream& infile, std::ofstream& outfile, size_t lines)
{
	size_t shortest_quote = 10;
	size_t longest_quote = 30;
	size_t sz = quote_sources.size();

	std::vector<std::string> last_quote(sz, "");

	while(lines--)
	{
		size_t q_src_num = random_num(sz);
		std::string q_src = quote_sources[q_src_num];
		std::string q_data = random_str(random_num(shortest_quote, longest_quote));

		infile << q_src << ", " << q_data << std::endl;

		last_quote[q_src_num] = q_data;
	}

	for(int i=0; i<sz; i++)
	{
		outfile << quote_sources[i] << ", " << last_quote[i] << std::endl;
	}
};