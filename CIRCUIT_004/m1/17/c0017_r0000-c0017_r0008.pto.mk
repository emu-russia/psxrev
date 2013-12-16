
# makefile for panorama stitching, created by hugin using the new makefilelib

# set temporary directory for not UNIX_LIKE
TMP=D:\TEMP

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
HUGIN_PROJECTION=2
HUGIN_HFOV=5
HUGIN_WIDTH=1862
HUGIN_HEIGHT=8938

# options for the programs
NONA_LDR_REMAPPED_COMP=-z LZW
NONA_OPTS=
ENBLEND_OPTS= -f1553x5806+109+371
ENBLEND_LDR_COMP=--compression=LZW
ENBLEND_EXPOSURE_COMP=--compression=LZW
ENBLEND_HDR_COMP=
HDRMERGE_OPTS=-m avg -c
ENFUSE_OPTS=
EXIFTOOL_COPY_ARGS=-ImageDescription -Make -Model -Artist -WhitePoint -Copyright -GPS:all -DateTimeOriginal -CreateDate -UserComment -ColorSpace -OwnerName -SerialNumber
EXIFTOOL_INFO_ARGS="-Software=Hugin 2012.0.0.a6e4184ad538 built by Matthew Petroff" "-UserComment<$${UserComment}&\#xd;&\#xa;Projection: Equirectangular (2)&\#xd;&\#xa;FOV: 5 x 24&\#xd;&\#xa;Ev: 0.24" -f

# the output panorama
LDR_REMAPPED_PREFIX=c0017_r0000-c0017_r0008
LDR_REMAPPED_PREFIX_SHELL="c0017_r0000-c0017_r0008"
HDR_STACK_REMAPPED_PREFIX=c0017_r0000-c0017_r0008_hdr_
HDR_STACK_REMAPPED_PREFIX_SHELL="c0017_r0000-c0017_r0008_hdr_"
LDR_EXPOSURE_REMAPPED_PREFIX=c0017_r0000-c0017_r0008_exposure_layers_
LDR_EXPOSURE_REMAPPED_PREFIX_SHELL="c0017_r0000-c0017_r0008_exposure_layers_"
PROJECT_FILE=C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0000-c0017_r0008.pto
PROJECT_FILE_SHELL="C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0000-c0017_r0008.pto"
LDR_BLENDED=c0017_r0000-c0017_r0008.tif
LDR_BLENDED_SHELL="c0017_r0000-c0017_r0008.tif"
LDR_STACKED_BLENDED=c0017_r0000-c0017_r0008_fused.tif
LDR_STACKED_BLENDED_SHELL="c0017_r0000-c0017_r0008_fused.tif"
LDR_EXPOSURE_LAYERS_FUSED=c0017_r0000-c0017_r0008_blended_fused.tif
LDR_EXPOSURE_LAYERS_FUSED_SHELL="c0017_r0000-c0017_r0008_blended_fused.tif"
HDR_BLENDED=c0017_r0000-c0017_r0008_hdr.exr
HDR_BLENDED_SHELL="c0017_r0000-c0017_r0008_hdr.exr"

# first input image
INPUT_IMAGE_1=C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0000.jpg
INPUT_IMAGE_1_SHELL="C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0000.jpg"

# all input images
INPUT_IMAGES=C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0000.jpg\
C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0001.jpg\
C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0002.jpg\
C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0003.jpg\
C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0004.jpg\
C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0005.jpg\
C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0006.jpg\
C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0007.jpg\
C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0008.jpg
INPUT_IMAGES_SHELL="C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0000.jpg"\
"C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0001.jpg"\
"C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0002.jpg"\
"C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0003.jpg"\
"C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0004.jpg"\
"C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0005.jpg"\
"C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0006.jpg"\
"C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0007.jpg"\
"C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0008.jpg"

# remapped images
LDR_LAYERS=c0017_r0000-c0017_r00080000.tif\
c0017_r0000-c0017_r00080001.tif\
c0017_r0000-c0017_r00080002.tif\
c0017_r0000-c0017_r00080003.tif\
c0017_r0000-c0017_r00080004.tif\
c0017_r0000-c0017_r00080005.tif\
c0017_r0000-c0017_r00080006.tif\
c0017_r0000-c0017_r00080007.tif\
c0017_r0000-c0017_r00080008.tif
LDR_LAYERS_SHELL="c0017_r0000-c0017_r00080000.tif"\
"c0017_r0000-c0017_r00080001.tif"\
"c0017_r0000-c0017_r00080002.tif"\
"c0017_r0000-c0017_r00080003.tif"\
"c0017_r0000-c0017_r00080004.tif"\
"c0017_r0000-c0017_r00080005.tif"\
"c0017_r0000-c0017_r00080006.tif"\
"c0017_r0000-c0017_r00080007.tif"\
"c0017_r0000-c0017_r00080008.tif"

