#ifndef GATE_H
#define GATE_H

#include <string>

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
};

#endif // GATE_H