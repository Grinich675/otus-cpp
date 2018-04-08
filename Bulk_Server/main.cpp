//#include "async.h"

#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "Loggers.hpp"
#include "server.hpp"

using namespace bulk::server;
int main(int argc, char* argv[])
{
	std::size_t block_size=5;
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
		block_size = std::stoi(std::string(argv[2]));
	}

	internals::Saver::GetDefaultSaver()->AddLogger(std::make_shared<internals::ConsoleLogger>(1));
	internals::Saver::GetDefaultSaver()->AddLogger(std::make_shared<internals::FileLogger>(2));

	try
	  {
	    // Initialise the server.
	    server s(listen_port,block_size);

	    // Run the server until stopped.
	    s.run();
	  }
	  catch (std::exception& e)
	  {
	    std::cerr << "exception: " << e.what() << "\n";
	  }

  return 0;
}

