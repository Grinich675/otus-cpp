#include "Session.hpp"

#include <boost/system/error_code.hpp>
#include <algorithm>
#include <string>

#include "Session_manager.hpp"

namespace bulk {
namespace server {

session::session(session_manager& manager,tcp::socket socket,std::size_t block_size)
: ses_manager_(manager),
  socket_(std::move(socket)),
  conn(block_size,
		  socket_.remote_endpoint().address().to_string()+std::to_string(socket_.remote_endpoint().port()))
{
}

void session::start()
{
	do_read();
}


void session::stop()
{
	socket_.close();
	conn.Stop();
}

void session::do_read()
{
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(data_, max_length),
			[this, self](boost::system::error_code ec, std::size_t length)
			{
				if(!ec)
				{
					conn.receive(data_,length);
					do_read();
				}
				else if (ec != boost::asio::error::operation_aborted)
				{

					ses_manager_.stop(shared_from_this());
				}
			});
}

}
}

