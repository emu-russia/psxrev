# PCB Revisions

![fixme](/wiki/imgstore/fixme.gif) Раздел требует заполнения.

It seems that IC's 702+704+706 was later replaced by IC722 (DRIVER) and IC's 703+708 was replaced by IC723 (RF), but IC602(RIPPLE) was added on SCPH-7500 series.

IC's 305(CONTROLLER)+308(SPU)+701(CD DSP) was later replaced by IC732 (CDROM CONTROLLER + CD DSP + SPU) on SCPH-7500 series.

## PU-7

![PU7-A](/wiki/imgstore/PU7-A.jpg)
![PU7_sideB](/wiki/imgstore/PU7_sideB.jpg)

The chipset is quite different: GPU is split into two chips with 160+64pins (instead one 208pin chip), VRAM also in two chips, SPU has older part number, BIOS has 40pin (not 32pin), cdrom sub cpu has 80pin (not 52pin), and cdrom signal processor is 100pin CXD2516Q (not 80pin CXD2510Q). Oh, and it does actually have a CXD1199BQ cdrom decoder.

(с) Martin Korth.

Эта материнка использовалась в очень редкой самой первой версии PSX SCPH-1000, выпущенной только в Японии.

## PU-8

Motherboard from SCPH-1001 consoles.

PU-8 exist in two revisions:

older version, slighty modified PU-7:

![PU-8_sideA](/wiki/imgstore/PU-8_sideA.jpg)
![PU-8_sideB](/wiki/imgstore/PU-8_sideB.jpg)

newer version:

![PU-8(2)-front](/wiki/imgstore/PU-8(2)-front.jpg)

Difference between older PU-8 and newer revision:

- On older PU-8 GPU is implemented as two IC's (same as on PU-7):
   - 160pin IC203 CXD1815Q
   - 64pin IC207 CXD2923AR
- On newer version, GPU become single 208-pin package, which is commonly seen on later boards.
- Boot ROM in 40-pin package (same as on PU-7)
- Different SPU package numbers (CXD2922BQ on older same as on PU-7, CXD2925Q on newer)
- VRAM is divided on 2 chips on older version, same as on PU-7
- 80pin SUB-CPU with MC68HC05L16 core (newer version has 52pin package with G6 revision of 68HC05), same as on PU-7
- Older PU-8 use BQ-revision of CPU (same as PU-7), newer PU-8 use CQ-revision.

Difference between PU-7 and early PU-8:

- PU-7 using 100pin package CD DSP (CXD2516Q), PU-8 use 80pin package (CXD2510Q)
- PU-7 using older version of CD-controller (CXD1199BQ), PU-8 use CXD1815Q

## PU-18

![PU-18a](/wiki/imgstore/PU-18a.jpg)
![PU-18b](/wiki/imgstore/PU-18b.jpg)

