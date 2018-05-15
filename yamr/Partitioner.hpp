#pragma once

template <class KEY,class VALUE>
class Partitioner
{
public:
	Partitioner()=default;
	~Partitioner()=default;
	int getPartition(KEY key, VALUE value, int numPartitions)
	{
		return key%numPartitions;
	}
};
