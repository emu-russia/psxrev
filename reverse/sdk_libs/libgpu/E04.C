// Choose default display settings.
#include <sys/types.h>
#include <kernel.h>
#include <libgte.h>
#include <libgpu.h>

DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h)
{
    setRECT (&env->disp, x, y, w, h);
    setRECT (&env->screen, 0, 0, 0, 0);

    disp->isinter = disp->isrgb24 = 0;
    disp->pad0 = disp->pad1 = 0;

    return env;
}
