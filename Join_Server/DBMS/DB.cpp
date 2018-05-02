#include "DB.hpp"

#include "../Client.hpp"
#include <sstream>

bool DB::DropTable(std::string Tname)
{
	normalize_name(Tname);

	std::lock_guard<std::shared_timed_mutex> exclk(mut);
	auto search = db.find(Tname);
	if(search != db.end())
	{
		//drop table;
		db.erase(search);
		return true;
	}
	else
	{
		//no such table;
		return false;
	}
}

bool DB::AddTable(std::string Tname)
{
	normalize_name(Tname);

	std::lock_guard<std::shared_timed_mutex> exlk (mut);

	auto search = db.find(Tname);
	if(search != db.end())
	{
		//table with such name already exists
		return false;
	}
	else
	{
		//insert new node (new Table_ptr+ key)
		db.emplace(Tname,std::make_shared<Table>());
	}
	return true;
}

std::shared_ptr<Table> DB::GetTable(std::string Tname)
{
	normalize_name(Tname);

	std::shared_lock<std::shared_timed_mutex> shl (mut);

	auto search = db.find(Tname);
	if(search != db.end())
	{
		return search->second;
	}
	else
	{
		return std::shared_ptr<Table>(nullptr);
	}

}

int DB::Intersection(std::string first_T_name,std::string second_T_name,client* client_)
{
	normalize_name(first_T_name);
	normalize_name(second_T_name);

	int result_rows_cnt=-1;

	//get Table pointers
	auto first = GetTable(first_T_name);
	if(!first)
	{
		client_->send("ERR:\tNo table named:"+first_T_name+"\r\n");
		return result_rows_cnt;
	}

	auto second = GetTable(second_T_name);
	if(!second)
	{
		client_->send("ERR:\tNo table named:"+second_T_name+"\r\n");
		return result_rows_cnt;
	}

	{
		result_rows_cnt=0;

		//lock rows for shared access and get rows collection
		auto first_rows = first->GetRows();
		auto second_rows = second->GetRows();

		//get iterators for begin and end of collections
		auto first_iter1 = first_rows.second->begin();
		auto first_iter2 = second_rows.second->begin();

		auto last_iter1 = first_rows.second->end();
		auto last_iter2 = second_rows.second->end();

		while (first_iter1 != last_iter1 && first_iter2 != last_iter2)
		{
			if (first_iter1->first < first_iter2->first)
			{
				++first_iter1;
			}
			else
			{
				if (!(first_iter2->first < first_iter1->first))
				{
					auto row= *first_iter1++;
					std::stringstream buf;
					buf<<row.first<<","<<row.second<<","<<first_iter2->second<<"\r\n";
					client_->send(buf.str());

				}
				++first_iter2;
			}
			result_rows_cnt++;
		}

	}

	return result_rows_cnt;
}

int DB::Symmetric_Difference(std::string first_T_name,std::string second_T_name,client* client_)
{
	normalize_name(first_T_name);
	normalize_name(second_T_name);

	int result_rows_cnt=-1;

	//get Table pointers
	auto first = GetTable(first_T_name);
	if(!first)
	{
		client_->send("ERR:\tNo table named:"+first_T_name+"\r\n");
		return result_rows_cnt;
	}

	auto second = GetTable(second_T_name);
	if(!second)
	{
		client_->send("ERR:\tNo table named:"+second_T_name+"\r\n");
		return result_rows_cnt;
	}

	{
		result_rows_cnt=0;

		//lock rows for shared access and get rows collection
		auto first_rows = first->GetRows();
		auto second_rows = second->GetRows();

		//get iterators for begin and end of collections
		auto first_iter1 = first_rows.second->begin();
		auto first_iter2 = second_rows.second->begin();

		auto last_iter1 = first_rows.second->end();
		auto last_iter2 = second_rows.second->end();


		//iterate through whole first table
		while (first_iter1 != last_iter1)
		{

			//reached end of first table -just print remaining from first table
			if (first_iter2 == last_iter2)
			{

				for(;first_iter1!=last_iter1;++first_iter1)
				{
					std::stringstream buf_;
					auto row= *first_iter1;
					buf_<<row.first<<","<<row.second<<","<<""<<"\r\n";
					client_->send(buf_.str());
					++result_rows_cnt;
				}
				return result_rows_cnt;
			}

			//we have key in first table less than in second, so
			//result contains values from first table
			//and incrementing iterator on first table
			if (first_iter1->first < first_iter2->first)
			{
				auto row= *first_iter1++;
				std::stringstream buf_;
				buf_<<row.first<<","<<row.second<<","<<""<<"\r\n";
				client_->send(buf_.str());
			}
			else
			{
				//we have key in second table less than in second, so
				//result contains values from second table
				if (first_iter1->first > first_iter2->first )
				{
					std::stringstream buf_;
					buf_<<first_iter2->first<<","<<","<<first_iter2->second<<"\r\n";
					client_->send(buf_.str());
				}
				//equal keys, skip them
				else
				{
					++first_iter1;
				}

				++first_iter2;
			}
		}

		for(;first_iter2!=last_iter2;++first_iter2)
		{
			auto row= *first_iter2;
			std::stringstream buf_;
			buf_<<row.first<<","<<","<<row.second<<"\r\n";
			++result_rows_cnt;
			client_->send(buf_.str());
		}

	}

	return result_rows_cnt;
}


void DB::normalize_name(std::string& name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
}
