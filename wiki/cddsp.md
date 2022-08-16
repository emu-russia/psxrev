# CD-DSP

CD-DSP занимается декодированием EFM-сигнала в цифровую форму, контролем за ошибками, а также позиционирование лазерной головки и вообще за правильностью работы CD-привода.

Управление CD-DSP производится специальными командами, которые подаются [SUB-CPU](subcpu.md) на последовательный порт. Выходные CD-данные подаются на [CD-контроллер](cdctrl.md) также через последовательный интерфейс.

Изначально CD-DSP был в виде отдельной микросхемы `IC701` (CXD2510/CXD2545), в последствии он был включен в общую [большую микросхему](subic.md).

![CXD2510Q_package](/wiki/imgstore/CXD2510Q_package.jpg)

К счастью, у нас есть мануалы на эту микросхему ([CXD2510Q](/docs/CXD2510Q.pdf), [CXD2545Q](/docs/CXD2545Q.pdf)), поэтому мы имеем возможность полностью изучить подсистему PSX. Если бы не было мануала, то без знания внутренних команд CD-DSP ничего бы не вышло.

Есть 2 картинки CD-DSP, одна похуже (из сервисного мануала), вторая получше (из мануала по CD-DSP):

![CDDSP_pinout1](/wiki/imgstore/CDDSP_pinout1.jpg)

![CDDSP_pinout2](/wiki/imgstore/CDDSP_pinout2.jpg)

Обозначения важных контактов и их назначение:

- Слева внизу находятся контакты управления движками шпинделя, каретки и катушками фокусировки и трекинга. Эти сигналы подаются на [CD-Driver](cddriver.md).
- Справа внизу находится вход с [CD-RF](cdrf.md) - EFM-сигнал (RFAC/RFDC) и сигналы ошибок FE/TE. RFAC используется для чтения данных, а RFDC для управления сервоприводом.
- Слева вверху группа контактов для чтения данных субканала Q. Данные субканала поступают в [SUB-CPU](subcpu.md).
- Контакт XRST заведен на общий сброс RESET3.3
- Справа находится группа контактов для чтения CD-данных (BCLK, LRCK, DATA)
- Слева находятся контакты для отправки управляющих команд в DSP
- Назначение выходных контактов COUT, EMPH пока не понятно
- SENS совместно с SCLK используется для считывания разнообразной информации 
- C2PO представляет собой статус ошибок при передаче данных
- Тактовые сигналы FSOF, XTAI и XTAO (идущий на VCKI).
- Входной сигнал SSTP (обнаружение самого первого внутреннего трека) соединен с LIMIT SWITCH (ограничитель каретки). То есть самым первым треком считается трек, который находится в начальном положении каретки.

Остальные контакты заведены на питание/землю и играют второстепенную роль.

Функционально DSP делится на 2 большие части: схема чтения данных и контроля за ошибками и схема управления CD-приводом.

![CDDSP_block_diagram](/wiki/imgstore/CDDSP_block_diagram.jpg)

## Clock generator и PLL

Большое количество сигналов, связанных с тактовым сигналом и ФАПЧ (PLL), но на самом деле важных не так много (входная частота - XTAI). Аналоговая ФАПЧ похоже вообще не используется, вместо этого используется встроенная цифровая ФАПЧ.

- FSOF: 1/4 от входной тактовой частоты
- C16M: 16.9344 MHz выход (но меняется при изменении частоты variable pitch playback)
- FSTO: 2/3 от входной тактовой частоты
- XTAI: Master CLK input 16.9344 MHz / 33.8688 MHz
- XTAO: Master CLK output
- XTSL: настройка входной тактовой частоты (0: 16.9344 MHz, 1: 33.8688 MHz)
- VCKI: Variable pitch clock input (from external VCO). Fcenter = 16.9344 MHz
- VPCO: Variable pitch PLL charge pump output

- PDO: Analog EFM PLL charge pump output
- VCOI: Analog EFM PLL circuit input (Flock = 8.6436 MHz)
- VCOO: Analog EFM PLL circuit output
- PCO: Master PLL charge pump output
- FILI: Master PLL filter input
- FILO: Master PLL filter output
- CLTV: Master PLL VCO control voltage input

