
# makefile for panorama stitching, created by hugin using the new makefilelib

# Force using cmd.exe
SHELL=C:\Windows\system32\cmd.exe

# Tool configuration
NONA="C:/Program Files/Hugin/bin/nona"
PTSTITCHER="PTStitcher"
PTMENDER="C:/Program Files/Hugin/bin/PTmender"
PTBLENDER="C:/Program Files/Hugin/bin/PTblender"
PTMASKER="C:/Program Files/Hugin/bin/PTmasker"
PTROLLER="C:/Program Files/Hugin/bin/PTroller"
ENBLEND="C:/Program Files/Hugin/bin/enblend"
ENFUSE="C:/Program Files/Hugin/bin/enfuse"
SMARTBLEND="smartblend.exe"
HDRMERGE="C:/Program Files/Hugin/bin/hugin_hdrmerge"
RM=del
EXIFTOOL="C:/Program Files/Hugin/bin/exiftool"

# Project parameters
HUGIN_PROJECTION=1
HUGIN_HFOV=3
HUGIN_WIDTH=4909
HUGIN_HEIGHT=6546

# options for the programs
NONA_LDR_REMAPPED_COMP=-z LZW 
NONA_OPTS=
ENBLEND_OPTS= -f2795x4607+1002+1232
ENBLEND_LDR_COMP=--compression=80
ENBLEND_EXPOSURE_COMP=--compression=LZW 
ENBLEND_HDR_COMP=
HDRMERGE_OPTS=-m avg -c
ENFUSE_OPTS=
EXIFTOOL_COPY_ARGS=-ImageDescription -Make -Model -Artist -WhitePoint -Copyright -GPS:all -DateTimeOriginal -CreateDate -UserComment -ColorSpace -OwnerName -SerialNumber
EXIFTOOL_INFO_ARGS="-Software=Hugin 2013.0.0.0d404a7088e6 built by Matthew Petroff" "-UserComment<$${UserComment}&\#xd;&\#xa;Projection: Cylindrical (1)&\#xd;&\#xa;FOV: 3 x 4&\#xd;&\#xa;Ev: 0.07" -f

# the output panorama
LDR_REMAPPED_PREFIX=DCache
LDR_REMAPPED_PREFIX_SHELL="DCache"
HDR_STACK_REMAPPED_PREFIX=DCache_hdr_
HDR_STACK_REMAPPED_PREFIX_SHELL="DCache_hdr_"
LDR_EXPOSURE_REMAPPED_PREFIX=DCache_exposure_layers_
LDR_EXPOSURE_REMAPPED_PREFIX_SHELL="DCache_exposure_layers_"
PROJECT_FILE=C:/Andruxa/psxdev/circuits/DCache/DCache.pto
PROJECT_FILE_SHELL="C:/Andruxa/psxdev/circuits/DCache/DCache.pto"
LDR_BLENDED=DCache.jpg
LDR_BLENDED_SHELL="DCache.jpg"
LDR_STACKED_BLENDED=DCache_fused.jpg
LDR_STACKED_BLENDED_SHELL="DCache_fused.jpg"
LDR_EXPOSURE_LAYERS_FUSED=DCache_blended_fused.jpg
LDR_EXPOSURE_LAYERS_FUSED_SHELL="DCache_blended_fused.jpg"
HDR_BLENDED=DCache_hdr.exr
HDR_BLENDED_SHELL="DCache_hdr.exr"

# first input image
INPUT_IMAGE_1=C:/Andruxa/psxdev/circuits/DCache/r0061_c0010.jpg
INPUT_IMAGE_1_SHELL="C:/Andruxa/psxdev/circuits/DCache/r0061_c0010.jpg"

# all input images
INPUT_IMAGES=C:/Andruxa/psxdev/circuits/DCache/r0061_c0010.jpg\
C:/Andruxa/psxdev/circuits/DCache/r0062_c0010.jpg\
C:/Andruxa/psxdev/circuits/DCache/r0063_c0010.jpg\
C:/Andruxa/psxdev/circuits/DCache/r0064_c0010.jpg\
C:/Andruxa/psxdev/circuits/DCache/r0065_c0010.jpg\
C:/Andruxa/psxdev/circuits/DCache/r0061_c0011.jpg\
C:/Andruxa/psxdev/circuits/DCache/r0062_c0011.jpg\
C:/Andruxa/psxdev/circuits/DCache/r0063_c0011.jpg\
C:/Andruxa/psxdev/circuits/DCache/r0064_c0011.jpg\
C:/Andruxa/psxdev/circuits/DCache/r0065_c0011.jpg
INPUT_IMAGES_SHELL="C:/Andruxa/psxdev/circuits/DCache/r0061_c0010.jpg"\
"C:/Andruxa/psxdev/circuits/DCache/r0062_c0010.jpg"\
"C:/Andruxa/psxdev/circuits/DCache/r0063_c0010.jpg"\
"C:/Andruxa/psxdev/circuits/DCache/r0064_c0010.jpg"\
"C:/Andruxa/psxdev/circuits/DCache/r0065_c0010.jpg"\
"C:/Andruxa/psxdev/circuits/DCache/r0061_c0011.jpg"\
"C:/Andruxa/psxdev/circuits/DCache/r0062_c0011.jpg"\
"C:/Andruxa/psxdev/circuits/DCache/r0063_c0011.jpg"\
"C:/Andruxa/psxdev/circuits/DCache/r0064_c0011.jpg"\
"C:/Andruxa/psxdev/circuits/DCache/r0065_c0011.jpg"

