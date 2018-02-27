#include "VecPrimitives.h"
#include "serializers/SerializersBase.hpp"
#include <cmath>
#include <iostream>

void Ellipse::Draw()
{

	std::cout<<"##############"<<std::endl;
	std::cout<<"draw Ellipse:"<<std::endl;
	std::cout<<"Center:"<<VObject::center.x<<"."<<VObject::center.y<<std::endl;
	std::cout<<"--------------"<<std::endl;
}

void Ellipse::Assign(VObject* other)
{
	Ellipse* obj =  reinterpret_cast<Ellipse*> (other);

	this->attr=obj->attr;
	this->center=obj->center;
	this->focal_distance = obj->focal_distance;
	this->radius = obj->radius;
}

VObject* Ellipse::clone()
{
	return new Ellipse(this->attr,this->center,this->radius, this->focal_distance);

}

void Ellipse::save(Serializer *v) {
    v->serialize(this);
}

 bool Ellipse::Contains(Coordinates point)
 {
	 return false;
 }

void Circle::Draw()
{
	//use some optimizations, or just call draw from Ellipse
	//Ellipse::Draw();
	std::cout<<"##############"<<std::endl;
	std::cout<<"draw Circle:"<<std::endl;
	std::cout<<"Center:"<<VObject::center.x<<"."<<VObject::center.y<<std::endl;
	std::cout<<"--------------"<<std::endl;

}

void Circle::Assign(VObject* other)
{
	Circle* obj =  reinterpret_cast<Circle*> (other);
	this->attr=obj->attr;
	this->center=obj->center;
	this->radius=obj->radius;
}

VObject* Circle::clone()
{
	return new Circle(this->attr,this->center,this->radius);
}

bool Circle::Contains(Coordinates point)
{
	auto a= std::fabs(VObject::center.x - point.x);
	auto b = std::fabs(VObject::center.y - point.y);

	auto h = std::sqrt(a*a+b*b);

	if( h > radius)
	{
		return false;
	}

	return true;
}

void Circle::save(Serializer *v)
{
    v->serialize(this);
}


void Parallelogram::Draw()
{
	std::cout<<"##############"<<std::endl;
	std::cout<<"draw Parallelogram:"<<std::endl;
	std::cout<<"Center:"<<VObject::center.x<<"."<<VObject::center.y<<std::endl;
	std::cout<<"--------------"<<std::endl;
}

void Parallelogram::Assign(VObject* other)
{
	Parallelogram* obj =  reinterpret_cast<Parallelogram*> (other);

	this->attr=obj->attr;
	this->center=obj->center;
	this->first_angle=obj->first_angle;
	this->second_angle=obj->second_angle;
	this->length=obj->length;
	this->width=obj->width;
}

VObject* Parallelogram::clone()
{
	return new Parallelogram(this->attr,this->center,this->length,this->width,this->first_angle,this->second_angle);
}

bool Parallelogram::Contains(Coordinates point)
{
	return	false;
}

void Parallelogram::save(Serializer *v) {
    v->serialize(this);
}


void Rectangle::Draw()
{
	std::cout<<"##############"<<std::endl;
	std::cout<<"draw Rectangle:"<<std::endl;
	std::cout<<"Center:"<<VObject::center.x<<"."<<VObject::center.y<<std::endl;
	std::cout<<"--------------"<<std::endl;
}

void Rectangle::Assign(VObject* other)
{
	Rectangle* obj = reinterpret_cast<Rectangle*>(other);

	this->attr=obj->attr;
	this->center=obj->center;
	this->first_angle=obj->first_angle;
	this->second_angle=obj->second_angle;
	this->length=obj->length;
	this->width=obj->width;
}

bool Rectangle::Contains(Coordinates point)
{
	return	false;
}

VObject* Rectangle::clone()
{
	return new Rectangle(this->attr,this->center,this->length,this->width);
}

void Rectangle::save(Serializer *v) {
    v->serialize(this);
}


void Rhombus::Draw()
{
	std::cout<<"##############"<<std::endl;
	std::cout<<"draw Rhombus:"<<std::endl;
	std::cout<<"Center:"<<VObject::center.x<<"."<<VObject::center.y<<std::endl;
	std::cout<<"--------------"<<std::endl;
}

void Rhombus::Assign(VObject* other)
{
	Rhombus* obj = reinterpret_cast<Rhombus*>(other);

	this->attr=obj->attr;
	this->center=obj->center;
	this->first_angle=obj->first_angle;
	this->second_angle=obj->second_angle;
	this->length=obj->length;
	this->width=obj->width;
}

