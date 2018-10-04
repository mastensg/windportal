# Windmachine
An interactive installation demonstrating the power of the wind.

## Status

## TODO

Tuesday

- [x] Estimate hours / cost for project

Key questions

- [x] OK with FOSS (Sjur)
- [ ] Measure/test valve control. torque/speed. (dep: Peter)
- [ ] Fan control interface. Analog or modbus (dep: Peter)
- [ ] OK with LCD display, or other (dep: Sjur)
- [ ] Decide ownership domain,server,SIM (dep: Sjur)

Risk moments

- [ ] Interfacing fan,valve control
- [ ] Data communication

Milestone 0: Ready to work

- [ ] Copy Ansible skeleton (J) 5
- [ ] Define MQTT interface (J)
- [ ] Setup RPi3b+ base (J)
- [ ] Setup firmware base (J)
- [ ] Fetch data using YR (J) 3
- [ ] Setup server. Linode,DNS,email,Mosquitto in Ansible (M) 5
    - [ ] Linode
    - [x] DNS
    - [ ] Sendgrid
    - [ ] Mosquitto
    - [ ] Ansible configuration

Wed 3 - Fri 5
Milestone A: Device-Server datapaths ready

- [ ] Server is deployed
- [ ] Server fetches current wind data from API
- [ ] Device can fetch current wind data from server
- [ ] RPI automatically sets up reverse SSH
- [ ] Server reports last seen time basic. 2
- [ ] Fetch data using HTML page (J) 3

Milestone E: All electronics/hardware ordered.
Fri 5 - Mon 8.

- [ ] Resolve questions

Mon 8
Milestone B: All electronics assembled

- [ ] RPi powered and running
- [ ] Arduino with daughterboard
- [ ] LEDs in control panel
- [ ] Potmeter in control panel
- [ ] Test button in control panel
- [ ] User button plugged in
- [ ] Display connected over HDMI
- [ ] Fan power module connected

Fri 12 - Mon 15
Milestone C: I/O complete, from RPi

- [ ] Turn status LEDs on/off
- [ ] Get user button status (turn on)
- [ ] Get adjust potiometer (power level)
- [ ] Get test button stats (full power)
- [ ] Set valve position
- [ ] Set fan power
- [ ] Show text on display

Thurs 25.
Milestone D: System complete

- [ ] Server monitoring/reporting (M) 5
- [ ] Windgust animation works
- [ ] Tested device poweroffs
- [ ] Tested server downtime
- [ ] Tested full gust button
- [ ] Tested connectivity loss


Failure modes

- I/O failure. Software does right thing, but right action does not happen
- RPi fails to fetch server. 3G connection, HTTP error, server down
- [...]

## Worklog

Date,person,hours,area

    02.10.2018,jon,8,planning
    02.10.2018,martin,8,planning

Jon.
project management 10-15
server software 5-10
device firmware base. 5-10

20-35
25

Martin
Project 15
Electronics. 10-15
Server. 10
Device. 15

50-55t

70-90t

## Status
Power,booted,modem,SSH-call-home,servercomm,graphicservice,controlservice
8 LEDS

## System

* Electronics (M)
Fancontrol. 10 (modbus), 5 (analog)
Valvecontrol.
Statuspanel. 5


* Device firmware
Base setup. (Jon)
Ansible. (Jon)
Load data (Jon)
Report FBP (Jon) 10
USB modem (Jon) 5
Windgust animation. Py1. (Martin) 10
Interface electronics. Py1. (Martin)
User logic. Py1. (Martin) 5
Display graphics. C++ (Martin) 5
Monitoring service. (Martin) 3

* Server software.
Ansible (Martin)
SSH server for reverse
Weatherdata scraper. (Jon) 10
Monitoring/reporting (Martin) 

## Costs estimate

Brainz. RPi,Arduino etc. 1500 NOK
Connectivity. 3G modem, 1000 NOK
Display. 10" LCD sunlight. 3000 NOK
Fan control. 500 NOK
Valve control. 2000-3500 NOK

Est 9500 NOK

Server,backup,domain,SIMkort: 2000/aar

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

