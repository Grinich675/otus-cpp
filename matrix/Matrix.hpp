#include <map>
#include <tuple>


/**
 \brief N dimensional matrix, which stores only occupied elements

\details
N dimensional matrix, which stores only occupied elements, differ from default value.
Collection based on std::map container and metaprogramming.
Each dimension is a map(row) , which elements are pars [key-value].
Key - index of element in dimension
Value - same type Matrix of N-1 dimension, and same type and default value (subtree)

\tparam T Type of stored elements
\tparam N Size (dimensions)
\tparam default_value
*/
template<typename T, std::size_t N,T default_value>
class Matrix
{
	using element_type = Matrix<T,N-1,default_value>;

	using row_type = std::map<long,element_type>;

	row_type row;

public:

	Matrix() =default;
	~Matrix()=default;

	/**
	 \brief special proxy reference type (like bitset's)

	\details
	Special type, used as reference to N'th dimension in matrix,
	can be used to address sub-dimensions or change dimension(not implemented)

	\tparam Tuple type of tuple,which consists of positions in upper level dimensions
	*/
	template <typename Tuple>
	struct reference
	{
		Matrix<T,N,default_value>& search_col;//!< reference to associated collection(N'th dimension) in higher level collection (N+1 dimension)
		Tuple positions;//!< Keys, used for accessing this dimension

		auto operator [](std::size_t pos)
		{
			auto& element =search_col.row[pos];
			auto pos_tup = std::tuple_cat(positions,std::make_tuple(pos));
			auto res = typename Matrix<T,N-1,default_value>::template reference<decltype(pos_tup)>{.search_col=element,.positions = pos_tup};

			return res;
		};
	};

	/**
	 \brief Iterator class,used for iterating over stored elements

	\details
	Special type, used as Iterator for stored elements.
	*/
	class M_iter
	{
		row_type* assoc_coll;//!< N'th dimension collection, associated with this iterator. needed to determine where we reached end()

		typename row_type::iterator curr_level;//!< Iterator for stored elements in N'th dimension collection

		typename element_type::M_iter next;//!< Iterator for stored elements in N-1'th dimension collection (compile-time recursion)
	public:

		M_iter(row_type* _coll,typename row_type::iterator _it_cur, typename element_type::M_iter _it_next):
		assoc_coll(_coll),curr_level(_it_cur),next(_it_next)
		{
		}

		/**
		 \brief Method for incrementing stored-elements-iterator

		\details
		Tries to increment 1'st dimension iterator ( recursive call).
		If it reached end(), then tries increment 2'nd dimension iterator... etc
		\returns true, if was able to increment iterator on sub-level, or on this level
		*/
		bool Increment()
		{
			//Trying to increment on sub-levels
			if( next.Increment() )
				return true;

			//sub-level increment failed, so we need to try increment this level.
			//Need to choose first element, which size!=0 (have leaf elements)
			for(++curr_level;(curr_level!=assoc_coll->end()) && (curr_level->second.size() == 0);++curr_level)
			{

			}

			//we reached end() on this level, so no incrementing need to be done, just return false
			//and higher level iterator(if exists) will decide what to do
			if(curr_level == assoc_coll->end())
			{
				return false;
			}

			//so, we found our next element on N'th dimension. Need to init sub-level iterator (first occupied element)
			next = (curr_level->second.begin());
			return true;
		}

		auto operator++()
		{
			Increment();
			return *this;
		}

		auto operator++(int)
		{
			Increment();
			return *this;
		}

		/**
		 \brief operator for accessing element

		\details
		Returns tuple, which contains dimension indexes and value, where iterator's element located
		*/
		auto  operator*()
		{
		  return std::tuple_cat(std::make_tuple(curr_level->first),*next);
		}

		auto operator=(const M_iter& other)
		{
			assoc_coll = other.assoc_coll;
			curr_level = other.curr_level;
			next = other.next;
			return *this;
		}

