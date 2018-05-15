#include "FileOutputFormat.hpp"

FileOutputFormat::FileOutputFormat(JobConf& Config):
	base_output_name(Config.job_name),out_dir("")
	{}

RecordWriter FileOutputFormat::getRecordWriter(std::size_t part)
{
	auto fname = base_output_name+"_"+std::to_string(part);
	std::fstream f(fname,std::ios_base::out|std::ios_base::trunc);

	return RecordWriter(std::move(f));
}
