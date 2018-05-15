#pragma once
#include <fstream>


class RecordWriter
{
public:

	~RecordWriter()=default;
	RecordWriter(RecordWriter&&)=default;
	RecordWriter(RecordWriter&)=delete;
	RecordWriter()=delete;
	RecordWriter& operator=( const RecordWriter& ) = delete;

	RecordWriter(std::fstream&& out_stream):
		out(std::move(out_stream))
	{
	}

	template <class K,class V>
	void write(K key, V value)
	{
		if(out.is_open())
		{
			out<<key<<"\t"<<value<<std::endl;
		}
	}


private:
	std::fstream out;
};
