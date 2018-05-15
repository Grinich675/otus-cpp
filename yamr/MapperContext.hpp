#pragma once
#include <map>
#include <functional>

template <class KEY,class VALUE>
class map_contex
{
public:
	map_contex()=default;
	void write(KEY k,VALUE val)
	{
		collection.emplace(k, val);
	}

	auto begin()
	{
		return collection.begin();
	}

	auto end()
	{
		return collection.end();
	}

private:
	 std::multimap<KEY, VALUE> collection;
};
