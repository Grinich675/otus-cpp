#pragma once

#include <map>
#include <memory>
#include <shared_mutex>
#include <string>
#include <utility>



class Table
{
public:
	using uniq_key_t = unsigned long;
	using row_t = std::string;//std::tuple<std::string>
	using table_t = std::map<uniq_key_t,row_t>;
	using table_pointer = std::shared_ptr<table_t>;

	Table();

	void Truncate();

	std::pair<std::shared_lock<std::shared_timed_mutex>,table_pointer> GetRows();

	bool Add(uniq_key_t key,row_t row);

private:
	std::shared_timed_mutex mut;
	table_pointer rows_coll;
};
