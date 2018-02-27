#pragma once
#include "SerializersBase.hpp"


#include <iostream>

/**
  \ingroup Serializers
 \brief Class of JSON serialization format
 \details
 Class contains logic for serializing and deserializing vector object primitives to\from JSON format
 \warning Implemented for debug purposes only.
 */
class JsonSerializer : public Serializer
{

	void serialize(Color clr, std::string&& name)
		{
			std::cout<<"\""<<name<<"\":{";
			std::cout<<"\"red\":"<<(unsigned int)clr.r<<",";
			std::cout<<"\"green\":"<<(unsigned int)clr.g<<",";
			std::cout<<"\"blue\":"<<(unsigned int)clr.b<<",";
			std::cout<<"\"alpha\":"<<(unsigned int)clr.a;
			std::cout<<"}";
		}

	void serialize(Attributes& _attr)
	{
		std::cout<<"\"properties\": {";
		this->serialize(_attr.fill,"fill color");
		std::cout<<",";
		this->serialize(_attr.border_color,"border color");
		std::cout<<",";
		std::cout<<"\"border thickness\":"<<_attr.border_thickness;
		std::cout<<"}";
	};


	void serialize(VObject* obj,std::string&& type)
	{
        std::cout << "\"type\": \""<<type<<"\",";
        serialize(obj->attr);
        std::cout<<",";
        std::cout<<"\"center\":["<<obj->center.x<<","<<obj->center.y<<"]";
	}

public:

	JsonSerializer()=default;

	~JsonSerializer()=default;

	void serialize(Circle *obj) override
    {
		auto pointer = (Circle*)obj;
        std::cout << "{";
        serialize((VObject*)pointer,"Circle");
        std::cout<<",";
        std::cout<<"\"radius\":"<<pointer->radius;
        std::cout<<"}";
    }

    void serialize( Ellipse *obj) override
    {
		auto pointer = (Ellipse*)obj;
        std::cout << "{";
        serialize((VObject*)pointer,"Ellipse");
        std::cout<<",";
        std::cout<<"\"focal distance\":"<<pointer->focal_distance;
        std::cout<<",";
        std::cout<<"\"radius\":"<<pointer->radius;
        std::cout<<"}";
    }

    void serialize(  Parallelogram* obj)override
    {
		auto pointer = (Parallelogram*)obj;
    	std::cout << "{";
		serialize((VObject*)pointer,"Parallelogram");
		std::cout<<",";
        std::cout<<"\"sides\":["<<pointer->width<<","<<pointer->length<<"]";
		std::cout<<",";
        std::cout<<"\"angles\":["<<pointer->first_angle<<","<<pointer->second_angle<<"]";
    	std::cout << "}";

    };

	void serialize(  Rectangle* obj)override
	{
		auto pointer = (Rectangle*)obj;
    	std::cout << "{";
		serialize((VObject*)pointer,"Rectangle");
		std::cout<<",";
        std::cout<<"\"sides\":["<<pointer->width<<","<<pointer->length<<"]";
    	std::cout << "}";
	};

	void serialize( Rhombus* obj) override
	{
		auto pointer = (Rhombus*)obj;
    	std::cout << "{";
		serialize((VObject*)pointer,"Rhombus");
		std::cout<<",";
        std::cout<<"\"sides\":"<<pointer->length;
		std::cout<<",";
        std::cout<<"\"angles\":["<<pointer->first_angle<<","<<pointer->second_angle<<"]";
    	std::cout << "}";
	};

	void serialize(  Triangle* obj)override
	{
		auto pointer = (Triangle*)obj;
        std::cout << "{";
        serialize((VObject*)pointer,"Triangle");
        std::cout<<",";

        std::cout<<"\"Vertexes\":[";
		std::cout<<"["<<pointer->A.x<<","<<pointer->A.y<<"],";
		std::cout<<"["<<pointer->B.x<<","<<pointer->B.y<<"],";
		std::cout<<"["<<pointer->C.x<<","<<pointer->C.y<<"]";
        std::cout<<"]";

        std::cout<<"}";

	};

	void serialize(  Hexagon* obj)override
	{
		auto pointer = (Hexagon*)obj;
        std::cout << "{";
        serialize((VObject*)pointer,"Hexagon");
        std::cout<<",";
        std::cout<<"\"circumradius\":"<<pointer->circumradius;
        std::cout<<"}";
	};

	void serialize(  Square* obj)override
	{
		auto pointer = (Square*)obj;
    	std::cout << "{";
		serialize((VObject*)pointer,"Square");
		std::cout<<",";
        std::cout<<"\"sides\":"<<pointer->length;
    	std::cout << "}";
	};

	void serialize(  Text* obj)override
	{
		auto pointer = (Text*)obj;
    	std::cout << "{";
		serialize((VObject*)pointer,"Text");
		std::cout<<",";
        std::cout<<"\"content\": \""<<pointer->str<<"\"";
		std::cout<<",";
        std::cout<<"\"font size\":"<<pointer->font_size;
    	std::cout << "}";
	};

	void serialize( class Bezier* obj) override
	{
		auto pointer = (Bezier*)obj;

        std::cout << "{";
        serialize((VObject*)pointer,"Bezier");
        std::cout<<",";

        std::cout<<"\"Control points\":[";
			for(auto point= pointer->control_points.begin();point!=--pointer->control_points.end();++point)
			{
				std::cout<<"["<<point->x<<","<<point->y<<"],";

			}

			std::cout<<"["<< (--pointer->control_points.end())->x<<","<<(--pointer->control_points.end())->y <<"]";
        std::cout<<"]";

        std::cout<<"}";
	}

	/**
	\warning  For debug purposes adds Circle object to result collection .
	*/
	void Deserialize(std::list<VObject*>& result) override
	{
		Attributes a
		{
			Color{.r=255,.g=255,.b=0,.a=255},
			Color{.r=255,.g=255,.b=0,.a=255},
			1
		};
			Circle* cr = new Circle(a,Coordinates{.x=0,.y=0},2);

			result.push_back(cr);
	};

	void flush()override{};

};

