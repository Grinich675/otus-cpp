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

	std::string endpoint;
	CommandProcessor* processor;

	CommandStorage* storage;
	Saver * saver;


	struct Counters
	{
		int strings=0;
		int commands=0;
		int blocks=0;
	}cnt;

	void DumpCounters()
	{
		std::stringstream ss;
		ss<<"Endpoint"<<endpoint<<std::endl;
		ss<<"\tStrings: "<<cnt.strings<<std::endl;
		ss<<"\tCommands: "<<cnt.commands<<std::endl;
		ss<<"\tBlocks: "<<cnt.blocks<<std::endl;

		std::cout<<ss.str()<<std::endl;
	}

public:
	~Controller()
	{
		delete processor;
		storage->clear();

		delete storage;
		delete saver;
	}


	Controller():block_size(15),mode(opMode::DynamicBlock),endpoint("default")
	{
		processor = new DynamicProcessor();
		storage = new CommandStorage();
		saver = new Saver();
	}


	Controller(int _block_size):block_size(_block_size),mode(opMode::StaticBlock),endpoint("default")
	{
		processor = new StaticProcessor(_block_size);
		storage = new CommandStorage();
		saver = new Saver();
	}

	Controller(int _block_size,std::string&& ep):block_size(_block_size),mode(opMode::StaticBlock),endpoint(ep)
	{
		processor = new StaticProcessor(_block_size);
		storage = new CommandStorage();
		saver = new Saver();
	}

	void Process(std::string& _cmd)
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

				if(storage->save(saver,endpoint))
					cnt.blocks++;

				storage->reInit();
				break;
			case Action::ChangeMode:

				if(storage->save(saver,endpoint))
					cnt.blocks++;
				storage->reInit();
				delete processor;

				if(mode == opMode::StaticBlock)
				{
					processor = new DynamicProcessor();
					mode =opMode::DynamicBlock;
				}
				else
				{
					processor = new StaticProcessor(block_size);
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
			if(storage->save(saver,endpoint))
				cnt.blocks++;
		}

		storage->clear();
		DumpCounters();
	}

	void AddLogger(LogInstance* logger)
	{
		saver->AddLogger( logger);
	}

	void RemoveLoggers()
	{
		saver->RemoveAll();
	}

};
