#include <sys/types.h>

/*
 * Prototypes
 */

extern void InterruptCallback(int, void (*)());
static void trapIntrDMA();
static void *setIntrDMA(int, void (*)());
static void memclr(u_long *, int);

static volatile u_long *dicr = (void *)0x1f8010f4;

static void (*cbIntrDMA[8])();

typedef struct
{
	u_long  madr;
	u_long  bcr;
	u_long  chcr;
	long    pad;
} DMA;

static DMA *dma = (void *)0x1f801080;

void *startIntrDMA()
{
	memclr(cbIntrDMA, 8);
	*dicr = 0;
	InterruptCallback(3, trapIntrDMA);
	return setIntrDMA;
}

static void trapIntrDMA()
{
	int i;
	u_long mask;

	while(mask = (*dicr >> 24) & 0x7f)
	{
		i = 0;

		while(mask && i<7)
		{
			if(mask & 1)
			{
				*dicr &= (1 << (24 + i)) | 0xffffff;

				if(cbIntrDMA[i])
					cbIntrDMA[i]();
			}

			i++;
			mask >>= 1;
		}
	}

	if(((*dicr & 0xff000000) == 0x80000000) || (*dicr & 0x8000))
	{
		printf("DMA bus error: code=%08x\n", *dicr);
		for(i=0; i<7; i++)
			printf("MADR[%d]=%08x\n", i, dma[i].madr);
	}
}

static void *setIntrDMA(int index, void (*callback)())
{
	void (*old)() = cbIntrDMA[index];

	if(callback != old)
	{
		if(callback)
		{
			cbIntrDMA[index] = callback;
			*dicr = ((*dicr & 0xffffff) | 0x800000) |  (1 << (index + 16));
		}
		else
		{
			cbIntrDMA[index] = 0;
			*dicr = ((*dicr & 0xffffff) | 0x800000) & ~(1 << (index + 16));
		}
	}

	return old;
}

static void memclr(u_long *ptr, int n)
{
	while(n--)
		*ptr++ = 0;
}