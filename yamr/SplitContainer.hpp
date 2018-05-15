#pragma once
#include <mutex>
#include "JobConf.hpp"
#include "FileInputFormat.hpp"

class SplitContainer
{
public:
	SplitContainer()=default;
	~SplitContainer()=default;

	void Init(JobConf& config)
	{
		FileInputFormat format(config);
		splits = std::move(format.getSplits());
		curr = splits.begin();
	}

	bool nextSplit(FileSplit& split)
	{
		if(curr!= splits.end())
		{
			std::lock_guard<std::mutex> lk{mut};
			split=*curr;
			std::advance(curr,1);
		}
		else
		{
			return false;
		}

		return true;
	}


private:
	std::list<FileSplit> splits;
	std::list<FileSplit>::iterator curr;
	std::mutex mut;
};
