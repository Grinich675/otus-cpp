#pragma once

#include "JobConf.hpp"
#include "RecordWriter.hpp"


class FileOutputFormat
{
public:
	FileOutputFormat()=delete;
	~FileOutputFormat()=default;

	FileOutputFormat(JobConf& Config);

	RecordWriter getRecordWriter(std::size_t part);

private:
	std::string base_output_name;
	std::string out_dir;
};
