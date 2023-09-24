# PlayStation Development

This is where you can find information about the different hardware and software that was used for official PlayStation development.

There's a lot of information here: http://www.lingjr.com/collection_sony_ps.htm

## General overview

### PsyQ

The developer kits are quite a mess :smiley:

The main development tool was PsyQ from SN SYSTEMS. PsyQ contained a C compiler (CCPSX), an assembler (ASPSX), a linker (PSYLINK), and a library manager (PSYLIBD).

In addition to the utilities for building programs it also included libraries written by SONY to interact with the PlayStation hardware, as well as header files (INCLUDE) for these libraries. There were several versions of PsyQ and they differed with minor changes (bug fixes in the libraries).

PsyQ also included the basic documentation: Library Overview (LIBOVR) and Library Reference (LIBREF).

Known versions of PsyQ (the list will be extended with information about the release date and, if possible, a list of important fixes):
- 2.6
- 3.0
- 3.3
- 3.4
- 3.5
- 3.6.0
- 3.6.1
- 3.7
- 4.0
- 4.1
- 4.2
- 4.3
- 4.3.1
- 4.4
- 4.6: 27/JUL/1999
- 4.7: 02/FEB/2000

There is a great repository for reverse-engineering programs written with PsyQ SDK: https://github.com/lab313ru/ghidra_psx_ldr

### Documentation

The documentation was delivered on special CDs, or via email. Also, there were conferences where developers were taught the tricky details of working with hardware (GTE, MDEC), because for example the GTE programming language is very bluntly written. Ambiguous function names and a very "low-level" environment. In fact, the developers were offered to control GTE registers :smiley:

Several versions of the documentation disks are known, codenamed `DTL-S2003`.

### Tools

Aside from the documentation, there were also separate disks with a set of programs (for graphics, sound, etc.).

Several versions of disks with programs are known under the codename `DTL-S2002`.

CodeWarrior was offered as an IDE, but I doubt it was widely used. The easiest thing to do was to use MAKE files.

### Hardware

Developers were offered both special devkits (development kit), which were modified PlayStation, and "spares", for programming individual PSX parts.

What were the devkits and the hardware:

Devkits in the form of computer boards, the distinguishing feature being increased RAM size to 8MB:
- DTL-H500, DTL-H505: sort of the very first devkit
- DTL-H2000: newer model, 2 ISA boards
- DTL-H2500: already on PCI
- DTL-H2700: also PCI

There were also special "test" consoles which were painted blue and green. They had the same RAM size as the Retail version (2MB):
- DTL-H1000: blue, "B" revision chips
- DTL-H1100: blue, "B" revision chips
- DTL-H1200: Green, "C" revision chips

The difference from normal consoles was the lack of CD copy protection.

![DTL-H1202](/wiki/imgstore/DTL-H1202.jpg)

And finally there was NetYaroze: a devkit for non-professionals, which came with the NetYaroze SDK. We know quite a lot about this devkit.

All debug versions of PSX had a special BIOS, often significantly different from the "game" version. One significant difference is the absence of Kanji ROM fonts, which were flashed in the Retail-version BIOS, a special patch is used for this in the devkits.

A distinctive feature is also a huge number of patches in general. Patches apparently distributed via email, because the Internet as such did not exist at that time. Apparently it was very difficult for the developers to be sure that they use all actual patches for Sony libraries :-)

## DTL-H700

DTL-H700 - Sound artist board. The kit includes a special Mac optical output board with the original SPU chip on board, as well as the DTL-S710 (Sound artist tool for Macintosh) software.

http://www.lingjr.com/images/sony_ps/ps_dtlh700_boxed.JPG

## DTL-H800

The IBM PC version of the Sound artist board.

http://www.lingjr.com/images/sony_ps/ps_dtlh800_boxed_1.JPG

## DTL-H500 / DTL-H505

The very first Devkit:

![DTL-H500](/wiki/imgstore/DTL-H500.jpg)

A quote from http://playstationmuseum.com/

The DTL-H505, codename MW.3, is the earliest development hardware with final PlayStation silicon and appeared in late Spring 1994. Most of these target boxes lacked sound hardware (or libraries to make use of it) and there was no CD drive. The museum is under the assumption that it utilized the CD-emulator board. Sony's next move was to consolidate the target box to a standard PC platform by way of two ISA cards (DTL-H2000) and develop an external CD-ROM drive (DTL-H2010).

Note: As you can imagine, there does exist an MW.2 which is believed to be similar to MW.3 but with beta silicon (meaning the PlayStation architecture was not finalized).

## DTL-H2000

http://www.lingjr.com/images/sony_ps/ps_dtlh2000_boxed_1.JPG

This model has documentation from which you can understand what this kit is:
- Hardware-wise, this engineering marvel is two full-size ISA boards (lol), for an IBM PC compatible computer
- The hardware on the boards is identical to the Retail console: CPU, GPU, SPU, and CD-ROM
- RAM is increased to 8MB, but you can also boot the system with a 2MB limit, like on a real console
- Controllers and SIO/PIO were modified ports

