#pragma once

#include <cstddef>
#include <deque>
#include <memory>
#include <string>

#include "DBMS/Commands.hpp"

class DB;

class client;

class reqHandler
{
public:
	void receive(const char *data, std::size_t size,std::shared_ptr<DB>, client* client);
	reqHandler()=default;

	~reqHandler()=default;

private:
	std::deque<std::string> _internal_storage;
	CMDCreator crt;
	void form_n_process_string(const char* data,std::size_t count,std::shared_ptr<DB>,client* client);
};