В нашем случае случае выходной master CLK (XTAO) идёт на вход variable pitch clock input, это значит что функция variable pitch playback не используется.

Variable pitch playback можно представить себе когда диджей крутит пластинку в разные стороны, во время воспроизведения. Только с лазерным диском. На некоторых CD-проигрывателях раньше была кнопка fast-forward или если зажать Next, то диск как-будто "проматывался" вперёд. Вот это и есть variable pitch playback.

Выходная FSOF (1/4 от частоты CD-DSP) идёт на вход OSC1 [SUB-CPU](subcpu.md).

## C2PO

Этот контакт (С2 Pointer) используется для обнаружения ошибок C2 во время передачи данных. Он тактируется одновременно с сигналом LRCK (левый/правый канал), либо по сигналу WCLK (счетчик "слов") (зависит от режима выдачи CDROM звук/данные).

Если C2PO = 1 для пачки бит это значит, что данные неверные.

Как известно CD-данные поглощает CD-контроллер, причем хавает их сразу секторами. Очевидно, что SUB-CPU просто следит во время передачи за сигналом C2PO и если во время чтения сектора С2PO = 1, то весь сектор забраковывается.

## Команды DSP

Передача DSP-команд производится через контакты последовательного интерфейса DATA, CLOK и XLAT. Данные DATA тактируются CLOK. XLAT сигнализирует окончание пакета данных. Пакеты имеют переменный размер от 8 до 24 бит.

![CDDSP_cpu_interface](/wiki/imgstore/CDDSP_cpu_interface.jpg)

Такой интерфейс позволяет складывать переменное количество входных битов во временной FIFO-буфер (но не более 24х), а после прихода сигнала XLAT обработать только нужное количество бит (остальные игнорируются).

Вот так выглядит отправка 8-битной команды CD-DSP со стороны SUB-CPU:

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

Результаты выполнения команд идут на регистры DSP. Размер команд определяется размером регистров:

|Регистры|Размер|
|---|---|
|0-2|8 бит|
|3|8-24 бит|
|4-6|16 бит|
|7|20 бит|
|8-0xA|16 бит|
|0xB|20 бит|
|0xC-0xE|16 бит|

Описание команд есть в мануале, но для удобство мы утащим сюда наиболее важные (команды установки DSP-коэффициентов нам не особо нужны).

![CDDSP_regs](/wiki/imgstore/CDDSP_regs.jpg)

## Команды посылаемые SUB CPU

```
0x00        Focus servo off. 0V out.

0x20        Sled and tracking servo off.

0x9B00      Function specification. (Команда различается установкой обычной или двойной скорости проигрывания)
0x9F00      0x0020 - (0) FLFC.
            0x0040 - (0) BiliGL Sub (with 0x0080 bit are setting for stereo).
            0x0080 - (0) BiliGL Main (with 0x0040 bit are setting for stereo).
            0x0100 - (1) RFPLL is digital. PDO (pin 18) is high impedance.
            0x0200 - (1) A SEQ on.
            0x0400 - (0 or 1) Double speed playback. Can be set on (1) and off (0).
            0x0800 - (1) Digital CLV on (FSW and MON not required).

0xC600      Servo coefficient setting. (Команда всегда посылается всегда именно в таком виде)
            0x0040 - (0) Gain DCLV1.
            0x0100 - (0) Gain MDS0.
            0x0200 - (1) Gain MDS1.
            0x0400 - (1) Gain MDP0.
            0x0800 - (0) Gain MDP1.

0xD7        Constant linear velocity сontrol. (Команда всегда посылается всегда именно в таком виде)
            0x01 - (1) Gain CLVS.
            0x02 - (1) Peak hold in CLVS mode at cycle of RFCK/2.
            0x04 - (1) Bottom hold in CLVS and CLVH modes at cycle of RFCK/16.
            0x08 - (0) Ternary MDP values are output.

0xE0        Spindle motor stop mode.
```
