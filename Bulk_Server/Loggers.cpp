#include "Loggers.hpp"

namespace bulk {
namespace server {
namespace internals{

//{ Region FileLogger.

FileLogger::~FileLogger()
{
	std::clog<<"Stopping File logger...\n";

	stop_flag=true;
	cv.notify_all();

	for(auto& thr: processing_threads)
	{
		thr.join();
	}

	DumpCounters();

	clearQueue();
}


FileLogger::FileLogger(int threads)
{
	stop_flag=false;
	_threads_counters.reserve(threads);

	for (int i= 0; i< threads;++i)
	{
		processing_threads.emplace_back(&FileLogger::doLog,this,i);
		_threads_counters.emplace_back();
	}
}

void FileLogger::doLog(int thr_idx)
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

void FileLogger::log(std::shared_ptr<Transaction> trans)
{
	{
		std::lock_guard<std::mutex> lk(cv_m);
		_transactionQueue.push(trans);
	}
	cv.notify_one();
}

void FileLogger::processTransaction(std::shared_ptr<Transaction> trans,int thr_idx)
{
	std::stringstream fname;
	fname <<"bulk"<<std::chrono::duration_cast<std::chrono::seconds>(trans->start_timestamp.time_since_epoch()).count()
						<<"_"<<thr_idx<<"_"<<_threads_counters[thr_idx].blocks<<".log";

	std::ofstream log_file(fname.str());

	for(auto cmd: *(trans->commands))
	{
		log_file<<cmd<<std::endl;
	}

	log_file.close();

	++_threads_counters[thr_idx].blocks;
	_threads_counters[thr_idx].commands+= trans->commands->size();
}


void FileLogger::DumpCounters()
{
	std::stringstream ss;
	for(unsigned int i=0 ; i< processing_threads.size();++i)
	{
		ss<<"File log thread "<<i<<": " <<std::endl;
		ss<<"\tCommands: "<< _threads_counters[i].commands<<std::endl;
		ss<<"\tBlocks: "<<_threads_counters[i].blocks<<std::endl;
	}

	ss<<std::endl;
	std::clog<<ss.str();
}

void FileLogger::clearQueue()
{
	while(!_transactionQueue.empty())
	{
		_transactionQueue.pop();
	}
}

//}

//{ Region ConsoleLogger.

void ConsoleLogger::clearQueue()
{
	while(!_transactionQueue.empty())
	{
		_transactionQueue.pop();
	}
}

void ConsoleLogger::DumpCounters()
{
	std::stringstream ss;
	for(unsigned int i=0 ; i< processing_threads.size();++i)
	{
		ss<<"Console log thread "<<i<<": " <<std::endl;
		ss<<"\tCommands: "<< _threads_counters[i].commands<<std::endl;
		ss<<"\tBlocks: "<<_threads_counters[i].blocks<<std::endl;
	}

	ss<<std::endl;
	std::clog<<ss.str();
}


void ConsoleLogger::processTransaction(std::shared_ptr<Transaction> trans,int thr_idx)
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
void ConsoleLogger::doLog(int thr_idx)
{
	using namespace std::chrono_literals;

	auto & q = _transactionQueue;
	auto& flag = stop_flag;

	auto pred = [&q,&flag](){return  q.size()||flag ;};

	while(true)
	{

		std::unique_lock<std::mutex> lk(cv_m);

		if(cv.wait_for(lk,1s, pred))
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

ConsoleLogger::ConsoleLogger(int threads)
{
	stop_flag=false;
	_threads_counters.reserve(threads);

	for (int i= 0; i< threads;++i)
	{
		processing_threads.emplace_back(&ConsoleLogger::doLog,this,i);
		_threads_counters.emplace_back();
	}
}

ConsoleLogger::~ConsoleLogger()
{
	std::clog<<"Stopping Console logger...\n";

	stop_flag=true;
	cv.notify_all();

	for(auto& thr: processing_threads)
	{
		thr.join();
	}

	DumpCounters();
	clearQueue();
}

void ConsoleLogger::log(std::shared_ptr<Transaction> trans)
{
	{
		std::lock_guard<std::mutex> lk(cv_m);
		_transactionQueue.push(trans);
	}
	cv.notify_one();
}
//}


}
}
}
