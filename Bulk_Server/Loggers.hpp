#pragma once
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <list>
#include <vector>
#include <memory>

#include <shared_mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <atomic>
#include <functional>

namespace bulk {
namespace server {
namespace internals{

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
public:

	FileLogger(int threads =1);

	~FileLogger();

	void log(std::shared_ptr<Transaction> trans) override;

private:
	std::atomic<bool> stop_flag;

	std::condition_variable cv;
	mutable std::mutex cv_m;
	std::queue<std::shared_ptr<Transaction>> _transactionQueue;

	std::vector<std::thread> processing_threads;
	std::vector<counters> _threads_counters;

	void DumpCounters();

	void clearQueue();

	void processTransaction(std::shared_ptr<Transaction> trans,int thr_idx);

	void doLog(int thr_idx);
};

/**
 \brief Console Logger

\details
Writes each batch of commands to stdout in format
"bulk: %cmd_i%,%cmd_(i+1)%, ... ,%cmd_(N)% "
*/
class ConsoleLogger:public LogInstance
{
public:
	ConsoleLogger(int threads = 1);

	~ConsoleLogger();

	void log(std::shared_ptr<Transaction> trans) override;

private:
	std::atomic<bool> stop_flag;
	std::condition_variable cv;
	std::mutex cv_m;
	std::queue<std::shared_ptr<Transaction>> _transactionQueue;

	std::vector<std::thread> processing_threads;
	std::vector<counters> _threads_counters;

	void clearQueue();

	void DumpCounters();

	void processTransaction(std::shared_ptr<Transaction> trans,int thr_idx);

	//thread function
	void doLog(int thr_idx);


};


/**
 \brief Log outputs observer

 \details
 Ships transactions for each registered log instance

*/
class Saver
{
	std::shared_timed_mutex mut;
	std::vector< std::shared_ptr<LogInstance> > loggers;
public:

	void AddLogger(std::shared_ptr<LogInstance> logger)
	{
		if(logger)
		{
			std::unique_lock<std::shared_timed_mutex> lk (mut);
			loggers.push_back(logger);
		}
	}

	void RemoveAll()
	{
		std::unique_lock<std::shared_timed_mutex> lk (mut);
		loggers.clear();
	}

	void Log(std::shared_ptr<Transaction> trans)
	{
		std::shared_lock<std::shared_timed_mutex> lk (mut);

		for (auto logger : loggers)
		{
			logger->log(trans);
		}
	}

	//Default saver
	static std::shared_ptr<Saver> GetDefaultSaver()
	{
		static Saver Instance;
		return std::shared_ptr<Saver>(&Instance,[](Saver*){});
	}


};

}
}
}