# remapped images
LDR_LAYERS=DCache0000.tif\
DCache0001.tif\
DCache0002.tif\
DCache0003.tif\
DCache0004.tif\
DCache0005.tif\
DCache0006.tif\
DCache0007.tif\
DCache0008.tif\
DCache0009.tif
LDR_LAYERS_SHELL="DCache0000.tif"\
"DCache0001.tif"\
"DCache0002.tif"\
"DCache0003.tif"\
"DCache0004.tif"\
"DCache0005.tif"\
"DCache0006.tif"\
"DCache0007.tif"\
"DCache0008.tif"\
"DCache0009.tif"

# remapped images (hdr)
HDR_LAYERS=DCache_hdr_0000.exr\
DCache_hdr_0001.exr\
DCache_hdr_0002.exr\
DCache_hdr_0003.exr\
DCache_hdr_0004.exr\
DCache_hdr_0005.exr\
DCache_hdr_0006.exr\
DCache_hdr_0007.exr\
DCache_hdr_0008.exr\
DCache_hdr_0009.exr
HDR_LAYERS_SHELL="DCache_hdr_0000.exr"\
"DCache_hdr_0001.exr"\
"DCache_hdr_0002.exr"\
"DCache_hdr_0003.exr"\
"DCache_hdr_0004.exr"\
"DCache_hdr_0005.exr"\
"DCache_hdr_0006.exr"\
"DCache_hdr_0007.exr"\
"DCache_hdr_0008.exr"\
"DCache_hdr_0009.exr"

# remapped maxval images
HDR_LAYERS_WEIGHTS=DCache_hdr_0000_gray.pgm\
DCache_hdr_0001_gray.pgm\
DCache_hdr_0002_gray.pgm\
DCache_hdr_0003_gray.pgm\
DCache_hdr_0004_gray.pgm\
DCache_hdr_0005_gray.pgm\
DCache_hdr_0006_gray.pgm\
DCache_hdr_0007_gray.pgm\
DCache_hdr_0008_gray.pgm\
DCache_hdr_0009_gray.pgm
HDR_LAYERS_WEIGHTS_SHELL="DCache_hdr_0000_gray.pgm"\
"DCache_hdr_0001_gray.pgm"\
"DCache_hdr_0002_gray.pgm"\
"DCache_hdr_0003_gray.pgm"\
"DCache_hdr_0004_gray.pgm"\
"DCache_hdr_0005_gray.pgm"\
"DCache_hdr_0006_gray.pgm"\
"DCache_hdr_0007_gray.pgm"\
"DCache_hdr_0008_gray.pgm"\
"DCache_hdr_0009_gray.pgm"

