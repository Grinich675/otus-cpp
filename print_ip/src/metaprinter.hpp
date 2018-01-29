#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <type_traits>
#include <tuple>

/**
 * \brief Namespace contains little helpers for meta_ip_printers.
 * \details Namespace contains helper structures and functions, used internally in meta_ip_printers *
 * */
namespace helpers
{

	/**
	 \brief internal helper structure for printing second through last  elements from tuple

	\details
	Internal structure for printing all elements from tuple.
	Actual printing happens in print function

	\tparam Tuple Type of tuple
	\tparam N index of element which will print this instance (actually index+1)
	*/
	template<class Tuple, std::size_t N>
	struct TuplePrinter {

		/**
		\details Internal function used for printing all elements from tuple.
		Recursively instantiate TruePrinter with next element and calls print function from it.
		Prints (using cout) delimiter and N'th element.
		\param [in] t object of Tuple type which will be printed
		 */
		static void print(const Tuple& t)
		{
			TuplePrinter<Tuple, N-1>::print(t);
			std::cout << "." << std::get<N-1>(t);
		}
	};

	/**
	 * \brief internal specialization of TuplePrinter for printing first element from tuple
	 * \details Specialization of TuplePrinter.
	 * Used to stop recursion calls and print first element from tuple
	 *
	 *\tparam Tuple Type of tuple
	 *\tparam 1 means call std::get<0>
	 */
	template<class Tuple>
	struct TuplePrinter<Tuple, 1> {

		/**
		\details Internal function used for printing first element from tuple.
		Needed to stop recursion.
		Prints out first element from tuple (accessed via std::get<0>).
		\param [in] t object of Tuple type which will be printed
		 */
		static void print(const Tuple& t)
		{
			std::cout << std::get<0>(t);
		}
	};

	/**
	 * \brief global helper function for printing all values from tuple
	 * \details helper function for printing tuples element by element using "." (dot) delimiter.
	 * Instantiate TuplePrinter structure and calls function print from it.
	 *
	 * \tparam Args parameter pack which consists of all types from tuple
	 * \param [in] tup Tuple which will be printed out
	 */
	template<class... Args>
	void print_tuple(const std::tuple<Args...>& tup)
	{

		TuplePrinter<decltype(tup), sizeof...(Args)>::print(tup);
		std::cout<<std::endl;

	}

	/**
	 *
	 * \brief global helper structure for compile-time checking if type T is STL-compatible container.
	 *
	 *\tparam T type to check
	 *
	 * \details Based on SFINAE check if type is STL-compatible container.
	 * Check based on fact that STL container have typedefs and methods:
	 * - value_type
	 * - size_type
	 * - allocator_type
	 * - iterator
	 * - const_iterator
	 * - size()
	 * - begin()
	 * - end ()
	 * - cbegin()
	 * - cend()
	 *
	 * For check instantiate is_container_helper like that
	 \code{.cpp}
	 is_container_helper<
	                typename T::value_type,
	                typename T::size_type,
	                typename T::allocator_type,
	                typename T::iterator,
	                typename T::const_iterator,
	                decltype(std::declval<T>().size()),
	                decltype(std::declval<T>().begin()),
	                decltype(std::declval<T>().end()),
	                decltype(std::declval<T>().cbegin()),
	                decltype(std::declval<T>().cend())
	                >
	 \endcode
	 if instantiation successful, then is_container will be equal to std::true_tupe
	 */
	template<typename T, typename _ = void>
	struct is_container : std::false_type {};

	/**
	 *\brief Internal helper structure used inside is_container.
	 *\details Empty structure. Needed only for fact can it be instantiated or not
	 */
	template<typename... Ts>
	struct is_container_helper {};

