# SUB-CPU

В некоторых сервисных мануалах данная микросхема называется `Mechanism Controller` (MechaCon), а в некоторых `SUB CPU`. Будем использовать название SUB-CPU.

SUB-CPU используется для программного управления [CD RF](cdrf.md), [CD DRIVER](cddriver.md) и [CD-DSP](cddsp.md). SUB-CPU посылает в CD-DSP команды управления через последовательный интерфейс, а также управляет включением лазера, детектированием открытия крышки, устанавливает скорость привода 1X/2X и пр. То есть можно сказать SUB-CPU подготавливает систему CD для чтения данных, а также следит за правильностью её работы и реагирует на внешние факторы. Дополнительно SUB-CPU разбирает команды, поступаемые на CD-декодер и управляет его внутренними регистрами.

Непосредственно CD-данные передаются в последовательной форме сразу на [CD-декодер](cddec.md).

На материнских платах микросхема представлена как `IC304`, при этом она никогда не интегрировалась в состав более крупных микросхем и присутствует на всех ревизиях. 

Известно множество версий чипа, рассмотрим "L16" и "G6". L16 (80pin) использовалась на более старых материнках (PU-7 и более старые PU-8), начиная с новых PU-8 используется ревизия G6 (52pin).

Микрофотографии чипа:

|L16|G6|
|---|---|
|![IC304_L16_sm](/wiki/imgstore/IC304_L16_sm.jpg)|![SubCpu_chip](/wiki/imgstore/SubCpu_chip.jpg)|

На ядро процессора 6805, которое тут используется, доступен [Datasheet](/docs/MC68HC05L16.pdf).

## Аппаратный интерфейс

|Сервисный мануал для PU-18|Сервисный мануал для PU-22|
|---|---|
|![IC304_PU18](/wiki/imgstore/IC304_PU18.jpg)|![IC304_pinout](/wiki/imgstore/IC304_pinout.jpg)|

PORT A и PORT E соединяются с [CD-декодером](cddec.md), через шины MCD и MCA соответственно. При этом хозяином адресной шины MCA выступает именно SUB-CPU, то есть он обращается к CD-декодеру, а не наоборот. Направление шины данных MCD задается сигналами MRD/MWR, а "выбор чипа" CD-декодера осуществляется сигналом MCS. Очевидно это необходимо для доступа ко внутренним регистрам CD-декодера. На вход SUB-CPU со стороны декодера поступает только сигнал прерывания MINT.

Что касается других сигналов, то они соединяются с остальными компонентами CD:

