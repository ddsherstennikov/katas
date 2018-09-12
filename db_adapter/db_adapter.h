#pragma once

#include <iostream>
#include <vector>

#include <sqlite3.h>



class DBAdapter
{
public:
	DBAdapter(const std::string& dbname, const std::string& tblname, const std::string& datacolname);

	~DBAdapter();

	void Read(const std::string& key, size_t count, std::vector<std::string>& result);
	void Write(const std::string& key, const std::string& data);

private:
	char* zErrMsg = 0;

	std::string dbname_;
	std::string tblname_;
	std::string datacolname_;

	sqlite3* db_;
};

