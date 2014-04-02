#ifndef GATE_H
#define GATE_H

#include <string>

template<typename T>
class Reactor;

class Gate
{
public:
	enum  {OPEN=0, CLOSE};
public:
	Gate(const std::string ip, int port);
	~Gate();
public:
	void open();
	void close();
private:
	bool dispatch_guest_to_hub(int sock_fd);
	// for epoll
	void *dispatch_guest_to_hub_epollwrapper(int sock_fd);
	void wait_for_guest();
private:
	static void alarm_handler(int);

private:
	static void *wrapper(void *);
	static void *dispatch_wrapper(void *);
private:
	std::string 	m_gate_ip_;
	int				m_gate_port_;
	int				m_gate_status_;
private:
	static Gate 	*s_self;
	static Reactor<Gate> *s_reactor_;
};

#endif // GATE_H