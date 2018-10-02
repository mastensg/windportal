# Windmachine
An interactive installation demonstrating the power of the wind.

## TODO

Tuesday

* Estimate part costs
* Estimate hours / cost for project

* Measure/test torque/speed needed for valve control
* 

## Costs estimate

Brainz. RPi,Arduino etc. 1500 NOK
Connectivity. 3G modem, 1000 NOK
Display. 10" LCD sunlight. 3000 NOK
Fan control. 500 NOK
Valve control. 2000-3500 NOK

Est 9500 NOK

Server, SIMkort: 1500/aar

# Software

# Components

## Brainz
Control:
Fan(s) 1-2

+ Firmata on Arduino for GPIO extender

Raspberry PI
5V power supply
SD card
Enclosure
Arduino Leonardo: IO extender

1500 NOK

## Connectivity
USB modem 3g

modem. 500 NOK
SIM for test. 200 

1000 NOK

## Control panel
3x buttons
3x LEDs

## Fan control
Analog 0-10V or

Modbus?
https://github.com/riptideio/pymodbus
https://pypi.org/project/MinimalModbus/
RS-485 or RS-232
USB adapter?

500 NOK

## Valve control
Servo or stepper(+encoder/endstop)

Speed needed: 0.5 sec/90 grad

Torque needed?
GUESS: 10kg/cm, 1Nm ++

Servo motors AC
https://www.digikey.com/products/en/motors-solenoids-driver-boards-modules/motors-ac-dc/178?FV=1b804d8%2C1b805db%2Cffe000b2%2C380522%2C380732%2C380733%2C3800bc&quantity=0&ColumnSort=1000011&page=1&stock=1&nstock=1&pageSize=25

DC servo motor
Up to 6Nm. HobbyKing
1Nm https://www.digikey.com/product-detail/en/adafruit-industries-llc/1142/1528-1083-ND/5154658

Nema23, 10-20 kg/cm


Absolute encoder, SPI
https://www.digikey.com/product-detail/en/cui-inc/AMT203-V/102-2050-ND/2278846

Budsjett:
Motors (incl drivers) 2x 1500kr
500 NOK

3500 NOK

## Display

### LCD high brightness
https://blog.peerless-av.com/digital-signage-basics-nits-brightness-led-vs-lcd-displays/
Common brightness requirements are 800 to 1,500 nits for indoor displays,
and 1,500 to 2,500 nits for indoor displays in direct sunlight.

For outdoors, on the other hand, up to 5,000 nits is generally recommended
and for those in direct sunlight, 5,000+ nits is recommended.

LCD panel, 10", 1000 nits
TOOD: link
3k NOK 

10k NOK
https://prisguiden.no/produkt/philips-signage-solutions-p-line-42bdl5057p-340915#detaljer
https://prisguiden.no/kategorier/pc-skjerm?f[90][]=Minst%20600%20cd%2Fm2&f[90][]=Minst%201000%20cd%2Fm2&s=price%20asc

### Analog gauge.
Servo + vinyl
1000k

7-segment
150mm, red, needs custom driver
https://www.sparkfun.com/products/8530
https://www.digikey.com/products/en/optoelectronics/display-modules-led-character-and-numeric/92?k=&pkeyword=&sf=0&FV=4dc0065%2C4dc006f%2C4dc0075%2C4dc0077%2C4dc007f%2C4dc0081%2C4dc0082%2C4dc0091%2C4dc0048%2C4dc0049%2C4dc0008%2C4dc0055%2C4dc0056%2C4dc005e%2C4dc0060%2Cffe0005c&quantity=&ColumnSort=1000011&page=1&stock=1&pageSize=25

LED dot matrix (flippers)
5k

