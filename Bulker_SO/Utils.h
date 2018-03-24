#pragma once
#include <chrono>
#include <list>
#include <string>

#include <iostream>
/**
 \brief Representation of ready for output command batch
*/
struct Transaction
{
	using ts_t = std::chrono::time_point<std::chrono::system_clock>;
	ts_t start_timestamp;
	std::list<std::string>* commands;
	std::string endpoint;

	~Transaction()
	{
		delete commands;
	}
};

/**
 \brief All possible actions taken after command check
*/
enum class Action
{
	Add,
	Skip,
	ADDnFlush,
	ChangeMode
};

/**
 \brief Possible operation modes
*/
enum class opMode
{
	StaticBlock, //Transaction completes by s
	DynamicBlock //Transaction completes by escape-sequence
};
