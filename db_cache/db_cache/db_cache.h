#pragma once

#include <string>
#include <unordered_map>
#include <chrono>
#include <mutex>
#include <set>

#include <boost/thread/shared_mutex.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "db_adapter/db_adapter.h"



class DBCache
{
//	struct Record
//	{
//		std::string key;
//		std::string data;
//		std::timed_mutex timed_mx;
//	};

public:
	DBCache(const std::string& dbname, const std::string& tblname, const std::string& datacolname);

	bool Read(const std::string& key, std::string& req_data);
	bool Write(const std::string& key, const std::string& data);

private:
	void sync();

private:
	mutable std::unordered_map<std::string, std::unique_ptr<std::timed_mutex>> locks_;
	std::unordered_map<std::string, std::string> table_;

	DBAdapter dba_;

	boost::shared_mutex smx_;
	std::set<std::string> changes_modified_;
	std::set<std::string> changes_created_;

	boost::asio::io_service io;
	boost::asio::steady_timer timer_;

	const std::chrono::duration<int> timeout_ = std::chrono::seconds(1);
};