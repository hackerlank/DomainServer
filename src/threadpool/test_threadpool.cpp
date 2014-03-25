#include "singleton.hpp"
#include <iostream>
#include <unistd.h>
using namespace std;

void *p1(void *arg);
void *p2(void *arg);
void *p3(void *arg);

int main()
{
	ThreadPoolSingleton::instance().create(4);
	sleep(3);
	ThreadPoolSingleton::instance().add_task(p1, NULL);
	ThreadPoolSingleton::instance().add_task(p2, NULL);
	ThreadPoolSingleton::instance().add_task(p3, NULL);
	ThreadPoolSingleton::instance().add_task(p1, NULL);
	ThreadPoolSingleton::instance().add_task(p2, NULL);
	ThreadPoolSingleton::instance().add_task(p3, NULL);
	ThreadPoolSingleton::instance().add_task(p1, NULL);
	ThreadPoolSingleton::instance().add_task(p2, NULL);
	ThreadPoolSingleton::instance().add_task(p3, NULL);
	ThreadPoolSingleton::instance().add_task(p1, NULL);
	ThreadPoolSingleton::instance().add_task(p2, NULL);
	ThreadPoolSingleton::instance().add_task(p3, NULL);

	sleep(10);
	cout << "task all done\n";
	return 0;
}

void *p1(void *arg)
{
	cout << "this is function p1\n";
	return NULL;
}

void *p2(void *arg)
{
	cout << "this is function p2\n";
	return NULL;
}

void *p3(void *arg)
{
	cout << "this is function p3\n";
	sleep(1);
	return NULL;
}