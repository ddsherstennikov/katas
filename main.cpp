#include <iostream>
#include <vector>

#include "db_adapter/db_adapter.h"



int main()
{
	DBAdapter dba("db1", "t", "data");

	std::vector<std::string> result;
    dba.Read("obiwan", 2, result);
    
    std::cout << result[0] << std::endl << result[1] << std::endl;

    dba.Write("T800", "I`ll be back.");

	result.clear();
	dba.Read("T800", 1, result);

	std::cout << result[0] << std::endl ;

    return 0;
}