#pragma once

#include <iostream>
#include <vector>
#include <mutex>

#include <sqlite3.h>



class DBAdapter
{
public:
	DBAdapter(const std::string& dbname, const std::string& tblname, const std::string& datacolname);

	~DBAdapter();

	bool Read(const std::string& key, std::string& data);
	bool Insert(const std::string& key, const std::string& data);
	bool Update(const std::string& key, const std::string& data);
	bool Delete();

	bool Read(std::vector<std::string>& result, size_t count);

private:
	char* zErrMsg = 0;

	const std::string dbname_;
	const std::string tblname_;
	const std::string datacolname_;

	sqlite3* db_;
	std::mutex dbmx_;
};