Motherboard that has been seen in SCPH-550X, SCPH-5552, early SCPH-700X, early SCPH-750X series consoles (Source: https://www.psdevwiki.com/ps1/Motherboards)

- revision: -11/-21, -31/-41, -52/-62, -72/-82
- serial: 1-664-537-62
- ports: Parallel I/O, Serial I/O, AV Multiout
- bios: ???

List of parts:

![PU-18a_blocks](/wiki/imgstore/PU-18a_blocks.jpg)
![PU-18b_blocks](/wiki/imgstore/PU-18b_blocks.jpg)

| IC  | Type | Name |
|---|---|---|
| IC 102 | ROM BIOS | (C) SONY COMPUTER ENTERTAINMENT INC. M534031E-04 JAPAN 809238C |
| IC 103 | CPU | SONY (c) Sony Computer Entertainment Inc. CXD8606AQ L9A0082 NNG 9805 △△ WL00174 HONG KONG |
| IC 106, IC 107, IC 108, IC 109 | CPU DRAM || SEC PORTUGAL 810 KM48V514DJ-6 SQA016JI ([Datasheet](/docs/KM48V514DJ-6.pdf)) |
| IC 201 | GPU SG-RAM | NEC JAPAN D481850GF-A12 -T 9811L9010 |
| IC 202 | Video DAC | MC141685FT JB09812A ([Datasheet](/docs/MC141685FT.pdf)) |
| IC 203 | GPU | SONY CXD8561BQ 1996 Sony Computer Entertainment Inc. JAPAN 9807ECI D0396ZAC |
| IC 303 | CDROM SRAM (256Kb) | 62W256LTM8 9809 000 JAPAN1G33S |
| IC 304 | SUB-CPU (M68HC05 core) | C 3030 SC430929PB G63C 185 SSBS9809B (SIDE B) |
| IC 305 | CDROM Controller | SONY CXD1815Q 810H25V ([Similar CXD1812Q Datasheet](/docs/CXD1812Q.pdf)) |
| IC 308 | SPU | SONY CXD2925Q 811R12F |
| IC 310 | Sound DRAM | MALASIYA 814260-70 9750 F98 PJ |
| IC 402 | Audio DAC | AKM AK4309AVM 9A804N ([Datasheet](/docs/AK4309.pdf)) |
| IC 501 | RGB Encoder | SONY CXA1645M 805A40E (SIDE B) |
| IC 601 | 8V=>5V | TA78M05F 8B |
| IC 701 | CD DSP | SONY JAPAN CXD2545Q 808B30H ([Datasheet](/docs/CXD2545Q.pdf)) |
| IC 702 | MDP x SPEED Amplifier | 2904 8099B JRC (SIDE B) ([Datasheet](NJM2904.pdf)) |
| IC 703 | RF Amplifier | A1791N 809J3 ([Datasheet](/docs/CXA1791N.pdf)) |
| IC 704 | CD Driver | BA6392FP 810 251 (SIDE B) ([Datasheet](/docs/BA6392FP.pdf)) |
| IC 706 | IC with 4 analog circuits | A3821 8109 (SIDE B), uPC5023GR-compatible ([Datasheet](/docs/UPC5023GR.pdf)) |
| IC 708 | Dual Operational Amplifier | 2100 836B JRC  ([Datasheet](/docs/NJM2100.pdf)) |

![SCPH_5000_block_diagram](/wiki/imgstore/SCPH_5000_block_diagram.jpg)

Support ampliefer IC's 702, 706 and 708 are not present in this diagram.

## PU-20

Seen on SCPH-7000 series.

![PU-20_sideA](/wiki/imgstore/PU-20_sideA.jpg)

## PU-22

Motherboards from SCPH-7500 series consoles.

![PU22-A](/wiki/imgstore/PU22-A.jpg)
![PU22-B](/wiki/imgstore/PU22-B.jpg)

## PU-23

![PU-23a](/wiki/imgstore/PU-23a.jpg)
![PU-23b](/wiki/imgstore/PU-23b.jpg)

Motherboards from SCPH-9000 series consoles.

- revision: -11/-21/-31/-41/-51
- serial: 1-674-987-41
- ports: Serial I/O, AV Multiout
- bios: ???

List of parts:

![PU-23a_blocks](/wiki/imgstore/PU-23a_blocks.jpg)

|IC| Type | Label |
|---|---|---|
|IC 102 | ROM BIOS | (C) SONY COMPUTER ENTERTAINMENT INC. M534031E-10 JAPAN 930232B|
| IC 103 | CPU | SONY (c) Sony Computer Entertainment Inc. CXD8606BQ L9B0082 J 9927 △△ WLB26068 HONG KONG|
| IC 106 | CPU DRAM (16 Mbit) | TOSHIBA B16517 9929KAD JAPAN T7X16|
| IC 201 | GPU SG-RAM | SEC KOREA 931 KM4132G271BQ-10 TAF148HAS|
| IC 203 | GPU | SONY CXD8561CQ 1996 Sony Computer Entertainment Inc. KOREA 9932EBI F5541ZA3|
| IC 204 | Clock Distribution | 2294A 9882 |
| IC 304 | SUB-CPU (M68HC05 core) | C 3060 SC430943PB. G63C 185 SSIW9931D|
| IC 310 | Sound DRAM (4 Mbit) | NPNX JAPAN NN514265ALTT-50 9850 B37342C|
| IC 405 | Audio DAC | 3527 9855 |
| IC 502 | Video DAC + RGB Encoder | H7240AKV 932 155|
| IC 601 | 8V=>5V | 178M05 97 64 |
| IC 602 | RIPPLE Filter | LAIB DAC |
| IC 722 | CD Driver | Panasonic JAPAN AN8732SB 929A138B |
| IC 723 | CD RF | A2575N 928C3 |
| IC 732 | CDROM Controller + CD DSP + SPU | SONY CXD2938Q 933B32W |

## PM-41

![PM-41_front](/wiki/imgstore/PM-41_front.jpg)

## PM-41(2)

![PM-41-2_front](/wiki/imgstore/PM-41-2_front.jpg)
![PM-41-2_B](/wiki/imgstore/PM-41-2_B.jpg)