# remapped images (hdr)
HDR_LAYERS=c0017_r0000-c0017_r0008_hdr_0000.exr\
c0017_r0000-c0017_r0008_hdr_0001.exr\
c0017_r0000-c0017_r0008_hdr_0002.exr\
c0017_r0000-c0017_r0008_hdr_0003.exr\
c0017_r0000-c0017_r0008_hdr_0004.exr\
c0017_r0000-c0017_r0008_hdr_0005.exr\
c0017_r0000-c0017_r0008_hdr_0006.exr\
c0017_r0000-c0017_r0008_hdr_0007.exr\
c0017_r0000-c0017_r0008_hdr_0008.exr
HDR_LAYERS_SHELL="c0017_r0000-c0017_r0008_hdr_0000.exr"\
"c0017_r0000-c0017_r0008_hdr_0001.exr"\
"c0017_r0000-c0017_r0008_hdr_0002.exr"\
"c0017_r0000-c0017_r0008_hdr_0003.exr"\
"c0017_r0000-c0017_r0008_hdr_0004.exr"\
"c0017_r0000-c0017_r0008_hdr_0005.exr"\
"c0017_r0000-c0017_r0008_hdr_0006.exr"\
"c0017_r0000-c0017_r0008_hdr_0007.exr"\
"c0017_r0000-c0017_r0008_hdr_0008.exr"

# remapped maxval images
HDR_LAYERS_WEIGHTS=c0017_r0000-c0017_r0008_hdr_0000_gray.pgm\
c0017_r0000-c0017_r0008_hdr_0001_gray.pgm\
c0017_r0000-c0017_r0008_hdr_0002_gray.pgm\
c0017_r0000-c0017_r0008_hdr_0003_gray.pgm\
c0017_r0000-c0017_r0008_hdr_0004_gray.pgm\
c0017_r0000-c0017_r0008_hdr_0005_gray.pgm\
c0017_r0000-c0017_r0008_hdr_0006_gray.pgm\
c0017_r0000-c0017_r0008_hdr_0007_gray.pgm\
c0017_r0000-c0017_r0008_hdr_0008_gray.pgm
HDR_LAYERS_WEIGHTS_SHELL="c0017_r0000-c0017_r0008_hdr_0000_gray.pgm"\
"c0017_r0000-c0017_r0008_hdr_0001_gray.pgm"\
"c0017_r0000-c0017_r0008_hdr_0002_gray.pgm"\
"c0017_r0000-c0017_r0008_hdr_0003_gray.pgm"\
"c0017_r0000-c0017_r0008_hdr_0004_gray.pgm"\
"c0017_r0000-c0017_r0008_hdr_0005_gray.pgm"\
"c0017_r0000-c0017_r0008_hdr_0006_gray.pgm"\
"c0017_r0000-c0017_r0008_hdr_0007_gray.pgm"\
"c0017_r0000-c0017_r0008_hdr_0008_gray.pgm"

