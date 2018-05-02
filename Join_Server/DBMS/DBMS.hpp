#pragma once

#include <memory>
#include <map>
#include "DB.hpp"

class DBMS
{
		static std::mutex mut;

		using dbms_storage_t =std::map<std::string, std::shared_ptr<DB>>;

		static dbms_storage_t& GetInstance();

		static void normalize_name(std::string& name);

	public:
		DBMS()=delete;

		~DBMS();
		static std::shared_ptr<DB> GetDB(std::string DBname);

		static bool DropDB(std::string DBname);

		static bool CreateDB(std::string DBname);

};

