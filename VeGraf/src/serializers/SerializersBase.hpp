#pragma once
#include "../VecPrimitives.h"
#include <list>

/**
 \ingroup Serializers
 \brief Base class of serialization format
 \details
 Class used as base for all serialization format.
 Every serialization format inherits from it.
 */
class Serializer
{
public:
	virtual ~Serializer() =default;

	Serializer()=default;

	/**
	 \brief Function for serializing Circle object primitive.

	\details
	 Function serializes object of Circle type primitive.

	\param obj Circle object to serialize
	*/
	virtual void serialize( Circle* obj)=0;

	/**
	 \brief Function for serializing Ellipse object primitive.

	\details
	 Function serializes object of Ellipse type primitive.

	\param obj Ellipse object to serialize
	*/
	virtual void serialize( Ellipse* obj) =0;

	/**
	 \brief Function for serializing Parallelogram object primitive.

	\details
	 Function serializes object of Parallelogram type primitive.

	\param obj Parallelogram object to serialize
	*/
	virtual void serialize( Parallelogram* obj)=0;

	/**
	 \brief Function for serializing Rectangle object primitive.

	\details
	 Function serializes object of Rectangle type primitive.

	\param obj Rectangle object to serialize
	*/
	virtual void serialize( Rectangle* obj)=0;

	/**
	 \brief Function for serializing Rhombus object primitive.

	\details
	 Function serializes object of Rhombus type primitive.

	\param obj Rhombus object to serialize
	*/
	virtual void serialize( Rhombus* obj)=0;

	/**
	 \brief Function for serializing Triangle object primitive.

	\details
	 Function serializes object of Triangle type primitive.

	\param obj Triangle object to serialize
	*/
	virtual void serialize( Triangle* obj)=0;

	/**
	 \brief Function for serializing Hexagon object primitive.

	\details
	 Function serializes object of Hexagon type primitive.

	\param obj Hexagon object to serialize
	*/
	virtual void serialize( Hexagon* obj)=0;

	/**
	 \brief Function for serializing Square object primitive.

	\details
	 Function serializes object of Square type primitive.

	\param obj Square object to serialize
	*/
	virtual void serialize( Square* obj)=0;

	/**
	 \brief Function for serializing Text object primitive.

	\details
	 Function serializes object of Text type primitive.

	\param obj Text object to serialize
	*/
	virtual void serialize( Text* obj)=0;

	/**
	 \brief Function for serializing Bezier curve object primitive.

	\details
	 Function serializes object of Bezier curve type primitive.

	\param obj Bezier curve object to serialize
	*/
	virtual void serialize( Bezier* obj)=0;

	/**
	 \brief Function for deserializing object primitives.

	\details
	 Function deserializes object primitives.

	\param result storage for deserialized objects
	*/
	virtual void Deserialize(std::list<VObject*>& result)=0;


	virtual void flush()=0;
};
