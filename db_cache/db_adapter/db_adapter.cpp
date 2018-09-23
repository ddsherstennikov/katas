#include "db_adapter/db_adapter.h"



DBAdapter::DBAdapter(const std::string& dbname, const std::string& tblname, const std::string& datacolname)
		: dbname_(dbname), tblname_(tblname), datacolname_(datacolname)
{
	int rc = sqlite3_open(dbname_.c_str(), &db_);
	if(rc)
	{
		sqlite3_close(db_);
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db_));
		throw(std::runtime_error("Can't open database."));
	}
}

DBAdapter::~DBAdapter()
{
	sqlite3_close(db_);
}



bool DBAdapter::Read(const std::string& key, std::string& data)
{
	std::string s_cmd = "select " + datacolname_ +
	                    " from " + tblname_ +
	                    " where key = \"" + key + "\";";

	auto inserter_lambda = [](void* vs, int n_cols, char** str_array, char** azColName)->int {
		auto s_ptr = reinterpret_cast<std::string*>(vs);
		s_ptr->append(str_array[0]);
		return 0;
	};

	int rc = sqlite3_exec(db_, s_cmd.c_str(), inserter_lambda, &data, &zErrMsg);

	if(rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);

		return false;
	}

	return true;
}



bool DBAdapter::Read(const std::string& key, std::vector<std::string>& result, size_t count)
{
	std::string s_cmd = "select " + datacolname_ +
						" from " + tblname_ +
						" where key = \"" + key +
						"\" limit " + std::to_string(count) + ";";

	auto inserter_lambda = [](void* vs, int n_cols, char** str_array, char** azColName)->int {
		auto vs_ptr = reinterpret_cast<std::vector<std::string>*>(vs);
		vs_ptr->emplace_back(str_array[0]);
		return 0;
	};

	int rc = sqlite3_exec(db_, s_cmd.c_str(), inserter_lambda, &result, &zErrMsg);

	if(rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);

		return false;
	}

	return true;
}



bool DBAdapter::Write(const std::string& key, const std::string& data)
{
	std::string s_cmd = "insert into " + tblname_ +
	                    " values ('" + key + "', '" + data + "')";

	int rc = sqlite3_exec(db_, s_cmd.c_str(), 0, 0, &zErrMsg);

	if(rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);

		return false;
	}

	return true;
}