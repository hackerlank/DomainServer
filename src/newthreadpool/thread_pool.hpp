#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include "taskobj.hpp"
#include <thread>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <queue>

namespace DomainServer
{
	namespace threadpool
	{
		class ThreadPool
		{
			typedef std::queue<TaskObject> task_queue_t;
			typedef std::shared_ptr<TaskObject> task_ptr_t;
		public:
			ThreadPool(int max_size):
				max_size_(max_size), cur_size_(0), keep_alive_(true)
			{}
			~ThreadPool();
		
			/* data */
		public:
			template<typename ...Arg>
			bool add_task(Arg ...args)
			{
				auto task = make_taskobj(args);
				return this->add_one(task);
			}
		private:
			TaskObject fetch_one()
			{
				task_queue_locker_.lock();
				if(task_queue_.empty())
				{
					task_queue_locker_.unlock();
					throw 1;
				}
				auto task = task_queue_.front();
				task_queue_locker_.unlock();
				return task; 
			}
			bool add_one(TaskObject& task)
			{
				task_queue_locker_.lock();
				task_queue_.push(task);
				task_queue_locker_.unlock();
				return true;
			}
			void thread_proc()
			{
				while(keep_alive_)
				{
					wakeup_signal_.wait_for();
				}
			}
		private:
			int							max_size_;
			int							cur_size_;
			std::condition_variable		wakeup_signal_;
			std::mutex					task_queue_locker_;
			bool 						keep_alive_;
			task_queue_t 				task_queue_;					
		};
	}
}
#endif //THREAD_POOL_HPP