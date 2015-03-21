
int JpegLoad(
    char *filename,
    void(*AddScanlineData) (unsigned char *buffer, int stride, void *Param),
    unsigned char **DecodeBuffer,
    long *DecodeBufferSize,
    long *DecodeWidth,
    long *DecodeHegith,
    void *Param);

HBITMAP CreateBitmapFromPixels(HDC hDC, UINT uWidth, UINT uHeight, UINT uBitsPerPixel, PBYTE pBits);
