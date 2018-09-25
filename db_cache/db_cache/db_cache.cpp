#include <mutex>
#include <chrono>
#include <thread>

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "db_cache/db_cache.h"



DBCache::DBCache(const std::string& dbname, const std::string& tblname, const std::string& datacolname)
: dba_(dbname, tblname, datacolname), timer_(io_)
{
	timer_.expires_from_now(boost::posix_time::seconds(1));
	timer_.async_wait(boost::bind(&DBCache::sync, this, boost::asio::placeholders::error));

	io_thread_ = std::thread( [this](){ io_.run(); } );
}

DBCache::~DBCache()
{
	io_.stop();

	io_thread_.join();

	sync(boost::system::error_code());
}



bool DBCache::Read(const std::string& key, std::string& req_data)
{
	boost::shared_lock<boost::shared_mutex> sl(smx_);

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
		dba_.Insert(key, req_data);

		locks_.emplace(key, std::make_unique<std::timed_mutex>());
		table_.emplace(key, req_data);

		return true;
	}
}



bool DBCache::Write(const std::string& key, const std::string& data)
{
	boost::upgrade_lock<boost::shared_mutex> usl(smx_);

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
		boost::upgrade_to_unique_lock<boost::shared_mutex> ul(usl);

		locks_.emplace(key, std::make_unique<std::timed_mutex>());
		table_.emplace(key, data);
		changes_created_.insert(key);

		return true;
	}
}



void DBCache::sync(const boost::system::error_code& ec)
{
	if (ec == boost::asio::error::operation_aborted)
		return;
	else if (ec)
		boost::asio::detail::throw_error(ec);

	boost::unique_lock<boost::shared_mutex> ul(smx_);

	for (const std::string& modified_key : changes_modified_)
	{
		dba_.Update(modified_key, table_[modified_key]);
	}

	for (const std::string& created_key : changes_created_)
	{
		dba_.Insert(created_key, table_[created_key]);
	}

	if (!io_.stopped())     // restart timer
	{
		timer_.expires_from_now(boost::posix_time::seconds(1));
		timer_.async_wait(boost::bind(&DBCache::sync, this, boost::asio::placeholders::error));
	}
}