# stacked hdr images
HDR_STACK_0=DCache_stack_hdr_0000.exr
HDR_STACK_0_SHELL="DCache_stack_hdr_0000.exr"
HDR_STACK_0_INPUT=DCache_hdr_0000.exr
HDR_STACK_0_INPUT_SHELL="DCache_hdr_0000.exr"
HDR_STACK_1=DCache_stack_hdr_0001.exr
HDR_STACK_1_SHELL="DCache_stack_hdr_0001.exr"
HDR_STACK_1_INPUT=DCache_hdr_0001.exr
HDR_STACK_1_INPUT_SHELL="DCache_hdr_0001.exr"
HDR_STACK_2=DCache_stack_hdr_0002.exr
HDR_STACK_2_SHELL="DCache_stack_hdr_0002.exr"
HDR_STACK_2_INPUT=DCache_hdr_0002.exr
HDR_STACK_2_INPUT_SHELL="DCache_hdr_0002.exr"
HDR_STACK_3=DCache_stack_hdr_0003.exr
HDR_STACK_3_SHELL="DCache_stack_hdr_0003.exr"
HDR_STACK_3_INPUT=DCache_hdr_0003.exr
HDR_STACK_3_INPUT_SHELL="DCache_hdr_0003.exr"
HDR_STACK_4=DCache_stack_hdr_0004.exr
HDR_STACK_4_SHELL="DCache_stack_hdr_0004.exr"
HDR_STACK_4_INPUT=DCache_hdr_0004.exr
HDR_STACK_4_INPUT_SHELL="DCache_hdr_0004.exr"
HDR_STACK_5=DCache_stack_hdr_0005.exr
HDR_STACK_5_SHELL="DCache_stack_hdr_0005.exr"
HDR_STACK_5_INPUT=DCache_hdr_0005.exr
HDR_STACK_5_INPUT_SHELL="DCache_hdr_0005.exr"
HDR_STACK_6=DCache_stack_hdr_0006.exr
HDR_STACK_6_SHELL="DCache_stack_hdr_0006.exr"
HDR_STACK_6_INPUT=DCache_hdr_0006.exr
HDR_STACK_6_INPUT_SHELL="DCache_hdr_0006.exr"
HDR_STACK_7=DCache_stack_hdr_0007.exr
HDR_STACK_7_SHELL="DCache_stack_hdr_0007.exr"
HDR_STACK_7_INPUT=DCache_hdr_0007.exr
HDR_STACK_7_INPUT_SHELL="DCache_hdr_0007.exr"
HDR_STACK_8=DCache_stack_hdr_0008.exr
HDR_STACK_8_SHELL="DCache_stack_hdr_0008.exr"
HDR_STACK_8_INPUT=DCache_hdr_0008.exr
HDR_STACK_8_INPUT_SHELL="DCache_hdr_0008.exr"
HDR_STACK_9=DCache_stack_hdr_0009.exr
HDR_STACK_9_SHELL="DCache_stack_hdr_0009.exr"
HDR_STACK_9_INPUT=DCache_hdr_0009.exr
HDR_STACK_9_INPUT_SHELL="DCache_hdr_0009.exr"
HDR_STACKS_NUMBERS=0 1 2 3 4 5 6 7 8 9 
HDR_STACKS=$(HDR_STACK_0) $(HDR_STACK_1) $(HDR_STACK_2) $(HDR_STACK_3) $(HDR_STACK_4) $(HDR_STACK_5) $(HDR_STACK_6) $(HDR_STACK_7) $(HDR_STACK_8) $(HDR_STACK_9) 
HDR_STACKS_SHELL=$(HDR_STACK_0_SHELL) $(HDR_STACK_1_SHELL) $(HDR_STACK_2_SHELL) $(HDR_STACK_3_SHELL) $(HDR_STACK_4_SHELL) $(HDR_STACK_5_SHELL) $(HDR_STACK_6_SHELL) $(HDR_STACK_7_SHELL) $(HDR_STACK_8_SHELL) $(HDR_STACK_9_SHELL) 

# number of image sets with similar exposure
LDR_EXPOSURE_LAYER_0=DCache_exposure_0000.tif
LDR_EXPOSURE_LAYER_0_SHELL="DCache_exposure_0000.tif"
LDR_EXPOSURE_LAYER_0_INPUT=DCache_exposure_layers_0000.tif\
DCache_exposure_layers_0001.tif\
DCache_exposure_layers_0002.tif\
DCache_exposure_layers_0003.tif\
DCache_exposure_layers_0004.tif\
DCache_exposure_layers_0005.tif\
DCache_exposure_layers_0006.tif\
DCache_exposure_layers_0007.tif\
DCache_exposure_layers_0008.tif\
DCache_exposure_layers_0009.tif
LDR_EXPOSURE_LAYER_0_INPUT_SHELL="DCache_exposure_layers_0000.tif"\
"DCache_exposure_layers_0001.tif"\
"DCache_exposure_layers_0002.tif"\
"DCache_exposure_layers_0003.tif"\
"DCache_exposure_layers_0004.tif"\
"DCache_exposure_layers_0005.tif"\
"DCache_exposure_layers_0006.tif"\
"DCache_exposure_layers_0007.tif"\
"DCache_exposure_layers_0008.tif"\
"DCache_exposure_layers_0009.tif"
LDR_EXPOSURE_LAYER_0_INPUT_PTMENDER=DCache0000.tif\
DCache0001.tif\
DCache0002.tif\
DCache0003.tif\
DCache0004.tif\
DCache0005.tif\
DCache0006.tif\
DCache0007.tif\
DCache0008.tif\
DCache0009.tif
LDR_EXPOSURE_LAYER_0_INPUT_PTMENDER_SHELL="DCache0000.tif"\
"DCache0001.tif"\
"DCache0002.tif"\
"DCache0003.tif"\
"DCache0004.tif"\
"DCache0005.tif"\
"DCache0006.tif"\
"DCache0007.tif"\
"DCache0008.tif"\
"DCache0009.tif"
LDR_EXPOSURE_LAYER_0_EXPOSURE=0.0738879
LDR_EXPOSURE_LAYERS_NUMBERS=0 
LDR_EXPOSURE_LAYERS=$(LDR_EXPOSURE_LAYER_0) 
LDR_EXPOSURE_LAYERS_SHELL=$(LDR_EXPOSURE_LAYER_0_SHELL) 
LDR_EXPOSURE_LAYERS_REMAPPED=DCache_exposure_layers_0000.tif\
DCache_exposure_layers_0001.tif\
DCache_exposure_layers_0002.tif\
DCache_exposure_layers_0003.tif\
DCache_exposure_layers_0004.tif\
DCache_exposure_layers_0005.tif\
DCache_exposure_layers_0006.tif\
DCache_exposure_layers_0007.tif\
DCache_exposure_layers_0008.tif\
DCache_exposure_layers_0009.tif
LDR_EXPOSURE_LAYERS_REMAPPED_SHELL="DCache_exposure_layers_0000.tif"\
"DCache_exposure_layers_0001.tif"\
"DCache_exposure_layers_0002.tif"\
"DCache_exposure_layers_0003.tif"\
"DCache_exposure_layers_0004.tif"\
"DCache_exposure_layers_0005.tif"\
"DCache_exposure_layers_0006.tif"\
"DCache_exposure_layers_0007.tif"\
"DCache_exposure_layers_0008.tif"\
"DCache_exposure_layers_0009.tif"

