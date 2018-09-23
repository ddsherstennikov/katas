#include <mutex>
#include <chrono>

#include "db_cache/db_cache.h"



// TODO: return by value or reference?
void DBCache::Read(const std::string& key, std::string& data)
{
	auto it = locks_.find(key);

	if (it != locks_.end()) // exists in cache
	{
		std::unique_lock<std::mutex> l(it->second, std::defer_lock);

		if (l.try_lock_for(timeout_))   // can be acquired within timeout
		{
			data = table_[key];
		}
		else                            // too busy
		{
			throw(std::runtime_error("cache too busy"));
		}
	}
	else if (1)/* if (db.get()) */
	{
		// get from DB
	}
	else
	{
		// insert into db
	}


}

void DBCache::Write(const std::string& key, const std::string& data)
{

}