bool Rhombus::Contains(Coordinates point)
{
	return	false;
}

VObject* Rhombus::clone()
{
	return new Rhombus(this->attr,this->center,this->length,this->first_angle,this->second_angle);
}

void Rhombus::save(Serializer *v) {
    v->serialize(this);
}

void Square::Draw()
{
	std::cout<<"##############"<<std::endl;
	std::cout<<"draw Square:"<<std::endl;
	std::cout<<"Center:"<<VObject::center.x<<"."<<VObject::center.y<<std::endl;
	std::cout<<"--------------"<<std::endl;
}

void Square::Assign(VObject* other)
{
	Square* obj = reinterpret_cast<Square*> (other);

	this->attr= obj->attr;
	this->center=obj->center;
	this->first_angle=obj->first_angle;
	this->second_angle=obj->second_angle;
	this->length=obj->length;
	this->width=obj->width;


}

bool Square::Contains(Coordinates point)
{
	return	false;
}

VObject* Square::clone()
{
	return new Square(this->attr,this->center,this->length);
}

void Square::save(Serializer *v) {
    v->serialize(this);
}

void Triangle::Draw()
{
	std::cout<<"##############"<<std::endl;
	std::cout<<"draw Triangle:"<<std::endl;
	std::cout<<"Center:"<<VObject::center.x<<"."<<VObject::center.y<<std::endl;
	std::cout<<"--------------"<<std::endl;
}

void Triangle::Assign(VObject* other)
{
	Triangle* obj = reinterpret_cast<Triangle*> (other);

	this->attr=obj->attr;
	this->center=obj->center;
	this->A=obj->A;
	this->B=obj->B;
	this->C=obj->C;
}


bool Triangle::Contains(Coordinates point)
{
	return	false;
}

VObject* Triangle::clone()
{
	return new Triangle(this->attr,this->center,this->A,this->B,this->C);
}

void Triangle::save(Serializer *v) {
    v->serialize(this);
}


void Hexagon::Draw()
{
	std::cout<<"##############"<<std::endl;
	std::cout<<"draw Hexagon:"<<std::endl;
	std::cout<<"Center:"<<VObject::center.x<<"."<<VObject::center.y<<std::endl;
	std::cout<<"--------------"<<std::endl;
}

void Hexagon::Assign(VObject* other)
{
	Hexagon* obj = reinterpret_cast<Hexagon*>(other);

	this->attr=obj->attr;
	this->center=obj->center;
	this->circumradius=obj->circumradius;
}

bool Hexagon::Contains(Coordinates point)
{
	return	false;
}

VObject* Hexagon::clone()
{
	return new Hexagon(this->attr,this->center,this->circumradius);
}

void Hexagon::save(Serializer *v) {
    v->serialize(this);
}


void Text::Draw()
{
	std::cout<<"##############"<<std::endl;
	std::cout<<"draw text:"<<this->str<<std::endl;
	std::cout<<"Center:"<<VObject::center.x<<"."<<VObject::center.y<<std::endl;
	std::cout<<"--------------"<<std::endl;
}

void Text::Assign(VObject* other)
{
	Text* obj = reinterpret_cast<Text*> (other);

	this->attr=obj->attr;
	this->center=obj->center;
	this->font_size=obj->font_size;
	this->str.assign(obj->str);
}

bool Text::Contains(Coordinates point)
{
	return	false;
}

VObject* Text::clone()
{
	return new Text(this->attr,this->center,this->str,this->font_size);
}

void Text::save(Serializer *v) {
    v->serialize(this);
}

void Bezier::Draw()
{
	std::cout<<"##############"<<std::endl;
	std::cout<<"draw Bezier:"<<std::endl;
	std::cout<<"Center:"<<VObject::center.x<<"."<<VObject::center.y<<std::endl;
	std::cout<<"order:"<<control_points.size()-1<<std::endl;
	std::cout<<"--------------"<<std::endl;
}

void Bezier::Assign(VObject* other)
{
	Bezier* obj = reinterpret_cast<Bezier*> (other);

	this->attr= obj->attr;
	this->center=obj->center;
	this->control_points=obj->control_points;
}

VObject* Bezier::clone()
{
	return new Bezier(this->attr,this->center,this->control_points);
}

bool Bezier::Contains(Coordinates point)
{
	return	false;
}

void Bezier::save(Serializer *v) {
    v->serialize(this);
}
