#include "gate.h"
#include "log.hpp"
#include "singleton.hpp"
#include "EpollReactor.hpp"
#include <signal.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

Gate *Gate::s_self = NULL;
Reactor<Gate> *Gate::s_reactor_ = NULL;

Gate::Gate(const std::string ip, int port)
	: m_gate_ip_(ip), m_gate_port_(port), m_gate_status_(CLOSE)
{
	// set alarm handler
	signal(SIGALRM, alarm_handler);
	ThreadPoolSingleton::instance().create(10);
	Logging::basic_config(Logging::DEBUG, "gate");
	s_self = this;
}

Gate::~Gate()
{}

void Gate::wait_for_guest()
{
	int sock_listenner = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in gate_addr, client_addr;

	gate_addr.sin_family = AF_INET;
	gate_addr.sin_port = htons(m_gate_port_);
	gate_addr.sin_addr.s_addr = inet_addr(m_gate_ip_.c_str());
	memset(&gate_addr.sin_zero, 0, sizeof(gate_addr.sin_zero));

	bind(sock_listenner, (struct sockaddr *)&gate_addr, sizeof(gate_addr));
	listen(sock_listenner, 100);
	// while(m_gate_status_==OPEN)
	// {
	// 	socklen_t addr_len = sizeof(client_addr);
	// 	int sock_client = accept(sock_listenner, (struct sockaddr *)&client_addr, &addr_len);
	// 	dispatch_guest_to_hub(sock_client);
	// }
	Reactor<Gate> reactor(10, this);
	s_reactor_ = &reactor;
	reactor.reg_ev(sock_listenner, &Gate::dispatch_guest_to_hub_epollwrapper);
	reactor.run();
}

bool Gate::dispatch_guest_to_hub(int sock_fd)
{
	// use domain socket to pass fd
	// TODO
	Logging::debug("Gate::dispatch_guest_to_hub\n");
	::close(sock_fd);
	return true;
}

void *Gate::dispatch_guest_to_hub_epollwrapper(int sock_fd)
{
	// use domain socket to pass fd
	// TODO
	Logging::debug("Gate::dispatch_guest_to_hub_epollwrapper\n");
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
	int sock_client = accept(sock_fd, (struct sockaddr *)&client_addr, &addr_len);
	dispatch_guest_to_hub(sock_client);
	return NULL;
}

void Gate::alarm_handler(int)
{
	Logging::warning("Catch a SIGALRM signal, gate start closing...\n");
	s_self->m_gate_port_ = CLOSE;
	s_reactor_->stop();
}


void Gate::close()
{
	Logging::info("gate start closing...\n");
	m_gate_status_ = CLOSE;
}

void Gate::open()
{
	m_gate_status_ = OPEN;
	sleep(3);
	//ThreadPoolSingleton::instance().add_task(wrapper, this);
	wrapper(this);
}

void *Gate::wrapper(void *arg)
{
	Gate *gate = static_cast<Gate *>(arg);
	gate->wait_for_guest();
}

void *Gate::dispatch_wrapper(void *arg)
{
	Gate *gate = static_cast<Gate *>(arg);
	// gate->dispatch_guest_to_hub
}