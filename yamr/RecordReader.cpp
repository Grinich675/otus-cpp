#include "RecordReader.hpp"
#include <stdexcept>
#include <limits>

RecordReader::RecordReader(FileSplit _split):
	split(_split),file(),curr_key(-1),curr_value()
{
	file.open(_split.getFilePath(),std::ios::in|std::ios::binary);
	if(!file.is_open())
	{
		throw std::runtime_error("given file does not exist!");
	}

	file.seekg(split.getStartOffset(),std::ios::beg);

	// /n hack. needed for not skipping when on start of line
	if(split.getStartOffset()!=0)
	{
		file.seekg(-1,std::ios::cur);
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}


}

long RecordReader::getCurrentKey()
{
	return curr_key;
}

std::string RecordReader::getCurrentValue()
{
	return curr_value;
}

bool RecordReader::nextKeyValue()
{
	curr_key=file.tellg();
	if(!std::getline(file,curr_value))
		return false;

	if(curr_key - split.getStartOffset() >= split.getChunkLength())
	{
		return false;
	}

	return true;
}
