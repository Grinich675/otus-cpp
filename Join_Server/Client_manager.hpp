#pragma once
#include <set>
#include "Client.hpp"


class client_manager
{
public:
	client_manager(const client_manager&) = delete;
	client_manager& operator=(const client_manager&) = delete;

	client_manager()=default;

	void start(std::shared_ptr<client> cli)
	{
		{
			std::lock_guard<std::mutex> lk{mut};
			clients_.insert(cli);
		}
		cli->start();
	}

	void stop(std::shared_ptr<client> cli)
	{
		{
			std::lock_guard<std::mutex> lk{mut};
			clients_.erase(cli);
		}
		cli->stop();
	}

	void stop_all()
	{
		std::lock_guard<std::mutex> lk{mut};

		for (auto c: clients_)
		{
			c->stop();
		}
		clients_.clear();
	}
private:

	mutable std::mutex mut;
	std::set<std::shared_ptr<client>> clients_;

};
