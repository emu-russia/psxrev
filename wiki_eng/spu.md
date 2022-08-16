# SPU

The SPU (Sound Processing Unit) plays back audio. The SPU uses a special ADPCM compression format to store sound data (similar to the XA-ADPCM format).

The PSX sound subsystem is a bundle of chips for CD-ROM operation (which includes [CD decoder](cddec.md), [CD DSP](cddsp.md) and [SUB-CPU](subcpu.md)), the SPU itself and the output DAC.

The hardware implementation of all parts of the sound subsystem changed in different versions of the motherboard.

Here we will look at the SPU itself, when it was implemented as a separate chip `IC308` (CXD2922Q/CXD2925Q).

|![CXD2922BQ_package](/wiki/imgstore/CXD2922BQ_package.jpg)|![CXD2925Q](/wiki/imgstore/CXD2925Q.jpg)|
|---|---|

Microphotograph of the CXD2925Q chip (4X magnification), with labeled cell domains and units:

![Spu_annotated_regions](/wiki/imgstore/Spu_annotated_regions.jpg)

Technology: 3 layers of metal, standard cells and custom blocks (internal memory, registers).

The top 2 layers of metal are used for wiring (Channel router). M1 is used for the circuitry of standard cells and internal units.

![Spu_metal_layers](/wiki/imgstore/Spu_metal_layers.jpg)

## SPU Features

The SPU contains 24 audio channels (voice). The sound source is data from the SPU memory (512 KB), in ADPCM format.
It is possible to input noise with adjustable frequency instead of ADPCM.

You can do pitch modulation over ADPCM, but not over noise. An example of the pitch effect is the change of tone of the voice after inhaling helium :smiley:

The input is then fed to the ADSR envelope generator and then to the general mixer.

![SPU_overview](/wiki/imgstore/SPU_overview.jpg)

As you can see, the sound from the CD-ROM is additionally mixed to the sound of the 24 channels.

It is possible to add reverb effects (echo, etc.).

The volume is adjustable for: sound channels (separately for L/R), CD-ROM inputs, reverb, and there is a general volume control.

Data exchange between the SPU and CPU RAM takes place via DMA, and the SPU operation (sound) is not interrupted.

The output digital sound is fed to the DAC.

## Hardware Implementation

Below is a picture from the service manual with the SPU pins and its connection to the SPU memory and other devices:

![SPU_hardware](/wiki/imgstore/SPU_hardware.jpg)

The picture is only relevant for PU-18 and below motherboards, as the newer ones have changed the layout of the chips.

Terminal description:
- On the left are the pins for streaming audio data from the CD-ROM decoder: DTIA, LRIA, BCIA.
- The digital audio output (in serial form) through the DATO, LRCO, BCKO pins is fed to the DAC. The XCK clock frequency is also fed to the DAC.
- Additionally, streaming audio from the parallel port (PIO) can be fed to the SPU input, via the DTIB pin. In this case the sampling frequency is the same as the output audio. The BCIB is connected to ground for some reason, while LRIB goes to the [CD-DSP](cddsp.md) LRCK pin.
- Below are the pins for SPU memory: XOE0 (enable exchange), XWE0 (write enable), MA (address bus, only 9 bits are used), MD (data bus, 16 bits), XCAS/XRAS for DRAM refresh. Note that there is also XWE1 and XOE1, possibly for additional SPU memory. A special feature of the SPU memory is that the exchange takes place in 16-bit words (16-bit memory). Therefore, the output address from the SPU pins is multiplied by 2 (output pin D0 is wired to memory pin D1 and so on) to get the "index" of the memory cell.
- Main clock signal for the PSX subsystem: SYSCK
- TEST and TES2 test signals
- The XRST reset signal is wired via capacitors to VSS to allow for reset time after power up
- Interface to the CPU: HD (host data, 16 bit), HA (host address, 9 bit), XCS (chip select), XRD/XWR (read/write), DACK/DREQ (DMA acknowledge/request), XIRQ (SPU interrupt). The external bus is also exchanged in 16-bit words, with the address multiplied by 2.
- The MUTE signal (?)

Abbreviations: BC,BCK - bit clock, LR,LRC - left/right clock, DAT,DT - data. What does it all mean? These three signals are a serial interface for stereo sound: bit clock is the clock frequency at which the data is transmitted. LRC is the frequency that separates the packet of bits into left and right channels. When LRC=1, bits for the left channel are input, when LRC=0 for the right channel respectively.

Signals whose names begin with "X" have inverse logic (active low).

## SPU Memory

As already mentioned, a feature of SPU memory is its 16-bit "slotting". Therefore, instead of the term "address", the term "index" is better used here.

The memory capacity is 512 KB.

The SPU memory is used to store ADPCM samples for playback and tables for reverb effects.

The SPU also stores in special "circular" buffers some of the data it plays back so that the CPU can use it for its own purposes.

SPU memory map:
```
 00000h-003FFh  CD Audio left  (1Kbyte) ;\CD Audio after Volume processing
 00400h-007FFh  CD Audio right (1Kbyte) ;/signed 16bit samples at 44.1kHz
 00800h-00BFFh  Voice 1 mono   (1Kbyte) ;\Voice 1 and 3 after ADSR processing
 00C00h-00FFFh  Voice 3 mono   (1Kbyte) ;/signed 16bit samples at 44.1kHz
 01000h-xxxxxh  ADPCM Samples  (first 16bytes usually contain a Sine wave)
 xxxxxh-7FFFFh  Reverb work area
```

Only the input data from the CD and channels 1 and 3 are stored (why exactly 1 and 3 - Sony engineers decided so :smiley:)

The data exchange between SPU memory and CPU memory is done via DMA, parallel to audio playback.

## What's different about the new versions of the motherboards

The CD/Audio system has changed in the SCPH-7500 (PU-22) series. The schematic is from the service manual:

![SPU_new](/wiki/imgstore/SPU_new.jpg)

The SPU has now become part of [big chip](subic.md) with the index IC732, as Audio DSP. The most noticeable change:
- They added a SUB-CPU interface to the picture for some reason, not sure why yet. Perhaps newer versions of SUB-CPU contain a new version of ROM, which controls the work of Audio DSP. Or maybe to show that the CD decoder is controlled by the CPU through the Host interface, which passes control further down the chain, to the SUB-CPU interface.
- The DAC has now become part of the chip. For this reason, the older versions of the PSX are more appreciated by audiophiles, because of the greater modding possibilities. In the days when the SPU was a separate chip, the output (as serial data) was fed to a DAC made by Asahi Kasei.

## DAC

The DAC looks like this:

![AudioDAC](/wiki/imgstore/AudioDAC.jpg)

Datasheet available: [Datasheet](/docs/AK4309.pdf)

The input is 1-bit serial data from SPU (LRCK, BICK, SDATA), inside DAC contains a lot of interpolators and other incomprehensible devices, as a result on the output we get perfect analog stereo signal (AOUTL, AOUTR).

As with any DAC, two power circuits are used here - digital (DVDD,DVSS) and analog (AVDD,AVSS). The voltages VREFL and VREFH set the lower and upper limit of the analog signal voltage, respectively (usually VREFH=AVDD, VREFL=AVSS).

The DZF output pin is set if there has been no sound on the input for too long (SDATA=0), reset automatically.

The sample format is 16-bit with a sign.
