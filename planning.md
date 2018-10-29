
Key questions

- [x] OK with FOSS (Sjur)
- [x] Fan control interface. Analog or modbus
- [x] Decide display. LCD, dial indicator or 7-segment.
- [x] Get a custom domain (dep: Sjur) `windportal.dept.no`
- [ ] Ownership server,SIM (dep: Sjur)

Risk moments

- Interfacing fan,display
- Data communication device,server,datasource
- In-field failures

Bugs

- [ ] If Arduino is unplugged, wp_firmata keeps running using 100% CPU.
- [ ] ssh-call-home sometimes hangs, does not reconnect after network or server disconnect

Tools

- [ ] CLI for sending IPC message `wp_ipc KEY VALUE`

## Deadlines


### Tuesday 16 October
- Specification frozen.
- Decided display type
- All electronics/hardware ordered.

## Done

Kickoff

- [x] Estimate hours / cost for project

Milestone 0: Ready to work

- [x] Copy Ansible skeleton (J) 5
- [x] Setup RPi3b+ base (J)
- [x] Setup firmware base (J)
- [x] Setup server. Linode,DNS,email,Mosquitto in Ansible (M) 5
- [x] Server is deployed

RPI ready

- [x] RPI automatically sets up reverse SSH (J)
- [x] Setup/test 3G modem on RPI (J)
- [x] Device can fetch current wind data from server (J)
- [x] Test Firmata at same time as ModemManager, ttyACMx. (J)

Milestone FANCONTROL:

- [x] Tested control of fan from software (via Firmata, analog/PWM) 

Control panel

- [x] Status LEDs
- [x] Potmeter animation scale
- [x] Potmeter manual fan control
- [x] Software/manual switch

Milestone B: All electronics assembled (M)

- [x] Baseboard for mounting in box
- [x] Optocoupler for fancontrol. Non-inverting
- [x] USB to serial in dashboard
- [x] RPi powered and running
- [x] Arduino with daughterboard
- [x] Display connected
- [x] Fan power module connected

## TODO

### System check, Oct 23

Documentation

- [x] Make block diagram of electronics

Windgust animation

- [x] First version. Should depend on (average) windspeed
- [x] Move animation to dedicated program (J)
- [x] systemd/Ansible for services (J)

Display:

- [x] Test servocontrol for dial type display. Incl a simple hand. (M)
- [x] Screen tested OK
- [x] Program to display windspeed

Milestone C: I/O complete, from RPi (M)

- [x] Get adjust potentiometer (power level)
- [x] Set fan power
- [x] Show text on display

Server datafetching

- [x] Define MQTT interface (J)
- [x] Server fetches data using YR (J) 3
- [x] Fetch data using scraped HTML page (J) 3
- [x] Deploy data fetcher to hub

## Assembly, Friday Oct 26

Electronics

- [x] Buy power supplies. 5V + 12V
- [x] Buy enclosure(s) PSU
- [x] Buy Wago connectors
- [x] Use cable glands on wires

User button

- [x] Test user button input
- [x] Firmata: get user button status (turn on)
- [x] Scale gusts based on user input

Display

- [ ] Add wind description label "frisk bris" under windspeed
- [ ] Hide startup rainbow, linux messages from screen
- [ ] Hide low-power symbol
- [ ] Add a small variation to shown windspeed

Monitoring

- [ ] Turn status LEDs in panel on/off
- [ ] Device reports heartbeat to server on MQTT (`fbp`)
- [ ] Server stores heartbeat events

System complete checklist

- [ ] Windgust animation works
- [ ] Monitoring exists
- [ ] Tested device poweroffs
- [ ] Tested device disconnect/reconnect
- [ ] Tested server downtime, recovers automatically
- [ ] Tested windpower override switch

## Failure modes

- I/O failure. Software does right thing, but right action does not happen
- RPi fails to fetch server. 3G connection, HTTP error, server down
- [...]

## Worklog

Date,person,hours,area

    02.10.2018,jon,8,planning
    02.10.2018,martin,8,planning
    03.10.2018,jon,2,firmware
    07.10.2018,jon,4,firmware
    09.10.2018,jon,1,planning
    09.10.2018,jon,4,io


