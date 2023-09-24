# Последовательный интерфейс (SIO)

## Интерфейс процессора

![sio](/wiki/imgstore/sio.jpg)

Процессор выступает в роли DCE. Соответственно оконечные устройства на портах SIO 0/1 (контроллеры, карты памяти, модем, Data-link приёмник) выступают в роли DTE.

|Сигнал|Описание|
|---|---|
|Группа сигналов для портов расширения контроллеров/карт памяти||
|/INTIN10|Прерывание SIO; Совмещено с прерыванием PIO (скорее всего через pullup резистор, как это обычно делают)|
|/DSR0|Data Set Ready. DCE is ready to receive and send data.|
|/DTR0B|Data Terminal Ready. DTE is ready to receive, initiate, or continue a call (Port B)|
|/DTR0A|Data Terminal Ready. DTE is ready to receive, initiate, or continue a call (Port A)|
|RXD0|Received Data. Carries data from DCE to DTE (Controllers/Memcards)|
|TXD0|Transmitted Data. Carries data from DTE to DCE (Controllers/Memcards)|
|/SCK0|Контроллеры/карты памяти не используют обычный обмен данными с использованием старт/стоп битов. Вместо это используется Serial CLK.|
|Группа сигналов для порта SIO||
|RXD1|Received Data. Carries data from DCE to DTE (SIO port)|
|TXD1|Transmitted Data. Carries data from DTE to DCE (SIO port)|
|/DSR1|Data Set Ready. DCE is ready to receive and send data (SIO port)|
|/DTR1|Data Terminal Ready. DTE is ready to receive, initiate, or continue a call (SIO port)|
|/CTS1|Clear To Send. DCE is ready to accept data from the DTE (SIO port)|
|/RTS1|Request To Send. DTE requests the DCE prepare to transmit data (SIO port)|

## CN102

![cn102](/wiki/imgstore/cn102.jpg)

## CN104

![cn104](/wiki/imgstore/cn104.jpg)
