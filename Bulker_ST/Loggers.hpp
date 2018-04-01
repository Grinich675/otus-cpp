#pragma once
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <list>
#include <vector>



/**
 \brief Interface for loggers
*/
class LogInstance
{
public:
	virtual ~LogInstance() =default;
	LogInstance()=default;
	virtual void log(Transaction&)=0;
};



/**
 \brief Logger for file output

\details
Writes each batch of commands in file "bulk%datetime%.log" , where %datetime%  - first command from batch recieve time
*/
class FileLogger:public LogInstance
{

public:

	FileLogger()=default;
	~FileLogger()=default;

	void log(Transaction& trans) override
	{
		std::stringstream fname;
		fname <<"bulk"<<std::chrono::duration_cast<std::chrono::seconds>(trans.start_timestamp.time_since_epoch()).count()<<".log";

		std::ofstream log_file(fname.str());

		for(auto cmd: *(trans.commands))
		{
			log_file<<cmd<<std::endl;
		}

		log_file.close();

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
public:
	~ConsoleLogger()=default;

	void log(Transaction& trans) override
	{
		std::cout<<"bulk: ";

		auto it = trans.commands->begin();
		std::cout<<*it;

		for(++it;it!=trans.commands->end();++it)
		{
			std::cout<<","<<*it;
		}

		std::cout<<std::endl;
	}
};


/**
 \brief Log outputs observer

 \details
 Ships transactions for each registered log instance

*/
class Saver
{
	std::vector<std::shared_ptr<LogInstance>> loggers;
public:
	void AddLogger(std::shared_ptr<LogInstance> logger)
	{
		loggers.push_back(logger);
	}

	void Log(Transaction& trans)
	{
		for (auto logger : loggers) {
			logger->log(trans);
		}
	}

};