# stacked ldr images
LDR_STACK_0=DCache_stack_ldr_0000.tif
LDR_STACK_0_SHELL="DCache_stack_ldr_0000.tif"
LDR_STACK_0_INPUT=DCache_exposure_layers_0000.tif
LDR_STACK_0_INPUT_SHELL="DCache_exposure_layers_0000.tif"
LDR_STACK_1=DCache_stack_ldr_0001.tif
LDR_STACK_1_SHELL="DCache_stack_ldr_0001.tif"
LDR_STACK_1_INPUT=DCache_exposure_layers_0001.tif
LDR_STACK_1_INPUT_SHELL="DCache_exposure_layers_0001.tif"
LDR_STACK_2=DCache_stack_ldr_0002.tif
LDR_STACK_2_SHELL="DCache_stack_ldr_0002.tif"
LDR_STACK_2_INPUT=DCache_exposure_layers_0002.tif
LDR_STACK_2_INPUT_SHELL="DCache_exposure_layers_0002.tif"
LDR_STACK_3=DCache_stack_ldr_0003.tif
LDR_STACK_3_SHELL="DCache_stack_ldr_0003.tif"
LDR_STACK_3_INPUT=DCache_exposure_layers_0003.tif
LDR_STACK_3_INPUT_SHELL="DCache_exposure_layers_0003.tif"
LDR_STACK_4=DCache_stack_ldr_0004.tif
LDR_STACK_4_SHELL="DCache_stack_ldr_0004.tif"
LDR_STACK_4_INPUT=DCache_exposure_layers_0004.tif
LDR_STACK_4_INPUT_SHELL="DCache_exposure_layers_0004.tif"
LDR_STACK_5=DCache_stack_ldr_0005.tif
LDR_STACK_5_SHELL="DCache_stack_ldr_0005.tif"
LDR_STACK_5_INPUT=DCache_exposure_layers_0005.tif
LDR_STACK_5_INPUT_SHELL="DCache_exposure_layers_0005.tif"
LDR_STACK_6=DCache_stack_ldr_0006.tif
LDR_STACK_6_SHELL="DCache_stack_ldr_0006.tif"
LDR_STACK_6_INPUT=DCache_exposure_layers_0006.tif
LDR_STACK_6_INPUT_SHELL="DCache_exposure_layers_0006.tif"
LDR_STACK_7=DCache_stack_ldr_0007.tif
LDR_STACK_7_SHELL="DCache_stack_ldr_0007.tif"
LDR_STACK_7_INPUT=DCache_exposure_layers_0007.tif
LDR_STACK_7_INPUT_SHELL="DCache_exposure_layers_0007.tif"
LDR_STACK_8=DCache_stack_ldr_0008.tif
LDR_STACK_8_SHELL="DCache_stack_ldr_0008.tif"
LDR_STACK_8_INPUT=DCache_exposure_layers_0008.tif
LDR_STACK_8_INPUT_SHELL="DCache_exposure_layers_0008.tif"
LDR_STACK_9=DCache_stack_ldr_0009.tif
LDR_STACK_9_SHELL="DCache_stack_ldr_0009.tif"
LDR_STACK_9_INPUT=DCache_exposure_layers_0009.tif
LDR_STACK_9_INPUT_SHELL="DCache_exposure_layers_0009.tif"
LDR_STACKS_NUMBERS=0 1 2 3 4 5 6 7 8 9 
LDR_STACKS=$(LDR_STACK_0) $(LDR_STACK_1) $(LDR_STACK_2) $(LDR_STACK_3) $(LDR_STACK_4) $(LDR_STACK_5) $(LDR_STACK_6) $(LDR_STACK_7) $(LDR_STACK_8) $(LDR_STACK_9) 
LDR_STACKS_SHELL=$(LDR_STACK_0_SHELL) $(LDR_STACK_1_SHELL) $(LDR_STACK_2_SHELL) $(LDR_STACK_3_SHELL) $(LDR_STACK_4_SHELL) $(LDR_STACK_5_SHELL) $(LDR_STACK_6_SHELL) $(LDR_STACK_7_SHELL) $(LDR_STACK_8_SHELL) $(LDR_STACK_9_SHELL) 
DO_LDR_EXPOSURE_LAYERS_FUSED=1

