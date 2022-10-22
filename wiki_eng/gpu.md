# GPU

GPU (Graphics Processor Unit) draws two-dimensional primitives in the video memory and outputs the picture to the TV.

Additionally, the GPU is engaged in video signal mastering (for example, manages Color Burst), although a separate chip - DAC, based on digital RGB output signals coming from the GPU, performs direct video signal generation.

GPU control from the CPU side is carried out only two IO-ports `GP0` and `GP1`, as well as DMA, for the transfer of display lists (DL).

The GPU is built using the same technology as the CPU (standard cells + custom blocks):

|![GPU_chip_lowres](/wiki/imgstore/GPU_chip_lowres.jpg)|![GPU_standard_cells](/wiki/imgstore/GPU_standard_cells.jpg)|
|---|---|

- The old versions of motherboards (PU-7 and old PU-8) used 2 GPU chips: 160pin CXD8514Q (IC203) and 64pin CXD2923AR (IC207). One of them was busy drawing primitives (the bigger 160pin one) and the other one was obviously outputting the picture from the video memory to the TV (the 64pin one). We have no manuals, so you can only find out more accurately from the motherboard pictures (trace the wires).
- All other versions used 208pin versions of the GPU, the index remained the same (IC203).

|PCB|PU-7 / older PU-8|newer PU-8|PU-18|PU-20|PU-22|PU-23|PM-41|PM-41(2)|
|---|---|---|---|---|---|---|---|---|
|IC203|![PU7_gpu_package](/wiki/imgstore/PU7_gpu_package.jpg)|![NewPU8_gpu_package](/wiki/imgstore/NewPU8_gpu_package.jpg)|![PU18_gpu_package](/wiki/imgstore/PU18_gpu_package.jpg)|???|![PU22_gpu_package](/wiki/imgstore/PU22_gpu_package.jpg)|![PU23_gpu_package](/wiki/imgstore/PU23_gpu_package.jpg)|![PM41_gpu_package](/wiki/imgstore/PM41_gpu_package.jpg)|![PM412_gpu_package](/wiki/imgstore/PM412_gpu_package.jpg)|
|CXD|8514Q/2923AR|8561Q|8561BQ|???|8561CQ|8561CQ|8561CQ|???|

## Hardware interface (old GPU 160-pin)

The old GPU consisting of two chips (CXD 8514Q/2923AR) works as follows:

- The interface between the GPU and the CPU has not changed in subsequent versions.
- The larger CXD 8514Q chip (160 pins) draws primitives into dedicated dual-port DRAM. Accordingly it also contains DRAM Refresh logic. The situation is a bit more complicated by the fact that the VRAM is divided into two banks (2 chips).
- The small CXD 2923AR chip (64 pins) is sampling pixels from the VRAM and is an RGB DAC

![old_gpu1](/wiki/imgstore/old_gpu1.png)
