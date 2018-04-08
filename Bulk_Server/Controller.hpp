#pragma once
#include "Utils.h"
#include "Loggers.hpp"
#include "Storage.hpp"
#include "Processors.hpp"

namespace bulk {
namespace server {
namespace internals{

/**
 * \brief Processes commands.
 */
class Controller
{
public:
	~Controller()=default;

	Controller();
	Controller(std::size_t _block_size);
	Controller(std::size_t _block_size,std::string&& ep);
	Controller(std::shared_ptr<Saver> _saver);

	void Process(std::string& _cmd);

	void Stop();

private:
	std::size_t block_size;
	opMode mode;

	std::string endpoint;
	std::unique_ptr<CommandProcessor> processor;

	//used for custom saver logic
	std::shared_ptr<Saver> saver;

	std::shared_ptr<CommandStorage> storage;

	struct Counters
	{
		int strings=0;
		int commands=0;
		int blocks=0;
	}cnt;

	void DumpCounters();
};

}
}
}
