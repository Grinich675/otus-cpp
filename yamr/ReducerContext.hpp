#pragma once

#include <map>
#include <list>
#include "RecordWriter.hpp"

template <class INKEY,class INVALUE,
		  class OUTKEY,class OUTVALUE>
class reduce_contex
{
public:
	reduce_contex()=default;

	void add(INKEY k,INVALUE val)
	{
		in_collection[k].push_back(val);
	}

	void add(INKEY k,INVALUE&& val)
	{
		in_collection[k].emplace_back(std::forward(val));
	}

	void write(OUTKEY k,OUTVALUE val)
	{
		out_collection.emplace(k, val);
	}

	void write(RecordWriter& writer)
	{
		for(auto& reduce_out:out_collection)
		{
			writer.write(reduce_out.first,reduce_out.second);
		}

		out_collection.clear();
	}

	auto begin()
	{
		return in_collection.begin();
	}

	auto end()
	{
		return in_collection.end();
	}

private:
	 std::multimap<OUTKEY, OUTVALUE> out_collection;
	 std::map<INKEY,std::list<INVALUE>> in_collection;
};
