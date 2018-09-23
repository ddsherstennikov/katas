#pragma once

#include <string>
#include <unordered_map>
#include <atomic>
#include <condition_variable>
#include <chrono>

#include "db_adapter/db_adapter.h"



class DBCache
{
public:
	DBCache(const std::string& dbname, const std::string& tblname, const std::string& datacolname)
	: dba_(dbname, tblname, datacolname)
	{}

	void Read(const std::string& key, std::string& data);
	void Write(const std::string& key, const std::string& data);

private:
	std::unordered_map<std::string, std::mutex> locks_;
	std::unordered_map<std::string, std::string> table_;

	DBAdapter dba_;

	const std::chrono::duration<int> timeout_ = std::chrono::seconds(1);
};