#include "Storage.hpp"
namespace bulk {
namespace server {
namespace internals {

//{ Region LocalStorage.
LocalStorage::LocalStorage():
	CommandStorage(Saver::GetDefaultSaver()),
	Storage(new std::list<std::string>() )
{}

LocalStorage::LocalStorage(std::shared_ptr<Saver> _saver):
	CommandStorage(_saver),
	Storage(new std::list<std::string>() )
{}


void LocalStorage::add(std::string& cmd)
{
	if(!Storage)
	{
		Storage.reset( new std::list<std::string>());
	}

	if(Storage->size()==0)
	{
		fist_cmd_time= std::chrono::system_clock::now();
	}

	Storage->push_back(cmd);
}

bool LocalStorage::save ()
{
	if(!Storage || !saver)
	{
		clear();
		return false;
	}

	if(Storage->size()>0)
	{

		std::shared_ptr<Transaction> trans (new Transaction {
			.start_timestamp = fist_cmd_time,
					.commands= std::move(Storage),
		});

		saver->Log(trans);
		return true;
	}
	return false;
}

void LocalStorage::clear()
{
	if(Storage)
		Storage->clear();
}

//}

//{ Region SharedStorage.

void SharedStorage::DumpStorage()
{
	if(!Storage || !saver)
	{
		if(Storage)
			Storage->clear();

		return;
	}

	if(Storage->size()==0)
		return;

	std::shared_ptr<Transaction> trans (new Transaction {
		.start_timestamp = fist_cmd_time,
				.commands= std::move(Storage)
	});

	saver->Log(trans);
}


SharedStorage::~SharedStorage()
{
	timer.cancel();
	if(timer_thr.joinable())
		timer_thr.join();
	DumpStorage();
}



SharedStorage::SharedStorage(std::size_t _bulk_size):
			SharedStorage(_bulk_size,Saver::GetDefaultSaver())
{}

SharedStorage::SharedStorage(std::size_t _bulk_size,std::shared_ptr<Saver> _saver):
			CommandStorage(_saver),
			Storage(new std::list<std::string>() ),
			bulk_size(_bulk_size),
			fist_cmd_time(std::chrono::system_clock::now()),
			last_cmd_time(std::chrono::system_clock::now()),
			mut(),
			io_context_(1),
			timer(io_context_, std::chrono::seconds(2))

{
	timer.async_wait([this](const boost::system::error_code& e){timer_func(e);});
	timer_thr = std::thread([this](){io_context_.run();});

}


void SharedStorage::timer_func(const boost::system::error_code& e)
{
	if(!e /*!= boost::asio::error::operation_aborted*/)
	{
		std::lock_guard<std::mutex> lk{mut};
		if(last_cmd_time < (std::chrono::system_clock::now() - std::chrono::seconds(2) ) )
		{
			DumpStorage();
		}

		 timer.expires_from_now(std::chrono::seconds(2));
		 timer.async_wait([this](const boost::system::error_code& e){timer_func(e);});
	}

}

void SharedStorage::add(std::string& cmd)
{
	std::lock_guard<std::mutex> lk(mut);

	if(!Storage)
	{
		Storage.reset( new std::list<std::string>());
	}

	if(Storage->size()==0)
	{
		fist_cmd_time= std::chrono::system_clock::now();
	}

	Storage->push_back(cmd);
	last_cmd_time = std::chrono::system_clock::now();

	if(Storage->size()==bulk_size)
		DumpStorage();
}

bool SharedStorage::save ()
{
	return true;
}

void SharedStorage::clear()
{
	return;
}
//}
}

//{ Region GlobalStorage.
GlobalStorage::storage_t& GlobalStorage::GetInstance()
{
	static storage_t instance;
	return instance;
}

std::shared_ptr<internals::SharedStorage> GlobalStorage::GetStorage(std::size_t bulk_size)
{
	auto& instance = GetInstance();

	std::lock_guard<std::mutex> lk(mut);

	auto i = instance.find(bulk_size);
	if (i == instance.end())
	{
		bool b;
		std::tie(i, b) = instance.emplace(
				std::make_pair(
						bulk_size,
						std::make_shared<internals::SharedStorage>(bulk_size,internals::Saver::GetDefaultSaver())
				));
	}

	return i->second;
}

void GlobalStorage::ClearStorage()
{
	GetInstance().clear();
}

std::mutex GlobalStorage::mut;

//}
}
}
