## CPU

The CPU on all revisions of the motherboard is represented by one large chip with 208 pins, under the designation IC103.

|PCB|PU-7|older PU-8|newer PU-8|PU-18|PU-20|PU-22|PU-23|PM-41|PM-41(2)|
|---|---|---|---|---|---|---|---|---|---|
|IC103|![CXD8530BQ_package](/wiki/imgstore/CXD8530BQ_package.jpg)|![8530BQ_PU8_package](/wiki/imgstore/8530BQ_PU8_package.jpg)|![8530CQ_package](/wiki/imgstore/8530CQ_package.jpg)|![CXD8606AQ_package](/wiki/imgstore/CXD8606AQ_package.jpg)|![CXD8606BQ_package](/wiki/imgstore/CXD8606BQ_package.jpg)|![8606BQ_PU22_package](/wiki/imgstore/8606BQ_PU22_package.jpg)|![8606BQ_PU23_package](/wiki/imgstore/8606BQ_PU23_package.jpg)|![8606BQ_PM41_package](/wiki/imgstore/8606BQ_PM41_package.jpg)|![CXD8606CQ_package](/wiki/imgstore/CXD8606CQ_package.jpg)|
||8530BQ|8530BQ|8530CQ|8606AQ|8606BQ|8606BQ|8606BQ|8606BQ|8606CQ|
||L9A0025|L9A0025|L9A0048|L9A0082|L9B0082|L9B0082|L9B0082|L9B0082|L9A0182|

The CPU consists of the following components:

- Slightly modified LSI LR33300 core
- System coprocessor 2 (GTE)
- MDEC (JPEG-like video decoder)
- DMA controller (DMAC) (7 channels)
- Interrupt controller (INTC)
- DRAM controller (DRAMC)
- Bus controller (interface for ROM BIOS / GPU)
- SIO controller (RS-232-like serial interface), for two ports (SIO0 and SIO1)
- PIO controller (expansion port for additional devices)
- Root counters (3 hardware counters)
- Built-in instruction cache and data cache (data cache with direct access capability)
- Dedicated "mini-caches" (R-buffer and W-buffer)

Microphotograph of a chip:

![Cpu_chip](/wiki/imgstore/Cpu_chip.jpg)

As you can see most of the chip is taken up by the "mess" of synthesized HDL logic, and at the edges there are various memory and registers.

## CPU Revisions

- The very first Japanese consoles (SCPH-1000 / PU-7) and old versions of PU-8 came with revision 90025.
- Then they were quickly replaced by newer consoles (there was some bug in MDEC) which already had the revision 90048 chip.
- In consoles since SCPH-5500 (PU-18) the 90082 revision of the chips were added. These chips were present in all latest PSX models, also in the first version of PSOne motherboards (PM-41).
- The latest versions of PSOne with PM-41(2) motherboards contained 90182 revision of the chip.

You can find out the revision of the chip from the marking on the cover, removing unnecessary letters (e.g. L9A0048 means revision 90048). There must be some sense in the letters, but most likely it is related to the improvement of the technological process.

There is also a tattoo on the lower right corner of the chip. The revision of the chip is indicated in the first line:

![6f18eaaedc260890621c89afba5b0b46](/wiki/imgstore/6f18eaaedc260890621c89afba5b0b46.jpg)

## Related ICs

Related chips include [CPU DRAM](dram.md) and [ROM BIOS](bios.md).

In early versions of the motherboard, the DRAM was represented by four 8-bit memory packages. Later it was replaced by a single 32-bit package.

![Cpu_ram_nec_424805al-a60](/wiki/imgstore/Cpu_ram_nec_424805al-a60.jpg)

## Hardware Interface

We will be guided by the picture from the PU-22 (SCPH-7500) service manual when the CPU was most fully connected to the other parts.

Starting with PU-23 (SCPH-9000) the parallel port (PIO) was taken away from it, and in PM-41 (PSOne) the serial port (SIO) was also taken away.

![CPU_Block](/wiki/imgstore/CPU_Block.jpg)

