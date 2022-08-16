# SUB-CPU

The SUB-CPU is used for the software control of [CD RF] (cdrf.md), [CD DRIVER] (cddriver.md) and [CD-DSP] (cddsp.md). SUB-CPU sends commands to the CD-DSP control via a serial interface, and also controls the turn-on laser, detect the opening of the cover, set the speed of the drive 1X/2X, etc. In other words, SUB-CPU prepares the CD system for data reading as well as monitors its correct operation and reacts to external factors. In addition, SUB-CPU parses the commands received by the CD-decoder and manages its internal registers.

CD-data are transferred in serial form directly to the [CD-decoder](cddec.md).

On motherboards, the chip is represented as `IC304`, while it has never been integrated into larger chips and is present on all revisions. 

Known two versions of the chip "L16" and "G6". L16 (80pin) was used on older motherboards (PU-7 and older PU-8), starting with the new PU-8 the G6 (52pin) revision is used. According to unconfirmed reports the MASK ROM of both revisions of the chip hasn't been changed.

Microphotos of the chip (left L16, right G6):

|![IC304_L16_sm](/wiki/imgstore/IC304_L16_sm.jpg)|![SubCpu_chip](/wiki/imgstore/SubCpu_chip.jpg)|
|---|---|

The 6805 processor core used here has a [Datasheet](/docs/MC68HC05L16.pdf) available.

## Hardware interface

The picture on the left is from the PU-18 service manual, and the picture on the right is from the PU-22 service manual:

|![IC304_PU18](/wiki/imgstore/IC304_PU18.jpg)|![IC304_pinout](/wiki/imgstore/IC304_pinout.jpg)|
|---|---|

PORT A and PORT E connect to [CD decoder](cddec.md), via the MCD and MCA buses respectively. At the same host address bus MCA is exactly SUB-CPU, that is, it addresses the CD-decoder, and not vice versa. The direction of the MCD data bus is set by MRD/MWR signals, and the "chip selection" of the CD-decoder is carried out by the MCS signal. Obviously this is necessary to access the internal registers of the CD-decoder. The SUB-CPU input from the decoder side receives only the MINT interrupt signal.

As for the other signals, they are connected to the rest of the CD components:

| Number | Name | Connects to | Description |
|---|---|---|---|
|1, 3-5, 25, 42, 50-52|NC| |Not connected.|
|2, 32|Vdd| |Power|
|6-10|DECA0-4|=> CD-decoder|MCA bus (5 bits) to set the address to the CD decoder to read or write data on the MCD bus. It is controlled by the internal register 0x04 (Port E).<br/>pin6  - DECA4 - 0x10<br/>pin7  - DECA3 - 0x08<br/>pin8  - DECA2 - 0x04<br/>pin9  - DECA1 - 0x02<br/>pin10 - DECA0 - 0x01|
|11, 39|Vss| |Ground|
|12|NDLY| |Connected to ground|
|13|RESET|<= power supply|Reset circuits 3.3V|
|14|OSC1|<= CD DSP|FSOF output line with CD DSP (1/4 of the CD-DSP frequency)|
|15|OSC2||Not connected.|
|16|FOK (focus OK)| |Not connected. Supposed to be used for the corresponding CD-DSP contact.|
|17|CG|=> IC 706|Only present up to and including PU-18, no such signal in later versions. Connected to IC 706 chip (some custom analog placer). Most likely a clock signal for it.|
|18|LMTSW|<= CD-drive|Carriage limit sensor. Available from the code as register 0x01 (Port B), bit 0x04.|
|19|DOOR|<= motherboard|Connects to the drive cover open sensor button. Available from the code as register 0x01 (Port B), bit 0x08.|
|20-21|TEST1-2| |Not connected.|
|22|COUT| |Not connected. Supposed to use CD-DSP (COUT - track counter) for the corresponding contact|
|23|SENSE|<= CD DSP|A pin for reading various information from the DSP (the clock signal for reading is set by SCLK on the DSP and goes to the ROMSEL pin). It is described in the datasheet in paragraph $3-13. It is available from the code as register 0x01 (Port B), bit 0x80.|
|24|SUBQ|<= CD DSP|Data from the SUB Q channel. From the code is available as register 0x02 (Port C), bit 0x04.|
|26|SQCK|=> CD DSP|The clock pulse for the SUB Q transmission. Controlled by internal register 0x02 (Port C), bit 0x08.|
|27|SPEED|=> CD DRIVER|Sets the speed of the 1x/2x drive. Controlled by internal register 0x02 (Port C), bit 0x10.|
|28|AL/TE (MIRROR)|=> CD RF|The assignment is unknown. Controlled by internal register 0x01 (Port B), bit 0x02.|
|29|ROMSEL|<= CD DSP|Pulse for reading SENSE information |
|30|XINT|<= CD-decoder|Interrupt from the CD decoder. Available from the code as register 0x02 (Port C), bit 0x20.|
|31|SCOR|<= CD DSP|Subcode sync output. From the code is available as register 0x02 (Port C), bit 0x02.|
|33-38, 40-41|DECD0-7|<=> CD-decoder|MCD bus (8 bits) to communicate with the CD decoder. It is controlled by the internal register 0x00 (Port A).<br/>pin33 - DECD0 - 0x01<br/>pin34 - DECD1 - 0x02<br/>pin35 - DECD2 - 0x04<br/>pin36 - DECD3 - 0x08<br/>pin37 - DECD4 - 0x10<br/>pin38 - DECD5 - 0x20<br/>pin40 - DECD6 - 0x40<br/>pin41 - DECD7 - 0x80|
|43|DATA|=> CD DSP|Serial data input (DATA, XLT and CLK work together). Controlled by internal register 0x03 (Port D), bit 0x02.|
|44|XLT|=> CD DSP|Latch input (DATA, XLT and CLK work together). Controlled by internal register 0x03 (Port D), bit 0x04.|
|45|CLK|=> CD DSP|Serial data transfer clock input (DATA, XLT and CLK work together). Controlled by internal register 0x03 (Port D), bit 0x08.|
|46|DECCS|=> CD-decoder|Sets the CD decoder to work with MCA and MCD. Controlled by internal register 0x03 (Port D), bit 0x10.|
|47|DECWA|=> CD-decoder|Sets the direction of the MCD bus to write. Controlled by internal register 0x03 (Port D), bit 0x20.|
|48|DECRD|=> CD-decoder|Sets the direction of the MCD bus to read. Controlled by internal register 0x03 (Port D), bit 0x40.|
|49|LDON|=> CD RF|Turns the laser on. Controlled by internal register 0x03 (Port D), bit 0x80.|

