
typedef struct _JPEG_SAVE_CONTEXT
{
    long        Dummy;

} JPEG_SAVE_CONTEXT, *PJPEG_SAVE_CONTEXT;

int JpegSave(char * filename,
    void(*SaveScanlineData) (unsigned char *buffer, int stride, PJPEG_SAVE_CONTEXT SaveContext),
    unsigned char *RawBuffer,
    long Width,
    long Hegith,
    PJPEG_SAVE_CONTEXT Context);
