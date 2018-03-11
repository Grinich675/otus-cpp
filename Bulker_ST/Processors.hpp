#pragma once
#include "Utils.h"

/**
 *\brief Interface for commands checkers. Each operational mode has its own Processor
 */
class CommandProcessor
{
public:
	virtual ~CommandProcessor() =default;
	CommandProcessor()=default;

	virtual Action Check(std::string& cmd) =0;
};

/**
 *\brief Command processor for StaticBlock mode
 *\details
  Transaction completes when reached limit
*/
class StaticProcessor: public CommandProcessor
{
	int block_size;
	int curr_block_sz;
public:
	StaticProcessor(int _block_size):block_size(_block_size),curr_block_sz(0)
	{}

	~StaticProcessor()=default;

	Action Check(std::string& cmd) override
	{

		if(cmd == "{")
		{
			curr_block_sz= 0;
			return Action::ChangeMode;
		}

		if(curr_block_sz+1 == block_size)
		{
			curr_block_sz=0;
			return Action::ADDnFlush;

		}

		++curr_block_sz;

		return Action::Add;
	}
};

/**
 *\brief Command processor for DynamicBlock mode
 *\details
  Transaction completes when closes block by "{" command
*/
class DynamicProcessor: public CommandProcessor
{
	int opened_blocks;
public:
	DynamicProcessor():opened_blocks(1)
	{}

	~DynamicProcessor()=default;

	Action Check(std::string& cmd) override
	{

		if(cmd == "{")
		{
			++opened_blocks;
			return Action::Skip;
		}

		if(cmd == "}")
		{
			--opened_blocks;

			if(opened_blocks == 0)
			{
				return Action::ChangeMode;
			}

			return Action::Skip;
		}

		return Action::Add;
	}
};
