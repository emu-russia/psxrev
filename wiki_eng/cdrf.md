# CD-RF

The CD RF is the chip responsible for controlling the laser and getting the EFM signal as well as the laser focus error signal, tracking, and tracking out of the track to an empty space.

A detailed description of the laser device and motor is available here: http://www.radiofan.ru/faq/cd/part1.htm

I will include a couple of pictures here.

|![Detectors](/wiki/imgstore/Detectors.gif)|![Laser_beam_Eng](/wiki/imgstore/Laser_beam_Eng.gif)|
|---|---|

On older motherboards CD-RF chip was represented by IC703 chip, but we'd rather rely on a newer implementation, based on `IC723 `.

## IC703

![Cxa1791n_sm](/wiki/imgstore/Cxa1791n_sm.jpg)

Full resolution: https://drive.google.com/open?id=1UqA_r6dA5rLRS88AjS1HgKMCfHwhpUQD

## Principle of Operation

![IC723_overview](/wiki/imgstore/IC723_overview.jpg)

On the left in the picture there is a carriage with a matrix of photodetectors A-D (EFM), detectors E and F (side beams), as well as an output contact to the laser diode LD and an input signal from the photodiode PD for automatic correction of the laser radiation power.

At that, A+C are combined into PD1 and B+D are combined into PD2.

Output and control signals:

- LDON: laser on
- AL/TE: it is not yet clear what it does
- RFO: EFM output signal
- FE: focus error
- TE: tracking error
- TEO: ?
- MIRR DET: determines when the beam is not on the track at all (shines on the reflective surface of the disk)
