#include <sys/types.h>

/*
 * Prototypes
 */

static void v_wait(long, long);

extern volatile int Vcount;

static volatile u_long *gp1 = (void *)0x1f801814;
static volatile u_long *t1_count = (void *)0x1f801110;

volatile int Hcount;
static int oldVcount;

int VSync(int mode)
{
	u_long gpu = *gp1;
	volatile int count;
	long delta;

	while(1)
	{
		count = *t1_count;
		if(count == *t1_count) break;
	}

	delta = (count - Hcount) & 0xffff;

	if(mode < 0) return Vcount;

	if(mode != 1)
	{
		v_wait(mode > 0 ? oldVcount + (mode - 1) : oldVcount, mode <= 0 ? 0 : mode - 1);
		gpu = *gp1;
		v_wait(Vcount + 1, 1);

		if(gpu & 0x00400000)
			while(!((gpu ^ *gp1) & 0x80000000)) ;

		oldVcount = Vcount;

		while(1)
		{
			Hcount = *t1_count;
			if(Hcount == *t1_count) break;
		}
	}

	return delta;
}

static void v_wait(long n, long scale)
{
	volatile long timeout = scale << 15;

	while(Vcount < n)
	{
		if(!timeout--)
		{
			puts("VSync: timeout\n");
			ChangeClearPAD(0);
			ChangeClearRCnt(3, 0);
			break;
		}
	}
}