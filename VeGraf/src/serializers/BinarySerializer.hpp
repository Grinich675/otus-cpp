#pragma once
#include "SerializersBase.hpp"



#include <fstream>

/**
  \ingroup Serializers
 \brief Class of Binary serialization format
 \details
 Class contains logic for serializing and deserializing vector object primitives to\from binary format
 \warning not Implemented.
 */
class BinarySerializer: public Serializer
{
	std::string filename{"autosave.bin"};
	std::fstream file;

public:

	~BinarySerializer() =default;

	BinarySerializer()
	{
		file.open(filename,std::ios::trunc|std::ios::binary|std::ios::in|std::ios::out);
	}

	BinarySerializer(std::string filename_)
	{
		filename=filename_;
		file.open(filename,std::ios::trunc|std::ios::binary|std::ios::in|std::ios::out);
	}

	void serialize( Circle* obj)override{};
	void serialize( Ellipse* obj) override{};
	void serialize( Parallelogram* obj)override{};
	void serialize( Rectangle* obj)override{};
	void serialize( Rhombus* obj)override{};
	void serialize( Triangle* obj)override{};
	void serialize( Hexagon* obj)override{};
	void serialize( Square* obj)override{};
	void serialize( Text* obj)override{};
	void serialize( Bezier* obj)override{};

	void Deserialize(std::list<VObject*>& result){};

	void flush() override{};

};
