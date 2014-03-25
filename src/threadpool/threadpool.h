#ifndef THREADPOOL_H
#define THREADPOOL_H 

#include "taskobj.hpp"
#include "exceptions.hpp"
#include <queue>
#include <vector>
#include <memory>
#include <pthread.h>

class ThreadPool
{
protected:
	ThreadPool();
	~ThreadPool();
public:
	typedef void *(*task_ptr)(void *);
	static bool create(unsigned int max_size);
	static bool destroy();
	static bool add_task(task_ptr tp, void *arg);

private:
	static void *thread(void *arg);
	void wait_for_wakeup_signal();
	std::shared_ptr<TaskObject> fetch_one_task();
	void wakeup_one_thread();

private:
	std::queue<std::shared_ptr<TaskObject> > 	m_task_queue_;
	pthread_mutex_t 							m_task_queue_mutex_;
	unsigned int 								m_queue_max_size_;
	std::vector<pthread_t> 						m_thread_id_vector_;
	pthread_mutex_t								m_wakeup_mutex_;
	pthread_cond_t								m_wakeup_cond_;
	bool 										m_pool_destroyed_;
	static ThreadPool 							*s_pool_instance_ptr;
};


#endif //THREADPOOL_H