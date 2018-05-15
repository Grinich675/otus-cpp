#pragma once
#include <string>

class FileSplit
{
public:
	FileSplit();

	FileSplit(std::string file, long _start_offsset, long _length);
	~FileSplit()=default;

	long getChunkLength();
	std::string getFilePath();
	long getStartOffset();

private:
	std::string path;
	long offset;
	long length;
};
