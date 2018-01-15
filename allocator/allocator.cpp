#include <iostream>
#include <map>

#include "custom_allocator.hpp"
#include "custom_list.hpp"


constexpr int factorial(int x)
{
    return x == 0 ? 1 : x * factorial(x-1);
}

template <typename T,size_t size=10>
void fill_map(T& map)
{
	for(int idx=0;idx<size;++idx)
	{
		map.insert(std::make_pair(idx,factorial(idx)));
	}
}

template<class Ch, class Tr, class K, class V>
decltype(auto) operator<<(std::basic_ostream<Ch, Tr>& os,const std::pair<K,V>& t)
{
    os<<t.first<<" "<<t.second;
    return os << std::endl;
}


int main()
{

	auto regular_map = std::map<int,int>();
	fill_map<decltype(regular_map),10>(regular_map);

	auto custom_map =std::map<int,int,std::less<int>,custom_allocator<std::pair<const int,int>,5>>();
	fill_map<decltype(custom_map),10>(custom_map);

	for(auto node:custom_map)
		std::cout<<node;

	custom_list<int> stl_alloc_list;

	for(int i=0;i<10;++i)
		stl_alloc_list.push_back(i);

	custom_list<int,custom_allocator<int,10>> custom_alloc_list;

	for(int i=0;i<10;++i)
		custom_alloc_list.push_back(i);


	for(auto it:custom_alloc_list)
	{
		std::cout<<it<<std::endl;
	}


	return 0;
}


