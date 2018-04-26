#pragma once
#include "Utils.h"
#include "Loggers.hpp"
#include <map>

#include "boost/asio.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace bulk {
namespace server {
namespace internals{
/**
 * \brief Interface for command storages
 * */
class CommandStorage
{
public:
	virtual ~CommandStorage() =default;
	CommandStorage()=delete;

	virtual void add(std::string& )=0;
	virtual bool save () = 0;
	virtual void clear() =0;

protected:
	std::shared_ptr<Saver> saver;
	CommandStorage(std::shared_ptr<Saver> _saver):saver(_saver){};
};


/**
 * \brief Stores command locally for client, owning object of this type.
 * Client need to call save manually
 * */
class LocalStorage: public CommandStorage
{
public:
	~LocalStorage()=default;

	LocalStorage();

	LocalStorage(std::shared_ptr<Saver> _saver);

	 void add(std::string& cmd)override;

	 bool save ()override;

	 void clear()override;

private:
	using ts_t = std::chrono::time_point<std::chrono::system_clock>;
	using storage_t = std::list<std::string>;

	std::unique_ptr<storage_t> Storage;
	ts_t fist_cmd_time;
};

/**
 * \brief Stores commands for multiple clients
 * */
class SharedStorage:public CommandStorage
{
public:
	~SharedStorage();

	SharedStorage()=delete;

	SharedStorage(std::size_t _bulk_size);

	SharedStorage(std::size_t _bulk_size,std::shared_ptr<Saver> _saver);

	void add(std::string& cmd)override;

	bool save ()override;

	void clear()override;
private:
	using ts_t = std::chrono::time_point<std::chrono::system_clock>;
	using storage_t = std::list<std::string>;

	std::unique_ptr<storage_t> Storage;
	std::size_t bulk_size;
	ts_t fist_cmd_time;
	ts_t last_cmd_time;
	mutable std::mutex mut;

	boost::asio::io_service io_context_;
	boost::asio::deadline_timer timer;
	std::thread timer_thr;


	void DumpStorage();
	void timer_func(const boost::system::error_code& /*e*/);
};
}

/**
 * \brief multiton based on bulk_size for SharedStorages
 * */
class GlobalStorage
{
	static std::mutex mut;

	using storage_t =std::map<std::size_t, std::shared_ptr<internals::SharedStorage>>;

	static storage_t& GetInstance();

public:
	static std::shared_ptr<internals::SharedStorage> GetStorage(std::size_t bulk_size);

	static void ClearStorage();
};

}
}

