#include <boost/test/unit_test.hpp>

#include "filter_utils.h"

#include <iostream>

 ip_pool_t ip_pool;

static inline std::ostream& operator<<(std::ostream& os, const ip_type& ip)
{

	for(auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part)
            {
        		if (ip_part != ip.cbegin())
                {
        			std::cout << ".";
                }
                std::cout << *ip_part;
        	}
    return os;
}


namespace boost { 
	namespace test_tools 
	{
		namespace tt_detail
		{
			template<>           
			struct print_log_value<ip_type> 
			{
				void operator()( std::ostream& os, const ip_type& ts)
				{
				    ::operator<<(os,ts);
				}
			};  
		}                                                        
	}
}


// initialization function:
bool init_unit_test()
{
	std::string filename = "test_data.tsv";
	std::fstream s(filename,   s.in );

	//assert( (!s.is_open()) );

	for(std::string line; std::getline(s, line);)
	{
		tokenized_string v = split(line, '\t');
		ip_pool.push_back(split(v.at(0), '.'));
	}
	 s.close();

	std::sort(ip_pool.begin(),ip_pool.end(),reverse_lex_sorter);
}

// entry point:
int main(int argc, char* argv[])
{
  return boost::unit_test::unit_test_main( &init_unit_test, argc, argv );
}




namespace tt = boost::test_tools;



BOOST_AUTO_TEST_SUITE(test_split_suite)


// ("",  '.') -> [""]
BOOST_AUTO_TEST_CASE(test_split_empty)
{
	auto res = split(std::string(""),'.');
  	BOOST_TEST(res.size()==1);
  	BOOST_TEST(res.at(0)== std::string("") );
}

// ("11", '.') -> ["11"]
BOOST_AUTO_TEST_CASE(test_split_no_sep)
{
	auto res = split(std::string("11"),'.');
  	BOOST_TEST(res.size()==1);
  	BOOST_TEST(res.at(0)== std::string("11") );
}

// ("..", '.') -> ["", "", ""]
BOOST_AUTO_TEST_CASE(test_split_only_seps)
{
	tokenized_string reference_value ({"","",""});

	auto res = split(std::string(".."),'.');

  	BOOST_TEST(res==reference_value ,tt::per_element() );
}

// ("11.", '.') -> ["11", ""]
BOOST_AUTO_TEST_CASE(test_split_no_chars_after_seps)
{
	tokenized_string reference_value ({"11",""});

	auto res = split(std::string("11."),'.');

  	BOOST_TEST(res==reference_value ,tt::per_element() );
}

// (".11", '.') -> ["", "11"]
BOOST_AUTO_TEST_CASE(test_split_no_chars_before_seps)
{
	tokenized_string reference_value ({"", "11"});

	auto res = split(std::string(".11"),'.');

  	BOOST_TEST(res==reference_value ,tt::per_element() );
}

// ("11.22", '.') -> ["11", "22"]
BOOST_AUTO_TEST_CASE(test_split_regular)
{
	tokenized_string reference_value ({"11", "22"});

	auto res = split(std::string("11.22"),'.');

  	BOOST_TEST(res==reference_value ,tt::per_element() );
}

BOOST_AUTO_TEST_SUITE_END()



 BOOST_AUTO_TEST_SUITE(test_filter_suite)


BOOST_AUTO_TEST_CASE(filter_by_first_byte)
{
	ip_pool_t reference_value {{"1","231","69","33"},
							   {"1","87","203","225"},
							   {"1","70","44","170"},
							   {"1","29","168","152"},
							   {"1","1","234","8"}
								};

    ip_pool_t::const_iterator seq_beg_it,seq_end_it;

    std::tie(seq_beg_it, seq_end_it) = filter(ip_pool,std::string("1"));

    ip_pool_t res(seq_beg_it,seq_end_it) ;


  	BOOST_TEST(res==reference_value ,tt::per_element() );
}


BOOST_AUTO_TEST_CASE(filter_by_first_and_second_bytes)
{
	ip_pool_t reference_value {{"46","70","225","39"},
							   {"46","70","147","26"},
							   {"46","70","113","73"},
							   {"46","70","29","76"}
								};

    ip_pool_t::const_iterator seq_beg_it,seq_end_it;

    std::tie(seq_beg_it, seq_end_it) = filter(ip_pool,std::string("46"),std::string("70"));

    ip_pool_t res(seq_beg_it,seq_end_it) ;


  	BOOST_TEST(res==reference_value ,tt::per_element() );
}



 BOOST_AUTO_TEST_SUITE_END()