## Memory Map

|Address|Description|
|---|---|
|0x00 - 0x0F|Dual mapped I/O registers (16 bytes)|
|0x10 - 0x3F|Other I/O registers (48 bytes)|
|0x40 - 0x23F|Internal RAM (512 bytes)|
|0x1000 - 0x4FFF|MASK ROM (16 KB)|
|0xFE00 - 0xFFFF|SELF TEST ROM and interrupt vectors|

```
/*
// REGISTERS
000h        PORT A (MCD - data)

001h        PORT B
            0x08 (pin 19) - shell open (0-closed, 1-open)

003h        PORT D
            0x02 (pin 43 DATA) - serial data input (DATA, XLT и CLK работают вместе).
            0x04 (pin 44 XLT) - latch input (DATA, XLT и CLK работают вместе).
            0x08 (pin 45 CLK) - serial data transfer clock input (DATA, XLT и CLK работают вместе). 
            0x10 (pin 46 DECCS) - sets the CD decoder to work with MCA and MCD.
            0x20 (pin 47 DECWA) - sets the direction of the MCD bus to write.
            0x40 (pin 48 DECRD) - sets the direction of the MCD bus to read.
            0x80 (pin 49 LDON) - turning on the laser.

004h        PORT E (MCA) CD Controller read and write registers.
            READ
            0x01 - DECSTS (decoder status) register
                0x01 NOSYNC Indicates that the sync mark was inserted because one was not detected at the prescribed position.
                0x02 SHRTSCT (short sector) Indicates that the sync mark interval was less than 2351 bytes. This sector does not remain in the buffer memory.
                0x20 RTADPBSY (real-time ADPCM busy) This is high for real-time ADPCM playback.
            0x07 - INTSTS (interrupt status) register. The value of each bit in this register indicates that of the corresponding interrupt status. These bits are not affected by the values of the INTMSK register bits.
                0x01 HCRISD (host chip reset issued).
                0x02 HSTCMND (host command). The HSTCMND status is established when the host writes a command in the command register.
                0x04 DECINT (decoder interrupt).
                0x08 HDMACMP (host DMA complete).
                0x10 RTADPEND (real-time ADPCM end).
                0x20 RSLTEMPT (RESULT empty).
                0x40 DECTOUT (decoder time out). The DECTOUT status is established when the sync mark is not detected even after 3 sectors (40.6 ms at normal speed playback) have elapsed after the decoder has been set to the monitor-only, write-only or real-time correction mode.
                0x80 DRVOVRN (drive overrun). The DRVOVRN status is established when the ENDLADR bit (bit 7) of the DECCTL register is set high and DADRC and DLADR become equal while the decoder is in the write-only or real-time correction mode. It is also established when they become equal while the decoder is in the CD-DA mode regardless of the ENDLADR bit value.
            0x11 - HIFSTS (host interface status) register.
                0x80 BUSYSTS (busy status) This has the same value as BUSYSTS (bit 7) of the host HSTS register. It is set high when the host writes a command into the command register and low when the sub CPU sets CLRBUSY of the CLRCTL register.
                0x40 RSLWRDY (result write ready) The result register is not full when this bit is high. The sub CPU can write the result of the command execution into this register.
                0x20 RSLEMPT (result empty) The result register is empty when this bit is high. It indicates that all the status sent from the sub CPU to the host (result register) have been read out by the host.
                0x10 PRMRRDY (parameter read ready) The HSTPRM register is not empty when this bit is high. The sub CPU can read out the command parameters from the HSTPRM register.
                0x08 DMABUSY (DMA busy) This is high when data is being transferred between the buffer memory and the host. It is high when the host sets BFRD (bit 7) or BFWR (bit 6) of the HCHPCTL register high. It is low in the case below: When the data transfer FIFO (WRDATA, RDDATA registers) is empty after the level of HXFRC has dropped to 00HEX.
                0x04 HINTSTS#2 (host interrupt status #2) This is high when the sub CPU writes data into HINT#2 (HIFCTL register bit 2) and low when the host writes “high” into CLRINT#2 (HCLRCTL register bit 2). It is used to monitor interrupts for the host.
                0x02 HINTSTS#1 (host interrupt status #1) This is high when the sub CPU writes data into HINT#1 (HIFCTL register bit 1) and low when the host writes “high” into CLRINT#1 (HCLRCTL register bit 1). It is used to monitor interrupts for the host.
                0x01 HINTSTS#0 (host interrupt status #0) This is high when the sub CPU writes data into HINT#0 (HIFCTL register bit 0) and low when the host writes “high” into CLRINT#0 (HCLRCTL register bit 0). It is used to monitor interrupts for the host.
            WRITE
            0x03 - DECCTL (decoder control) register.
                0x07 DECMD2 to 0 (decoder mode 2 to 0).
                    00X - Decoder disable
                    01X - Monitor-only mode
                    100 - Write-only mode
                    101 - Real-time correction mode
                    110 - Repeat correction mode
                    111 - CD-DA mode
                0x08 AUTODIST (auto distinction)
                    1 : Errors are corrected according to the MODE byte and FORM bit read from the drive.
                    0 : Errors are corrected according to the MODESEL and FORMSEL bits (bits 5 and 4).
            0x04 - DLADR-L While the decoder is in the write-only, real-time correction or CD-DA mode, the last address is set for the buffer write data from the drive. When the ENDLADR bit (bit 7) of the DECCTL register is high and the data from the drive is written into the address assigned by DLADR while the decoder is in any of the above modes, all subsequent writing in the buffer is prohibited.
            0x05 - DLADR-M same as above.
            0x06 - DLADR-H same as above.
            0x07 - CHPCTL (chip control) register
                0x10 CD-DA
                    1 : Set high for playing back the audio signals of a CD-DA (digital audio) disc. Setting this bit high is prohibited for ADPCM decoding playback.
                    0 : Set low for not playing back the audio signals of a CD-DA (digital audio) disc.
                0x20 CD-DA MUTE (When bit 4 is high and this bit is also set high for CD-DA (digital audio) disc playback, the audio output is muted.
                When bit 4 is low, this bit has no effect on the audio output.)
                add 0E0h if we mute.
                remove 01Fh if we unmute.
            0x0A - CLRCTL (clear control) register. When each bit of the register is set high, the corresponding chip, status, register, interrupt status and ADPCM playback are cleared. After clearing, the bit concerned is automatically set low. There is therefore no need for the sub CPU to reset low.
                0x20 CLRRSLT (clear result) The RESULT register is cleared when this bit is set high.
                0x40 CLRBUSY (clear busy) The BUSYSTS bit of the HIFSTS register is cleared when this bit is set high.
            0x0B - CLRINT (clear interrupt status) register. When each bit of this register is set high, the corresponding interrupt status is cleared. The bit concerned is automatically set low after its interrupt status has been cleared. There is therefore no need for the sub CPU to reset low.
                0x01 HCRISD (host chip reset issued).
                0x02 HSTCMND (host command).
                0x04 DECINT (decoder interrupt).
                0x08 HDMACMP (host DMA complete).
                0x10 RTADPEND (real-time ADPCM end).
                0x20 RSLTEMP (reset empty).
                0x40 DECTOUT (decoder time out).
                0x80 DRVOVRN (drive overrun).
            0x16 - HIFCTL (host interface control) register.
                0x01 : HINT#0 The value of this bit becomes that of HINTSTS#0 in the HINTSTS register on the host side.
                0x02 : HINT#1 The value of this bit becomes that of HINTSTS#1 in the HINTSTS register on the host side.
                0x04 : HINT#2 The value of this bit becomes that of HINTSTS#2 in the HINTSTS register on the host side.
            0x17 - RESULT register The host reads the results of the command execution through this register. The register has ah 8-byte FIFO configuration.

03Eh        some specific register. According to Christopher Tarnovsky it is clock divider control.
            0x01 - add flag before writing to 000h and remove right after that.



// MEMORY
040h        looks like inner cd subsystem state.
            0x02 - add flag if we mute CDDA streaming to SPU. Remove when we demute.
            0x08 - add this flag when we set CD-DA MUTE to CHPCTL CD Controller register.
            0x80 - set to 1 when TOC loaded.

042h        add flag 0x04 during 0x15 SeekL.
043h        remove flag 0x02 and 0x04 during 0x15 SeekL.

046h        some flags.
            0x40 - add flag when we set minutes seconds and sector with 0x02 Setloc.

049h        remove flag 0x01 during 0x15 SeekL.

04Dh        store here minutes set in 0x02 Setloc during 0x15 SeekL.
04Eh        store here seconds set in 0x02 Setloc during 0x15 SeekL.
04Fh        store here sector set in 0x02 Setloc during 0x15 SeekL.
050h        if bit 0x40 not set during DECTOUT interrupt then we enable CD-DA mode.

05Dh        store here minutes set in 0x02 Setloc during 0x15 SeekL.
05Eh        store here seconds set in 0x02 Setloc during 0x15 SeekL.
05Fh        store here sector set in 0x02 Setloc during 0x15 SeekL.

06Ah        add flag 0x01 during 0x15 SeekL. Set this to 0 when we clear TOC.
06Bh        store here 001h during 0x15 SeekL.

06Dh        state of cdrom??
            0x01 - set during initialization.
            0x20 - set during 0x15 SeekL.

070h        store here 07Dh during 0x15 SeekL.

088h        temporary storage for many things.
08Bh        counter for command params length.

093h        error check flags.
            0x80 - if this flag is set we not add new error on following addreses.
094h        store error code here.

0A0h        store value set to DLADR-L of CD Controller register here.
0A1h        store value set to DLADR-M of CD Controller register here.
0A2h        store value set to DLADR-H of CD Controller register here.

0B1h        data from 0x01 DECSTS (decoder status) CD Controller register.
0B3h        data from 0x07 INTSTS (interrupt status) CD Controller register.
0B5h        data from 0x11 HIFSTS (host interface status) CD Controller register.

0B7h        data for 0x07 CHPCTL (chip control) CD Controller register.
0B9h        data for 0x16 HIFCTL (host interface control) CD Controller register.
0BAh        data for 0x0A CLRCTL (clear control) CD Controller register.

0BCh        cdrom status
            0x01 - invalid command/parameters/state of cdrom
            0x02 - spindle motor (0=motor off, or in spin-up phase, 1=motor on)
            0x04 - seek error (0=okay, 1=seek error)
            0x08 - id error (0=okay, 1=GetID denied)
            0x10 - shell open (0-closed, 1-is/was open).
            0x20 - reading data sectors.
            0x40 - seeking.
            0x80 - playing CD-DA.

0BFh        store error code here.

100h        TOC entries for Start of Tracks 1-99 (size 0x02 [MM][SS]).
1C6h        TOC first track number (usually 01h)
1C7h        TOC last track number (usually 01h or higher)
1C8h        TOC entry for start of Lead-Out minutes.
1C9h        TOC entry for start of Lead-Out seconds.
1CAh        TOC entry for start of Lead-Out sector.

1E0h        store command here.
1E1h        store params for command from here til end flag.

1EDh        store here minutes set in 0x02 Setloc.
1EEh        store here seconds set in 0x02 Setloc.
1EFh        store here sector set in 0x02 Setloc.

1F0h        store first return value here.
1F1h        store second return value here.
1F2h        store third return value here.

20Ch        store here strange byte in command execution.
```
