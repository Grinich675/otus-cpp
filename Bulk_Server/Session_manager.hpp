#pragma once
#include <set>
#include "Session.hpp"

namespace bulk {
namespace server {

class session_manager
{
public:
	session_manager(const session_manager&) = delete;
	session_manager& operator=(const session_manager&) = delete;

	session_manager()=default;

  void start(std::shared_ptr<session> ses)
  {
	  sessions_.insert(ses);
	  ses->start();
  }

  void stop(std::shared_ptr<session> ses)
  {
	  sessions_.erase(ses);
	  ses->stop();
  }

  void stop_all()
  {
    for (auto s: sessions_)
    {
    	s->stop();
    }
      sessions_.clear();
  }
private:

  std::set<std::shared_ptr<session>> sessions_;
};

}
}
