#pragma once
#include <vector>
#include <future>
#include <thread>
#include <algorithm>

#include "SplitContainer.hpp"
#include "FileInputFormat.hpp"
#include "FileOutputFormat.hpp"

#include "RecordReader.hpp"
#include "RecordWriter.hpp"

#include "JobConf.hpp"
#include "MapperContext.hpp"
#include "Mapper.hpp"
#include "ReducerContext.hpp"
#include "reducer.hpp"
#include "Partitioner.hpp"


#include <iostream>

template<
class MAP_KEY_IN,
class MAP_VALUE_IN,
class MAP_KEY_OUT,
class MAP_VALUE_OUT,
class REDUCE_KEY_OUT,
class REDUCE_VALUE_OUT>
class Engine
{
public:

	using mapper_t = Mapper<MAP_KEY_IN,MAP_VALUE_IN,MAP_KEY_OUT,MAP_VALUE_OUT>;
	using mapper_ctx_t = map_contex<MAP_KEY_OUT,MAP_VALUE_OUT>;
	using reduce_ctx_t = reduce_contex<MAP_KEY_OUT,MAP_VALUE_OUT,REDUCE_KEY_OUT,REDUCE_VALUE_OUT>;

	Engine()=delete;
	Engine(JobConf& _conf):
		config(_conf),splits(),output_format(_conf)
	{}

	void run()
	{
		splits.Init(config);

		std::vector<mapper_ctx_t> m_contexts(config.num_mappers);

		run_maps(m_contexts);

		//now all results in m_contexts

		std::vector<reduce_ctx_t> r_contexts(config.num_reducers);

		std::list<std::future<void>> reduce_futures;
		std::list<std::future<void>> map_out_clear_flags;

		for(std::size_t part = 0; part< config.num_reducers; ++part)
		{
			std::promise<void> barrier;
			map_out_clear_flags.emplace_back(barrier.get_future());
			reduce_task(m_contexts,std::ref(r_contexts.at(part)),part,std::move(barrier));
		}

		for(auto& fut:map_out_clear_flags)
		{
			fut.wait();
		}

		//now it is safe to remove all map output
		m_contexts.clear();

		//wait for all reducers to finish
		for (auto& fut : reduce_futures) {
			try {
				fut.get();
			} catch (const std::exception& e)
			{
				std::string ErrMsg ("Reduce Error: ");
				ErrMsg+=e.what();
				ErrMsg+="\n";
				std::cerr<<ErrMsg;
			}
		}

	}

	~Engine()=default;

private:

	void map_task(mapper_ctx_t& ctx,FileSplit split)
	{
		mapper_t m{};

		auto map_routine = [&m,&ctx](FileSplit& split){
			RecordReader reader {split};
			while(reader.nextKeyValue())
			{
				m.Map(reader.getCurrentKey(),reader.getCurrentValue(),ctx);
			}
		};

		map_routine(split);

		while(splits.nextSplit(split))
		{
			map_routine(split);
		}
	}

	void reduce_task(std::vector<mapper_ctx_t>& m_contexts,reduce_ctx_t& reduce_context ,std::size_t Partition,std::promise<void> shuffle_end)
	{
		//shuffle& sort
		if(!merge_shuffle(m_contexts,reduce_context,Partition))
		{
			//there is no data for reducer
			return;
		}
		shuffle_end.set_value();

		Reducer<MAP_KEY_OUT,MAP_VALUE_OUT,REDUCE_KEY_OUT,REDUCE_VALUE_OUT> r{};

		//writer = fileoutput.getwriter
		auto writer = output_format.getRecordWriter(Partition);


		//reduce and write output
		for(auto& KV: reduce_context)
		{
			r.reduce(KV.first,KV.second,std::ref(reduce_context));
			reduce_context.write(writer);
		}


	}

	bool merge_shuffle(std::vector<mapper_ctx_t>& map_results,reduce_ctx_t& reduce_context,std::size_t ReducerID)
	{
		using iter_type = typename std::multimap<MAP_KEY_OUT, MAP_VALUE_OUT>::iterator;
		std::list<std::pair<iter_type,iter_type>> iter_pairs{};

		Partitioner<MAP_KEY_OUT,MAP_VALUE_OUT> part{};



		auto partition_predicate =
				[&ReducerID,&part,&partitions = config.num_reducers](const std::pair<MAP_KEY_OUT,MAP_VALUE_OUT> &a)
				{
					if(part.getPartition(a.first,a.second,partitions) == ReducerID)
						return true;
					return false;
				};

		//find first element for given reducer from each map result set
		for(auto& result_set : map_results )
		{
			auto beg = std::find_if(result_set.begin(),result_set.end(),partition_predicate);

			if(beg!= result_set.end())
			{
				iter_pairs.push_back(std::make_pair(beg,result_set.end()));
			}
		}

		//there is no data for this reducer
		if(iter_pairs.size()==0)
		{
			return false;
		}

		//start merging
		while(iter_pairs.size()>1)
		{
			//find and push smallest element from found ones
			auto min= std::min_element(iter_pairs.begin(),iter_pairs.end(),
					[](const std::pair<iter_type,iter_type> &a, const std::pair<iter_type,iter_type> &b)
					{
						if( *(a.first) < *(b.first) )
							return true;
						return false;
					});

			auto& kv_pair = *(min->first);
			reduce_context.add(kv_pair.first, kv_pair.second);

			//find next value for this reducer from max's map out
			std::advance(min->first,1);
			auto next= std::find_if(min->first,min->second,partition_predicate);

			//if first == second == end , need to delete this pair from collection
			if(next == min->second)
			{
				iter_pairs.erase(min);
				continue;
			}

			//update iterator pair
			min->first = next;
		}

		//merge everything from left map output
		auto last_part = iter_pairs.front();
		for(auto it = last_part.first;it!= last_part.second;++it)
		{
			if(partition_predicate(*it))
			{
				reduce_context.add(it->first, it->second);
			}
		}

		return true;

	}

	void run_maps(std::vector<mapper_ctx_t>& m_contexts)
	{
		std::vector<std::future<void> > map_futures { };
		map_futures.reserve(config.num_mappers);

		//start up to 'num_mappers' map threads(tasks)
		for (std::size_t i = 0; i < config.num_mappers; ++i) {
			FileSplit initial_split;
			if (!splits.nextSplit(initial_split)) {
				//no more splits
				break;
			}
			map_futures.push_back(
					std::async(std::launch::async,
							&Engine<MAP_KEY_IN, MAP_VALUE_IN, MAP_KEY_OUT,
							MAP_VALUE_OUT, REDUCE_KEY_OUT,
							REDUCE_VALUE_OUT>::map_task, this,
							std::ref(m_contexts.at(i)), initial_split));
		}

		//wait for all map threads to finish and
		//check all futures for exceptions. delete corrupt from m_contexts
		auto ctx_it = m_contexts.begin();
		for (auto& fut : map_futures) {
			try {
				fut.get();
				std::advance(ctx_it, 1);
			} catch (const std::exception& e)
			{
				m_contexts.erase(ctx_it);

				std::string ErrMsg ("Map Error: ");
				ErrMsg+=e.what();
				ErrMsg+="\n";
				std::cerr<<ErrMsg;
			}
		}
	}


	JobConf config;
	SplitContainer splits;
	FileOutputFormat output_format;
};
