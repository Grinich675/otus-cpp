#pragma once
#include "SerializersBase.hpp"



#include <iostream>
#include <fstream>

/**
  \ingroup Serializers
 \brief Class of VML serialization format
 \details
 Class contains logic for serializing and deserializing vector object primitives to\from VML format
 \warning not Implemented.
 */
class VMLSerializer:public Serializer
{
	std::fstream file;
		std::string filename;


		void init()
		{
			file.open(filename, std::ios::trunc|std::ios::out|std::ios::in);
		};

	public:
		VMLSerializer()=delete;

		~VMLSerializer()
		{
			if(file.is_open())
			{
				file.close();
			}
		};

		VMLSerializer(std::string&& _filename):
			filename(_filename)
		{};

		void serialize(Circle* obj)override{};
		void serialize( Ellipse* obj) override{};
		void serialize(Parallelogram* obj)override{};
		void serialize(Rectangle* obj)override{};
		void serialize(Rhombus* obj)override{};
		void serialize(Triangle* obj)override{};
		void serialize(Hexagon* obj)override{};
		void serialize(Square* obj)override{};
		void serialize(Text* obj)override{};
		void serialize(Bezier* obj)override{};

		void Deserialize(std::list<VObject*>& result)=0;

		void flush()override{};
};
