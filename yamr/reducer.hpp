#pragma once
#include <iterator>
#include <list>
#include "ReducerContext.hpp"

template <class KEYIN,class VALUEIN,class KEYOUT,class VALUEOUT>
class Reducer
{
public:
	~Reducer()=default;
	void reduce(KEYIN key, std::list<VALUEIN>& values, reduce_contex<KEYIN,VALUEIN,KEYOUT,VALUEOUT>& context);
private:
};
