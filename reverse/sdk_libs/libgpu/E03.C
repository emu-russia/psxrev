// Choose default drawing environment.
#include <sys/types.h>
#include <kernel.h>
#include <libgte.h>
#include <libgpu.h>

DRAWENV *SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h)
{
    int mode = GetVideoMode ();

    setRECT (&env->clip, x, y, w, h);
    setRECT (&env->tw, 0, 0, 0, 0);
    setRGB0 (env, 0, 0, 0);

    env->dtd = 1;
    env->dfe = mode ? h < 289 : h < 257;
    env->ofs[0] = x;
    env->ofs[1] = y;
    env->tpage = getTPage (0, 0, 640, 0);
    env->isbg = 0;

    return env;
}