	///Specialization,which really checks. If it can't be created,then std::false_type(generic)
	template<typename T>
	struct is_container<
	        T,
	        std::conditional_t<
	            false,
	            is_container_helper<
	                typename T::value_type,
	                typename T::size_type,
	                typename T::allocator_type,
	                typename T::iterator,
	                typename T::const_iterator,
	                decltype(std::declval<T>().size()),
	                decltype(std::declval<T>().begin()),
	                decltype(std::declval<T>().end()),
	                decltype(std::declval<T>().cbegin()),
	                decltype(std::declval<T>().cend())
	                >,
	            void
	            >
	        > : public std::true_type {};


	/**
	 * \brief Compile-time conjunction.
	 */
	template<typename... Conds>
	  struct and_
	  : std::true_type
	  { };

	/**
	 * \brief Compile-time conjunction.
	 * \details SFINAE check if all conditions are true_type using recursing instantiation and_.
	 * \tparam Cond condition which will be checked by this instance
	 * \tparam Conds list of remaining conditions to check for
	 */
	template<typename Cond, typename... Conds>
	  struct and_<Cond, Conds...>
	  : std::conditional_t<Cond::value, and_<Conds...>, std::false_type>
	  { };

}


/**
 *\brief Namespace contains structures and functions for printing out IP addresses
 *\details Namespace contains structures and functions for printing out IP addresses using metaprogramming paradigm.
 * entrypoint ::meta_ip_print .
 * Supported types for IP:
 * - all integral types (int,long,double,float etc). prints byte by byte, using dot delimiter
 * - STL-compatible containers. prints element by element using dot delimiter
 * - std::string representation. Doesn't check format just prints as it is
 * - std::tuple where all elements type are same
 * */
namespace meta_ip_printers
{

	/**
	 *\brief Structure declaration used for routing to  print_ip function.
	 *\details Structure instantiated in ::meta_ip_print and used for routing to
	 * container_helper<T,true>::print_ip or container_helper<T,false>::print_ip function,
	 * depending if type is STL-compatible container. For checking used  helpers::is_container
	 *
	 * \tparam T type of element which contains IP address
	 */
	template<typename T, bool = helpers::is_container<T>::value>
	struct container_helper { };

	/**
	 *\brief Specialization of container_helper for printing out container types.
	 *\details Specialization of container_helper for printing out STL-compatible container types using container_helper<T,true>::print_ip function
	 *\tparam T type of container
	 *\tparam true means this is container type
	 */
    template<typename T>
    struct container_helper<T, true>
    {
    	/**
    	 *\brief Function used for printing out IP address contained in STL-compatible containers.
    	 *\tparam V type of container, by default equals to T from container_helper<T,true>
    	 *\details General-purpose function used for printing out IP addressed contained in STL-compatible containers.
    	 * Prints out element by element from container with "." (dot) delimiter, using std::cout and contained iterators
    	 *\param in container STL-compatible container
    	 * */
    	template <typename V= T>
        static void print_ip(V container)
        {
        	for(auto byte= container.begin();byte!= --container.end();++byte)
        	{
        		std::cout<<*byte<<".";
        	}
        	std::cout<<*(--container.end())<<std::endl;
        }

    	/**
    	 *\brief Overloading for STL-string.
    	 *\detais Function used for printing out IP address contained in STL-string.
    	 *
    	 *\warning Doesn't not check format of given string just prints it as it is.
    	 *
    	 *\param in str string containing IP, which will be printed out
    	 * */
    	static void print_ip(std::string str)
		{
    		std::cout<<str<<std::endl;
		}

    };

    /**
    	 *\brief Specialization of container_helper for printing out non-container types.
    	 *\details Specialization of container_helper for printing out non-container types using container_helper<T,false>::print_ip function
    	 *\tparam T type containing IP address
    	 *\tparam false means this is not a container type
    	 */
    template<typename T>
    struct container_helper<T, false>
    {

