#include <iostream>
#include <vector>
#include <list>
#include <cstdlib>
#include <string>
#include <ctime>
#include <sys/time.h>
#include <fstream>
#include <cstddef>
#include <algorithm>
#include <random>
#include <thread>

#include <boost/algorithm/string.hpp>

#include "db_adapter/db_adapter.h"
#include "db_cache/db_cache.h"
#include "quote_genr/quote_genr.h"



size_t n_threads = std::thread::hardware_concurrency();



uint64_t get_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return tv.tv_sec * 1'000'000LL + tv.tv_usec;
}



using seq = std::vector<std::pair<std::string, std::string>>;

seq get_sequence(std::ifstream& file)
{
	seq result;

	std::vector<std::string> vs;
	std::string line;

	while(std::getline(file, line))
	{
		boost::split(vs, line, boost::is_any_of(","));
		result.emplace_back(std::make_pair(vs[0], vs[1]));
	}

	return result;
}



struct TestContext
{
	TestContext(const std::string& dbname, const std::string& tblname, const std::string& datacolname, std::vector<seq>&& in)
	: dba(dbname, tblname, datacolname), dbc(dbname, tblname, datacolname), input(in)
	{}

	TestContext() = delete;

	std::vector<seq> input;

	DBAdapter dba;
	DBCache dbc;
};



void direct_thread_function(DBAdapter& db_adapter, seq& input)
{
	for (auto p : input)
	{
		db_adapter.Update(p.first, p.second);

		std::string s;
		for(int i=0;i<10;i++)
		{
			db_adapter.Read(quote_sources[random_num(0, 19)], s);
		}
	}
}

void cached_thread_function(DBCache& db_cache, seq& input)
{
	for (auto p : input)
	{
		db_cache.Write(p.first, p.second);

		std::string s;
		for(int i=0;i<10;i++)
		{
			db_cache.Read(quote_sources[random_num(0, 19)], s);
		}
	}

	db_cache.sync(boost::system::error_code());
}



void test_direct(TestContext& ctx)
{
	std::vector<std::thread> workers;

	uint64_t total_time = 0;
	auto timestamp_us = get_time();

	for (size_t i = 0; i < n_threads; ++i)
		workers.emplace_back(std::thread(direct_thread_function, std::ref(ctx.dba), std::ref(ctx.input[i])));

	for (std::thread& t: workers)
		if (t.joinable())
			t.join();

	total_time += get_time() - timestamp_us;

	std::vector<std::string> final_dump;
	ctx.dba.Read(final_dump, 20);

	std::cout << "test_direct" << std::endl;
	std::cout << "total time: " << total_time/1000000LL << " sec" << std::endl << std::endl;
	std::cout << "DUMP:" << std::endl;

	for (auto s : final_dump)
		std::cout << s << std::endl;

	std::cout << std::endl;
}

void test_cached(TestContext& ctx)
{
	std::vector<std::thread> workers;

	uint64_t total_time = 0;
	auto timestamp_us = get_time();

	for (size_t i = 0; i < n_threads; ++i)
		workers.emplace_back(std::thread(cached_thread_function, std::ref(ctx.dbc), std::ref(ctx.input[i])));

	for (std::thread& t: workers)
		if (t.joinable())
			t.join();

	total_time += get_time() - timestamp_us;

	std::vector<std::string> final_dump;
	ctx.dba.Read(final_dump, 20);

	std::cout << "test_cached" << std::endl;
	std::cout << "total time: " << total_time/1000000LL << " sec" << std::endl;
	std::cout << "DUMP:" << std::endl;

	for (auto s : final_dump)
		std::cout << s << std::endl;

	std::cout << std::endl;
}


void populate()
{
	std::ofstream infile("input.txt", std::ios::out);
	std::ofstream outfile("output.txt", std::ios::out);

	genr_io(infile, outfile, 100'000);
}



int main()
{	std::ifstream infile("input.txt");
	seq input = get_sequence(infile);

	std::vector<seq> seqvec(n_threads, seq());
	size_t part = input.size() / n_threads;
	auto begin = input.begin();

	for (int i=0; i<n_threads; i++)
	{
		std::move(begin, begin + part, std::back_insert_iterator(seqvec[i]));
		begin += part;

		if (i == n_threads-1)
			std::move(begin, input.end(), std::back_insert_iterator(seqvec[i]));
	}

	TestContext ctx(/*database=*/ "qdb", /*table=*/ "quotes", /*datacolumn=*/ "data", std::move(seqvec));

	test_direct(ctx);

	test_cached(ctx);

	return EXIT_SUCCESS;
}