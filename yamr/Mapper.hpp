#pragma once
#include "MapperContext.hpp"

template<class KEYIN,class VALUEIN,class KEYOUT, class VALUEOUT>
class Mapper
{
public:
	Mapper()=default;
	~Mapper()=default;
	void Map(KEYIN key,VALUEIN value, map_contex<KEYOUT,VALUEOUT>& context );
};
