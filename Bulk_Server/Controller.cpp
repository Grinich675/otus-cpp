#include "Controller.hpp"

namespace bulk {
namespace server {
namespace internals{

void Controller::DumpCounters()
{
	std::stringstream ss;
	ss<<"Endpoint "<<endpoint<<std::endl;
	ss<<"\tStrings: "<<cnt.strings<<std::endl;
	ss<<"\tCommands: "<<cnt.commands<<std::endl;
	ss<<"\tBlocks: "<<cnt.blocks<<std::endl;

	std::clog<<ss.str()<<std::endl;
}



Controller::Controller(std::shared_ptr<Saver> _saver):
				block_size(15),mode(opMode::DynamicBlock),endpoint("default"),
				processor(new DynamicProcessor()),
				saver(_saver),
				storage(new LocalStorage(_saver))
{}


Controller::Controller():
				block_size(15),mode(opMode::DynamicBlock),endpoint("default"),
				processor(new DynamicProcessor()),
				saver(nullptr),
				storage(new LocalStorage())//Gets GlobalSaver
{
}

Controller::Controller(std::size_t _block_size):
				Controller(_block_size,"default")
{
}

Controller::Controller(std::size_t _block_size,std::string&& ep):
				block_size(_block_size),mode(opMode::StaticBlock),endpoint(ep),
				processor(new StaticProcessor(_block_size)),
				saver(nullptr),
				storage(GlobalStorage::GetStorage(_block_size))
{
}


void Controller::Process(std::string& _cmd)
{
	cnt.strings++;
	auto res = processor->Check(_cmd);

	switch (res) {
	case Action::Add:
		storage->add(_cmd);
		cnt.commands++;
		break;

	case Action::ADDnFlush:

		storage->add(_cmd);
		cnt.commands++;

		if(storage->save())
			cnt.blocks++;
		break;

	case Action::ChangeMode:

		if(storage->save())
			cnt.blocks++;


		if(mode == opMode::StaticBlock)
		{
			processor.reset( new DynamicProcessor());
			mode =opMode::DynamicBlock;
			storage = std::make_shared<LocalStorage>();

		}
		else
		{
			processor.reset( new StaticProcessor(block_size));
			mode= opMode::StaticBlock;
			storage= GlobalStorage::GetStorage(block_size);
		}
		break;
	default:
		break;
	}
}

void Controller::Stop()
{
	if (mode != opMode::DynamicBlock)
	{
		if(storage->save())
			cnt.blocks++;
	}

	storage->clear();
	DumpCounters();
}


}
}
}
