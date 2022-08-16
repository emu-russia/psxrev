#include <sys/types.h>

/*
 * Prototypes
 */

extern void InterruptCallback(int, void (*)());
static void trapIntrVSync();
static void *setIntrVSync(int, void (*)());
static void memclr(u_long *, int);

static void (*cbIntrVSync[8])();

int Vcount;

static volatile u_long *t1_mode = (void *)0x1f801114;

void *startIntrVSync()
{
	*t1_mode = 0x0100;
	Vcount = 0;
	memclr(cbIntrVSync, 8);
	InterruptCallback(0, trapIntrVSync);
	return setIntrVSync;
}

static void trapIntrVSync()
{
	int i;

	Vcount++;

	for(i=0; i<8; i++)
	{
		if(cbIntrVSync[i])
			cbIntrVSync[i]();
	}
}

static void *setIntrVSync(int index, void (*callback)())
{
	void (*old)() = cbIntrVSync[index];

	if(callback != old)
		cbIntrVSync[index] = callback;

	return old;
}

static void memclr(u_long *ptr, int n)
{
	while(n--)
		*ptr++ = 0;
}