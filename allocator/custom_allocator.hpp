#pragma once

#include <memory>
#include <list>


template<typename T>
struct mem_chunk
{
	size_t destroyed;
	size_t unused;
	size_t size;
	T* alloc_buf;

	mem_chunk():
		destroyed(0),unused(0),size(0),alloc_buf(nullptr)
	{}

	mem_chunk(size_t _alloc_size):
		destroyed(0),unused(_alloc_size),size(_alloc_size),alloc_buf(nullptr)
	{
		auto p = std::malloc(_alloc_size*sizeof ( T ) ) ;
		if ( ! p )
			throw std::bad_alloc( ) ;

		alloc_buf=reinterpret_cast<T*> (p);
	}

	bool contains(T* base, size_t n)
	{
		if( (alloc_buf<= base) && (alloc_buf+size >= base+n))
		{
			return true;
		}

		return false;
	}

	void free()
	{
		std::free(alloc_buf);
	}
};


template <typename T,size_t default_alloc_size=10>
struct custom_allocator
{
	using value_type = T;
	using chunk_type = mem_chunk<value_type>;
	using mem_chunk_list = std::list<chunk_type>;
	using free_mem_chunk_list =  std::list< typename mem_chunk_list::iterator>;

	mem_chunk_list chunks;
	free_mem_chunk_list avail_chunks;


    template<typename U>
    struct rebind {typedef custom_allocator<U,default_alloc_size> other;};


    void create_new_chunk(size_t size)
    {
    	//create 1 mem_chunk<T>(alloc_size)
    	auto _new_chunk = chunk_type(size);

    	//add chunk to global list of chunks
    	chunks.emplace_back(_new_chunk);

    	//add iterator to avail_chunks
    	avail_chunks.push_back(--chunks.end());
    }

	custom_allocator():chunks{},avail_chunks{}
    {
    	create_new_chunk(default_alloc_size);
    }


	custom_allocator(const custom_allocator<value_type,default_alloc_size>& other)
	{
	}

	~custom_allocator()
	{
		for(auto chunk: chunks)
			{
				chunk.free();
			}
		avail_chunks.clear();
		chunks.clear();
	}

	T* allocate ( std::size_t n )
	{
		//check all chunks with free space for needed memory
		for(auto chunk: avail_chunks)
		{
			if(chunk->unused >= n)
			{
				T* addr =&(chunk->alloc_buf[chunk->size - chunk->unused ]);
				chunk->unused-=n;
				return addr;
			}
		}

		//Couldn't find enough mem in allocated chunks, so allocating one

		size_t alloc_size = n > default_alloc_size ? n : default_alloc_size;

		create_new_chunk(alloc_size);

		chunks.rbegin()->unused -=n;

		return  chunks.rbegin()->alloc_buf;
	} ;

	template<typename U,class... Args >
	void construct( U* p, Args&&... args )
	{

		::new(p) U(std::forward<Args>(args)...);

	}

	void deallocate( T* p, std::size_t n )
	{

		//check all chunks
		for(auto chunk = chunks.begin();chunk != chunks.end();++chunk)
		{
			if( chunk->contains(p,n) )
			{
				chunk->destroyed +=n;

				//all elements from chunk redy to be destroyed
				if(chunk->destroyed >= chunk->size)
				{

					//free memory in chunk
					chunk->free();
					//delete chunk from
					chunks.erase(chunk);
					break;
				}
			}
		}

	}

	template< class U >
	void destroy( U* p )
	{
		p->~U();
	}

};


