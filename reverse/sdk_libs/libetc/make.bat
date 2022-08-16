@echo off
echo Building LIBETC.LIB
ccpsx -O2 -G0 -c PAD.C
if errorlevel=1 goto err
ccpsx -O2 -G0 -c PADSTOP.C
if errorlevel=1 goto err
ccpsx -O2 -G0 -c VSYNC.C
if errorlevel=1 goto err
ccpsx -O2 -G0 -c INTR.C
if errorlevel=1 goto err
ccpsx -O2 -G0 -c INTR_DMA.C
if errorlevel=1 goto err
ccpsx -O2 -G0 -c INTR_VB.C
if errorlevel=1 goto err
ccpsx -O2 -G0 -c VMODE.C
if errorlevel=1 goto err

psylibd /u LIBETC.LIB PAD.OBJ > nul
psylibd /u LIBETC.LIB PADSTOP.OBJ > nul
psylibd /u LIBETC.LIB VSYNC.OBJ > nul
psylibd /u LIBETC.LIB INTR.OBJ > nul
psylibd /u LIBETC.LIB INTR_DMA.OBJ > nul
psylibd /u LIBETC.LIB INTR_VB.OBJ > nul
psylibd /u LIBETC.LIB VMODE.OBJ > nul

del *.obj

if exist LIBETC.LIB goto okay
:err
echo Error!
goto end
:okay
psylibd /l LIBETC.LIB
echo Build successful.
:end
pause > nul