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
#include <boost/asio/thread_pool.hpp>

#include "db_adapter/db_adapter.h"
#include "db_cache/db_cache.h"
#include "quote_genr/quote_genr.h"



uint64_t get_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return tv.tv_sec * 1000000LL + tv.tv_usec;
}



using seq = std::list<std::pair<std::string, std::string>>;

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





void test(const seq& input, const seq& final)
{
	// TODO: create thread pool
	// TODO: create synchronization over input seq
	// TODO: run threads until input seq is empty
	// TODO: compare DBCache and DB with final seq
	// TODO: do this for (1) direct accessor
	// TODO:             (2) cached accessor
	// TODO: measure time FROM {threads run} TO {input seq empty}


	boost::asio::thread_pool pool(std::thread::hardware_concurrency());

	









//	uint64_t total_time = 0;
//
//	write_sequence::const_iterator iitr = write.begin();
//	read_sequence::const_iterator ritr = read.begin();
//
//	while (iitr != write.end() && ritr != read.end()) {
//		timestamp_us = get_time();
//		st.insert(*iitr);
//		total_time += get_time() - timestamp_us;
//
//		timestamp_us = get_time();
//		const std::string &str = st.get(ritr->first);
//		total_time += get_time() - timestamp_us;
//
//		if (ritr->second != str) {
//			std::cout << "test failed" << std::endl;
//			return;
//		}
//
//		iitr++;
//		ritr++;
//	}
//
//	std::cout << "total time: " << total_time/1000000LL << " sec" << std::endl;
}




void populate()
{
	std::ofstream infile("input.txt", std::ios::out);
	std::ofstream outfile("output.txt", std::ios::out);
	genr_io(infile, outfile, 100'000);
}



int main()
{
	std::ifstream infile("input.txt");
	std::ifstream outfile("output.txt");

	seq writes_in_db = get_sequence(infile);
	seq final_db_state = get_sequence(outfile);

	test(writes_in_db, final_db_state);
}






//void direct_session()
//{
//	DBAdapter dba(/*database=*/"db1", /*table=*/"t", /*datacolumn=*/"data");
//
//	dba.Write("obiwan", "and may the Force be with you... always.");
//	dba.Write("yoda", "Do! Or do not. There is no \"try\".");
//	dba.Write("obiwan", "these are not the droids you are looking for");
//	dba.Write("T800", "I`ll be back.");
//
////	std::vector<std::string> result;
////    dba.Read("obiwan", result, 2);
////
////    std::cout << result[0] << std::endl << result[1] << std::endl;
////
////	result.clear();
////	dba.Read("T800", result, 1);
////
////	std::cout << result[0] << std::endl ;
//}



//void cached_session()
//{
//	DBCache dbc(/*database=*/"db1", /*table=*/"t", /*datacolumn=*/"data");
//
////	std::vector<std::string> result;
////	dba.Read("obiwan", result, 2);
////
////	std::cout << result[0] << std::endl << result[1] << std::endl;
////
////	//dba.Write("T800", "I`ll be back.");
////
////	result.clear();
////	dba.Read("T800", result, 1);
////
////	std::cout << result[0] << std::endl ;
//}