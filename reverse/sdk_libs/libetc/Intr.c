#include <sys/types.h>
#include <libapi.h>
#include <setjmp.h>

/*
 * Prototypes
 */

static void *startIntr();
static void trapIntr();
static void *setIntr(int, void (*)());
static void *stopIntr();
static void *resetIntr();
static void memclr(u_long *, int);

static struct
{
	u_short     started;
	u_short     trapped;
	void        (*cbIntr[11])();
	u_short     mask, oldmask;
	u_long      dma;
	jmp_buf     trap;
	u_char      stack[0x1000];
} intrData;

typedef struct
{
	char    *id;
	void    *(*setIntrDMA)(int, void (*)());
	void    *(*setIntr)(int, void (*)());
	void    *(*startIntr)();
	void    *(*stopIntr)();
	void    *(*setIntrVSync)(int, void (*)());
	void    *(*resetIntr)();
} INTRCB;

static INTRCB _intrCB_ = {
	"$Id: intr.c,v 1.75 1997/02/07 09:00:36 makoto Exp $",
	NULL,
	setIntr,
	startIntr,
	stopIntr,
	NULL,
	resetIntr
};

static INTRCB *intrCB = &_intrCB_;

static volatile u_short *int_reg = (void *)0x1f801070;
static volatile u_short *int_mask = (void *)0x1f801074;
static u_long *dpcr = (void *)0x1f8010f0;
static int timeout;

void *ResetCallback()
{
	return intrCB->startIntr();
}

void *InterruptCallback(int index, void (*callback)())
{
	return intrCB->setIntr(index, callback);
}

void *DMACallback(int index, void (*callback)())
{
	return intrCB->setIntrDMA(index, callback);
}

void *VSyncCallback(void (*callback)())
{
	return intrCB->setIntrVSync(4, callback);
}

void *VSyncCallbacks(int index, void (*callback)())
{
	return intrCB->setIntrVSync(index, callback);
}

void *StopCallback()
{
	return intrCB->stopIntr();
}

int RestartCallback()
{
	return intrCB->resetIntr();
}

int CheckCallback()
{
	return intrData.trapped;
}

u_short GetIntrMask()
{
	return *int_mask;
}

u_short SetIntrMask(u_short mask)
{
	u_short old = *int_mask;
	*int_mask = mask;
	return old;
}

static void *startIntr()
{
	if(!intrData.started)
	{
		*int_reg = *int_mask = 0;
		*dpcr = 0x33333333;
		memclr(&intrData, sizeof(intrData) / 4);
		if(setjmp(intrData.trap))
			trapIntr();
		(void *)intrData.trap[JB_SP] = &intrData.stack[0xfb0];
		HookEntryInt(intrData.trap);
		intrData.started = 1;
		intrCB->setIntrVSync = startIntrVSync();
		intrCB->setIntrDMA = startIntrDMA();
		_96_remove();
		ExitCriticalSection();
		return &intrData;
	}
	else return NULL;
}

static void trapIntr()
{
	u_short intr;
	int i;

	if(!intrData.started)
	{
		printf("unexpected interrupt(%04x)\n", *int_reg);
		ReturnFromException();
	}

	intrData.trapped = 1;

	while(intr = (intrData.mask & *int_reg) & *int_mask)
	{
		for(i=0; i<11,intr; i++,intr>>=1)
		{
			if(i >= 11) break;

			if(intr & 1)
			{
				*int_reg = ~(1 << i);
				if(intrData.cbIntr[i])
					intrData.cbIntr[i]();
			}
		}
	}

	if(*int_reg & *int_mask)
	{
		if(timeout++ > 2048)
		{
			printf("intr timeout(%.04x:%04x)\n", *int_reg, *int_mask);
			timeout = 0;
			*int_reg = 0;
		}
	} else timeout = 0;

	intrData.trapped = 0;
	ReturnFromException();
}

static void *setIntr(int index, void (*callback)())
{
	void (*old)() = intrData.cbIntr[index];
	int mask;

	if(callback == old || !intrData.started)
		return old;
	   
	mask = *int_mask;
	*int_mask = 0;

	if(callback)
	{
		intrData.cbIntr[index] = callback;
		mask |= 1 << index;
		intrData.mask |= 1 << index;
	}
	else
	{
		intrData.cbIntr[index] = 0;
		mask &= ~(1 << index);
		intrData.mask &= ~(1 << index);
	}

	if(index == 0)
	{
		ChangeClearPAD(callback == 0);
		ChangeClearRCnt(3, callback == 0);
	}
	if(index == 4)
		ChangeClearRCnt(0, callback == 0);
	if(index == 5)
		ChangeClearRCnt(1, callback == 0);
	if(index == 6)
		ChangeClearRCnt(2, callback == 0);
	
	*int_mask = mask;

	return old;
}

static void *stopIntr()
{
	if(intrData.started)
	{
		EnterCriticalSection();
		intrData.oldmask = *int_mask;
		intrData.dma = *dpcr;
		*int_reg = *int_mask = 0;
		*dpcr &= 0x77777777;
		ResetEntryInt();
		intrData.started = 0;
		return &intrData;
	}
	else return NULL;
}

static void *resetIntr()
{
	if(!intrData.started)
	{   
		HookEntryInt(intrData.trap);
		intrData.started = 1;
		*int_mask = intrData.oldmask;
		*dpcr = intrData.dma;
		ExitCriticalSection();
		return &intrData;
	}
	else return NULL;
}

static void memclr(u_long *ptr, int n)
{
	while(n--)
		*ptr++ = 0;
}
