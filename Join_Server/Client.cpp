#include "Client.hpp"

#include <boost/asio/basic_socket.hpp>
#include <boost/asio/basic_stream_socket.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>

#include "Client_manager.hpp"




client::client(boost::asio::io_context& io_context,client_manager& manager,tcp::socket soc)
: client_manager_(manager),
  socket_(std::move(soc)),
  strand_(io_context),
  handler(),
  db_ptr{DBMS::GetDB("master")},
  mut{},
  write_in_progress(false),
  results{}
{
}

void client::start()
{
	do_read();
}


void client::stop()
{
	socket_.close();
}

void client::do_read()
{
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(data_, max_length),
			[this, self](boost::system::error_code ec, std::size_t length)
			{
				if(!ec)
				{
					handler.receive(data_,length,db_ptr,this);
					do_read();
				}
				else if (ec != boost::asio::error::operation_aborted)
				{
					client_manager_.stop(shared_from_this());
				}
			});
}

void client::send(std::string result_row)
{
	{
		std::lock_guard<std::mutex> lk {mut};
		results.emplace_back(std::move(result_row));
		if(!write_in_progress)
		{
			write_in_progress=true;
			do_write();
		}
	}
}

void client::do_write()
{
	auto self(shared_from_this());
	socket_.async_write_some(boost::asio::buffer(results.front().data(), results.front().length()),
			boost::asio::bind_executor(strand_,
			[this, self](boost::system::error_code ec,std::size_t )
			{
				if(!ec)
				{
					std::lock_guard<std::mutex> lk{mut};

					results.pop_front();

					if(results.size()>0)
					{
						do_write();

					}
					else
					{
						write_in_progress=false;
					}
				}
				else if (ec != boost::asio::error::operation_aborted)
				{
					std::lock_guard<std::mutex> lk{mut};
					write_in_progress=false;
				}
			}));
}





