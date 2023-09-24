# DRAM

In terms of CPU memory, there is a bit of a mess. There used to be 4 chips (NEC 424805AL-A60), then 1 (Samsung K4Q153212M-JC60, Toshiba T7X16), then the memory became on-chip.
In short, we need to figure it out.

## Features

Let's consider only the scenario with 1 chip, on the example of Samsung K4Q153212M-JC60, as there is a datasheet for it.

|Signal|Description|
|---|---|
|A0-9|Used to specify a row (A0-9, Row) and a column (A0-8, Col); that is, dual assignment. By sequentially specifying Row first, then Column, a full 19-bit word address (512K * 4 bytes) is formed|
|DQ0-31|Connects to the CPU data bus|
|/RAS|Row Address Strobe|
|/CAS0, /CAS1, /CAS2, /CAS3|Column Address Strobe. 4 signals are used for each byte of 32-bit data DQ0-31 (/CAS0 = lsb)|
|/OE|Data Output Enable. :warning: /OE is not used because the processor does not know how. The /OE DRAM signal is connected to GND.|
|/WE|Read/Write Input|

And further it is better to look at the datasheet, because the combination of RAS/CAS/OE/WE signals defines many different modes of operation.

List of operations (Row is selected first, then Column), `WORD` in the operation name means 16-bit part:
- 2 WORDS READ CYCLE: read DQ0-31
- BYTE WIDE READ CYCLE: read DQ0-7 + DQ16-23
- 2 WORDS WRITE CYCLE (EARLY WRITE): write DQ0-31
- BYTE WIDE WRITE CYCLE (EARLY WRITE): write DQ0-7 + DQ16-23
- 2 WORDS WRITE CYCLE (/OE CONTROLLED WRITE): read DQ0-31, controlled by /OE
- 2 WORDS READ-MODIFY-WRITE CYCLE: DQ0-31 reads are done first (driven by /OE), then DQ0-31 writes are done (driven by /WE)
- HYPER PAGE MODE READ CYCLE: Hyper Page operations allow data to be streamed if they are within the same row. Column can be set arbitrarily.
- HYPER PAGE MODE WRITE CYCLE (EARLY WRITE)
- 2 WORDS HYPER PAGE READ-MODIFY-WRITE CYCLE
- /RAS-ONLY REFRESH CYCLE
- /CAS-BEFORE-/RAS REFRESH CYCLE
- HIDDEN REFRESH CYCLE (READ)
- HIDDEN REFRESH CYCLE (WRITE)
- /CAS-BEFORE-/RAS SELF REFRESH CYCLE
