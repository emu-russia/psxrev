# CD-DSP

The CD-DSP takes care of decoding the EFM signal into a digital form, error control, as well as laser head positioning and generally proper operation of the CD drive.

The CD-DSP is controlled by special commands that are sent by [SUB-CPU](subcpu.md) via the serial port. The output CD data is fed to [CD-decoder](cddec.md) also via the serial interface.

Originally the CD-DSP was as a separate chip `IC701` (CXD2510/CXD2545), later it was included in the shared [big chip](subic.md).

![CXD2510Q_package](/wiki/imgstore/CXD2510Q_package.jpg)

Fortunately, we have manuals for this chip ([CXD2510Q](/docs/CXD2510Q.pdf), [CXD2545Q](/docs/CXD2545Q.pdf)), so we are able to fully explore the PSX subsystem. If there was no manual, without knowing the internal commands of the CD-DSP, nothing would happen.

There are 2 pictures of the CD-DSP, one worse (from the service manual), the other better (from the CD-DSP manual):

![CDDSP_pinout2](/wiki/imgstore/CDDSP_pinout2.jpg)

![CDDSP_pinout1](/wiki/imgstore/CDDSP_pinout1.jpg)

Important pin designations and their purpose:

- On the bottom left are the control pins for the spindle motors, carriage, and focus and tracking coils. These signals are fed to the [CD-Driver](cddriver.md).
- The bottom right input from [CD-RF](cdrf.md) is the EFM signal (RFAC/RFDC) and the FE/TE error signals. RFAC is used to read data and RFDC to control the servo.
- The upper left is the contact group for reading subchannel Q data. The data of the subchannel goes to [SUB-CPU](subcpu.md).
- The contact XRST is connected to the general reset RESET3.3
- On the right is the group of contacts for reading CD data (BCLK, LRCK, DATA)
- On the left are the pins to send control commands to the DSP
- The functions of the COUT, EMPH output pins are not clear yet
- SENS together with SCLK is used to read various information 
- C2PO represents the status of errors in data transfer
- The FSOF, XTAI and XTAO (going to VCKI) clock signals.
- The input signal SSTP (detecting the very first internal track) is connected to the LIMIT SWITCH (carriage limiter). That is, the very first track is the track that is in the initial position of the carriage.

The other pins are wired to power/ground and play a secondary role.

Functionally, the DSP is divided into 2 large parts: the data reading and error control circuitry and the CD drive control circuitry.

![CDDSP_block_diagram](/wiki/imgstore/CDDSP_block_diagram.jpg)

## Clock Generator and PLL

A large number of signals associated with the clock signal and PLL, but not really so many important ones (input frequency - XTAI). The analog PLL does not seem to be used at all, instead the built-in digital PLL is used.

- FSOF: 1/4 of the input clock frequency
- C16M: 16.9344 MHz output (but changes with frequency variable pitch playback)
- FSTO: 2/3 of input clock frequency
- XTAI: Master CLK input 16.9344 MHz / 33.8688 MHz
- XTAO: Master CLK output
- XTSL: setting the input clock frequency (0: 16.9344 MHz, 1: 33.8688 MHz)
- VCKI: Variable pitch clock input (from external VCO). Fcenter = 16.9344 MHz
- VPCO: Variable pitch PLL charge pump output

- PDO: Analog EFM PLL charge pump output
- VCOI: Analog EFM PLL circuit input (Flock = 8.6436 MHz)
- VCOO: Analog EFM PLL circuit output
- PCO: Master PLL charge pump output
- FILI: Master PLL filter input
- FILO: Master PLL filter output
- CLTV: Master PLL VCO control voltage input

In this case, the output master CLK (XTAO) goes to the variable pitch clock input, which means that the variable pitch playback function is not used.

Variable pitch playback can be imagined when a DJ spins a record in different directions during playback. Only with a laser disc. Some CD-players used to have a fast-forward button or if you pressed Next, it was like the disc would "fast-forward". This is what variable pitch playback is all about.

