#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <cstddef>
#include <memory>

#include "Handler.hpp"


using boost::asio::ip::tcp;

namespace bulk {
namespace server {

class session_manager;


class session
  : public std::enable_shared_from_this<session>
{
public:
  session(const session&) = delete;
  session& operator=(const session&) = delete;

  session(session_manager&,tcp::socket,std::size_t);

  void start();

  void stop();

private:
  void do_read();

  session_manager& ses_manager_;
  tcp::socket socket_;
  ReqHandler conn;
  enum { max_length = 1024 };
  char data_[max_length];

};

}
}
