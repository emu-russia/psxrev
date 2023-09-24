# DRAM

В отношении памяти CPU наблюдается некоторый бардачок.. Раньше чипов было 4 (NEC 424805AL-A60), потом 1 (Samsung K4Q153212M-JC60, Toshiba T7X16), потом память стала на кристалле.
Короче надо разобраться.

## Особенности

Рассмотрим только вариант с 1 чипом, на примере Samsung K4Q153212M-JC60, т.к. на неё имеется даташит.

|Сигнал|Описание|
|---|---|
|A0-9|Используется для задания ряда (A0-9, Row) и столбца (A0-8, Col); то есть двойного назначения. Последовательно задавая вначале Row, потом Column формируется полный 19-разрядный адрес слова (512K * 4 bytes)|
|DQ0-31|Подключается к шине данных CPU|
|/RAS|Row Address Strobe|
|/CAS0, /CAS1, /CAS2, /CAS3|Column Address Strobe. 4 сигнала используются для каждого байта 32-разрядных данных DQ0-31 (/CAS0 = lsb)|
|/OE|Data Output Enable. :warning: /OE не используется, так как процессор не умеет. Сигнал /OE DRAM соединен с GND.|
|/WE|Read/Write Input|

Ну а дальше лучше посмотреть даташит, т.к. комбинация RAS/CAS/OE/WE сигналов определяет множество разнообразных режимов работы.

Список операций (вначале выбирается Row, потом Column), `WORD` в названии операции означает 16-битную часть:
- 2 WORDS READ CYCLE: чтение DQ0-31
- BYTE WIDE READ CYCLE: чтение DQ0-7 + DQ16-23
- 2 WORDS WRITE CYCLE (EARLY WRITE): запись DQ0-31
- BYTE WIDE WRITE CYCLE (EARLY WRITE): запись DQ0-7 + DQ16-23
- 2 WORDS WRITE CYCLE (/OE CONTROLLED WRITE): запись DQ0-31, управляется /OE
- 2 WORDS READ-MODIFY-WRITE CYCLE: Вначале делается чтение DQ0-31 (управляемое /OE), потом запись DQ0-31 (управляемое /WE)
- HYPER PAGE MODE READ CYCLE: Операции Hyper Page позволяют проводить потоковую передачу данных, если они находятся внутри одного ряда. При этом Column можно задавать произвольно.
- HYPER PAGE MODE WRITE CYCLE (EARLY WRITE)
- 2 WORDS HYPER PAGE READ-MODIFY-WRITE CYCLE
- /RAS-ONLY REFRESH CYCLE
- /CAS-BEFORE-/RAS REFRESH CYCLE
- HIDDEN REFRESH CYCLE (READ)
- HIDDEN REFRESH CYCLE (WRITE)
- /CAS-BEFORE-/RAS SELF REFRESH CYCLE
