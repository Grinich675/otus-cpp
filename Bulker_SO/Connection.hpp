#pragma once
#include "Controller.hpp"
#include "Loggers.hpp"
#include <iterator>
#include <deque>

#include <atomic>
namespace connection
{
class Connection
{
	Controller ctrl;
	std::deque<std::string> _internal_storage;

	mutable std::mutex mut;
	bool stop_flag;


	static std::atomic<int> con_cnt;

	void form_n_process_string(const char* data,std::size_t count)
	{
		std::size_t str_size = count;

		if(!_internal_storage.empty())
		{
			for (auto& str_part: _internal_storage)
			{
				str_size+=str_part.size();
			}
		}

		//count was 0, so data just '\n'. needed to close old string
		if(str_size==0)
			return;

		std::string str(str_size,0);
		auto dest = str.begin();

		//copy from internal storage,if its not empty
		if(!_internal_storage.empty())
		{
			for (auto& str_part: _internal_storage)
			{
				//str_size+=str_part.size();
				std::copy(str_part.begin(),str_part.end(),dest);
				std::advance(dest,str_part.size());
			}

			_internal_storage.clear();
		}

		std::copy(data,data+count,dest);

		ctrl.Process(str);

	}

public:

	void AddLogger(std::shared_ptr<LogInstance> logger)
	{

		std::lock_guard<std::mutex> lk (mut);
		ctrl.AddLogger(logger);

	}


	void receive(const char *data, std::size_t size)
	{
		std::lock_guard<std::mutex> lk (mut);

		if(stop_flag)
			return;

		std::size_t ind =0,last_found=0;

		for(;ind<size;++ind)
		{

			if(data[ind]=='\n')
			{
				form_n_process_string(data+last_found,ind-last_found);

				last_found=ind+1;
			}
		}


		if(size - last_found > 0)
			_internal_storage.emplace_back(data+last_found,data+size);

	}

	void Stop()
	{
		std::lock_guard<std::mutex> lk (mut);

		//already stopped
		if(stop_flag)
			return;

		stop_flag=true;

		ctrl.Stop();
		ctrl.RemoveLoggers();

		_internal_storage.clear();

	}



	Connection()=delete;

	Connection(std::size_t block_size):
		ctrl(block_size,std::to_string(con_cnt++)),
		_internal_storage(),
		stop_flag(false)
	{

	}

	Connection(std::size_t block_size,std::string ep):
		ctrl(block_size,std::move(ep)),
		_internal_storage(),
		stop_flag(false)
	{
		con_cnt++;
	}

	~Connection()
	{
		{
			std::lock_guard<std::mutex> lock(mut);

			_internal_storage.clear();
		}
	}
};

std::atomic<int> Connection::con_cnt(0);

}