all : startStitching $(LDR_EXPOSURE_LAYERS_FUSED) 

startStitching : 
	@echo ===========================================================================
	@echo Stitching panorama
	@echo ===========================================================================

clean : 
	@echo ===========================================================================
	@echo Remove temporary files
	@echo ===========================================================================
	-$(RM) $(LDR_EXPOSURE_LAYERS_SHELL) $(LDR_EXPOSURE_LAYERS_REMAPPED_SHELL) 

test : 
	@echo ===========================================================================
	@echo Testing programs
	@echo ===========================================================================
	@echo Checking nona...
	@-$(NONA) --help > NUL 2>&1 && echo nona is ok || echo nona failed
	@echo Checking enblend...
	@-$(ENBLEND) -h > NUL 2>&1 && echo enblend is ok || echo enblend failed
	@echo Checking enfuse...
	@-$(ENFUSE) -h > NUL 2>&1 && echo enfuse is ok || echo enfuse failed
	@echo Checking hugin_hdrmerge...
	@-$(HDRMERGE) -h > NUL 2>&1 && echo hugin_hdrmerge is ok || echo hugin_hdrmerge failed
	@echo Checking exiftool...
	@-$(EXIFTOOL) -ver > NUL 2>&1 && echo exiftool is ok || echo exiftool failed

info : 
	@echo ===========================================================================
	@echo ***************  Panorama makefile generated by Hugin       ***************
	@echo ===========================================================================
	@echo System information
	@echo ===========================================================================
	@echo Operating System: Windows (6.2 )
	@echo Architecture: AMD64
	@echo Number of logical processors: 2
	@echo Physical memory: 8264496 kiB (95%% occupied)
	@echo Free space on disc: 355999 MiB
	@echo Active codepage: 1251 (Cyrillic Windows)
	@echo ===========================================================================
	@echo Output options
	@echo ===========================================================================
	@echo Hugin Version: 2013.0.0.0d404a7088e6 built by Matthew Petroff
	@echo Project file: C:\Andruxa\psxdev\circuits\DCache\DCache.pto
	@echo Output prefix: DCache
	@echo Projection: Cylindrical (1)
	@echo Field of view: 3 x 4
	@echo Canvas dimensions: 4909 x 6546
	@echo Crop area: (1002,1232) - (3797,5839)
	@echo Output exposure value: 0.07
	@echo Output stacks minimum overlap: 0.700
	@echo Output layers maximum Ev difference: 0.50
	@echo Selected outputs
	@echo Exposure fusion
	@echo * Blended and fused panorama
	@echo ===========================================================================
	@echo Input images
	@echo ===========================================================================
	@echo Number of images in project file: 10
	@echo Number of active images: 10
	@echo Image 0: C:\Andruxa\psxdev\circuits\DCache\r0061_c0010.jpg
	@echo Image 0: Size 1632x1224, Exposure: 0.07
	@echo Image 1: C:\Andruxa\psxdev\circuits\DCache\r0062_c0010.jpg
	@echo Image 1: Size 1632x1224, Exposure: 0.07
	@echo Image 2: C:\Andruxa\psxdev\circuits\DCache\r0063_c0010.jpg
	@echo Image 2: Size 1632x1224, Exposure: 0.07
	@echo Image 3: C:\Andruxa\psxdev\circuits\DCache\r0064_c0010.jpg
	@echo Image 3: Size 1632x1224, Exposure: 0.07
	@echo Image 4: C:\Andruxa\psxdev\circuits\DCache\r0065_c0010.jpg
	@echo Image 4: Size 1632x1224, Exposure: 0.07
	@echo Image 5: C:\Andruxa\psxdev\circuits\DCache\r0061_c0011.jpg
	@echo Image 5: Size 1632x1224, Exposure: 0.07
	@echo Image 6: C:\Andruxa\psxdev\circuits\DCache\r0062_c0011.jpg
	@echo Image 6: Size 1632x1224, Exposure: 0.07
	@echo Image 7: C:\Andruxa\psxdev\circuits\DCache\r0063_c0011.jpg
	@echo Image 7: Size 1632x1224, Exposure: 0.07
	@echo Image 8: C:\Andruxa\psxdev\circuits\DCache\r0064_c0011.jpg
	@echo Image 8: Size 1632x1224, Exposure: 0.07
	@echo Image 9: C:\Andruxa\psxdev\circuits\DCache\r0065_c0011.jpg
	@echo Image 9: Size 1632x1224, Exposure: 0.07