		bool operator==(const M_iter& __x) const
	    {
			if(next != __x.next)
				return false;
			return curr_level == __x.curr_level;
	    }

	    bool operator!=(const M_iter& __x) const
	    {
	    	if(next == __x.next)
	    		return false;
	    	return curr_level != __x.curr_level;
	    }

	};

	/**
	 \brief Method returns number of occupied leaf elements in all sub-levels

	\details
	Recursively calls size() on all sub-levels and accumulates values.
	*/
	std::size_t size()
	{
		std::size_t elements_cnt = 0;
		for(auto e:row)
		{
			elements_cnt+=e.second.size();

		}
		return elements_cnt;
	}

	/**
	 \brief operator for accessing dimension by given index.
	 \details
	 Operator accesses element in this (Nth) dimension. If it doesn't existed, it will be create, as it is not leaf-element.

	*\param  pos index of dimension
	 \returns special inner type reference for N-1 dimension
	*/
	auto operator [](std::size_t pos)
	{
		auto& element = row[pos];
		auto pos_tup = std::make_tuple(pos);
		auto res = typename Matrix<T,N-1,default_value>::
						template reference<decltype(pos_tup)>
						{
							.search_col=element,
							.positions = pos_tup
						};
		return res;
	}

	/**
	 \brief Method returns iterator for first leaf-stored-element
	*/
	auto begin()
	{
		using it_t = typename row_type::iterator;
		it_t it = row.begin();
		for(; (it!= row.end()) && (it->second.size() == 0) 	;++it)
		{
		}
		auto ff = M_iter(&row,it,it->second.begin());
		return ff;
	}

	/**
	 \brief Method returns iterator, on element after last one (stl-like)
	*/
	auto end()
	{
		using it_t = typename row_type::iterator;
		it_t it = row.end();
		auto ff = M_iter(&row,it,row.rbegin()->second.end());
		return ff;

	}

};

/**
 \brief Specialization of  N dimensional matrix, which stores only occupied elements

\details
Specialization of  N dimensional matrix for N =0. (Basically if dimension index =0 ,this is simple element of type T)
Needed only for internal special type reference.

\tparam T Type of stored elements
\tparam default_value
*/
template<typename T,T default_value>
class Matrix<T,0,default_value>
{

public:

	/**
	 \brief special proxy reference type (like bitset's)

	\details
	Special type, used as reference to leaf-element in matrix.
	Stores index of element in upper-level dimension and reference to Upper-level collection.
	Reference to Upper-level collection needed for adding\removing element from matrix

	\tparam Tuple type of tuple,which consists of positions in upper level dimensions
	*/
	template<typename Tuple>
	struct reference
	{
		Tuple positions;//!< Keys, used for accessing this element
		std::map<long,T>& parent_col;//!< reference to row (1st dimension) which contains this element


		/**
		 \brief Returns value, stored in matrix by given indexes

		\details
		Checks associated collection ( 1st dimension),
		if it has element with such key - returns value from collection,
		if not - returns default value
		*/
		auto get_value()
		{
			T val = default_value;
			long _row_pos =
					std::get<std::tuple_size<Tuple>::value-1> (positions);

			auto value_iter = parent_col.find(_row_pos);

			if(value_iter != parent_col.end())
			{
			  val=value_iter->second;
			}
			return val;
		}

		/**
		 \brief Assigns value to element in matrix

		\details
		Checks given value, if it differ from default - value in collection will be replaced or added.
		If given value equals to default, then checks collection for element:
		 if it has one, then it will be deleted.
		*/
		auto operator =(T new_val)
		{
			long _row_pos =
					std::get<std::tuple_size<Tuple>::value-1> (positions);

			if(new_val!=default_value)
			{
				parent_col[_row_pos]=new_val;
				return this;
			}

			auto value_iter = parent_col.find(_row_pos);
			if(value_iter != parent_col.end() )
			{
				parent_col.erase(_row_pos);
			}

			return this;
		}

		operator T()
		{
			return get_value();
		}

