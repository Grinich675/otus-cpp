#include "DBMS.hpp"

std::mutex DBMS::mut;

void DBMS::normalize_name(std::string& name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
}


DBMS::dbms_storage_t& DBMS::GetInstance()
{
	static dbms_storage_t instance {{"master",std::make_shared<DB>()},
									{"tempdb",std::make_shared<DB>()}};
	return instance;
}

std::shared_ptr<DB> DBMS::GetDB(std::string DBname)
{
	normalize_name(DBname);

	auto& instance = GetInstance();

	{
		std::lock_guard<std::mutex> lk(mut);

		auto i = instance.find(DBname);

		if (i == instance.end())
		{
			return std::shared_ptr<DB>(nullptr);
		}

		return i->second;
	}
}


bool DBMS::DropDB(std::string DBname)
{
	normalize_name(DBname);
	auto& instance = GetInstance();

	{
		std::lock_guard<std::mutex> lk(mut);

		auto i = instance.find(DBname);

		if (i == instance.end())
		{
			return  false;
		}

		instance.erase(i);
		return true;
	}

}

bool DBMS::CreateDB(std::string DBname)
{
	normalize_name(DBname);
	auto& instance = GetInstance();

	{
		std::lock_guard<std::mutex> lk(mut);

		auto i = instance.find(DBname);

		if (i != instance.end())
		{
			return  false;
		}

		instance.emplace(
				std::make_pair(
						DBname,
						std::make_shared<DB>()));
		return true;
	}
}

DBMS::~DBMS()
{
	auto& instance = GetInstance();
	instance.clear();
}
