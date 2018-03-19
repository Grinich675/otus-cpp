#include "Controller.hpp"
#include "Loggers.hpp"


int main(int argc, char** argv)
{

	int block_size = 1;
	int floggers=2;

	if(argc > 1)
	{
		block_size = std::stoi(std::string(argv[1]));
	}

	Controller ctrl(block_size);

#ifdef PERF_TEST

	if(argc > 2)
	{
		floggers = std::stoi(std::string(argv[2]));
	}

#endif

	ctrl.AddLogger(std::make_unique<ConsoleLogger>(1));
	ctrl.AddLogger(std::make_unique< FileLogger>(floggers));

	for(std::string line; std::getline(std::cin, line);)
	{
		ctrl.Process(line);
	}

	ctrl.Stop();
	//std::this_thread::sleep_for(std::chrono::seconds(5));
	ctrl.RemoveLoggers();


	return 0;
}
