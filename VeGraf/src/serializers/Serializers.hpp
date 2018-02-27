#pragma once
#include "SerializersBase.hpp"
#include "BinarySerializer.hpp"
#include "JsonSerializer.hpp"
#include "SVGSerializer.hpp"
#include "SWFSerializer.hpp"
#include "VMLSerializer.hpp"

/** \defgroup Serializers
 * \brief This group contains of all possible document store formats
 */

/**
 \ingroup Serializers
 \brief Describes all available serialization types
 */
enum class Serializers
{
	Binary,		/**< Binary store format     			*/
	Json,		/**< JSON (debug) format     			*/
	SVG,		/**< Scalable Vector Graphics format    */
	SWF,		/**< Small Web Format			        */
	VML			/**< Vector Markup Language format      */
};
