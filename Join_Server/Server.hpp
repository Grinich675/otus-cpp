#pragma once

#include <boost/asio.hpp>
#include <cstddef>

#include "Client_manager.hpp"

using boost::asio::ip::tcp;
class Server
{
public:
  Server(const Server&) = delete;
  ~Server()=default;
  Server& operator=(const Server&) = delete;

  explicit Server(int port,std::size_t thread_pool_size);

  void run();

private:

  void do_accept();

  void do_await_stop();

  std::size_t thread_pool_size_;

  boost::asio::io_service io_context_;

  boost::asio::signal_set signals_;

  boost::asio::ip::tcp::acceptor acceptor_;

  client_manager client_manager_;

  /// The next socket to be accepted.
  boost::asio::ip::tcp::socket socket_;
};
