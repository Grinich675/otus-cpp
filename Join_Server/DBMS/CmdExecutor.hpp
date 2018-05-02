#pragma once

#include <algorithm>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "../Client.hpp"
#include "Commands.hpp"
#include "DB.hpp"
#include "Table.hpp"

class Executor
{
private:
	static void execute_insert(InsertCMD* _cmd,std::shared_ptr<DB> db_ptr ,client* client_)
	{

		//validate & get table
		auto table = db_ptr->GetTable(_cmd->table_name);
		if(!table)
		{
			//no such table
			return execute_broken( (std::make_unique<BrokenCMD>("No table named: \""+_cmd->table_name+"\"")).get(),std::move(db_ptr),client_);
		}

		//validate values
		//values > 2 -> error to much values
		if(_cmd->values.size() > 2)
		{
			return execute_broken( (std::make_unique<BrokenCMD>("Too many parameters for INSERT command")).get(),std::move(db_ptr),client_);
		}
		//values > 2 -> error to less values
		else if(_cmd->values.size() < 2)
		{
			return execute_broken( (std::make_unique<BrokenCMD>("Not enough parameters for INSERT command")).get(),std::move(db_ptr),client_);
		}

		Table::uniq_key_t key;
		//try to convert first val
		try
		{
			key = std::stoul(_cmd->values.front());
		}
		catch (std::invalid_argument&)
		{
			return execute_broken( (std::make_unique<BrokenCMD>("Could not convert: \""+_cmd->values.front()+"\" to key type")).get(),std::move(db_ptr),client_);
		}
		catch (std::out_of_range&)
		{
			return execute_broken( (std::make_unique<BrokenCMD>("Out of range exception,converting \""+_cmd->values.front()+"\" to key type")).get(),std::move(db_ptr),client_);
		}
		catch(...)
		{
			return execute_broken( (std::make_unique<BrokenCMD>("Something really bad happened. Please get contact with your local DBA")).get(),std::move(db_ptr),client_);
		}

		if( !table->Add(key,_cmd->values.back()) )
		{
			return execute_broken( (std::make_unique<BrokenCMD>("Row with key \""+_cmd->values.front()+"\" already exists")).get(),std::move(db_ptr),client_);
		}

		client_->send("OK\t added successfully\r\n");
	}

	static void execute_truncate(TruncateCMD* _cmd,std::shared_ptr<DB> db_ptr , client* client_)
	{
		//validate & get table
		auto table = db_ptr->GetTable(_cmd->TableName());
		if(!table)
		{
			//no such table
			return execute_broken( (std::make_unique<BrokenCMD>("No table named: \""+_cmd->TableName()+"\"")).get(),std::move(db_ptr),client_);
		}

		table->Truncate();
		client_->send("OK\t table \'"+_cmd->TableName()+"\' truncated\r\n");
	}

	static void execute_intersect(IntersectCMD* _cmd,std::shared_ptr<DB> db_ptr , client* client_)
	{
		auto names = _cmd->TableNames();
		if(db_ptr->Intersection(std::get<0>(names),std::get<1>(names),client_) >= 0 )
		{
			client_->send("OK\r\n");
		}
	}

	static void execute_symm_diff(Symm_diff_CMD* _cmd,std::shared_ptr<DB> db_ptr , client* client_)
	{
		auto names = _cmd->TableNames();
		if(db_ptr->Symmetric_Difference(std::get<0>(names),std::get<1>(names),client_) >= 0)
		{
			client_->send("OK\r\n");
		}
	}

	static void execute_broken(BrokenCMD* _cmd,std::shared_ptr<DB> db_ptr , client* client_)
	{
		client_->send("ERR:\t"+_cmd->ErrMsg()+"\r\n");
	}

public:
	static void execute(std::unique_ptr<CMD> _cmd,std::shared_ptr<DB> db_ptr , client* client)
	{

		switch (_cmd->Type())
		{
			case cmd_type_t::Insert:
				execute_insert(reinterpret_cast<InsertCMD*> (_cmd.get()),std::move(db_ptr),client);
				break;

			case cmd_type_t::Truncate:
				execute_truncate(reinterpret_cast<TruncateCMD*> (_cmd.get()),std::move(db_ptr),client);
				break;

			case cmd_type_t::Symm_diff:
				execute_symm_diff(reinterpret_cast<Symm_diff_CMD*> (_cmd.get()),std::move(db_ptr),client);
				break;

			case cmd_type_t::Intersect:
				execute_intersect(reinterpret_cast<IntersectCMD*> (_cmd.get()),std::move(db_ptr),client);
				break;

			case cmd_type_t::Broken:
				execute_broken(reinterpret_cast<BrokenCMD*> (_cmd.get()),std::move(db_ptr),client);
				break;

			case cmd_type_t::Delete:
				//unimplemented
				//break;
			default:
				client->send("Unimplemented command\r\n");
				break;
		}

	}
};