    	/**
    	 *\brief Structure declaration used for routing to  print_ip function.
    	 *\details Structure instantiated in container_helper<T, false>::print_ip and used for routing to
    	 * type_helper<V,true>::print_ip or type_helper<V,false>::print_ip function,
    	 * depending if type is integral. For checking used std::is_integral
    	 *
    	 * \tparam V type of element which contains IP address
    	 */
    	template<typename V,bool = std::is_integral<V>::value>
    	struct type_helper{};

    	/**
    	 *\brief Specialization of type_helper for printing out integral types.
    	 *\details Specialization of type_helper for printing out integral types (int,long,double,float etc)
    	 *\  using meta_ip_printers::container_helper< T, false >::type_helper< V, true >::print_ip function
    	 *\tparam V type element
    	 *\tparam true means this is integral type
    	 */
    	template<typename V>
    	struct type_helper<V,true>
    	{
    		/**
    		 *\brief Function used for printing out IP address contained in integral type.
    		 *\tparam V type of element, by default equals to T from container_helper<T,false>
    		 *\details General-purpose function used for printing out IP addressed contained in integral type.
    		 * Prints out byte by byte from element with "." (dot) delimiter, using std::cout
    		 *\param in t integral type element, containing IP address
    		 * */
      		static void print_ip( V t )
    		{
      			auto byte = reinterpret_cast<unsigned char*>(&t);

      			for(auto i=sizeof(V)-1;i>0;--i)
      		       std::cout<<(unsigned)byte[i]<<".";

      			std::cout<<(unsigned)byte[0]<<std::endl;
    	   	}
    	};

    	/**
    	 *\brief Specialization of type_helper for printing out non-integral types.
    	 *\details Specialization of type_helper for printing out non-integral types
    	 * using meta_ip_printers::container_helper< T, false >::type_helper< V, false >::print_ip function
    	 *\tparam V type element
    	 *\tparam false means this is not integral type
    	 */
    	template<class V>
    	struct type_helper<V,false>
    	{
    		/**
    		 *\brief Function used for doing nothing.
    		 *\tparam L type of element, by default equals to V from type_helper<V,false>
    		 *\details General-purpose function used doing nothing. May be used to throw "not implemented" assert.
    		 *\param in val element of type L.
    		 *\warning Behavior not implemented
    		 * */
        	template <typename L>
            static void print_ip(L val)
    	    {
    	    	//std::cout<<"non-integral. not implemented"<<std::endl;
    	    }

    		/**
    		 *\brief Function used for printing out IP address contained in tuples with same types.
    		 *\tparam L type of first element
    		 *\tparam Ts second through last type of tuple elements
    		 *\details
    		 * Prints out element by element from tuple with "." (dot) delimiter, using helpers::print_tuple, only
    		 * if all types in tuple are same. For check using helpers::and_
    		 *\param in tup tuple, containing IP address
    		 * */
        	template<typename L,typename ...Ts
			//enabling specialization only if ALL types from Ts same as L (first type in tuple)
			,typename std::enable_if< helpers::and_<std::is_same< L,Ts>...>::value ,bool>::type = true
        	>
        	static void print_ip(std::tuple<L,Ts ...> tup)
        	{
        				helpers::print_tuple(tup);
        	}

    	};

    	/**
    	 *\brief Function used for printing out IP address contained in non-container types.
    	 *\details
    	 * Instantiates type_helper and calls type_helper<V,true>::print_ip or type_helper<V,false>::print_ip
    	 *\param in val element containing IP address
    	 * */
        static void print_ip(T val)
        {
        	type_helper<T>::print_ip(val);
        }
    };


}

/**
 *\brief Function used for printing out IP address element of type T.
 *\tparam T type of element containing IP address
 *\details
 * Instantiates meta_ip_printers::container_helper and calls meta_ip_printers::container_helper<T,true>::print_ip or meta_ip_printers::container_helper<T,false>::print_ip
 *\param in t element containing IP address
 * */
template<class T>
void meta_ip_print( T t )
{
	meta_ip_printers::container_helper<T>::print_ip(t);
}
