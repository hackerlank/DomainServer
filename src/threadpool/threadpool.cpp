#include "threadpool.h"
#include <iostream>

ThreadPool *ThreadPool::s_pool_instance_ptr = NULL;

ThreadPool::ThreadPool()
{
	pthread_mutex_init(&m_wakeup_mutex_, NULL);
	pthread_cond_init(&m_wakeup_cond_, NULL);
	m_pool_destroyed_ = false;
	ThreadPool::s_pool_instance_ptr = this;
}

ThreadPool::~ThreadPool()
{
	pthread_mutex_destroy(&m_wakeup_mutex_);
	pthread_cond_destroy(&m_wakeup_cond_);
}

// this will block until get a wakeup signal 
void ThreadPool::wait_for_wakeup_signal()
{
	pthread_mutex_lock(&m_wakeup_mutex_);
	pthread_cond_wait(&m_wakeup_cond_, &m_wakeup_mutex_);
	pthread_mutex_unlock(&m_wakeup_mutex_);
}

// this is internal thread function
void *ThreadPool::thread(void *arg)
{
	ThreadPool *pool = static_cast<ThreadPool *>(arg);
	while(!pool->m_pool_destroyed_)
	{
		std::cout << "waiting for wakeup signal, says thread: " << pthread_self() << std::endl;
		pool->wait_for_wakeup_signal();
		try
		{
			for(;;)
			{
				std::shared_ptr<TaskObject> task_ptr = pool->fetch_one_task();
				std::cout << "thread " << pthread_self() << " get a task from queue\n";
				(*task_ptr)();
			}
		}
		catch(ErrorQueueEmpty &e)
		{
			// queue is empty 
			std::cout << e.what() << " says thread: " << pthread_self() << std::endl;
		}
		catch(...)
		{
		}
	}
}


// get a task from queue in sync way
std::shared_ptr<TaskObject> ThreadPool::fetch_one_task()
{
	pthread_mutex_lock(&m_task_queue_mutex_);
	if(!m_task_queue_.empty())
	{
		std::shared_ptr<TaskObject> task = m_task_queue_.front();
		m_task_queue_.pop();
		pthread_mutex_unlock(&m_task_queue_mutex_);
		return task;
	}
	else
	{
		pthread_mutex_unlock(&m_task_queue_mutex_);
		throw ErrorQueueEmpty();
	}
}

bool ThreadPool::create(unsigned int max_size)
{
	ThreadPool::s_pool_instance_ptr->m_queue_max_size_ = max_size;
	for(int i=0; i<max_size; i++)
	{
		pthread_t thread_id;
		if(pthread_create(&thread_id, NULL, ThreadPool::thread, ThreadPool::s_pool_instance_ptr)<0)
		{
			continue;
		}
		ThreadPool::s_pool_instance_ptr->m_thread_id_vector_.push_back(thread_id);
	}
}

bool ThreadPool::destroy()
{

}
bool ThreadPool::add_task(task_ptr tp, void *arg)
{
	std::shared_ptr<TaskObject> task(new MakeTask(tp, arg));
	pthread_mutex_lock(&ThreadPool::s_pool_instance_ptr->m_task_queue_mutex_);
	ThreadPool::s_pool_instance_ptr->m_task_queue_.push(task);
	pthread_mutex_unlock(&ThreadPool::s_pool_instance_ptr->m_task_queue_mutex_);
	ThreadPool::s_pool_instance_ptr->wakeup_one_thread();
}

void ThreadPool::wakeup_one_thread()
{
	pthread_cond_signal(&m_wakeup_cond_);
}