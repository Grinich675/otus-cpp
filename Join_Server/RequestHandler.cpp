#include "RequestHandler.hpp"

#include "DBMS/CmdExecutor.hpp"

#include "Client.hpp"

void reqHandler::form_n_process_string(const char* data,std::size_t count,std::shared_ptr<DB> db_ptr,client* client_)
{
	std::size_t str_size = count;

	if(!_internal_storage.empty())
	{
		for (auto& str_part: _internal_storage)
		{
			str_size+=str_part.size();
		}
	}

	//count was 0, so data just '\n'. needed to close old string
	if(str_size==0)
		return;

	std::string str(str_size,0);
	auto dest = str.begin();

	//copy from internal storage,if its not empty
	if(!_internal_storage.empty())
	{
		for (auto& str_part: _internal_storage)
		{
			//str_size+=str_part.size();
			std::copy(str_part.begin(),str_part.end(),dest);
			std::advance(dest,str_part.size());
		}

		_internal_storage.clear();
	}

	std::copy(data,data+count,dest);


	auto cmd = crt.create_cmd(str);
	Executor::execute(std::move(cmd),std::move(db_ptr),client_);

}



void reqHandler::receive(const char *data, std::size_t size, std::shared_ptr<DB> db_ptr,client* client_)
{

	std::size_t ind =0,last_found=0;

	for(;ind<size;++ind)
	{

		if(data[ind]=='\n')
		{
			std::size_t sanitizer=0;
			if(data[ind-1]=='\r')
			{
				sanitizer=1;
			}

			form_n_process_string(data+last_found,ind-last_found-sanitizer,db_ptr,client_);

			last_found=ind+1;
		}
	}


	if(size - last_found > 0)
		_internal_storage.emplace_back(data+last_found,data+size);

}




