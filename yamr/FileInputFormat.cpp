#include "FileInputFormat.hpp"

#include <fstream>
#include <algorithm>

const std::size_t FileInputFormat::block_size =4096;

FileInputFormat::FileInputFormat(JobConf& conf):
	files{},split_size(0)
{
	//copy only existing files
	for (auto file :conf.input_files)
	{
		if(std::ifstream(file))
		{
			files.emplace_back(file);
		}
	}

	//determine split size
	split_size=std::max(conf.min_split_size, std::min(conf.max_split_size, FileInputFormat::block_size));
}

std::list<FileSplit> FileInputFormat::getSplits()
{
	std::list<FileSplit> splits{};

	for(auto file: files)
	{
		std::ifstream in(file, std::ifstream::ate | std::ifstream::binary);
		std::size_t file_size = in.tellg();
		in.close();
		std::size_t curr_offset = 0 ;

		while(curr_offset<file_size)
		{
			splits.emplace_back(file,curr_offset,split_size);
			curr_offset+=split_size;
		}
	}

	return splits;
}

