#pragma once

#include "JobConf.hpp"
#include "FileSplit.hpp"
#include <list>



class FileInputFormat
{
public:
	FileInputFormat()=delete;
	~FileInputFormat()=default;

	FileInputFormat(JobConf& conf);

	std::list<FileSplit> getSplits();

private:
	static const std::size_t block_size;
	std::list<std::string> files;
	std::size_t split_size;
};

