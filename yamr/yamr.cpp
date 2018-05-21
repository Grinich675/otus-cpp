#include <iostream>
#include "Engine.hpp"

#include <algorithm>
#include <cmath>


template<>
void Mapper<long,std::string,std::size_t,std::string>::Map(long,std::string val,map_contex<std::size_t,std::string>& ctx)
{
	for(std::size_t len = 1;len <= val.size();++len)
	{
		ctx.write(len,std::string(val,0,len));
	}
}


template <>
class Reducer<std::size_t,std::string,std::size_t,bool>
{
public:
	~Reducer()=default;
	void reduce(std::size_t key, std::list<std::string>& values, reduce_contex<std::size_t,std::string,std::size_t,bool>& context)
	{
		if(!found)
		{
			values.sort();
				auto repeated= std::adjacent_find(values.begin(),values.end());

				if(repeated==values.end())
				{
					//no repeats
					context.write(key,true);
					found=true;
				}
		}
	}
private:
	bool found = false;
};


template <>
void RecordWriter::write(std::size_t key,bool)
{
	if(out.is_open())
	{
		out<<key<<std::endl;
	}
}




int main(int argc , char* argv[])
{

	JobConf conf;
	//get filename
	if(argc >= 1)
	{
		conf.input_files.emplace_back(argv[1]);
	}
	//get num mappers
	if(argc >= 2)
	{
		conf.num_mappers = std::stoi(argv[2]);
	}
	//get num reducers
	if(argc>=3)
	{
		conf.num_reducers = std::stoi(argv[3]);
	}

	//compute split size
	std::ifstream in(conf.input_files.front(), std::ifstream::ate | std::ifstream::binary);
	std::size_t size= in.tellg();
	in.close();
	std::size_t split = std::lrint(std::ceil( (double)size/conf.num_mappers));


	conf.min_split_size=split;
	conf.max_split_size = conf.min_split_size;

	Engine<long,std::string,std::size_t,std::string,std::size_t,bool> eng(conf);

	eng.run();

	return 0;
}