# stacked hdr images
HDR_STACK_0=c0017_r0000-c0017_r0008_stack_hdr_0000.exr
HDR_STACK_0_SHELL="c0017_r0000-c0017_r0008_stack_hdr_0000.exr"
HDR_STACK_0_INPUT=c0017_r0000-c0017_r0008_hdr_0000.exr
HDR_STACK_0_INPUT_SHELL="c0017_r0000-c0017_r0008_hdr_0000.exr"
HDR_STACK_1=c0017_r0000-c0017_r0008_stack_hdr_0001.exr
HDR_STACK_1_SHELL="c0017_r0000-c0017_r0008_stack_hdr_0001.exr"
HDR_STACK_1_INPUT=c0017_r0000-c0017_r0008_hdr_0001.exr
HDR_STACK_1_INPUT_SHELL="c0017_r0000-c0017_r0008_hdr_0001.exr"
HDR_STACK_2=c0017_r0000-c0017_r0008_stack_hdr_0002.exr
HDR_STACK_2_SHELL="c0017_r0000-c0017_r0008_stack_hdr_0002.exr"
HDR_STACK_2_INPUT=c0017_r0000-c0017_r0008_hdr_0002.exr
HDR_STACK_2_INPUT_SHELL="c0017_r0000-c0017_r0008_hdr_0002.exr"
HDR_STACK_3=c0017_r0000-c0017_r0008_stack_hdr_0003.exr
HDR_STACK_3_SHELL="c0017_r0000-c0017_r0008_stack_hdr_0003.exr"
HDR_STACK_3_INPUT=c0017_r0000-c0017_r0008_hdr_0003.exr
HDR_STACK_3_INPUT_SHELL="c0017_r0000-c0017_r0008_hdr_0003.exr"
HDR_STACK_4=c0017_r0000-c0017_r0008_stack_hdr_0004.exr
HDR_STACK_4_SHELL="c0017_r0000-c0017_r0008_stack_hdr_0004.exr"
HDR_STACK_4_INPUT=c0017_r0000-c0017_r0008_hdr_0004.exr
HDR_STACK_4_INPUT_SHELL="c0017_r0000-c0017_r0008_hdr_0004.exr"
HDR_STACK_5=c0017_r0000-c0017_r0008_stack_hdr_0005.exr
HDR_STACK_5_SHELL="c0017_r0000-c0017_r0008_stack_hdr_0005.exr"
HDR_STACK_5_INPUT=c0017_r0000-c0017_r0008_hdr_0005.exr
HDR_STACK_5_INPUT_SHELL="c0017_r0000-c0017_r0008_hdr_0005.exr"
HDR_STACK_6=c0017_r0000-c0017_r0008_stack_hdr_0006.exr
HDR_STACK_6_SHELL="c0017_r0000-c0017_r0008_stack_hdr_0006.exr"
HDR_STACK_6_INPUT=c0017_r0000-c0017_r0008_hdr_0006.exr
HDR_STACK_6_INPUT_SHELL="c0017_r0000-c0017_r0008_hdr_0006.exr"
HDR_STACK_7=c0017_r0000-c0017_r0008_stack_hdr_0007.exr
HDR_STACK_7_SHELL="c0017_r0000-c0017_r0008_stack_hdr_0007.exr"
HDR_STACK_7_INPUT=c0017_r0000-c0017_r0008_hdr_0007.exr
HDR_STACK_7_INPUT_SHELL="c0017_r0000-c0017_r0008_hdr_0007.exr"
HDR_STACK_8=c0017_r0000-c0017_r0008_stack_hdr_0008.exr
HDR_STACK_8_SHELL="c0017_r0000-c0017_r0008_stack_hdr_0008.exr"
HDR_STACK_8_INPUT=c0017_r0000-c0017_r0008_hdr_0008.exr
HDR_STACK_8_INPUT_SHELL="c0017_r0000-c0017_r0008_hdr_0008.exr"
HDR_STACKS_NUMBERS=0 1 2 3 4 5 6 7 8 
HDR_STACKS=$(HDR_STACK_0) $(HDR_STACK_1) $(HDR_STACK_2) $(HDR_STACK_3) $(HDR_STACK_4) $(HDR_STACK_5) $(HDR_STACK_6) $(HDR_STACK_7) $(HDR_STACK_8) 
HDR_STACKS_SHELL=$(HDR_STACK_0_SHELL) $(HDR_STACK_1_SHELL) $(HDR_STACK_2_SHELL) $(HDR_STACK_3_SHELL) $(HDR_STACK_4_SHELL) $(HDR_STACK_5_SHELL) $(HDR_STACK_6_SHELL) $(HDR_STACK_7_SHELL) $(HDR_STACK_8_SHELL) 