# Rules for ordinary TIFF_m and hdr output

DCache0000.tif : C:/Andruxa/psxdev/circuits/DCache/r0061_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 0 $(PROJECT_FILE_SHELL)

DCache_hdr_0000.exr : C:/Andruxa/psxdev/circuits/DCache/r0061_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 0 $(PROJECT_FILE_SHELL)

DCache0001.tif : C:/Andruxa/psxdev/circuits/DCache/r0062_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 1 $(PROJECT_FILE_SHELL)

DCache_hdr_0001.exr : C:/Andruxa/psxdev/circuits/DCache/r0062_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 1 $(PROJECT_FILE_SHELL)

DCache0002.tif : C:/Andruxa/psxdev/circuits/DCache/r0063_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 2 $(PROJECT_FILE_SHELL)

DCache_hdr_0002.exr : C:/Andruxa/psxdev/circuits/DCache/r0063_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 2 $(PROJECT_FILE_SHELL)

DCache0003.tif : C:/Andruxa/psxdev/circuits/DCache/r0064_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 3 $(PROJECT_FILE_SHELL)

DCache_hdr_0003.exr : C:/Andruxa/psxdev/circuits/DCache/r0064_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 3 $(PROJECT_FILE_SHELL)

DCache0004.tif : C:/Andruxa/psxdev/circuits/DCache/r0065_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 4 $(PROJECT_FILE_SHELL)

DCache_hdr_0004.exr : C:/Andruxa/psxdev/circuits/DCache/r0065_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 4 $(PROJECT_FILE_SHELL)

DCache0005.tif : C:/Andruxa/psxdev/circuits/DCache/r0061_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 5 $(PROJECT_FILE_SHELL)

DCache_hdr_0005.exr : C:/Andruxa/psxdev/circuits/DCache/r0061_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 5 $(PROJECT_FILE_SHELL)

DCache0006.tif : C:/Andruxa/psxdev/circuits/DCache/r0062_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 6 $(PROJECT_FILE_SHELL)

DCache_hdr_0006.exr : C:/Andruxa/psxdev/circuits/DCache/r0062_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 6 $(PROJECT_FILE_SHELL)

DCache0007.tif : C:/Andruxa/psxdev/circuits/DCache/r0063_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 7 $(PROJECT_FILE_SHELL)

DCache_hdr_0007.exr : C:/Andruxa/psxdev/circuits/DCache/r0063_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 7 $(PROJECT_FILE_SHELL)

DCache0008.tif : C:/Andruxa/psxdev/circuits/DCache/r0064_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 8 $(PROJECT_FILE_SHELL)

DCache_hdr_0008.exr : C:/Andruxa/psxdev/circuits/DCache/r0064_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 8 $(PROJECT_FILE_SHELL)

DCache0009.tif : C:/Andruxa/psxdev/circuits/DCache/r0065_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 9 $(PROJECT_FILE_SHELL)

DCache_hdr_0009.exr : C:/Andruxa/psxdev/circuits/DCache/r0065_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 9 $(PROJECT_FILE_SHELL)

# Rules for exposure layer output

DCache_exposure_layers_0000.tif : C:/Andruxa/psxdev/circuits/DCache/r0061_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.0738879 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 0 $(PROJECT_FILE_SHELL)

DCache_exposure_layers_0001.tif : C:/Andruxa/psxdev/circuits/DCache/r0062_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.0738879 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 1 $(PROJECT_FILE_SHELL)

DCache_exposure_layers_0002.tif : C:/Andruxa/psxdev/circuits/DCache/r0063_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.0738879 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 2 $(PROJECT_FILE_SHELL)

DCache_exposure_layers_0003.tif : C:/Andruxa/psxdev/circuits/DCache/r0064_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.0738879 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 3 $(PROJECT_FILE_SHELL)

DCache_exposure_layers_0004.tif : C:/Andruxa/psxdev/circuits/DCache/r0065_c0010.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.0738879 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 4 $(PROJECT_FILE_SHELL)

DCache_exposure_layers_0005.tif : C:/Andruxa/psxdev/circuits/DCache/r0061_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.0738879 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 5 $(PROJECT_FILE_SHELL)

DCache_exposure_layers_0006.tif : C:/Andruxa/psxdev/circuits/DCache/r0062_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.0738879 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 6 $(PROJECT_FILE_SHELL)

DCache_exposure_layers_0007.tif : C:/Andruxa/psxdev/circuits/DCache/r0063_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.0738879 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 7 $(PROJECT_FILE_SHELL)

DCache_exposure_layers_0008.tif : C:/Andruxa/psxdev/circuits/DCache/r0064_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.0738879 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 8 $(PROJECT_FILE_SHELL)

