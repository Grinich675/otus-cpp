//#include <iostream>
//#include "ObjectAttributes.hpp"
//#include "serializers/Serializers.hpp"
//#include "VecPrimitives.h"
#include "Controller.hpp"


//#include <list>

int main() {


/*	Coordinates point {
		.x=15,
		.y=14};*/

	Color placeholder{
		.r=255,
		.g=255,
		.b=255,
		.a=0};

	Attributes a {
		.border_color=placeholder,
		.fill=placeholder,
		.border_thickness=1};


	std::vector<Coordinates> vec {{.x=1,.y=0},{.x=2,.y=0},{.x=3,.y=0},{.x=4,.y=0} };

	Serializer* serializer= new JsonSerializer();

	auto controller = VeGrafController();

	controller.Open(serializer);

	//add Bezier
	VObject* el = new Bezier(a,Coordinates{.x=15,.y=15},vec);
	controller.Add(el);
	delete el;
	std::cout<<std::endl<<std::endl<<std::endl;
	//--------------------

	//Add Hexagon
	el = new Hexagon(a,Coordinates{.x=40,.y=40},10.0);
	controller.Add(el);
	delete el;
	std::cout<<std::endl<<std::endl<<std::endl;
	//--------------------

	//Autosave
	std::cout<<"AutoSave:"<<std::endl;
	controller.AutoSave();
	std::cout<<std::endl<<std::endl<<std::endl;
	//--------------------

	//Add Circle
	el= new Circle(a, Coordinates{.x=0,.y=0},5);
	controller.Add(el);
	delete el;
	std::cout<<std::endl<<std::endl<<std::endl;
	//--------------------

	//Select circle
	auto sel = controller.Select(Coordinates{.x=2,.y=2});
	//--------------------

	//Modify Circle
	auto edited =sel->clone();
	edited->center = Coordinates{.x=5,.y=10};
	controller.Change(sel,edited);
	delete sel;
	delete edited;
	std::cout<<std::endl<<std::endl<<std::endl;
	//--------------------

	//Remove Circle
	controller.Delete(Coordinates{.x=5,.y=10});
	std::cout<<std::endl<<std::endl<<std::endl;
	//--------------------

	//Undo remove
	controller.Undo();
	std::cout<<std::endl<<std::endl<<std::endl;
	//--------------------

	//Save document
	controller.Save(serializer);
	std::cout<<std::endl<<std::endl<<std::endl;
	//--------------------

	//Clear everything
	controller.Clear();
	//--------------------

	//try save empty doc
	controller.Save(serializer);


	delete serializer;


	return 0;
}