# number of image sets with similar exposure
LDR_EXPOSURE_LAYER_0=c0017_r0000-c0017_r0008_exposure_0000.tif
LDR_EXPOSURE_LAYER_0_SHELL="c0017_r0000-c0017_r0008_exposure_0000.tif"
LDR_EXPOSURE_LAYER_0_INPUT=c0017_r0000-c0017_r0008_exposure_layers_0000.tif\
c0017_r0000-c0017_r0008_exposure_layers_0001.tif\
c0017_r0000-c0017_r0008_exposure_layers_0002.tif\
c0017_r0000-c0017_r0008_exposure_layers_0003.tif\
c0017_r0000-c0017_r0008_exposure_layers_0004.tif\
c0017_r0000-c0017_r0008_exposure_layers_0005.tif\
c0017_r0000-c0017_r0008_exposure_layers_0006.tif\
c0017_r0000-c0017_r0008_exposure_layers_0007.tif\
c0017_r0000-c0017_r0008_exposure_layers_0008.tif
LDR_EXPOSURE_LAYER_0_INPUT_SHELL="c0017_r0000-c0017_r0008_exposure_layers_0000.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0001.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0002.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0003.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0004.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0005.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0006.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0007.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0008.tif"
LDR_EXPOSURE_LAYER_0_INPUT_PTMENDER=c0017_r0000-c0017_r00080000.tif\
c0017_r0000-c0017_r00080001.tif\
c0017_r0000-c0017_r00080002.tif\
c0017_r0000-c0017_r00080003.tif\
c0017_r0000-c0017_r00080004.tif\
c0017_r0000-c0017_r00080005.tif\
c0017_r0000-c0017_r00080006.tif\
c0017_r0000-c0017_r00080007.tif\
c0017_r0000-c0017_r00080008.tif
LDR_EXPOSURE_LAYER_0_INPUT_PTMENDER_SHELL="c0017_r0000-c0017_r00080000.tif"\
"c0017_r0000-c0017_r00080001.tif"\
"c0017_r0000-c0017_r00080002.tif"\
"c0017_r0000-c0017_r00080003.tif"\
"c0017_r0000-c0017_r00080004.tif"\
"c0017_r0000-c0017_r00080005.tif"\
"c0017_r0000-c0017_r00080006.tif"\
"c0017_r0000-c0017_r00080007.tif"\
"c0017_r0000-c0017_r00080008.tif"
LDR_EXPOSURE_LAYER_0_EXPOSURE=0.243093
LDR_EXPOSURE_LAYERS_NUMBERS=0 
LDR_EXPOSURE_LAYERS=$(LDR_EXPOSURE_LAYER_0) 
LDR_EXPOSURE_LAYERS_SHELL=$(LDR_EXPOSURE_LAYER_0_SHELL) 
LDR_EXPOSURE_LAYERS_REMAPPED=c0017_r0000-c0017_r0008_exposure_layers_0000.tif\
c0017_r0000-c0017_r0008_exposure_layers_0001.tif\
c0017_r0000-c0017_r0008_exposure_layers_0002.tif\
c0017_r0000-c0017_r0008_exposure_layers_0003.tif\
c0017_r0000-c0017_r0008_exposure_layers_0004.tif\
c0017_r0000-c0017_r0008_exposure_layers_0005.tif\
c0017_r0000-c0017_r0008_exposure_layers_0006.tif\
c0017_r0000-c0017_r0008_exposure_layers_0007.tif\
c0017_r0000-c0017_r0008_exposure_layers_0008.tif
LDR_EXPOSURE_LAYERS_REMAPPED_SHELL="c0017_r0000-c0017_r0008_exposure_layers_0000.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0001.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0002.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0003.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0004.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0005.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0006.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0007.tif"\
"c0017_r0000-c0017_r0008_exposure_layers_0008.tif"

