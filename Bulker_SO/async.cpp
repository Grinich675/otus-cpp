#include "async.h"
#include "Connection.hpp"
#include <thread>
#include <mutex>
namespace async {

std::once_flag loggers_flag;
std::shared_ptr<ConsoleLogger> console;
std::shared_ptr<FileLogger> flog;

void InitLog()
{
	console = std::make_shared<ConsoleLogger>(1);
	flog = std::make_shared<FileLogger>(2);
}

handle_t connect(std::size_t bulk)
{
	std::call_once(loggers_flag,InitLog);

	auto _conn = new connection::Connection(bulk);
	_conn->AddLogger(console);
	_conn->AddLogger(flog);

    return _conn;
}

void receive(handle_t handle, const char *data, std::size_t size)
{
	auto _conn = reinterpret_cast<connection::Connection*> (handle);

	_conn->receive(data,size);
}

void disconnect(handle_t handle)
{
	auto _conn = reinterpret_cast<connection::Connection*> (handle);
	_conn->Stop();
	delete _conn;
}

}