The FSOF output (1/4 of the CD-DSP frequency) goes to the OSC1 [SUB-CPU](subcpu.md) input .

## C2PO

This pin (C2 Pointer) is used to detect C2 errors during data transfer. It is clocked simultaneously with the LRCK (left/right channel) signal, or by the WCLK (word count) signal (depending on the CDROM audio/data output mode).

If C2PO = 1 for a bunch of bits it means that the data is incorrect.

As you know, CD data is consumed by the CD decoder, and it gets them all at once by sectors. Obviously, the SUB-CPU simply monitors the C2PO signal during the transfer and if during the reading of a sector C2PO = 1, then the entire sector is rejected.

## DSP Commands

DSP commands are transmitted through the DATA, CLOK, and XLAT serial interface pins. DATA data is clocked by CLOK. XLAT signals the end of a data packet. Packets have variable size from 8 to 24 bits.

![CDDSP_cpu_interface](/wiki/imgstore/CDDSP_cpu_interface.jpg)

This interface allows you to stack a variable number of input bits into a temporary FIFO buffer (but no more than 24x), and after the XLAT signal arrives, process only the right number of bits (the rest are ignored).

This is what it looks like to send an 8-bit CD-DSP command from the SUB-CPU side:

```asm
CDDSPCommand8:
		bsr	SendCDDSPData
		nop
		bclr	PD2, PORTD	; XLTO = 0
		nop
		nop
		nop
		bset	PD2, PORTD	; XLTO = 1
		nop
		nop
		nop
		rts

SendCDDSPData:				
		ldx	#7
next_bit:				
		bclr	PD3, PORTD	; CLKO = 0
		lsra
		bcc	send_zero  	
		bset	PD1, PORTD	; DATO = 1
		bra	send_done	

send_zero:				
		bclr	PD1, PORTD	; DATO = 0

send_done:				
		bset	PD3, PORTD	; CLKO = 1
		decx
		bpl	next_bit
		rts
```

The results of the commands go to the DSP registers. The size of the commands is determined by the size of the registers:

|Registers|Size|
|---|---|
|0-2|8 bits|
|3|8-24 bits|
|4-6|16 bits|
|7|20 bits|
|8-0xA|16 bits|
|0xB|20 bits|
|0xC-0xE|16 bits|

The commands are described in the manual, but for convenience we will include the most important ones (we don't really need the DSP-coefficients setting commands).

![CDDSP_regs](/wiki/imgstore/CDDSP_regs.jpg)

## Commands sent by SUB CPU

```
0x00        Focus servo off. 0V out.

0x20        Sled and tracking servo off.

0x9B00      Function specification. (The command is differentiated by the normal or double playback speed setting)
0x9F00      0x0020 - (0) FLFC.
            0x0040 - (0) BiliGL Sub (with 0x0080 bit are setting for stereo).
            0x0080 - (0) BiliGL Main (with 0x0040 bit are setting for stereo).
            0x0100 - (1) RFPLL is digital. PDO (pin 18) is high impedance.
            0x0200 - (1) A SEQ on.
            0x0400 - (0 or 1) Double speed playback. Can be set on (1) and off (0).
            0x0800 - (1) Digital CLV on (FSW and MON not required).

0xC600      Servo coefficient setting. (The command is always sent in this form)
            0x0040 - (0) Gain DCLV1.
            0x0100 - (0) Gain MDS0.
            0x0200 - (1) Gain MDS1.
            0x0400 - (1) Gain MDP0.
            0x0800 - (0) Gain MDP1.

0xD7        Constant linear velocity сontrol. (The command is always sent in this form)
            0x01 - (1) Gain CLVS.
            0x02 - (1) Peak hold in CLVS mode at cycle of RFCK/2.
            0x04 - (1) Bottom hold in CLVS and CLVH modes at cycle of RFCK/16.
            0x08 - (0) Ternary MDP values are output.

0xE0        Spindle motor stop mode.
```