# stacked ldr images
LDR_STACK_0=c0017_r0000-c0017_r0008_stack_ldr_0000.tif
LDR_STACK_0_SHELL="c0017_r0000-c0017_r0008_stack_ldr_0000.tif"
LDR_STACK_0_INPUT=c0017_r0000-c0017_r0008_exposure_layers_0000.tif
LDR_STACK_0_INPUT_SHELL="c0017_r0000-c0017_r0008_exposure_layers_0000.tif"
LDR_STACK_1=c0017_r0000-c0017_r0008_stack_ldr_0001.tif
LDR_STACK_1_SHELL="c0017_r0000-c0017_r0008_stack_ldr_0001.tif"
LDR_STACK_1_INPUT=c0017_r0000-c0017_r0008_exposure_layers_0001.tif
LDR_STACK_1_INPUT_SHELL="c0017_r0000-c0017_r0008_exposure_layers_0001.tif"
LDR_STACK_2=c0017_r0000-c0017_r0008_stack_ldr_0002.tif
LDR_STACK_2_SHELL="c0017_r0000-c0017_r0008_stack_ldr_0002.tif"
LDR_STACK_2_INPUT=c0017_r0000-c0017_r0008_exposure_layers_0002.tif
LDR_STACK_2_INPUT_SHELL="c0017_r0000-c0017_r0008_exposure_layers_0002.tif"
LDR_STACK_3=c0017_r0000-c0017_r0008_stack_ldr_0003.tif
LDR_STACK_3_SHELL="c0017_r0000-c0017_r0008_stack_ldr_0003.tif"
LDR_STACK_3_INPUT=c0017_r0000-c0017_r0008_exposure_layers_0003.tif
LDR_STACK_3_INPUT_SHELL="c0017_r0000-c0017_r0008_exposure_layers_0003.tif"
LDR_STACK_4=c0017_r0000-c0017_r0008_stack_ldr_0004.tif
LDR_STACK_4_SHELL="c0017_r0000-c0017_r0008_stack_ldr_0004.tif"
LDR_STACK_4_INPUT=c0017_r0000-c0017_r0008_exposure_layers_0004.tif
LDR_STACK_4_INPUT_SHELL="c0017_r0000-c0017_r0008_exposure_layers_0004.tif"
LDR_STACK_5=c0017_r0000-c0017_r0008_stack_ldr_0005.tif
LDR_STACK_5_SHELL="c0017_r0000-c0017_r0008_stack_ldr_0005.tif"
LDR_STACK_5_INPUT=c0017_r0000-c0017_r0008_exposure_layers_0005.tif
LDR_STACK_5_INPUT_SHELL="c0017_r0000-c0017_r0008_exposure_layers_0005.tif"
LDR_STACK_6=c0017_r0000-c0017_r0008_stack_ldr_0006.tif
LDR_STACK_6_SHELL="c0017_r0000-c0017_r0008_stack_ldr_0006.tif"
LDR_STACK_6_INPUT=c0017_r0000-c0017_r0008_exposure_layers_0006.tif
LDR_STACK_6_INPUT_SHELL="c0017_r0000-c0017_r0008_exposure_layers_0006.tif"
LDR_STACK_7=c0017_r0000-c0017_r0008_stack_ldr_0007.tif
LDR_STACK_7_SHELL="c0017_r0000-c0017_r0008_stack_ldr_0007.tif"
LDR_STACK_7_INPUT=c0017_r0000-c0017_r0008_exposure_layers_0007.tif
LDR_STACK_7_INPUT_SHELL="c0017_r0000-c0017_r0008_exposure_layers_0007.tif"
LDR_STACK_8=c0017_r0000-c0017_r0008_stack_ldr_0008.tif
LDR_STACK_8_SHELL="c0017_r0000-c0017_r0008_stack_ldr_0008.tif"
LDR_STACK_8_INPUT=c0017_r0000-c0017_r0008_exposure_layers_0008.tif
LDR_STACK_8_INPUT_SHELL="c0017_r0000-c0017_r0008_exposure_layers_0008.tif"
LDR_STACKS_NUMBERS=0 1 2 3 4 5 6 7 8 
LDR_STACKS=$(LDR_STACK_0) $(LDR_STACK_1) $(LDR_STACK_2) $(LDR_STACK_3) $(LDR_STACK_4) $(LDR_STACK_5) $(LDR_STACK_6) $(LDR_STACK_7) $(LDR_STACK_8) 
LDR_STACKS_SHELL=$(LDR_STACK_0_SHELL) $(LDR_STACK_1_SHELL) $(LDR_STACK_2_SHELL) $(LDR_STACK_3_SHELL) $(LDR_STACK_4_SHELL) $(LDR_STACK_5_SHELL) $(LDR_STACK_6_SHELL) $(LDR_STACK_7_SHELL) $(LDR_STACK_8_SHELL) 
DO_LDR_BLENDED=1

all : startStitching $(LDR_BLENDED) 

startStitching : 
	@echo ===========================================================================
	@echo Stitching panorama
	@echo ===========================================================================

