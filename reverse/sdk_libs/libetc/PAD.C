#include <sys/types.h>
#include <libapi.h>

static u_long pad_buf;
int PadIdentifier;

void PadInit(int mode)
{
	PadIdentifier = mode;
	pad_buf = -1;
	ResetCallback();
	PAD_init(0x20000001, &pad_buf);
	ChangeClearPAD(0);
}

u_long PadRead(u_long id)
{
	PAD_dr();
	return ~pad_buf;
}
