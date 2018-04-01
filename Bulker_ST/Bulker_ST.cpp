#include "Controller.hpp"
#include "Loggers.hpp"

int main(int argc, char** argv)
{

	int block_size = 1;
	if(argc > 1)
	{
		block_size = std::stoi(std::string(argv[1]));
	}

	Controller ctrl(block_size);

	ctrl.AddLogger(std::make_shared<ConsoleLogger>());
	ctrl.AddLogger(std::make_shared<FileLogger>());

	for(std::string line; std::getline(std::cin, line);)
	{
		ctrl.Process(line);
	}

	ctrl.Stop();


	return 0;
}
