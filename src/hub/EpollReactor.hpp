#ifndef EPOLLREACTOR_HPP
#define EPOLLREACTOR_HPP

#include "log.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include <map>
#include <errno.h>

class Reactor
{
	typedef void *(handler)(int sock_fd);
	typedef std::pair<int, handler) p;
public:
	Reactor(int max_size);
	~Reactor();
public:
	bool reg_ev(int fd, handler h);
	bool del_ev(int fd);
	bool is_full();
	void run();
	void stop();
private:
	int 		m_cur_size_;
	int			m_max_size_;
	int			m_ep_fd_;
	std::map<int, handler> m_handler_map;
	bool 		m_running;
};

// ctor
Reactor::Reactor(int max_size)
	: m_cur_size_(0), m_max_size_(max_size), m_running(true)
{
	m_ep_fd_ = epoll_create(m_max_size_);
}
// dtor
Reactor::~Reactor()
{
	close(m_ep_fd_);
}
// register one fd
bool Reactor::reg_ev(int fd, handler h)
{
	if(m_cur_size_>=m_max_size_)
		return false;
	epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN|EPOLLET;
	epoll_ctl(m_ep_fd_, EPOLL_CTL_ADD, fd, &ev);
	m_cur_size_++;
	m_handler_map.insert(p(fd, handler));
	return true;
}
// remove one fd
bool Reactor::del_ev(int fd)
{
	if(m_handler_map.find(fd)==m_handler_map.end())
		return false;
	epoll_event ev;
	ev.data.fd = fd;
	epoll_ctl(m_ep_fd_, EPOLL_CTL_DEL, fd, &ev);
	--m_cur_size_;
	return true;
}
// get status
bool Reactor::is_full()
{
	return m_cur_size_>=m_max_size_;
}
// start reactor
void Reactor::run()
{
	std::map<int, handler>::iterator iter;
	while(m_running)
	{
		epoll_event events[4096];
		int ret = epoll_wait(m_ep_fd_, events, m_max_size_, 100);
		if(ret<0)
		{
			Logging::error("epoll_wait returns error,[%s]\n", strerr(errno));
			return;
		}
		for(int i=0; i<ret; ++i)
		{
			if(!(events[i].events&EPOLLIN))
				continue;
			iter = m_handler_map.find(events[i].data.fd);
			if(iter==m_handler_map.end())
			{
				// no handler found, remove it
				Logging::error("no handler found for socket<%d>, will close it...\n", events[i].data.fd);
				close(events[i].data.fd);
				this->del_ev(events[i].data.fd);
			}
			else
			{
				// dummy do something...
				iter.second(iter.first);
			}
		}
	}
	Logging::info("Reactor stop running...exiting...\n");
}
// stop reactor
void Reactor::stop()
{
	m_running = false;
}



#endif // EPOLLREACTOR_HPP