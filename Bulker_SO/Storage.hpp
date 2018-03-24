#pragma once
#include "Utils.h"
#include "Loggers.hpp"

/**
 * \brief Stores commands and some meta
 * */
class CommandStorage
{
	using ts_t = std::chrono::time_point<std::chrono::system_clock>;
	std::list<std::string>* Storage;
	ts_t fist_cmd_time;
public:

	ts_t GetTimestamp()
	{
		return fist_cmd_time;
	}

	~CommandStorage()
	{
		if(Storage!=nullptr)
		{
			delete Storage;
		}
	}

	CommandStorage():Storage(new std::list<std::string>() ){}

	 void add(std::string& cmd)
	 {
		 if(Storage->size()==0)
		 {
			 fist_cmd_time= std::chrono::system_clock::now();
		 }
		 Storage->push_back(cmd);
	 }

	 bool save (Saver* _saver,std::string ep)
	 {
		 if(Storage->size()>0)
		 {

			 std::shared_ptr<Transaction> trans (new Transaction {
 	 	 	 	 .start_timestamp = fist_cmd_time,
	 	 	 	 .commands= Storage,
				 .endpoint = ep
		 	 	 });
			 Storage=nullptr;
			 _saver->Log(trans );
			 return true;
		 }
		 return false;
	 }

	 void clear()
	 {
		 if(Storage!= nullptr)
			 Storage->clear();
	 }

	 void reInit()
	 {
		 Storage = new std::list<std::string>();
	 }
};
