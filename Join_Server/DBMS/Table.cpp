#include "Table.hpp"

#include <iostream>
#include <typeinfo>

Table::Table():
mut(),rows_coll(new table_t())
{
}

void Table::Truncate()
{
	std::lock_guard<std::shared_timed_mutex> exclk (mut);
	rows_coll.reset(new table_t());
}

std::pair<std::shared_lock<std::shared_timed_mutex>,Table::table_pointer> Table::GetRows()
{
	std::shared_lock<std::shared_timed_mutex> shlk(mut);
	return std::make_pair(std::move(shlk),rows_coll);
}


bool Table::Add(uniq_key_t key,row_t row)
{
	std::lock_guard<std::shared_timed_mutex> exclk (mut);

	auto search = rows_coll->find(key);
	if(search!= rows_coll->end())
	{
		//already has row with same key
		return false;
	}
	else
	{
		rows_coll->emplace(key,row);
	}
	return true;
}

