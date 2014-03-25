#ifndef TASKOBJ_H
#define TASKOBJ_H

#include <memory>

class TaskObject
{
public:
	TaskObject(){}
	virtual ~TaskObject(){}
public:
	virtual void operator()()=0;
};


class MakeTask: public TaskObject
{
	typedef	void *(*task_ptr)(void *);
public:
	MakeTask(task_ptr task_ptr, void *arg_ptr)
		: m_task_ptr_(task_ptr), m_arg_ptr_(arg_ptr)
	{}
	~MakeTask(){}
public:
	virtual void operator()()
	{
		(*m_task_ptr_)(m_arg_ptr_);
	}
private:
	task_ptr 	m_task_ptr_;
	void 		*m_arg_ptr_;
};


#endif // TASKOBJ_H