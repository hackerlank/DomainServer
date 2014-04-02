#include "gate.h"
#include <unistd.h>

int main()
{
	Gate g("0.0.0.0", 8888);
	g.open();
	//pause();
	g.close();
	return 0;
}