Also included in the devkit was the DTL-H2010 external CD drive.

DTL-H2080 (Controller & Memory Card Adaptor)

|![DTL-H2000-CPU1_01](/wiki/imgstore/DTL-H2000-CPU1_01.jpg)|![DTL-H2000-CPU2_01](/wiki/imgstore/DTL-H2000-CPU2_01.jpg)|
|---|---|

## DTL-H2500

Instead of two obsolete ISA cards, one PCI card is used.

http://www.lingjr.com/images/sony_ps/ps_dtlh2500_boxed_1.JPG

## DTL-H2700

is a three layer development tool, it's combined with DTL-H2700 ISA I/F Board , DTL-H2700 CPU Board & DTL-H2700 ANALYZER Board.
It seems the last version of PlayStation development board for IBM/ATs, because the DTL-T10000 (PlayStation 2 Development System) also runs PlayStation development programmes.

http://www.lingjr.com/images/sony_ps/ps_dtlh2700b.JPG

The references in the manuals also confirm that this was the last version of the "hardware" devkit for the PlayStation.

## Controller for DTL-H2000, DTL-H2500, DTL-H2700

It is equipped with a conventional COM port.

![DTLH2000_controller](/wiki/imgstore/DTLH2000_controller.jpg)

There is also a special DTL-H2080 adapter, which allows you to connect conventional controllers and memory cards to the devkit.

## CD Emulator

http://www.lingjr.com/images/sony_ps/ps_dtls2020_boxed_1.JPG

The DTL-S2020 is a special board and a set of utilities for simulating a real drive. The data is stored on a SCSI HDD.

## NetYaroze (DTL-H3000)

![Net-Yaroze-Full-Sdk](/wiki/imgstore/Net-Yaroze-Full-Sdk.jpg)

Included:
- Console (2MB RAM)
- 2 regular controllers
- A special dongle in the form of a memory card
- SIO cable
- printed manuals (DTL-S3000)
- Boot disk (DTL-S3030 for JAP, DTL-S3035 for EU)
- NetYaroze SDK disk (DTL-S3040 for JAP). GNU was used instead of proprietary compilers, and all libraries were repacked and merged into a single library, LIBPS.

There was also no regional protection.

Apparently, NetYaroze was just a repainted "test" version of Devkit (well, with accessories added).

## Developer software kit

In addition to the PsyQ toolchain, Sony offered developers many programs for editing graphics and sound data, CD emulators, and more. Let's take a look at all known programs.

According to the interface, some of them have been developed in Windows 3.11. It is amazing that they still run in Windows 7 :smiley:

### SN Debugger

SN SYSTEMS offered a debugger that included a STUB to interact with the debugger, a runtime (LIBSN) to integrate the debugger into the application, and a DLL to control the DTL-H2500.

Integrated Debugger Environment:

![SN_Debugger](/wiki/imgstore/SN_Debugger.jpg)

### Performance Analyzer

Quite a cool program, which was based on the possibility of sampling bus signals in the Devkit boards. Provided the developer with a variety of statistics. Screenshots:

|![PA_000](/wiki/imgstore/PA_000.jpg)|![PA_001](/wiki/imgstore/PA_001.jpg)|
|---|---|

### LightWave 3D

The LightWave 3D software, version 4.0, was offered for modeling 3D graphics

### Plugin for 3DS

A plugin for 3DS MAX that could save models in RSD format.

### MOVLIB 

Contains a library and a header for MDEC bitstream compression (MDEC Encoder), which can then be decompressed using the libpress library.

That is, the developer was invited to compress the original graphical data himself and store it the way they want.

### RSDTOOL

Texturing RSD models.

RSD is the "source" textual format for 3D models, which are then compiled into TMD binaries by the RSDLINK utility.

### TIMTOOL

TIM are graphics files adapted for fast loading into GPU memory. The peculiarity of TIM is a special "palette" format, which is hardware used by the GPU (CLUT). In this case, the palette indexes and the palette itself (CLUT) are stored in the video memory separately.

That is why there are many utilities to convert BMP to TIM, TIM-editors, etc.

### Tools for working with sound

Since the SPU uses a special storage format (ADPCM), it was necessary to convert the WAV to this format before using it.

The format of the PSX sound files is: `VAG`. Utilities WAV2VAG and AIFF2VAG were used to do the conversion.

There were also "bank" files - `VAB`, they were used to create MIDI-like tracks, in a special tracker program VABTOOL:

![VABTOOL](/wiki/imgstore/VABTOOL.jpg)

All this stuff was tested on special hardware DTL-H700 (for Mac) and DTL-H800 (for PC), respectively there were drivers for these boards in VABTOOL.

There was also an ENCVAG library to convert PCM to VAG format.

### CDEMU

Instead of real device SN SYSTEMS developed the emulator in the form of a special board. The data itself was stored on a SCSI hard disk.

TBD.