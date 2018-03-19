#pragma once
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <list>
#include <vector>
#include <memory>

#include <condition_variable>
#include <queue>
#include <thread>
#include <atomic>
#include <functional>

/**
 \brief Interface for loggers
*/
class LogInstance
{
public:
	virtual ~LogInstance() =default;
	LogInstance()=default;
	virtual void log(std::shared_ptr<Transaction>)=0;
	struct counters
	{
		int blocks;
		int commands;

		counters():blocks(0),commands(0){};
	};
};



/**
 \brief Logger for file output

\details
Writes each batch of commands in file "bulk%datetime%.log" , where %datetime%  - first command from batch recieve time
*/
class FileLogger:public LogInstance
{

	std::atomic<bool> stop_flag;

	std::condition_variable cv;
	mutable std::mutex cv_m;
	std::queue<std::shared_ptr<Transaction>> _transactionQueue;

	std::vector<std::thread> processing_threads;
	std::vector<counters> _threads_counters;


	void DumpCounters()
	{
		std::stringstream ss;
		for(unsigned int i=0 ; i< processing_threads.size();++i)
		{
			ss<<"File log thread "<<i<<": " <<std::endl;
			ss<<"\tCommands: "<< _threads_counters[i].commands<<std::endl;
			ss<<"\tBlocks: "<<_threads_counters[i].blocks<<std::endl;
		}

		ss<<std::endl;
		std::cout<<ss.str();
	}

	void clearQueue()
	{
		while(!_transactionQueue.empty())
		{
			_transactionQueue.pop();
		}
	}

#ifdef PERF_TEST
	void PerfTest(std::shared_ptr<Transaction> trans)
	{
		for (int i =0;i<10000;++i)
		{
			std::hash<decltype(trans)>{}(trans);
		}
	}
#endif

	void processTransaction(std::shared_ptr<Transaction> trans,int thr_idx)
	{

#ifdef PERF_TEST
		for (int i =0;i<10000;++i)
		{
			PerfTest(trans);
		}
#endif
		std::stringstream fname;
		fname <<"bulk"<<std::chrono::duration_cast<std::chrono::seconds>(trans->start_timestamp.time_since_epoch()).count()
		<<"_"<<_threads_counters[thr_idx].blocks<<"_"<<thr_idx<<".log";

		std::ofstream log_file(fname.str());

		for(auto cmd: *(trans->commands))
		{
			log_file<<cmd<<std::endl;
		}

		log_file.close();

		++_threads_counters[thr_idx].blocks;
		_threads_counters[thr_idx].commands+= trans->commands->size();
	}

	void doLog(int thr_idx)
	{
		using namespace std::chrono_literals;

		auto & q = _transactionQueue;
		auto& flag = stop_flag;

		auto pred = [&q,&flag](){return  q.size()||flag ;};


		while(true)
		{
			std::unique_lock<std::mutex> lk(cv_m);

			if(cv.wait_for(lk,1s,pred))
			{
				//we recieved signal,but we need to stop processing
				if(stop_flag)
				{
					//force process elements in Q without waiting
					while(q.size() > 0)
					{
						auto trans = _transactionQueue.front();
						_transactionQueue.pop();
						lk.unlock();
						processTransaction(trans,thr_idx);
						lk.lock();
					}

					break;
				}

				auto trans = _transactionQueue.front();
				_transactionQueue.pop();
				lk.unlock();
				processTransaction(trans,thr_idx);
			}
			else
			{
				//timeout. mutex locked, so just pop everything from Q & shutdown if stop_flag

				while(q.size() > 0)
				{
					auto trans = _transactionQueue.front();
					_transactionQueue.pop();
					lk.unlock();

					processTransaction(trans,thr_idx);
					lk.lock();
				}

				if(stop_flag)
					break;
			}

		}

	}

public:

	FileLogger(int threads =1)
	{

		stop_flag=false;
		_threads_counters.reserve(threads);

		for (int i= 0; i< threads;++i)
			processing_threads.emplace_back(&FileLogger::doLog,this,i);
	}

