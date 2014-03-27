#include "log.hpp"


int main()
{
	Logging::debug("this is test debug, %d\n", 1);
	Logging::info("this is test info, %d\n", 2);
	Logging::warning("this is test warning, %d\n", 3);
	Logging::basic_config(Logging::DEBUG, "LogTest");
	Logging::debug("this is test debug, %d\n", 4);
	Logging::info("this is test info, %d\n", 5);
	Logging::warning("this is test warning, %d\n", 6);
	return 0;
}