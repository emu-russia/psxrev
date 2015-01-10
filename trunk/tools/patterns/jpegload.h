
int JpegLoad(
    char *filename,
    void(*AddScanlineData) (unsigned char *buffer, int stride, void *Param),
    unsigned char **DecodeBuffer,
    int *DecodeBufferSize,
    int *DecodeWidth,
    int *DecodeHegith,
    void *Param);

HBITMAP CreateBitmapFromPixels(HDC hDC, UINT uWidth, UINT uHeight, UINT uBitsPerPixel, PBYTE pBits);
