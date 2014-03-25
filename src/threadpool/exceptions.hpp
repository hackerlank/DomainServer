#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>

class ErrorQueueEmpty: public std::exception
{
public:
	virtual const char *what() const throw()
	{
		return "queue is already EMPTY";
	}
};

#endif // EXCEPTIONS_HPP