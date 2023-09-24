# Serial Interface (SIO)

## CPU interface

![sio](/wiki/imgstore/sio.jpg)

The processor acts as a DCE. Accordingly, the terminal devices on the SIO 0/1 ports (controllers, memory cards, modem, data-link receiver) act as DTEs.

|Signal|Description|
|---|---|
|Signal group for controller/memory card ports||
|/INTIN10|SIO interrupt; Combined with PIO interrupt (most likely via pullup resistor, as is ordinary)|
|/DSR0|Data Set Ready. DCE is ready to receive and send data.|
|/DTR0B|Data Terminal Ready. DTE is ready to receive, initiate, or continue a call (Port B)|
|/DTR0A|Data Terminal Ready. DTE is ready to receive, initiate, or continue a call (Port A)|
|RXD0|Received Data. Carries data from DCE to DTE (Controllers/Memcards)|
|TXD0|Transmitted Data. Carries data from DTE to DCE (Controllers/Memcards)|
|/SCK0|Controllers/memory cards do not use ordinary start/stop bit communication. Instead, Serial CLK is used.|
|Signal group for SIO port||
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
