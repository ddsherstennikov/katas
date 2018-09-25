#pragma once

#include <string>
#include <unordered_map>
#include <chrono>
#include <mutex>
#include <set>
#include <thread>

#include <boost/thread/shared_mutex.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio.hpp>

#include "db_adapter/db_adapter.h"



class DBCache
{
public:
	DBCache(const std::string& dbname, const std::string& tblname, const std::string& datacolname);
	~DBCache();

	bool Read(const std::string& key, std::string& req_data);
	bool Write(const std::string& key, const std::string& data);
	void sync(const boost::system::error_code& ec);

private:
	mutable std::unordered_map<std::string, std::unique_ptr<std::timed_mutex>> locks_;
	std::unordered_map<std::string, std::string> table_;

	DBAdapter dba_;

	boost::shared_mutex smx_;
	std::set<std::string> changes_modified_;
	std::set<std::string> changes_created_;

	boost::asio::io_service io_;
	boost::asio::deadline_timer timer_;
	std::thread io_thread_;

	const std::chrono::duration<int> timeout_ = std::chrono::seconds(1);
};