DCache_exposure_layers_0009.tif : C:/Andruxa/psxdev/circuits/DCache/r0065_c0011.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.0738879 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 9 $(PROJECT_FILE_SHELL)

# Rules for LDR and HDR stack merging, a rule for each stack

$(LDR_STACK_0) : $(LDR_STACK_0_INPUT) 
	$(ENFUSE) $(ENFUSE_OPTS) -o $(LDR_STACK_0_SHELL) -- $(LDR_STACK_0_INPUT_SHELL)
	-$(EXIFTOOL) -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(LDR_STACK_0_SHELL)

$(HDR_STACK_0) : $(HDR_STACK_0_INPUT) 
	$(HDRMERGE) $(HDRMERGE_OPTS) -o $(HDR_STACK_0_SHELL) -- $(HDR_STACK_0_INPUT_SHELL)

$(LDR_STACK_1) : $(LDR_STACK_1_INPUT) 
	$(ENFUSE) $(ENFUSE_OPTS) -o $(LDR_STACK_1_SHELL) -- $(LDR_STACK_1_INPUT_SHELL)
	-$(EXIFTOOL) -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(LDR_STACK_1_SHELL)

$(HDR_STACK_1) : $(HDR_STACK_1_INPUT) 
	$(HDRMERGE) $(HDRMERGE_OPTS) -o $(HDR_STACK_1_SHELL) -- $(HDR_STACK_1_INPUT_SHELL)

$(LDR_STACK_2) : $(LDR_STACK_2_INPUT) 
	$(ENFUSE) $(ENFUSE_OPTS) -o $(LDR_STACK_2_SHELL) -- $(LDR_STACK_2_INPUT_SHELL)
	-$(EXIFTOOL) -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(LDR_STACK_2_SHELL)

$(HDR_STACK_2) : $(HDR_STACK_2_INPUT) 
	$(HDRMERGE) $(HDRMERGE_OPTS) -o $(HDR_STACK_2_SHELL) -- $(HDR_STACK_2_INPUT_SHELL)

$(LDR_STACK_3) : $(LDR_STACK_3_INPUT) 
	$(ENFUSE) $(ENFUSE_OPTS) -o $(LDR_STACK_3_SHELL) -- $(LDR_STACK_3_INPUT_SHELL)
	-$(EXIFTOOL) -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(LDR_STACK_3_SHELL)

$(HDR_STACK_3) : $(HDR_STACK_3_INPUT) 
	$(HDRMERGE) $(HDRMERGE_OPTS) -o $(HDR_STACK_3_SHELL) -- $(HDR_STACK_3_INPUT_SHELL)

$(LDR_STACK_4) : $(LDR_STACK_4_INPUT) 
	$(ENFUSE) $(ENFUSE_OPTS) -o $(LDR_STACK_4_SHELL) -- $(LDR_STACK_4_INPUT_SHELL)
	-$(EXIFTOOL) -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(LDR_STACK_4_SHELL)

$(HDR_STACK_4) : $(HDR_STACK_4_INPUT) 
	$(HDRMERGE) $(HDRMERGE_OPTS) -o $(HDR_STACK_4_SHELL) -- $(HDR_STACK_4_INPUT_SHELL)

$(LDR_STACK_5) : $(LDR_STACK_5_INPUT) 
	$(ENFUSE) $(ENFUSE_OPTS) -o $(LDR_STACK_5_SHELL) -- $(LDR_STACK_5_INPUT_SHELL)
	-$(EXIFTOOL) -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(LDR_STACK_5_SHELL)

$(HDR_STACK_5) : $(HDR_STACK_5_INPUT) 
	$(HDRMERGE) $(HDRMERGE_OPTS) -o $(HDR_STACK_5_SHELL) -- $(HDR_STACK_5_INPUT_SHELL)

$(LDR_STACK_6) : $(LDR_STACK_6_INPUT) 
	$(ENFUSE) $(ENFUSE_OPTS) -o $(LDR_STACK_6_SHELL) -- $(LDR_STACK_6_INPUT_SHELL)
	-$(EXIFTOOL) -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(LDR_STACK_6_SHELL)

$(HDR_STACK_6) : $(HDR_STACK_6_INPUT) 
	$(HDRMERGE) $(HDRMERGE_OPTS) -o $(HDR_STACK_6_SHELL) -- $(HDR_STACK_6_INPUT_SHELL)

$(LDR_STACK_7) : $(LDR_STACK_7_INPUT) 
	$(ENFUSE) $(ENFUSE_OPTS) -o $(LDR_STACK_7_SHELL) -- $(LDR_STACK_7_INPUT_SHELL)
	-$(EXIFTOOL) -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(LDR_STACK_7_SHELL)

$(HDR_STACK_7) : $(HDR_STACK_7_INPUT) 
	$(HDRMERGE) $(HDRMERGE_OPTS) -o $(HDR_STACK_7_SHELL) -- $(HDR_STACK_7_INPUT_SHELL)