		auto to_tuple ()
		{
			return std::tuple_cat(positions,std::make_tuple(get_value()));
		}
	};
};

/**
 \brief Specialization of N dimensional matrix, which stores only occupied elements
\details
Specialization of  N dimensional matrix for N =1, which stores only occupied elements, differ from default value.
Collection based on std::map container and metaprogramming.
Basically this is simple row from two-dimensional matrix, which stores only occupied elements

\tparam T Type of stored elements
\tparam N Size (dimensions)
\tparam default_value
*/
template<typename T,T default_value>
class Matrix<T,1,default_value>
{
	using row_type = std::map<long,T>;
	row_type row;

public:

	template <typename Tuple>
	struct reference
	{
		Matrix<T,1,default_value>& search_col;
		Tuple positions;

		auto operator [](std::size_t pos)
		{

			auto pos_tup = std::tuple_cat(positions,std::make_tuple(pos));

			auto res = typename Matrix<T,0,default_value>::
							template reference<decltype(pos_tup)>
							{
								.positions = pos_tup,
								.parent_col=search_col.row,
							};
			return res;
		}

	};

	/**
	 \brief Iterator class,used for iterating over stored elements

	\details
	Special type, used as Iterator for stored elements.
	Basically it is wrapper for iterator of row
	*/
	class M_iter
	{
		row_type* assoc_coll;//!< row associated with this iterator. needed to determine where we reached end()
		typename row_type::iterator curr_level; //!< Iterator for elements in row
	public:

		M_iter(row_type* _coll,typename row_type::iterator _it): assoc_coll(_coll),curr_level(_it)
		{
		}

		/**
		 \brief Method for incrementing stored-elements-iterator
		\details
		Tries to increment row iterator
		\returns true, if was able to increment row-iterator
		*/
		bool Increment()
		{
			//reached and(), do nothing and return false
			//upper level (if exists) will decide what to do
			if(curr_level == assoc_coll->end())
			{
				return false;
			}

			//this was last element with value.
			//So increment iterator in end() position and return false,
			//as end() points to place after last element and have some shitty value
			if(curr_level == --assoc_coll->end())
			{
				curr_level++;
				return false;
			}

			curr_level++;
			return true;
		}

		auto  operator*()
		{
		  return std::make_tuple(curr_level->first,curr_level->second);
		}

		auto operator=(const M_iter& other)
		{
			assoc_coll = other.assoc_coll;
			curr_level = other.curr_level;
			return *this;
		}

		bool operator==(const M_iter& __x) const
	    {
			return curr_level == __x.curr_level;
	    }

	    bool operator!=(const M_iter& __x) const
	    {
	    	return curr_level != __x.curr_level;
	    }

		auto operator++()
		{
			Increment();
			return *this;
		}

		auto operator++(int)
		{
			Increment();
			return *this;
		}

	};

	/**
	 \brief Method returns number of occupied leaf elements
	\details
	Returns number of elements,stored in a row
	*/
	std::size_t size()
	{
		return row.size();
	}

	/**
	 \brief Method returns iterator for first leaf-stored-element
	*/
	auto begin()
	{
		using it_t = typename row_type::iterator;
		it_t it = row.begin();
		auto ff = M_iter(&row,it);
		return ff;
	}

	/**
	 \brief Method returns iterator, on element after last one (stl-like)
	*/
	auto end()
	{
		using it_t = typename row_type::iterator;
		it_t it = row.end();
		auto ff = M_iter(&row,it);
		return ff;
	}

	/**
	 \brief operator for accessing element by given index.
	 \details
	 Operator creates reference to requested element

	*\param  pos index of element
	 \returns special inner type reference for element (Matrix<T,0,default_value>)
	*/
	auto operator [](std::size_t pos)
	{
		auto pos_tup = std::make_tuple(pos);
		auto res = typename Matrix<T,0,default_value>::
						template reference<decltype(pos_tup)>
						{
							.positions = pos_tup,
							.parent_col=row,
						};
		return res;
	}

};
