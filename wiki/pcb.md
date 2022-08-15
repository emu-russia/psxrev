# PCB Revisions

It seems that IC's 702+704+706 was later replaced by IC722 (DRIVER) and IC's 703+708 was replaced by IC723 (RF), but IC602(RIPPLE) was added on SCPH-7500 series.

IC's 305(CONTROLLER)+308(SPU)+701(CD DSP) was later replaced by IC732 (CDROM CONTROLLER + CD DSP + SPU) on SCPH-7500 series.

## PU-7

[[File:PU7-A.jpg|1000px]]

[[File:PU7 sideB.jpg]]

The chipset is quite different: GPU is split into two chips with 160+64pins (instead one 208pin chip), VRAM also in two chips, SPU has older part number, BIOS has 40pin (not 32pin), cdrom sub cpu has 80pin (not 52pin), and cdrom signal processor is 100pin CXD2516Q (not 80pin CXD2510Q). Oh, and it does actually have a CXD1199BQ cdrom decoder.

(с) Martin Korth.

Эта материнка использовалась в очень редкой самой первой версии PSX SCPH-1000, выпущенной только в Японии.

## PU-8

Motherboard from SCPH-1001 consoles.

PU-8 exist in two revisions :

* older version, slighty modified [[PU-7]].

[[File:PU-8 sideA.jpg|800px]]

[[File:PU-8 sideB.jpg|800px]]

* newer version

[[File:PU-8(2)-front.jpg]]

Difference between older PU-8 and newer revision:

* On older PU-8 GPU is implemented as two IC's (same as on PU-7) :
   * 160pin IC203 CXD1815Q
   * 64pin IC207 CXD2923AR
* On newer version, GPU become single 208-pin package, which is commonly seen on later boards.
* Boot ROM in 40-pin package (same as on PU-7)
* Different SPU package numbers (CXD2922BQ on older same as on PU-7, CXD2925Q on newer)
* VRAM is divided on 2 chips on older version, same as on PU-7
* 80pin SUB-CPU with MC68HC05L16 core (newer version has 52pin package with G6 revision of 68HC05), same as on PU-7
* Older PU-8 use BQ-revision of CPU (same as PU-7), newer PU-8 use CQ-revision.

Difference between PU-7 and early PU-8:

* PU-7 using 100pin package CD DSP (CXD2516Q), PU-8 use 80pin package (CXD2510Q)
* PU-7 using older version of CD-controller (CXD1199BQ), PU-8 use CXD1815Q

## PU-18

<div style="float:right">[[File:{{{motherboard}}}(a).jpg|thumb|left]] [[File:{{{motherboard}}}(b).jpg|thumb|right]]</div>

{{MotherboardData
|motherboard=PU-18
|description=Motherboard that has been seen in SCPH-5000 and SCPH-5500 series consoles.
|console=[[SCPH-5000|SCPH-5000 series]], [[SCPH-5500|SCPH-5500 series]]
|revision=-11/-21, -31/-41, -52/-62, -72/-82
|serial=1-664-537-62
|ports=Parallel I/O, Serial I/O, AV Multiout
|bios=???
}}

### List of parts

[[File:PU-18(a) blocks.jpg]]

[[File:PU-18(b) blocks.jpg]]

