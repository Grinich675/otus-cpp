#pragma once
#include "ObjectAttributes.hpp"

#include <string>
#include <vector>

/** \defgroup Primitives Vector graphic primitives
 * \brief This group contains of all primitives, used in program
 *  @{
 */


/**
 \brief Base class of all vector graphic primitives
 \details
 Class used as base for all vector graphic primitives.
 Every primitive inherits from it.
 */
class VObject
{
public:

	/**
	 \brief Common object attributes
	 */
	Attributes attr;

	/**
	 \brief Center of vector primitive
	 */
	Coordinates center;

	VObject()=delete;
	VObject(VObject&)=delete;

	VObject(Attributes _attr,Coordinates _cord):attr(_attr),center(_cord){};

	/**
	 \brief Function for copying the values to an object.

	\details
	 Function copies all the values of given object to a self object.

	\param VObject given object
	*/
	virtual void Assign(VObject*)=0;

	/**
	 \brief Function for creating deep copy

	\details
	 Function creates new object equal to *this

	\return VObject* deep copy of object
	*/
	virtual VObject* clone () =0;

	/**
	 \brief Function drawing this object.

	\details
	 Function implements logic of drawing this element.
	*/
	virtual void Draw() =0;

	virtual ~VObject() =default;

	/**
	 \brief Function for checking coordinates for belonging

	\details
	 Function determine if given coordinates belongs to this object

	\param Coordinates coordinates which will be checked
	\return
		true - if given coordinates belongs to this object
	*/
	virtual bool Contains(Coordinates)=0;

	/**
	 \brief Function for saving object

	\details
	Function which implements logic of saving object,using given serializer

	\param Serializer object of Serilizer class,which used for saving
	*/
	virtual void save(class Serializer * ) = 0;

};


/**
 \brief Class defines Ellipse primitive
 \details
 Class used as base for all Ellipse-based vector graphic primitives.
 Inherits from VObject.
 */
class Ellipse:public VObject
{
public:

	/**
	 \brief Radius of Ellipse
	 \details
	Radius of Ellipse. \a"a" in terms of wikipedia
	https://en.wikipedia.org/wiki/Ellipse#Definition_of_an_ellipse_as_locus_of_points
	 */
	double radius;// a

	/**
	 \brief Focal distance of Ellipse
	 \details
	 The distance \a "c"  of the foci to the center
	 https://en.wikipedia.org/wiki/Ellipse#Definition_of_an_ellipse_as_locus_of_points
	 */
	double focal_distance;// c


	//|F1 F2|=2c
	//center = middle of F1F2
	//|F1 M| + |F2 M|= 2a

	Ellipse(Attributes _attr, Coordinates _center, double _radius, double _focal_distance):
		VObject(_attr,_center),radius(_radius),focal_distance(_focal_distance)
	{};


	void Draw() override;

	void Assign(VObject*) override;

	VObject* clone() override ;

	bool Contains(Coordinates)override;

	void save(class Serializer* ser) override;


	~Ellipse()=default;
};

/**
 \brief Class defines Circle primitive
 \details
 Class is a special case of Ellipse,when focal distance eq 0
  Inherits from Ellipse.
 */
class Circle:public  Ellipse
{

public :
	Circle (Attributes _attr, Coordinates _center, double _radius):
		Ellipse(_attr,_center,_radius,0)
	{};

	~Circle()=default;

	VObject* clone() override ;

	void Draw() override;

	void Assign(VObject*) override;

	bool Contains(Coordinates)override;

	void save(class Serializer* ser)override;
};

/**
 \brief Class defines Parallelogram primitive
 \details
 Class used as base for all Parallelogram-based vector graphic primitives.
 Inherits from VObject
 */
class Parallelogram: public VObject
{
public:

	/**
	 \brief First pair of sides measure
	 */
	double length;

	/**
	 \brief Second pair of sides measure
	 */
	double width;

	/**
	 \brief first pair of angles measure
	 */
	double first_angle;

	/**
	 \brief second pair of angles measure
	 */
	double second_angle;

	Parallelogram(Attributes _attr, Coordinates _center,
			double _length, double _width, double _first_angle,double _second_angle) :
	VObject(_attr,_center),length(_length),width(_width),first_angle(_first_angle),second_angle(_second_angle)
	{

	};

	VObject* clone() override ;

	void Draw() override;

	void Assign(VObject*) override;

	bool Contains(Coordinates)override;

	void save(class Serializer* ser)override;
};

