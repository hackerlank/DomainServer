#ifndef LOG_HPP
#define LOG_HPP 

#include <syslog.h>

class Logging
{
public:
	enum  LVL {DEBUG, INFO, WARNING, ERROR, CRITICAL};
public:
	Logging()
	{

	}
	~Logging()
	{
		closelog();
	}
public:
	template<typename... TS>
	static void debug(const char *fmt, TS... args)
	{
		if(s_log_lvl_==LVL::DEBUG)
			syslog(LOG_USER|LOG_DEBUG, fmt, args...);
	}

	template<typename... TS>
	static void info(const char *fmt, TS... args)
	{
		if(s_log_lvl_<=LVL::INFO)
			syslog(LOG_USER|LOG_INFO, fmt, args...);
	}

	template<typename... TS>
	static void warning(const char *fmt, TS... args)
	{
		if(s_log_lvl_<=LVL::WARNING)
			syslog(LOG_USER|LOG_WARNING, fmt, args...);
	}

	template<typename... TS>
	static void error(const char *fmt, TS... args)
	{
		if(s_log_lvl_<=LVL::ERROR)
			syslog(LOG_USER|LOG_ERR, fmt, args...);
	}

	template<typename... TS>
	static void critical(const char *fmt, TS... args)
	{
		syslog(LOG_USER|LOG_CRIT, fmt, args...);
	}
public:
	// this config affects global
	static void basic_config(LVL log_lvl, const char *app_name)
	{
		s_log_lvl_ = log_lvl;
		if(s_using_default_ && (app_name!=nullptr))
		{
			openlog(app_name, LOG_PID|LOG_CONS, 0);
			s_using_default_ = false;
		}
	}
private:
	static bool    s_using_default_;
	static int 	s_log_lvl_;

};

bool Logging::s_using_default_ = true;
int Logging::s_log_lvl_ = Logging::WARNING;

#endif // LOG_HPP