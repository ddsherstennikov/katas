#include "db_adapter/db_adapter.h"



DBAdapter::DBAdapter(const std::string& dbname, const std::string& tblname, const std::string& datacolname)
		: dbname_(dbname), tblname_(tblname), datacolname_(datacolname)
{
	int rc = sqlite3_open_v2(dbname_.c_str(), &db_, SQLITE_OPEN_READWRITE, NULL);
	if(rc)
	{
		sqlite3_close(db_);
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db_));
		throw(std::runtime_error("Can't open database."));
	}

//	std::cout << "sqlite3_threadsafe" << sqlite3_threadsafe() << std::endl;
//	std::cout << "mutex not NULL? " << ((sqlite3_db_mutex(db_) != NULL) ? "y" : "n") << std::endl;

}

DBAdapter::~DBAdapter()
{
	std::lock_guard<std::mutex> lg(dbmx_);

	sqlite3_close(db_);
}



bool DBAdapter::Read(const std::string& key, std::string& data)
{
	int rc;
	std::string s_cmd = "select " + datacolname_ +
	                    " from " + tblname_ +
	                    " where key = \"" + key + "\";";

	auto inserter_lambda = [](void* vs, int n_cols, char** str_array, char** azColName)->int {
		auto s_ptr = reinterpret_cast<std::string*>(vs);
		s_ptr->append(str_array[0]);
		return 0;
	};

	{
		std::lock_guard<std::mutex> lg(dbmx_);
		rc = sqlite3_exec(db_, s_cmd.c_str(), inserter_lambda, &data, &zErrMsg);
	}

	if(rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);

		return false;
	}

	return true;
}

bool DBAdapter::Read(std::vector<std::string>& result, size_t count)
{
	int rc;
	std::string s_cmd = "select " + datacolname_ +
						" from " + tblname_ +
						" limit " + std::to_string(count) + ";";

	auto inserter_lambda = [](void* vs, int n_cols, char** str_array, char** azColName)->int {
		auto vs_ptr = reinterpret_cast<std::vector<std::string>*>(vs);
		vs_ptr->emplace_back(str_array[0]);
		return 0;
	};

	{
		std::lock_guard<std::mutex> lg(dbmx_);
		rc = sqlite3_exec(db_, s_cmd.c_str(), inserter_lambda, &result, &zErrMsg);
	}

	if(rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);

		return false;
	}

	return true;
}

bool DBAdapter::Insert(const std::string& key, const std::string& data)
{
	int rc;
	std::string s_cmd = "insert or ignore into " + tblname_ +
						" values ('" + key + "', '" + data + "');";

	{
		std::lock_guard<std::mutex> lg(dbmx_);
		rc = sqlite3_exec(db_, s_cmd.c_str(), 0, 0, &zErrMsg);
	}

	if(rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);

		return false;
	}

	return true;
}

bool DBAdapter::Update(const std::string& key, const std::string& data)
{
	int rc;
	std::string s_cmd = "update " + tblname_ + " set data='" + data + "' where key='" + key + "';";

	{
		std::lock_guard<std::mutex> lg(dbmx_);
		rc = sqlite3_exec(db_, s_cmd.c_str(), 0, 0, &zErrMsg);
	}

	if(rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);

		return false;
	}

	return true;
}


bool DBAdapter::Delete()
{
	int rc;
	std::string s_cmd = "delete from " + tblname_ + ";";

	{
		std::lock_guard<std::mutex> lg(dbmx_);
		rc = sqlite3_exec(db_, s_cmd.c_str(), 0, 0, &zErrMsg);
	}

	if(rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);

		return false;
	}

	return true;
}