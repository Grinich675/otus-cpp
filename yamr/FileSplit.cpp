#include "FileSplit.hpp"

FileSplit::FileSplit():
	path(""),offset(-1),length(-1)
{}

FileSplit::FileSplit(std::string file, long _start_offsset, long _length):
path(file),offset(_start_offsset),length(_length)
{}

long FileSplit::getChunkLength()
{
	return length;
}

std::string FileSplit::getFilePath()
{
	return path;
}

long FileSplit::getStartOffset()
{
	return offset;
}

