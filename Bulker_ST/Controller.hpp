#pragma once
#include "Utils.h"
#include "Loggers.hpp"
#include "Storage.hpp"
#include "Processors.hpp"

/**
 * \brief Processes commands.
 */
class Controller
{
	int block_size;
	opMode mode;
	std::unique_ptr<CommandProcessor> processor;

	std::unique_ptr<CommandStorage> storage;
	std::unique_ptr<Saver> saver;
public:
	~Controller()
	{
		/*delete processor;
		if (mode != opMode::DynamicBlock)
			storage->save(saver);
		storage->clear();
		

		delete storage;
		delete saver;*/
	}


	Controller():
		block_size(15),mode(opMode::DynamicBlock),
		processor(new DynamicProcessor()),
		storage(new CommandStorage()),
		saver (new Saver())
	{
	}


	Controller(int _block_size):
		block_size(_block_size),mode(opMode::StaticBlock),
		processor(new StaticProcessor(_block_size)),
		storage(new CommandStorage()),
		saver(new Saver())
	{
	}

	void Process(std::string& _cmd)
	{
		auto res = processor->Check(_cmd);

		switch (res) {
			case Action::Add:
				storage->add(_cmd);
				break;
			case Action::ADDnFlush:
				storage->add(_cmd);
				storage->save(saver.get());
				storage->clear();
				break;
			case Action::ChangeMode:
				storage->save(saver.get());
				storage->clear();

				if(mode == opMode::StaticBlock)
					{
						processor.reset( new DynamicProcessor());
						mode =opMode::DynamicBlock;
					}
				else
					{
						processor.reset( new StaticProcessor(block_size));
						mode= opMode::StaticBlock;
					}
				break;
			default:
				break;
		}
	}

	void Stop()
	{
		if (mode != opMode::DynamicBlock)
		{
			storage->save(saver.get());
			//block cnt++;
		}

		storage->clear();
		//dump counters
	}

	void AddLogger(std::shared_ptr<LogInstance> logger)
	{
		saver->AddLogger( logger);
	}


};
