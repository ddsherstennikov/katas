#include <mutex>
#include <chrono>

#include "db_cache/db_cache.h"



DBCache::DBCache(const std::string& dbname, const std::string& tblname, const std::string& datacolname)
		: dba_(dbname, tblname, datacolname), timer_(io, std::chrono::seconds(30))
{
	timer_.async_wait(boost::bind(&DBCache::sync, this));

	io.run();
}



bool DBCache::Read(const std::string& key, std::string& req_data)
{
	boost::shared_lock<boost::shared_mutex> shared_lock(smx_);

	auto it = locks_.find(key);

	if (it != locks_.end())             // exists in cache
	{
		std::unique_lock<std::timed_mutex> l(*it->second, std::defer_lock);

		if (l.try_lock_for(timeout_))   // can be acquired within timeout
		{
			req_data = table_[key];

			return true;
		}
		else                            // too busy
		{
			throw(std::runtime_error("cache too busy"));
		}
	}
	else if (dba_.Read(key, req_data))      // get from DB
	{
		locks_.emplace(key, std::make_unique<std::timed_mutex>());
		table_.emplace(key, req_data);

		return true;
	}
	else                                // put new into db
	{
		req_data = "";
		dba_.Write(key, req_data);

		locks_.emplace(key, std::make_unique<std::timed_mutex>());
		table_.emplace(key, req_data);

		return true;
	}
}



bool DBCache::Write(const std::string& key, const std::string& data)
{
	boost::upgrade_lock<boost::shared_mutex> shared_lock(smx_);

	auto it = locks_.find(key);

	if (it != locks_.end()) // exists in cache
	{
		std::unique_lock<std::timed_mutex> l(*it->second, std::defer_lock);

		if (l.try_lock_for(timeout_))   // can be acquired within timeout
		{
			table_[key] = data;
			changes_modified_.insert(key);

			return true;
		}
		else                            // too busy
		{
			throw(std::runtime_error("cache too busy"));
		}
	}
	else                                // add new record
	{
		boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(shared_lock);

		locks_.emplace(key, std::make_unique<std::timed_mutex>());
		table_.emplace(key, data);
		changes_created_.insert(key);

		return true;
	}
}



void DBCache::sync()
{
	boost::unique_lock<boost::shared_mutex> smx;

	for (const std::string& modified_key : changes_modified_)
	{
		dba_.Write(modified_key, table_[modified_key]);
	}

	for (const std::string& created_key : changes_created_)
	{
		dba_.Write(created_key, table_[created_key]);
	}

	timer_.expires_at(timer_.expires_at() + std::chrono::seconds(1));
	timer_.async_wait(boost::bind(&DBCache::sync, this));                   // restart timer
}