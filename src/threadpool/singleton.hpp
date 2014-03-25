#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include "threadpool.h"


class ThreadPoolSingleton: private ThreadPool
{
private:
	ThreadPoolSingleton(){}
	~ThreadPoolSingleton(){}
	ThreadPoolSingleton(const ThreadPoolSingleton &other){}
public:
	static ThreadPool &instance();
};


ThreadPool &ThreadPoolSingleton::instance()
{
	static ThreadPoolSingleton single_object;
	return single_object;
}

#endif //SINGLETON_HPP