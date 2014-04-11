#ifndef TASKOBJ_HPP
#define TASKOBJ_HPP


namespace DomainServer
{
	namespace threadpool
	{
		class TaskObject
		{
			// typedef std::function<void ()> task_data;
		public:
			TaskObject()=default;
			~TaskObject()=default;

			/* data */
		public:
			virtual void operator()() const =0;
		};

		<template TaskDataType>
		class TaskObjectMaker: public TaskObject
		{
		public:
			TaskObjectMaker(TaskDataType& data):
				internal_data_(data)
			{
				// no operate
			}
		
			/* data */
		public:
			void operator()() const override
			{
				internal_data_();
			}
		private:
			TaskDataType 		internal_data_;
		};

		// simple helper
		<template ...Arg>
		TaskObject make_taskobj(Arg ...args)
		{
			TaskObjectMaker tom(std::bind(args));
			return tom;
		}
	}

}

#endif // TASKOBJ_HPP