{| border="1" cellspacing="1" cellpadding="3" style="border: 1px solid black; border-collapse: collapse;"
|-
! style="background:rgb(204,204,204)" align="center" | IC
! style="background:rgb(204,204,204)" align="center" | Type
! style="background:rgb(204,204,204)" align="center" | Name
|-
| IC 102 || ROM BIOS || (C) SONY COMPUTER ENTERTAINMENT INC. M534031E-04 JAPAN 809238C
|-
| IC 103 || [[CPU]] || SONY (c) Sony Computer Entertainment Inc. CXD8606AQ L9A0082 NNG 9805 △△ WL00174 HONG KONG
|-
| IC 106, IC 107, IC 108, IC 109 || CPU DRAM || SEC PORTUGAL 810 KM48V514DJ-6 SQA016JI ([[media:KM48V514DJ-6.pdf|Datasheet]])
|-
| IC 201 || GPU SG-RAM || NEC JAPAN D481850GF-A12 -T 9811L9010
|-
| IC 202 || Video DAC || MC141685FT JB09812A ([[media:MC141685FT.pdf|Datasheet]])
|-
| IC 203 || GPU || SONY CXD8561BQ 1996 Sony Computer Entertainment Inc. JAPAN 9807ECI D0396ZAC
|-
| IC 303 || CDROM SRAM (256Kb) || 62W256LTM8 9809 000 JAPAN1G33S
|-
| IC 304 || [[SUB-CPU|SUB-CPU]] (M68HC05 core) || C 3030 SC430929PB G63C 185 SSBS9809B (SIDE B)
|-
| IC 305 || [[CD-CONTROLLER|CDROM Controller]] || SONY CXD1815Q 810H25V ([http://psxdev.ru/files/IC305/CXD1812Q.pdf Similar CXD1812Q Datasheet])
|-
| IC 308 || [[SPU]] || SONY CXD2925Q 811R12F
|-
| IC 310 || Sound DRAM || MALASIYA 814260-70 9750 F98 PJ
|-
| IC 402 || Audio DAC || AKM AK4309AVM 9A804N ([http://psxdev.ru/files/IC402/AK4309.pdf Datasheet])
|-
| IC 501 || RGB Encoder || SONY CXA1645M 805A40E (SIDE B)
|-
| IC 601 || 8V=>5V || TA78M05F 8B
|-
| IC 701 || CD DSP || SONY JAPAN CXD2545Q 808B30H ([[media:CXD2545Q.pdf|Datasheet]])
|-
| IC 702 || MDP x SPEED Amplifier || 2904 8099B JRC (SIDE B) ([http://psxdev.ru/files/IC702/NJM2904.pdf Datasheet])
|-
| IC 703 || RF Amplifier || A1791N 809J3 ([http://psxdev.ru/files/IC703/CXA1791N.pdf Datasheet])
|-
| IC 704 || CD Driver || BA6392FP 810 251 (SIDE B) ([http://psxdev.ru/files/IC704/BA6392FP.pdf Datasheet])
|-
| IC 706 || IC with 4 analog circuits || A3821 8109 (SIDE B), uPC5023GR-compatible ([http://psxdev.ru/files/IC706/UPC5023GR.pdf Datasheet])
|-
| IC 708 || Dual Operational Amplifier || 2100 836B JRC  ([http://psxdev.ru/files/IC708/NJM2100.pdf Datasheet])
|-
|}
<br />

[[File:SCPH_5000_block_diagram.jpg|1000px]]

Support ampliefer IC's 702, 706 and 708 are not present in this diagram.

## PU-20

Seen on SCPH-7000 series.

[[File:PU-20 sideA.jpg]]

## PU-22

Motherboards from SCPH-7500 series consoles.

[[File:PU22-A.jpg|1000px]]

[[File:PU22-B.jpg|1000px]]

## PU-23

<div style="float:right">[[File:{{{motherboard}}}(a).jpg|thumb|left]] [[File:{{{motherboard}}}(b).jpg|thumb|right]]</div>

{{MotherboardData
|motherboard=PU-23
|description=Motherboards from SCPH-9000 series consoles.
|console=[[SCPH-9000|SCPH-9000 series]]
|revision=-11/-21/-31/-41/-51
|serial=1-674-987-41
|ports=Serial I/O, AV Multiout
|bios=???
}}

### List of parts

[[File:PU-23(a) blocks.jpg]]

{| border="1" cellspacing="1" cellpadding="3" style="border: 1px solid black; border-collapse: collapse;"
|-
! style="background:rgb(204,204,204)" align="center" | IC
! style="background:rgb(204,204,204)" align="center" | Type
! style="background:rgb(204,204,204)" align="center" | Label
|-
| IC 102 || ROM BIOS || (C) SONY COMPUTER ENTERTAINMENT INC. M534031E-10 JAPAN 930232B
|-
| IC 103 || [[CPU|CPU]] || SONY (c) Sony Computer Entertainment Inc. CXD8606BQ L9B0082 J 9927 △△ WLB26068 HONG KONG
|-
| IC 106 || CPU DRAM (16 Mbit) || TOSHIBA B16517 9929KAD JAPAN T7X16
|-
| IC 201 || GPU SG-RAM || SEC KOREA 931 KM4132G271BQ-10 TAF148HAS
|-
| IC 203 || GPU || SONY CXD8561CQ 1996 Sony Computer Entertainment Inc. KOREA 9932EBI F5541ZA3
|-
| IC 204 || Clock Distribution || 2294A 9882
|-
| IC 304 || [[SUB-CPU|SUB-CPU]] (M68HC05 core) || C 3060 SC430943PB. G63C 185 SSIW9931D
|-
| IC 310 || Sound DRAM (4 Mbit) || NPNX JAPAN NN514265ALTT-50 9850 B37342C
|-
| IC 405 || Audio DAC || 3527 9855
|-
| IC 502 || Video DAC + RGB Encoder || H7240AKV 932 155
|-
| IC 601 || 8V=>5V || 178M05 97 64
|-
| IC 602 || RIPPLE Filter || LAIB DAC
|-
| IC 722 || CD Driver || Panasonic JAPAN AN8732SB 929A138B
|-
| IC 723 || CD RF || A2575N 928C3
|-
| IC 732 || CDROM Controller + CD DSP + [[SPU]] || SONY CXD2938Q 933B32W
|-
|}

## PM-41

[[File:PM-41 front.jpg|1000px]]

## PM-41(2)

[[File:PM-41-2 front.jpg|1000px]]

[[File:PM-41(2) B.jpg|1000px]]
