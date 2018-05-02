#pragma once
#include "RequestHandler.hpp"

#include <deque>
#include <mutex>
#include <boost/asio.hpp>

#include "DBMS/DBMS.hpp"

class client_manager;

using boost::asio::ip::tcp;

class client
		: public std::enable_shared_from_this<client>
{
public:
	client()=delete;
	client(const client&) = delete;
	client& operator=(const client&) = delete;

	client(boost::asio::io_service& io_context,client_manager& manager,tcp::socket soc);

	void start();

	void stop();

	void send(std::string result_row);

private:
	void do_read();
	void do_write();


	client_manager& client_manager_;

	tcp::socket socket_;
	boost::asio::io_service::strand strand_;

	reqHandler handler;

	std::shared_ptr<DB> db_ptr;

	mutable std::mutex mut;
	bool write_in_progress;
	std::deque<std::string> results;

	enum { max_length = 1024 };
	char data_[max_length];

};
