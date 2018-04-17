#include "server.hpp"

namespace bulk {
namespace server {

server::~server()
{
	  GlobalStorage::ClearStorage();
	  internals::Saver::GetDefaultSaver()->RemoveAll();
}

server::server(int port,std::size_t bulk_size):
io_context_(1),
socket_(io_context_),
signals_(io_context_),
acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)),
sess_manager_()
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

	/*if(boost::asio::socket_base::max_listen_connections < 1024)
		std::cerr<<"Warning! Low max listen connections: "<<boost::asio::socket_base::max_listen_connections<<std::endl;*/

	do_accept(bulk_size);
}

void server::run()
{
	io_context_.run();
}

void server::do_accept(std::size_t bulk_size)
{
	acceptor_.async_accept(socket_,
			[this,bulk_size](boost::system::error_code ec)
			{
		if (!acceptor_.is_open())
		{
			return;
		}

		if (!ec)
		{
			sess_manager_.start(std::make_shared<session>(
					sess_manager_,std::move(socket_,),bulk_size));
		}

		do_accept(bulk_size);
			});
}

void server::do_await_stop()
{
	signals_.async_wait(
			[this](boost::system::error_code, int )
			{
				acceptor_.close();
				sess_manager_.stop_all();
			});
}

}
}
