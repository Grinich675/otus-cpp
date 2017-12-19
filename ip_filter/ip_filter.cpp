#include <iostream>
#include "filter_utils.h"


int main(int , char const **)
{

	try
    {
        ip_pool_t ip_pool;


        for(std::string line; std::getline(std::cin, line);)

        {
            tokenized_string v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

        //
        auto print_ip = [](ip_pool_t::const_iterator ip )
        {
        	for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
            {
        		if (ip_part != ip->cbegin())
                {
        			std::cout << ".";
                }
                std::cout << *ip_part;
        	}
        };

        //using auto in params type only for feature demonstration
        auto print_ip_range = [&print_ip=print_ip](auto const_it_begin,auto const_it_end)
        {
        	for(;const_it_begin!=const_it_end;++const_it_begin)
        	{
        		print_ip(const_it_begin);
        		std::cout << std::endl;
        	}
         };

        // TODO reverse lexicographically sort
        std::sort(ip_pool.begin(),ip_pool.end(),reverse_lex_sorter);
        print_ip_range(ip_pool.cbegin(),ip_pool.cend());

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8



        ip_pool_t::const_iterator seq_beg_it,seq_end_it;
        // TODO filter by first byte and output
        // ip = filter(1)


        std::tie(seq_beg_it, seq_end_it) = filter(ip_pool,std::string("1"));
        print_ip_range(seq_beg_it,seq_end_it);


        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        std::tie(seq_beg_it, seq_end_it) = filter(ip_pool,std::string("46"),std::string("70"));
        print_ip_range(seq_beg_it,seq_end_it);

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter(46)
        auto results= search_for_bytes(ip_pool,46);
        for (auto ip : results)
        {
        	print_ip(ip);
        	std::cout<<std::endl;
        }
        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
