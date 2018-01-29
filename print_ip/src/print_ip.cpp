#include "metaprinter.hpp"






int main()
{

	meta_ip_print(char(-1));

	meta_ip_print(short(0));

	meta_ip_print(int(2130706433));

	meta_ip_print(long(8875824491850138409));


	meta_ip_print(std::string{"192.168.0.1"});


	meta_ip_print(std::vector<int>{10,0,0,1,15,2});


	std::tuple<int,int,int,int,int,int> tup1 {1,2,3,4,5,6};
	meta_ip_print(tup1);

	std::tuple<std::string,float> tup2 {"10",3.14};
	meta_ip_print(tup2);

	int arr[10];
	meta_ip_print(arr);
	return 0;
}
