#pragma once
#include "Controller.hpp"
#include "Loggers.hpp"
#include <iterator>
#include <deque>

#include <atomic>
namespace bulk {
namespace server {

class ReqHandler
{
public:

	void receive(const char *data, std::size_t size);

	void Stop();

	ReqHandler()=delete;

	ReqHandler(std::size_t block_size);

	ReqHandler(std::size_t block_size,std::string ep);

	~ReqHandler();

private:
	internals::Controller ctrl;
	std::deque<std::string> _internal_storage;
	mutable std::mutex mut;
	bool stop_flag;
	static std::atomic<int> con_cnt;

	void form_n_process_string(const char* data,std::size_t count);

};

}
}
