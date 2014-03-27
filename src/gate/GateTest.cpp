#include "gate.h"
#include <unistd.h>

int main()
{
	Gate g("127.0.0.1", 8888);
	g.open();
	pause();
	g.close();
	return 0;
}