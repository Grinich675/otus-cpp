#include "filter_utils.h"

auto split(const std::string &str, char d)-> tokenized_string
{
	tokenized_string r;

    std::string::size_type start = 0;
    auto stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

auto search_byte(ip_pool_t& ip_pool, ip_type::value_type b)-> std::vector<ip_pool_t::const_iterator>
{

	std::vector<ip_pool_t::const_iterator> ret;


	auto byte_check = [&b](const ip_type::value_type& ip_part)
			{
				return ip_part == b;
			};


	for(auto ip = ip_pool.cbegin(); ip!=ip_pool.cend(); ++ip)
	{
		if (std::any_of(ip->cbegin(), ip->cend(),byte_check))
		{
			ret.push_back(ip);
		}

	}

	return ret;
}
