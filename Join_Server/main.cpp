#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "DBMS/DB.hpp"
#include "DBMS/DBMS.hpp"
#include "Server.hpp"


int main(int argc, char* argv[])
{
	std::size_t pool_size=4;
	ushort listen_port = 5555;

	if(argc > 1)
	{
		auto port_int = std::stoi(std::string(argv[1]));
		if(port_int > 65535 || port_int < 1)
		{
			std::cerr<<"Wrong port number! ["<<port_int<<"]\n";
			return 1;
		}
		listen_port = static_cast<ushort>(port_int);
	}

	if(argc > 2)
	{
		pool_size = std::stoi(std::string(argv[2]));
	}

	auto table_creator = [](std::shared_ptr<DB> db,std::string name)
		{
			if(db->AddTable(name))
			{
				std::cout<<"OK\t"<< "Table "<<name<<"  created successfully\n";
			}
			else
			{
				std::cout<<"ERR\t"<<"Table "<<name<<"\talready exists\n";
			}
		};

	auto db_ptr = DBMS::GetDB("master");
	table_creator(db_ptr,"A");
	table_creator(db_ptr,"B");

	try
	  {
	    // Initialise the server.
	    Server s(listen_port,pool_size);

	    // Run the server until stopped.
	    s.run();
	  }
	  catch (std::exception& e)
	  {
	    std::cerr << "exception: " << e.what() << "\n";
	  }

  return 0;
}
