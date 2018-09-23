#pragma once

#include <iostream>
#include <vector>

#include <sqlite3.h>



class DBAdapter
{
public:
	DBAdapter(const std::string& dbname, const std::string& tblname, const std::string& datacolname);

	~DBAdapter();

	std::string Read(const std::string& key);
	void Write(const std::string& key, const std::string& data);

	void Read(const std::string& key, std::vector<std::string>& result, size_t count);

private:
	char* zErrMsg = 0;

	std::string dbname_;
	std::string tblname_;
	std::string datacolname_;

	sqlite3* db_;
};

