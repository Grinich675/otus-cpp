#pragma once
#include "Utils.h"
#include "Loggers.hpp"

/**
 * \brief Stores commands and some meta
 * */
class CommandStorage
{
	using ts_t = std::chrono::time_point<std::chrono::system_clock>;
	using storage_t = std::list<std::string>;

	std::unique_ptr<storage_t> Storage;
	ts_t fist_cmd_time;
public:


	~CommandStorage()=default;


	CommandStorage():Storage(new std::list<std::string>() ){}

	 void add(std::string& cmd)
	 {
		 if(!Storage)
			 Storage.reset(new std::list<std::string>() );

		 if(Storage->size()==0)
		 {
			 fist_cmd_time= std::chrono::system_clock::now();
		 }

		 Storage->push_back(cmd);
	 }

	 bool save (Saver* _saver)
	 {
		 if(!Storage || _saver==nullptr)
			 return false;


		 if(Storage->size()>0)
		 {

			 std::shared_ptr<Transaction> trans (new Transaction {
 	 	 	 	 .start_timestamp = fist_cmd_time,
	 	 	 	 .commands= std::move(Storage)
		 	 	 });

			 _saver->Log(trans );
			 return true;
		 }
		 return false;
	 }

	 void clear()
	 {
		 if(Storage)
			 Storage->clear();
	 }

	 void reInit()
	 {
		 Storage.reset( new std::list<std::string>());
	 }
};
