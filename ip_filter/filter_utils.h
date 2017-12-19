#pragma once

#include <string>
#include <vector>
#include <cassert>

#include <algorithm>
#include <functional>
#include <array>
#include <tuple>

#include <fstream>

#include <type_traits>
/*
 * aliases.
 * alias for ip and tokenized string are the same,
 * but ip type defined for future proper usage (like uint).
*/
using ip_type = std::vector<std::string>;
using ip_pool_t = std::vector<ip_type>;

using tokenized_string = std::vector<std::string>;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
auto split(const std::string &str, char d)-> tokenized_string;


//Lambda for sorting elements in ip_pool in reverse lexicographical order
// considering that ip consists of 4 numbers(ints), not 4 string with numbers
auto reverse_lex_sorter = [](const ip_type& a , const ip_type& b)
{
	for(auto byte_a = a.cbegin(), byte_b = b.cbegin();byte_a != a.cend();++byte_a,++byte_b)
	{

		if( stoi(*byte_a) < stoi(*byte_b))
			return false;
		else if (stoi(*byte_a) > stoi(*byte_b))
			return true;
	}
			return true;
};


template <typename ...Ts,typename = std::common_type_t<Ts...>>
auto filter ( ip_pool_t& ip_pool, Ts... Args)
{

	static_assert( (std::is_same<std::string,typename std::common_type<Ts ...>::type >::value),"bad news, filter accepts only std::string patterns" ) ;

	ip_pool_t bytes{};

	//Lambda comparator for searching all bytes 'b' in ip 'a' at appropriate positions
	auto fixed_bytes_eq = [](const ip_type& a, const ip_type& b)
			{
					for(auto byte_a = a.cbegin(), byte_b = b.cbegin();byte_b != b.cend();++byte_a,++byte_b)
					{
						if( *byte_a != *byte_b)
							return false;
					}
				return true;
			};

	//Lambda comparator for searching first element in collection, which does not contains bytes 'b' at appropriate positions
	auto fixed_bytes_not_eq=[](const ip_type& a, const ip_type& b)
					{
							for(auto byte_a = a.cbegin(), byte_b = b.cbegin();byte_b != b.cend();++byte_a,++byte_b)
							{
								if( *byte_a != *byte_b)
									return true;
							}
						return false;
					};


	bytes.push_back({(std::move(Args))...});

	//find first element in pool that meets criteria (has given bytes)
	auto seq_beg_it = std::search(ip_pool.cbegin(),ip_pool.cend(),bytes.begin(),bytes.end(),fixed_bytes_eq);

	//find first element in subsequence [seq_beg_it, pool.cend]  that meets criteria (does not have given bytes)
	auto seq_end_it= std::search(seq_beg_it,ip_pool.cend(),bytes.begin(),bytes.end(),fixed_bytes_not_eq);

	//thus pool is sorted, subsequence [seq_beg_it,seq_end_it] contains
	//ips in reverse lexicographical order having given bytes

	return std::make_tuple(seq_beg_it,seq_end_it);
}

auto search_byte(ip_pool_t& ip_pool, ip_type::value_type b)-> std::vector<ip_pool_t::const_iterator>;

//Template function for searching IP containing given bytes in int representation
template <typename ...Ts,typename = std::common_type_t<Ts...>>
auto  search_for_bytes ( ip_pool_t& ip_pool, Ts... Args)-> std::vector<ip_pool_t::const_iterator>
{

	static_assert( (std::is_same<int,typename std::common_type<Ts ...>::type >::value),"bad news, filter accepts only int patterns" ) ;

	std::vector<ip_pool_t::const_iterator> ret;


	std::array<decltype(ret),sizeof...(Args)> dummy = { ( search_byte(ip_pool,std::to_string(Args)))... };

    for(auto subseq: dummy)
    {
    	ret.insert(ret.end(),subseq.begin(),subseq.end());
    }


    return ret;
}

