static int vmode;

int SetVideoMode(int mode)
{
	int old = vmode;
	vmode = mode;
	return old;
}

int GetVideoMode()
{
	return vmode;
}