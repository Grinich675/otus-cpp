#include "Server.hpp"

#include <memory>
#include <vector>
#include <thread>
#include <iostream>


Server::Server(int port,std::size_t thread_pool_size):
thread_pool_size_(thread_pool_size),
io_context_(BOOST_ASIO_CONCURRENCY_HINT_SAFE),
signals_(io_context_),
acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)),
client_manager_()
{
	signals_.add(SIGINT);
	signals_.add(SIGTERM);
#if defined(SIGQUIT)
	signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)

	do_await_stop();

	//set rcv buffer 8 mb
	acceptor_.set_option(boost::asio::socket_base::receive_buffer_size(8*1024*1024));
	acceptor_.set_option(boost::asio::socket_base::keep_alive(true));

	if(boost::asio::socket_base::max_listen_connections < 1024)
		std::cerr<<"Warning! Low max listen connections: "<<boost::asio::socket_base::max_listen_connections<<std::endl;

	do_accept();
}

void Server::run()
{
  // Create a pool of threads to run all of the io_contexts.
  std::vector<std::shared_ptr<std::thread> > threads;
  for (std::size_t i = 0; i < thread_pool_size_; ++i)
  {
    std::shared_ptr<std::thread> thread(new std::thread( [this](){io_context_.run();} ));
    threads.push_back(thread);
  }

  // Wait for all threads in the pool to exit.
  for (std::size_t i = 0; i < threads.size(); ++i)
    threads[i]->join();
}

void Server::do_accept()
{
	acceptor_.async_accept(socket_,
	[this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
	{
		if (!acceptor_.is_open())
		{
			return;
		}

		if (!ec)
		{
			client_manager_.start(std::make_shared<client>(
					io_context_,client_manager_,std::move(socket)));
		}

		do_accept();
	});
}

void Server::do_await_stop()
{
	signals_.async_wait(
			[this](boost::system::error_code, int )
			{
				acceptor_.close();
				client_manager_.stop_all();
			});
}
