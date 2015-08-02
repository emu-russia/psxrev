// JPEG Save.

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <Windows.h>

#include "jpegsave.h"

extern "C"
{
#include "jpeglib.h"
};

struct my_error_mgr {
    struct jpeg_error_mgr pub;    /* "public" fields */

    jmp_buf setjmp_buffer;    /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;
 
METHODDEF(void)
my_error_exit(j_common_ptr cinfo)
{
    my_error_ptr myerr = (my_error_ptr)cinfo->err;
    (*cinfo->err->output_message) (cinfo);
    longjmp(myerr->setjmp_buffer, 1);
}

// borrowed from example.c
int JpegSave(char * filename,
    void(*SaveScanlineData) (unsigned char *buffer, int stride, PJPEG_SAVE_CONTEXT SaveContext),
    unsigned char *RawBuffer,
    long Width,
    long Hegith,
    PJPEG_SAVE_CONTEXT Context)
{
    return 0;
}
