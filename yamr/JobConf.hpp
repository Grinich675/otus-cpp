#pragma once
#include <string>
#include <list>

enum class SortOrder_t
{
	ascending,
	descending
};

struct JobConf
{
	std::size_t num_mappers;
	std::size_t num_reducers;
	std::size_t min_split_size;
	std::size_t max_split_size;
	SortOrder_t sort_order;
	std::string job_name;
	std::list<std::string> input_files;

	JobConf():
		num_mappers{1},num_reducers{1},min_split_size{1},max_split_size{0},
		sort_order{SortOrder_t::ascending},job_name("yamr_job"),input_files{}
	{}
};