	~FileLogger()
	{
		std::cout<<"Stopping File logger...\n";

		stop_flag=true;
		cv.notify_all();

		for(auto& thr: processing_threads)
		{
			thr.join();
		}

		DumpCounters();

		clearQueue();

	}

	void log(std::shared_ptr<Transaction> trans) override
	{
		{
			std::lock_guard<std::mutex> lk(cv_m);
			_transactionQueue.push(trans);
		}
		cv.notify_one();

	}
};

/**
 \brief Console Logger

\details
Writes each batch of commands to stdout in format
"bulk: %cmd_i%,%cmd_(i+1)%, ... ,%cmd_(N)% "
*/
class ConsoleLogger:public LogInstance
{

	std::atomic<bool> stop_flag;
	std::condition_variable cv;
	std::mutex cv_m;
	std::queue<std::shared_ptr<Transaction>> _transactionQueue;


	std::vector<std::thread> processing_threads;
	std::vector<counters> _threads_counters;

	void clearQueue()
	{
		while(!_transactionQueue.empty())
		{
			_transactionQueue.pop();
		}
	}

	void DumpCounters()
	{
		std::stringstream ss;
		for(unsigned int i=0 ; i< processing_threads.size();++i)
		{
			ss<<"Console log thread "<<i<<": " <<std::endl;
			ss<<"\tCommands: "<< _threads_counters[i].commands<<std::endl;
			ss<<"\tBlocks: "<<_threads_counters[i].blocks<<std::endl;
		}

		ss<<std::endl;
		std::cout<<ss.str();
	}


	void processTransaction(std::shared_ptr<Transaction> trans,int thr_idx)
	{
		std::stringstream ss;

		ss<<"bulk: ";

		auto it = trans->commands->begin();
		ss<<*it;

		for(++it;it!=trans->commands->end();++it)
		{
			ss<<","<<*it;
		}

		ss<<std::endl;
		std::cout<<ss.str();

		++(_threads_counters[thr_idx].blocks);
		_threads_counters[thr_idx].commands+= trans->commands->size();
	}

	//thread function
	void doLog(int thr_idx)
	{
		using namespace std::chrono_literals;

		auto & q = _transactionQueue;
		auto& flag = stop_flag;

		auto pred = [&q,&flag](){return  q.size()||flag ;};

		while(true)
		{

			std::unique_lock<std::mutex> lk(cv_m);

			cv.wait(lk, pred);
			{
				//we recieved signal,but we need to stop processing
				if(stop_flag)
				{
					//force process elements in Q without waiting
					while(q.size() > 0)
					{
						auto trans = _transactionQueue.front();
						_transactionQueue.pop();
						lk.unlock();
						processTransaction(trans,thr_idx);

						lk.lock();
					}

					break;
				}

				auto trans = _transactionQueue.front();
				_transactionQueue.pop();
				lk.unlock();
				processTransaction(trans,thr_idx);
			}
		}
	}

public:


	ConsoleLogger(int threads = 1)
	{
		stop_flag=false;
		_threads_counters.reserve(threads);
		for (int i= 0; i< threads;++i)
			processing_threads.emplace_back(&ConsoleLogger::doLog,this,i);
	}

	~ConsoleLogger()
	{
		std::cout<<"Stopping Console logger...\n";

		stop_flag=true;
		cv.notify_all();

		for(auto& thr: processing_threads)
		{
			thr.join();
		}

		DumpCounters();
		clearQueue();

	}

	void log(std::shared_ptr<Transaction> trans) override
	{
		{
			std::lock_guard<std::mutex> lk(cv_m);
			_transactionQueue.push(trans);
		}
		cv.notify_one();
	}
};


/**
 \brief Log outputs observer

 \details
 Ships transactions for each registered log instance

*/
class Saver
{
	std::vector< std::unique_ptr<LogInstance> > loggers;
public:
	void AddLogger(std::unique_ptr<LogInstance> logger)
	{
		loggers.push_back(std::move(logger));
	}

	void RemoveAll()
	{
		loggers.clear();
	}

	void Log(std::shared_ptr<Transaction> trans)
	{
		for (auto& logger : loggers)
		{
			logger->log(trans);
		}
	}

};