clean : 
	@echo ===========================================================================
	@echo Remove temporary files
	@echo ===========================================================================
	-$(RM) $(LDR_LAYERS_SHELL) 

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
	@echo Operating System: Windows 7 (6.1 Service Pack 1)
	@echo Architecture: x86
	@echo Number of logical processors: 2
	@echo Physical memory: 3587384 kiB (61%% occupied)
	@echo Free space on disc: 78140 MiB
	@echo Active codepage: 1251 (Cyrillic Windows)
	@echo ===========================================================================
	@echo Output options
	@echo ===========================================================================
	@echo Hugin Version: 2012.0.0.a6e4184ad538 built by Matthew Petroff
	@echo Project file: C:\Andruxa\psxdev\CIRCUIT_004\m1\17\c0017_r0000-c0017_r0008.pto
	@echo Output prefix: c0017_r0000-c0017_r0008
	@echo Projection: Equirectangular (2)
	@echo Field of view: 5 x 24
	@echo Canvas dimensions: 1862 x 8938
	@echo Crop area: (109,371) - (1662,6177)
	@echo Output exposure value: 0.24
	@echo Selected outputs
	@echo Normal panorama
	@echo * Blended panorama
	@echo ===========================================================================
	@echo Input images
	@echo ===========================================================================
	@echo Number of images in project file: 9
	@echo Number of active images: 9
	@echo Image 0: C:\Andruxa\psxdev\CIRCUIT_004\m1\17\c0017_r0000.jpg
	@echo Image 0: Size 1632x1224, Exposure: 0.24
	@echo Image 1: C:\Andruxa\psxdev\CIRCUIT_004\m1\17\c0017_r0001.jpg
	@echo Image 1: Size 1632x1224, Exposure: 0.24
	@echo Image 2: C:\Andruxa\psxdev\CIRCUIT_004\m1\17\c0017_r0002.jpg
	@echo Image 2: Size 1632x1224, Exposure: 0.24
	@echo Image 3: C:\Andruxa\psxdev\CIRCUIT_004\m1\17\c0017_r0003.jpg
	@echo Image 3: Size 1632x1224, Exposure: 0.24
	@echo Image 4: C:\Andruxa\psxdev\CIRCUIT_004\m1\17\c0017_r0004.jpg
	@echo Image 4: Size 1632x1224, Exposure: 0.24
	@echo Image 5: C:\Andruxa\psxdev\CIRCUIT_004\m1\17\c0017_r0005.jpg
	@echo Image 5: Size 1632x1224, Exposure: 0.24
	@echo Image 6: C:\Andruxa\psxdev\CIRCUIT_004\m1\17\c0017_r0006.jpg
	@echo Image 6: Size 1632x1224, Exposure: 0.24
	@echo Image 7: C:\Andruxa\psxdev\CIRCUIT_004\m1\17\c0017_r0007.jpg
	@echo Image 7: Size 1632x1224, Exposure: 0.24
	@echo Image 8: C:\Andruxa\psxdev\CIRCUIT_004\m1\17\c0017_r0008.jpg
	@echo Image 8: Size 1632x1224, Exposure: 0.24

# Rules for ordinary TIFF_m and hdr output

c0017_r0000-c0017_r00080000.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0000.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 0 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_hdr_0000.exr : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0000.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 0 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r00080001.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0001.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 1 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_hdr_0001.exr : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0001.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 1 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r00080002.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0002.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 2 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_hdr_0002.exr : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0002.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 2 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r00080003.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0003.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 3 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_hdr_0003.exr : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0003.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 3 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r00080004.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0004.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 4 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_hdr_0004.exr : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0004.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 4 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r00080005.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0005.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 5 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_hdr_0005.exr : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0005.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 5 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r00080006.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0006.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 6 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_hdr_0006.exr : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0006.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 6 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r00080007.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0007.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 7 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_hdr_0007.exr : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0007.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 7 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r00080008.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0008.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 8 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_hdr_0008.exr : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0008.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 8 $(PROJECT_FILE_SHELL)

# Rules for exposure layer output

c0017_r0000-c0017_r0008_exposure_layers_0000.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0000.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.243093 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 0 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_exposure_layers_0001.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0001.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.243093 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 1 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_exposure_layers_0002.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0002.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.243093 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 2 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_exposure_layers_0003.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0003.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.243093 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 3 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_exposure_layers_0004.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0004.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.243093 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 4 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_exposure_layers_0005.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0005.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.243093 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 5 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_exposure_layers_0006.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0006.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.243093 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 6 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_exposure_layers_0007.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0007.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.243093 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 7 $(PROJECT_FILE_SHELL)

c0017_r0000-c0017_r0008_exposure_layers_0008.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/17/c0017_r0008.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.243093 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 8 $(PROJECT_FILE_SHELL)

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
