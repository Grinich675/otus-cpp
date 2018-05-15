#pragma once
#include "FileSplit.hpp"
#include <fstream>

class RecordReader
{
public:
	~RecordReader()=default;
	RecordReader()=delete;

	RecordReader(FileSplit _split);

	long getCurrentKey();
	std::string getCurrentValue();
	bool nextKeyValue();

protected:
	FileSplit split;
	std::fstream file;

	long curr_key;
	std::string curr_value;
};