|Номер контакта |Название контакта | Соединяется с |Описание|
|---|---|---|---|
|1, 3-5, 25, 42, 50-52|NC| |Не подсоединен.|
|2, 32|Vdd| |Линия питания.|
|6-10|DECA0-4|=> CD-декодер|Шина MCA (5 бит) для установки адреса на CD-декодер для чтения или записи данных по шине MCD. Управляется внутренним регистром 0х04 (Port E).<br/>pin6  - DECA4 - 0x10<br/>pin7  - DECA3 - 0x08<br/>pin8  - DECA2 - 0x04<br/>pin9  - DECA1 - 0x02<br/>pin10 - DECA0 - 0x01|
|11, 39|Vss| |Линия земли.|
|12|NDLY| |Подключен к земле|
|13|RESET|<= блок питания|Сброс схем 3.3В|
|14|OSC1|<= CD DSP|Выходная линия FSOF с CD DSP (1/4 от частоты CD-DSP)|
|15|OSC2||Не подсоединен.|
|16|FOK (focus OK)| |Не подсоединен. Предполагалось использовать для соотв. контакта CD-DSP.|
|17|CG|=> IC 706|Присутствует только до PU-18 включительно, в более поздних версиях такого сигнала нет. Подсоединен к микросхеме IC 706 (какая-то заказная аналоговая россыпуха). Скорее всего тактовый сигнал для нее.|
|18|LMTSW|<= CD-привод|Датчик ограничения хода каретки. Из кода доступен как регистр 0х01 (Port B), бит 0x04.|
|19|DOOR|<= материнка|Соединяется с кнопкой датчика открытия крышки диска. Из кода доступен как регистр 0х01 (Port B), бит 0x08.|
|20-21|TEST1-2| |Не подсоединен.|
|22|COUT| |Не подсоединен. Предполагалось использовать для соотв. контакта CD-DSP (COUT - счетчик треков)|
|23|SENSE|<= CD DSP|Пин для считывания разнообразной информации с DSP (тактовый сигнал для считывания задается SСLK на DSP и приходит на контакт ROMSEL). В даташите описан в параграфе $3-13. Из кода доступен как регистр 0х01 (Port B), бит 0x80.|
|24|SUBQ|<= CD DSP|Данные от SUB Q канала. Из кода доступен как регистр 0х02 (Port C), бит 0x04.|
|26|SQCK|=> CD DSP|Тактовый импульс для передачи SUB Q. Управляется внутренним регистром 0х02 (Port C), бит 0х08.|
|27|SPEED|=> CD DRIVER|Задает скорость привода 1x/2x. Управляется внутренним регистром 0х02 (Port C), бит 0х10.|
|28|AL/TE (MIRROR)|=> CD RF|Назначение неизвестно. Управляется внутренним регистром 0х01 (Port B), бит 0х02.|
|29|ROMSEL|<= CD DSP|Тактовый импульс для считывания SENSE-информации |
|30|XINT|<= CD-декодер|Прерывание от CD-декодера. Из кода доступен как регистр 0х02 (Port C), бит 0x20.|
|31|SCOR|<= CD DSP|Subcode sync output. Из кода доступен как регистр 0х02 (Port C), бит 0x02.|
|33-38, 40-41|DECD0-7|<=> CD-декодер|Шина MCD (8 бит) для обмена данными с CD-декодером. Управляется внутренним регистром 0х00 (Port A).<br/>pin33 - DECD0 - 0x01<br/>pin34 - DECD1 - 0x02<br/>pin35 - DECD2 - 0x04<br/>pin36 - DECD3 - 0x08<br/>pin37 - DECD4 - 0x10<br/>pin38 - DECD5 - 0x20<br/>pin40 - DECD6 - 0x40<br/>pin41 - DECD7 - 0x80|
|43|DATA|=> CD DSP|Serial data input (DATA, XLT и CLK работают вместе). Управляется внутренним регистром 0х03 (Port D), бит 0х02.|
|44|XLT|=> CD DSP|Latch input (DATA, XLT и CLK работают вместе). Управляется внутренним регистром 0х03 (Port D), бит 0х04.|
|45|CLK|=> CD DSP|Serial data transfer clock input (DATA, XLT и CLK работают вместе). Управляется внутренним регистром 0х03 (Port D), бит 0х08.|
|46|DECCS|=> CD-декодер|Устанавливает CD-декодер для работы с MCA и MCD. Управляется внутренним регистром 0х03 (Port D), бит 0х10.|
|47|DECWA|=> CD-декодер|Устанавливает направление шины MCD на запись. Управляется внутренним регистром 0х03 (Port D), бит 0х20.|
|48|DECRD|=> CD-декодер|Устанавливает направление шины MCD на чтение. Управляется внутренним регистром 0х03 (Port D), бит 0х40.|
|49|LDON|=> CD RF|Включение лазера. Управляется внутренним регистром 0х03 (Port D), бит 0x80.|

## Карта памяти

|Адрес|Описание|
|---|---|
|0x00 - 0x0F|Dual mapped I/O registers (16 bytes)|
|0x10 - 0x3F|Other I/O registers (48 bytes)|
|0x40 - 0x23F|Internal RAM (512 bytes)|
|0x1000 - 0x4FFF|MASK ROM (16 KB)|
|0xFE00 - 0xFFFF|SELF TEST ROM и векторы прерываний|

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
            0x10 (pin 46 DECCS) - устанавливает CD-декодер для работы с MCA и MCD.
            0x20 (pin 47 DECWA) - устанавливает направление шины MCD на запись.
            0x40 (pin 48 DECRD) - устанавливает направление шины MCD на чтение.
            0x80 (pin 49 LDON) - включение лазера.

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
