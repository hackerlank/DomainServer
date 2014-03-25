#ifndef GATE_H
#define GATE_H

class Gate
{
public:
	Gate();
	~Gate();
public:
	void wait_for_guest();
private:
	bool dispatch_guest_to_hub();
private:
	static void alarm_handler();
};

#endif // GATE_H