Прежде чем изучать управляющие сигналы, нужно отметить, что CPU работает с двумя шинами, которыми управляет bus unit (B/U):
- Main bus: проходит внутри процессора, а также соединяется с [DRAM](dram.md) и [GPU](gpu.md)
- Sub bus: на этой шине располагаются все устройства подсистемы (ROM BIOS, [SPU](spu.md), [CD-ROM](cd.md)) и параллельный порт (который на самом деле тоже является своего рода внешним устройством)
- /SWR0, /SWR1: Sub bus write enable. SWR0 предназначен как для внутренних девайсов Sub bus, так и для PIO, а вот SWR1 подается почему-то эксклюзивно только на PIO.
- /SRD: Sub bus read enable.

PIO:
- /CS0: Выбрать PIO на Sub bus. Одновременно на шину может быть подключен только один девайс, во избежание конфликта шин. Этим управляют контакты группы "CS" (chip select).
- DACK5 / DREQ5: PIO DMA
- /INTIN10: контакт совмещен с контроллерами/картами памяти и PIO (общий сигнал прерывания).

SIO:
- RXD1, TXD1, /DSR1, /DTR1, /CTS1, /RTS1: классический последовательный интерфейс

Контроллеры/карты памяти: представляют собой вариацию SIO
- /SCK0: тайминг для контроллеров (строб?)
- RXD0, TXD0, /DSR0, /DTR0A, /DTR0B: последовательный интерфейс. DTR имеет два контакта, поскольку у нас два порта для контроллеров/карт памяти (Port A и Port B)

![Front_jack](/wiki/imgstore/Front_jack.jpg)

DRAM: в более новых материнках оперативная память представляет собой один чип (IC106), но раньше их было больше (4). Мы будем ориентироваться на более новый вариант, потому что это удобно.
- DD : шина данных 32-бит
- DA : адресная шина 10-бит
- /DWE : write enable
- /DRAS0, /DCAS0, /DCAS1, /DCAS2, /DCAS3 : рефреш

ROM BIOS: ROM подсоединен к Sub bus.
- /CS2: Подключить ROM к шине Sub bus. Непосредственно обмен данным (OE) включается, если активен сигнал read enable (/SRD). Ну это понятно, ROM можно только читать :smiley:

GPU:
- VD: шина данных 32-бит, подключена к Main bus
- VA2: адресная шина, 1-бит :smiley: Дело в том, что у GPU всего-лишь два 32-разрядных регистра (GP0/GP1), поэтому для их адресации достаточно одной адресной линии.
- /VRD, /VWR: read/write enable
- /CS7: Выбрать GPU
- DREQ2, DACK2: GPU DMA. Когда для передачи данных используется контроллер прямого доступа к памяти, графический 
процессор посылает сигнал запроса на захват шины, устанавливая низкий логический уровень на контакте GPUDREQ. Получив такой запрос, процессор освобождает шины, извещая об этом установкой низкого логического уровня на выходе GPUDACK.
- /INTIN0: сигнал прерывания GPU VBLANK
- TCLK0: выходит с контакта GPU PCK (pixel clock), может использоваться в качестве Root Counter 0 (счетчик точек)
- TCLK1: выходит с контакта GPU HBLANK, может использоваться в качестве Root Counter 1 (счетчик горизонтальных линий)
- /INTIN1: общее прерывание GPU (может быть вызвано отправкой специальной команды в GPU, но насколько мне известно в играх не используется)

CD-ROM: 
- /CS5: выбрать CD-контроллер
- /INTIN2: прерывание от CD-контроллера

SPU:
- /CS4: выбрать SPU
- /INTIN9: прерывание от SPU
- DACK4, /DREQ4: SPU DMA

Тайминг и сброс CPU:
- CRYSTALP: входной CLK, 67.73 MHz
- SYSCLK0: CLK на вход GPU (33.3 MHz)
- SYSCLK1: CLK на девайсы Sub bus (33.3 MHz)
- DSYSCLK: CLK * 2 на вход GPU (66.67 MHz NTSC, 64.5 MHz PAL)
- /EXT RESET: сброс (приходит с общего сигнала RES3.3 от блока питания)

![CPU_clk](/wiki/imgstore/CPU_clk.jpg)
