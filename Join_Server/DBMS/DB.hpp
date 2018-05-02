#pragma once

#include "Table.hpp"
#include <algorithm>


class client;

class DB
{
public:

	DB()=default;
	~DB()=default;

	bool DropTable(std::string Tname);

	bool AddTable(std::string Tname);

	std::shared_ptr<Table> GetTable(std::string Tname);

	int Intersection(std::string first_name,std::string second_name, client* client_);

	int Symmetric_Difference(std::string first_name,std::string second_name,client* client_);

private:

	static void normalize_name(std::string& name);

	std::shared_timed_mutex mut;
	std::map<std::string,std::shared_ptr<Table>> db;
};
