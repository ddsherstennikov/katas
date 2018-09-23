#include <iostream>
#include <vector>

#include "db_adapter/db_adapter.h"
#include "db_cache/db_cache.h"



void direct_session()
{
	DBAdapter dba(/*database=*/"db1", /*table=*/"t", /*datacolumn=*/"data");

	dba.Write("obiwan", "and may the Force be with you... always.");
	dba.Write("yoda", "Do! Or do not. There is no \"try\".");
	dba.Write("obiwan", "these are not the droids you are looking for");
	dba.Write("T800", "I`ll be back.");

//	std::vector<std::string> result;
//    dba.Read("obiwan", result, 2);
//
//    std::cout << result[0] << std::endl << result[1] << std::endl;
//
//	result.clear();
//	dba.Read("T800", result, 1);
//
//	std::cout << result[0] << std::endl ;
}

void cached_session()
{
	DBCache dbc(/*database=*/"db1", /*table=*/"t", /*datacolumn=*/"data");

//	std::vector<std::string> result;
//	dba.Read("obiwan", result, 2);
//
//	std::cout << result[0] << std::endl << result[1] << std::endl;
//
//	//dba.Write("T800", "I`ll be back.");
//
//	result.clear();
//	dba.Read("T800", result, 1);
//
//	std::cout << result[0] << std::endl ;
}

int main()
{

    return 0;
}