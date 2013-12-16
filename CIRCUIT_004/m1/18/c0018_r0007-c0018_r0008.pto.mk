
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
HUGIN_PROJECTION=0
HUGIN_HFOV=12
HUGIN_WIDTH=1449
HUGIN_HEIGHT=1938

# options for the programs
NONA_LDR_REMAPPED_COMP=-z LZW
NONA_OPTS=
ENBLEND_OPTS= -f1187x1598+159+195
ENBLEND_LDR_COMP=--compression=LZW
ENBLEND_EXPOSURE_COMP=--compression=LZW
ENBLEND_HDR_COMP=
HDRMERGE_OPTS=-m avg -c
ENFUSE_OPTS=
EXIFTOOL_COPY_ARGS=-ImageDescription -Make -Model -Artist -WhitePoint -Copyright -GPS:all -DateTimeOriginal -CreateDate -UserComment -ColorSpace -OwnerName -SerialNumber
EXIFTOOL_INFO_ARGS="-Software=Hugin 2012.0.0.a6e4184ad538 built by Matthew Petroff" "-UserComment<$${UserComment}&\#xd;&\#xa;Projection: Rectilinear (0)&\#xd;&\#xa;FOV: 12 x 16&\#xd;&\#xa;Ev: 0.42" -f

# the output panorama
LDR_REMAPPED_PREFIX=c0018_r0007-c0018_r0008
LDR_REMAPPED_PREFIX_SHELL="c0018_r0007-c0018_r0008"
HDR_STACK_REMAPPED_PREFIX=c0018_r0007-c0018_r0008_hdr_
HDR_STACK_REMAPPED_PREFIX_SHELL="c0018_r0007-c0018_r0008_hdr_"
LDR_EXPOSURE_REMAPPED_PREFIX=c0018_r0007-c0018_r0008_exposure_layers_
LDR_EXPOSURE_REMAPPED_PREFIX_SHELL="c0018_r0007-c0018_r0008_exposure_layers_"
PROJECT_FILE=C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0007-c0018_r0008.pto
PROJECT_FILE_SHELL="C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0007-c0018_r0008.pto"
LDR_BLENDED=c0018_r0007-c0018_r0008.tif
LDR_BLENDED_SHELL="c0018_r0007-c0018_r0008.tif"
LDR_STACKED_BLENDED=c0018_r0007-c0018_r0008_fused.tif
LDR_STACKED_BLENDED_SHELL="c0018_r0007-c0018_r0008_fused.tif"
LDR_EXPOSURE_LAYERS_FUSED=c0018_r0007-c0018_r0008_blended_fused.tif
LDR_EXPOSURE_LAYERS_FUSED_SHELL="c0018_r0007-c0018_r0008_blended_fused.tif"
HDR_BLENDED=c0018_r0007-c0018_r0008_hdr.exr
HDR_BLENDED_SHELL="c0018_r0007-c0018_r0008_hdr.exr"

# first input image
INPUT_IMAGE_1=C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0007.jpg
INPUT_IMAGE_1_SHELL="C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0007.jpg"

# all input images
INPUT_IMAGES=C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0007.jpg\
C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0008.jpg
INPUT_IMAGES_SHELL="C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0007.jpg"\
"C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0008.jpg"

# remapped images
LDR_LAYERS=c0018_r0007-c0018_r00080000.tif\
c0018_r0007-c0018_r00080001.tif
LDR_LAYERS_SHELL="c0018_r0007-c0018_r00080000.tif"\
"c0018_r0007-c0018_r00080001.tif"

# remapped images (hdr)
HDR_LAYERS=c0018_r0007-c0018_r0008_hdr_0000.exr\
c0018_r0007-c0018_r0008_hdr_0001.exr
HDR_LAYERS_SHELL="c0018_r0007-c0018_r0008_hdr_0000.exr"\
"c0018_r0007-c0018_r0008_hdr_0001.exr"

# remapped maxval images
HDR_LAYERS_WEIGHTS=c0018_r0007-c0018_r0008_hdr_0000_gray.pgm\
c0018_r0007-c0018_r0008_hdr_0001_gray.pgm
HDR_LAYERS_WEIGHTS_SHELL="c0018_r0007-c0018_r0008_hdr_0000_gray.pgm"\
"c0018_r0007-c0018_r0008_hdr_0001_gray.pgm"

# stacked hdr images
HDR_STACK_0=c0018_r0007-c0018_r0008_stack_hdr_0000.exr
HDR_STACK_0_SHELL="c0018_r0007-c0018_r0008_stack_hdr_0000.exr"
HDR_STACK_0_INPUT=c0018_r0007-c0018_r0008_hdr_0000.exr
HDR_STACK_0_INPUT_SHELL="c0018_r0007-c0018_r0008_hdr_0000.exr"
HDR_STACK_1=c0018_r0007-c0018_r0008_stack_hdr_0001.exr
HDR_STACK_1_SHELL="c0018_r0007-c0018_r0008_stack_hdr_0001.exr"
HDR_STACK_1_INPUT=c0018_r0007-c0018_r0008_hdr_0001.exr
HDR_STACK_1_INPUT_SHELL="c0018_r0007-c0018_r0008_hdr_0001.exr"
HDR_STACKS_NUMBERS=0 1 
HDR_STACKS=$(HDR_STACK_0) $(HDR_STACK_1) 
HDR_STACKS_SHELL=$(HDR_STACK_0_SHELL) $(HDR_STACK_1_SHELL) 

