#pragma once
#include "Utils.h"
#include "Loggers.hpp"

/**
 * \brief Stores commands and some meta
 * */
class CommandStorage
{
	using ts_t = std::chrono::time_point<std::chrono::system_clock>;
	std::list<std::string> Storage;
	ts_t fist_cmd_time;
public:

	ts_t GetTimestamp()
	{
		return fist_cmd_time;
	}

	CommandStorage()=default;

	 void add(std::string& cmd)
	 {
		 if(Storage.size()==0)
		 {
			 fist_cmd_time= std::chrono::system_clock::now();
		 }
		 Storage.push_back(cmd);
	 }

	 void save (Saver* _saver)
	 {
		 if(Storage.size()>0)
		 {
			 auto trans = Transaction {
	 	 	 	 	 .start_timestamp = fist_cmd_time,
		 	 	 	 .commands= &Storage
			 	 	 };
			 _saver->Log(trans );
		 }
	 }

	 void clear()
	 {
		 Storage.clear();
	 }
};