$(LDR_STACK_8) : $(LDR_STACK_8_INPUT) 
	$(ENFUSE) $(ENFUSE_OPTS) -o $(LDR_STACK_8_SHELL) -- $(LDR_STACK_8_INPUT_SHELL)
	-$(EXIFTOOL) -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(LDR_STACK_8_SHELL)

$(HDR_STACK_8) : $(HDR_STACK_8_INPUT) 
	$(HDRMERGE) $(HDRMERGE_OPTS) -o $(HDR_STACK_8_SHELL) -- $(HDR_STACK_8_INPUT_SHELL)

$(LDR_STACK_9) : $(LDR_STACK_9_INPUT) 
	$(ENFUSE) $(ENFUSE_OPTS) -o $(LDR_STACK_9_SHELL) -- $(LDR_STACK_9_INPUT_SHELL)
	-$(EXIFTOOL) -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(LDR_STACK_9_SHELL)

$(HDR_STACK_9) : $(HDR_STACK_9_INPUT) 
	$(HDRMERGE) $(HDRMERGE_OPTS) -o $(HDR_STACK_9_SHELL) -- $(HDR_STACK_9_INPUT_SHELL)

$(LDR_BLENDED) : $(LDR_LAYERS) 
	$(ENBLEND) $(ENBLEND_LDR_COMP) $(ENBLEND_OPTS) -o $(LDR_BLENDED_SHELL) -- $(LDR_LAYERS_SHELL)
	-$(EXIFTOOL) -E -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(EXIFTOOL_INFO_ARGS) $(LDR_BLENDED_SHELL)

$(LDR_EXPOSURE_LAYER_0) : $(LDR_EXPOSURE_LAYER_0_INPUT) 
	$(ENBLEND) $(ENBLEND_EXPOSURE_COMP) $(ENBLEND_OPTS) -o $(LDR_EXPOSURE_LAYER_0_SHELL) -- $(LDR_EXPOSURE_LAYER_0_INPUT_SHELL)
	-$(EXIFTOOL) -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(LDR_EXPOSURE_LAYER_0_SHELL)

$(LDR_STACKED_BLENDED) : $(LDR_STACKS) 
	$(ENBLEND) $(ENBLEND_LDR_COMP) $(ENBLEND_OPTS) -o $(LDR_STACKED_BLENDED_SHELL) -- $(LDR_STACKS_SHELL)
	-$(EXIFTOOL) -E -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(EXIFTOOL_INFO_ARGS) $(LDR_STACKED_BLENDED_SHELL)

$(LDR_EXPOSURE_LAYERS_FUSED) : $(LDR_EXPOSURE_LAYERS) 
	$(ENFUSE) $(ENBLEND_LDR_COMP) $(ENFUSE_OPTS) -o $(LDR_EXPOSURE_LAYERS_FUSED_SHELL) -- $(LDR_EXPOSURE_LAYERS_SHELL)
	-$(EXIFTOOL) -E -overwrite_original_in_place -TagsFromFile $(INPUT_IMAGE_1_SHELL) $(EXIFTOOL_COPY_ARGS) $(EXIFTOOL_INFO_ARGS) $(LDR_EXPOSURE_LAYERS_FUSED_SHELL)

$(HDR_BLENDED) : $(HDR_STACKS) 
	$(ENBLEND) $(ENBLEND_HDR_COMP) $(ENBLEND_OPTS) -o $(HDR_BLENDED_SHELL) -- $(HDR_STACKS_SHELL)

$(LDR_REMAPPED_PREFIX)_multilayer.tif : $(LDR_LAYERS) 
	tiffcp $(LDR_LAYERS_SHELL) $(LDR_REMAPPED_PREFIX_SHELL)_multilayer.tif

$(LDR_REMAPPED_PREFIX)_fused_multilayer.tif : $(LDR_STACKS) $(LDR_EXPOSURE_LAYERS) 
	tiffcp $(LDR_STACKS_SHELL) $(LDR_EXPOSURE_LAYERS_SHELL) $(LDR_REMAPPED_PREFIX_SHELL)_fused_multilayer.tif

$(LDR_REMAPPED_PREFIX)_multilayer.psd : $(LDR_LAYERS) 
	PTtiff2psd -o $(LDR_REMAPPED_PREFIX_SHELL)_multilayer.psd $(LDR_LAYERS_SHELL)

$(LDR_REMAPPED_PREFIX)_fused_multilayer.psd : $(LDR_STACKS) $(LDR_EXPOSURE_LAYERS) 
	PTtiff2psd -o $(LDR_REMAPPED_PREFIX_SHELL)_fused_multilayer.psd $(LDR_STACKS_SHELL)$(LDR_EXPOSURE_LAYERS_SHELL)
