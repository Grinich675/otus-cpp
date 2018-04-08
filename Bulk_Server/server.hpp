#pragma once

#include <boost/asio.hpp>
#include <cstddef>

#include "Session_manager.hpp"

using boost::asio::ip::tcp;

namespace bulk {
namespace server {

class server
{
public:
  server(const server&) = delete;
  ~server();
  server& operator=(const server&) = delete;

  explicit server(int port,std::size_t bulk_size);

  void run();

private:

  void do_accept(std::size_t bulk_size);

  void do_await_stop();


  boost::asio::io_context io_context_;

  boost::asio::signal_set signals_;

  boost::asio::ip::tcp::acceptor acceptor_;

  session_manager sess_manager_;
};

}
}
