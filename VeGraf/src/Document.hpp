#pragma once
#include "VecPrimitives.h"
#include "serializers/SerializersBase.hpp"

#include <list>
#include <algorithm>

/** \defgroup Document Document-object and related
 * \brief This group contains of Document object
 */

/**
 \ingroup Document
  \brief Alias for internal_storage iterator
 */
using document_iterator = std::list<VObject*>::iterator;


/**
 \ingroup Document
 \brief Class describes Document object
 \details
 Document object represents collection of all added to plane primitives
 */
class Document
{
	std::list<VObject*> internal_storage;//!< Storage for added primitives

public:
	Document()=default;

	/**
	\brief Function for adding vector primitive to this document.

	\details
	 Function add given object to the end of stored collection.

	\param [in] obj given object
	*/
	void add(VObject* obj)
	{
		internal_storage.push_back(obj);
	};

	/**
	\brief Function for adding vector primitive to this document at given position.

	\details
	 Function add given object after given position in stored collection.

	\param it given position
	\param obj given object
	*/
	void add(document_iterator it,VObject* obj)
	{
		internal_storage.insert(it,obj);
	};


	/**
	\brief Function for  removing vector primitive from this document.

	\details
	 Function removes given object, if there is one, from collection.

	\param obj object to remove
	\return document_iterator position of removed element if it was in collection, or space after collection ( end() )
	*/
	document_iterator remove(VObject* obj)
	{
			auto position= std::find(internal_storage.begin(),internal_storage.end(),obj);
			if(position!=internal_storage.end())
			{
				++position;
				internal_storage.remove(obj);

			}
			return position;

	};

	/**
	\brief Function for selecting element from collection by coordinates.

	\details
	 Function checks all elements in reverse direction, which contains point with given coordinates, and returns first suitable.

	\param point point on plane
	\return first suitable object from collection or, if no such - nullptr
	*/
	VObject* select(Coordinates point)
	{
		for(auto ret = internal_storage.rbegin(); ret!=internal_storage.rend();++ret)
		{
			if((*ret)->Contains(point))
				return *ret;
		}

		return nullptr;
	};

	/**
	\brief Function erasing internal storage.
	\details
	 Function clears internal storage.
	*/
	void clear()
	{

		internal_storage.clear();

	};

	/**
	\brief Function for saving current document.
	\details
	 Function saves all elements from internal storage, using given serializer object.
	\warning contains debug stub output for empty doc
	\param Serializer object,containing logic for serialization
	*/
	void save(Serializer* ser)
	{
		if(internal_storage.empty())
		{
			std::cout<<"##############"<<std::endl;
			std::cout<<"Empty doc"<<std::endl;
			std::cout<<"--------------"<<std::endl;
			return;
		}

		for (auto obj : internal_storage)
		{
			obj->save(ser);
		}
	};

	/**
	\brief Function for opening document.
	\details
	 Function calls Deserialize from given serializer object, which fills in internal storage
	\param Deserializer object,containing logic for deserialization
	*/
	void open(Serializer* Deserializer)
	{
		internal_storage.clear();
		Deserializer->Deserialize(internal_storage);
	};

	/**
	\brief Function drawing all elements from collection.
	\details
	 Function calls method draw for each element (direct direction) in internal collection.
	\warning contains debug stub output (stdout)
	*/
	void Draw()
	{
		std::cout<<"======Document======"<<std::endl;
		for(auto obj: internal_storage)
		{
			obj->Draw();
		}
		std::cout<<"======End======"<<std::endl;
	};

};