# number of image sets with similar exposure
LDR_EXPOSURE_LAYER_0=c0018_r0007-c0018_r0008_exposure_0000.tif
LDR_EXPOSURE_LAYER_0_SHELL="c0018_r0007-c0018_r0008_exposure_0000.tif"
LDR_EXPOSURE_LAYER_0_INPUT=c0018_r0007-c0018_r0008_exposure_layers_0000.tif\
c0018_r0007-c0018_r0008_exposure_layers_0001.tif
LDR_EXPOSURE_LAYER_0_INPUT_SHELL="c0018_r0007-c0018_r0008_exposure_layers_0000.tif"\
"c0018_r0007-c0018_r0008_exposure_layers_0001.tif"
LDR_EXPOSURE_LAYER_0_INPUT_PTMENDER=c0018_r0007-c0018_r00080000.tif\
c0018_r0007-c0018_r00080001.tif
LDR_EXPOSURE_LAYER_0_INPUT_PTMENDER_SHELL="c0018_r0007-c0018_r00080000.tif"\
"c0018_r0007-c0018_r00080001.tif"
LDR_EXPOSURE_LAYER_0_EXPOSURE=0.421541
LDR_EXPOSURE_LAYERS_NUMBERS=0 
LDR_EXPOSURE_LAYERS=$(LDR_EXPOSURE_LAYER_0) 
LDR_EXPOSURE_LAYERS_SHELL=$(LDR_EXPOSURE_LAYER_0_SHELL) 
LDR_EXPOSURE_LAYERS_REMAPPED=c0018_r0007-c0018_r0008_exposure_layers_0000.tif\
c0018_r0007-c0018_r0008_exposure_layers_0001.tif
LDR_EXPOSURE_LAYERS_REMAPPED_SHELL="c0018_r0007-c0018_r0008_exposure_layers_0000.tif"\
"c0018_r0007-c0018_r0008_exposure_layers_0001.tif"

# stacked ldr images
LDR_STACK_0=c0018_r0007-c0018_r0008_stack_ldr_0000.tif
LDR_STACK_0_SHELL="c0018_r0007-c0018_r0008_stack_ldr_0000.tif"
LDR_STACK_0_INPUT=c0018_r0007-c0018_r0008_exposure_layers_0000.tif
LDR_STACK_0_INPUT_SHELL="c0018_r0007-c0018_r0008_exposure_layers_0000.tif"
LDR_STACK_1=c0018_r0007-c0018_r0008_stack_ldr_0001.tif
LDR_STACK_1_SHELL="c0018_r0007-c0018_r0008_stack_ldr_0001.tif"
LDR_STACK_1_INPUT=c0018_r0007-c0018_r0008_exposure_layers_0001.tif
LDR_STACK_1_INPUT_SHELL="c0018_r0007-c0018_r0008_exposure_layers_0001.tif"
LDR_STACKS_NUMBERS=0 1 
LDR_STACKS=$(LDR_STACK_0) $(LDR_STACK_1) 
LDR_STACKS_SHELL=$(LDR_STACK_0_SHELL) $(LDR_STACK_1_SHELL) 
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
	@echo Physical memory: 3587384 kiB (52%% occupied)
	@echo Free space on disc: 19169 MiB
	@echo Active codepage: 1251 (Cyrillic Windows)
	@echo ===========================================================================
	@echo Output options
	@echo ===========================================================================
	@echo Hugin Version: 2012.0.0.a6e4184ad538 built by Matthew Petroff
	@echo Project file: C:\Andruxa\psxdev\CIRCUIT_004\m1\18\c0018_r0007-c0018_r0008.pto
	@echo Output prefix: c0018_r0007-c0018_r0008
	@echo Projection: Rectilinear (0)
	@echo Field of view: 12 x 16
	@echo Canvas dimensions: 1449 x 1938
	@echo Crop area: (159,195) - (1346,1793)
	@echo Output exposure value: 0.42
	@echo Selected outputs
	@echo Normal panorama
	@echo * Blended panorama
	@echo ===========================================================================
	@echo Input images
	@echo ===========================================================================
	@echo Number of images in project file: 2
	@echo Number of active images: 2
	@echo Image 0: C:\Andruxa\psxdev\CIRCUIT_004\m1\18\c0018_r0007.jpg
	@echo Image 0: Size 1632x1224, Exposure: 0.42
	@echo Image 1: C:\Andruxa\psxdev\CIRCUIT_004\m1\18\c0018_r0008.jpg
	@echo Image 1: Size 1632x1224, Exposure: 0.42

# Rules for ordinary TIFF_m and hdr output

c0018_r0007-c0018_r00080000.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0007.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 0 $(PROJECT_FILE_SHELL)

c0018_r0007-c0018_r0008_hdr_0000.exr : C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0007.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 0 $(PROJECT_FILE_SHELL)

c0018_r0007-c0018_r00080001.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0008.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -m TIFF_m -o $(LDR_REMAPPED_PREFIX_SHELL) -i 1 $(PROJECT_FILE_SHELL)

c0018_r0007-c0018_r0008_hdr_0001.exr : C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0008.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) -r hdr -m EXR_m -o $(HDR_STACK_REMAPPED_PREFIX_SHELL) -i 1 $(PROJECT_FILE_SHELL)

# Rules for exposure layer output

c0018_r0007-c0018_r0008_exposure_layers_0000.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0007.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.421541 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 0 $(PROJECT_FILE_SHELL)

c0018_r0007-c0018_r0008_exposure_layers_0001.tif : C:/Andruxa/psxdev/CIRCUIT_004/m1/18/c0018_r0008.jpg $(PROJECT_FILE) 
	$(NONA) $(NONA_OPTS) $(NONA_LDR_REMAPPED_COMP) -r ldr -e 0.421541 -m TIFF_m -o $(LDR_EXPOSURE_REMAPPED_PREFIX_SHELL) -i 1 $(PROJECT_FILE_SHELL)

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