/**
 \brief Class defines Rectangle primitive
 \details
 Class is a special case of Parallelogram,when all four angles of equal size
  Inherits from Parallelogram.
 */
class Rectangle: public Parallelogram
{
public:

	Rectangle(Attributes& _attr, Coordinates& _center,double _length, double _width):
		Parallelogram(_attr,_center,_length,_width,90.0,90.0)
	{
	};

	VObject* clone() override ;

	/*
	 *by default do not need Draw,cause its Parallelogram. May be used for optimization
	*/
	void Draw() override;

	void Assign(VObject*) override;

	bool Contains(Coordinates)override;

	void save(class Serializer* ser)override;
};

/**
 \brief Class defines Rhombus primitive
 \details
 Class is a special case of Parallelogram,when all four sides of equal length
  Inherits from Parallelogram.
 */
class Rhombus:public Parallelogram
{
public:

	Rhombus(Attributes _attr, Coordinates _center,double _length,double _first_angle,double _second_angle ):
		Parallelogram(_attr,_center,_length,_length,_first_angle,_second_angle)
	{

	};

	VObject* clone() override ;

	/*
	 *by default do not need Draw,cause its Parallelogram. May be used for optimization*/
	void Draw() override;

	void Assign(VObject*) override;

	bool Contains(Coordinates)override;

	void save(class Serializer* ser)override;
};

/**
 \brief Class defines Square primitive
 \details
 Class is a special case of Parallelogram,when four sides of equal length and angles of equal size
  Inherits from Parallelogram.
 */
class Square:public Parallelogram
{
public:

	Square(Attributes _attr, Coordinates _center,double _length):
		Parallelogram(_attr,_center,_length,_length,90.0,90.0)
	{

	};

	VObject* clone() override ;

	/*
	 *by default do not need Draw,cause its Parallelogram. May be used for optimization*/
	void Draw() override;

	void Assign(VObject*) override;

	bool Contains(Coordinates)override;

	void save(class Serializer* ser)override;
};

/**
 \brief Class defines Triangle primitive
 \details
 Class defines Triangle primitive.
 Inherits from VObject.
 */
class Triangle:public VObject
{
public:

	Coordinates A;//!< First vertex
	Coordinates B;//!< Second vertex
	Coordinates C;//!< Third vertex

	Triangle(Attributes _attr,Coordinates _center, Coordinates _a, Coordinates _b, Coordinates _c):
	VObject(_attr,_center),A(_a),B(_b),C(_c)
	{

	};

	VObject* clone() override ;

	void Draw() override;

	void Assign(VObject*) override;

	bool Contains(Coordinates)override;

	void save(class Serializer* ser)override;
};

/**
 \brief Class defines Hexagon primitive
 \details
 Class defines Hexagon primitive.
 Inherits from VObject.
 */
class Hexagon:public VObject
{
public:
	double circumradius;//!< Radius of circumscribed circle

	Hexagon(Attributes _attr,Coordinates _center, double _radius):
		VObject(_attr, _center),circumradius(_radius)
	{};


	~Hexagon()=default;

	VObject* clone() override ;

	void Draw() override;

	void Assign(VObject*) override;

	bool Contains(Coordinates)override;

	void save(class Serializer* ser)override;
};

/**
 \brief Class defines Text primitive
 \details
 Class defines Text(simple string) primitive.
 Inherits from VObject.
 */
class Text:public VObject
{
public:

	std::string str;//!< Content of Text object

	unsigned int font_size;//!< Size of font, used for drawing

	VObject* clone() override ;

	Text(Attributes _attr, Coordinates _center,std::string& text,unsigned int _font_size ):
		VObject(_attr,_center),str(text),font_size(_font_size)
	{};

	void Draw()override;

	void Assign(VObject*) override;

	bool Contains(Coordinates)override;

	void save(class Serializer* ser);

};

/**
 \brief Class defines Bézier curve primitive
 \details
 Class defines Bézier curve primitive.
 Inherits from VObject.
 */
class Bezier:public VObject
{
public:
	std::vector<Coordinates> control_points;//!< Control points, which defines curve

	Bezier(Attributes _attr,Coordinates center,std::vector<Coordinates>& points):
		VObject(_attr,center),control_points(points)
	{
	};

	VObject* clone() override ;

	void Draw() override;

	void Assign(VObject*) override;

	bool Contains(Coordinates)override;

	void save(class Serializer* ser)override;

};

/** @} */

