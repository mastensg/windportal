
Key questions

- [x] OK with FOSS (Sjur)
- [x] Fan control interface. Analog or modbus
- [x] Decide display. LCD, dial indicator or 7-segment.
- [x] Get a custom domain (dep: Sjur) `windportal.dept.no`
- [ ] Ownership server,SIM (dep: Sjur)

Risk moments

- [ ] Interfacing fan,display
- [ ] Data communication device,server,datasource
- [ ] In-field failures

Bugs

- [ ] If Arduino is unplugged, wp_firmata keeps running using 100% CPU.
- [ ] ssh-call-home sometimes hangs, does not reconnect after network or server disconnect
- [ ] 4G connection not showing up

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

## TODO

### System check, Oct 23

Buy

- [ ] 5V + 12V PSUs
- [ ] Power supply enclosure
- [ ] Wago connectors

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

Electronics requirements

- [ ] Test user button input
- [x] Test RPI USB serial terminal

Control panel

- [x] Status LEDs
- [x] Potmeter animation scale
- [x] Potmeter manual fan control
- [x] Software/manual switch

Milestone B: All electronics assembled (M)

- [x] Baseboard for mounting in box
- [x] Optocoupler for fancontrol. Non-inverting
- [ ] USB to serial in dashboard
- [ ] RPi powered and running
- [x] Arduino with daughterboard
- [ ] User button plugged in
- [ ] Display connected
- [ ] Fan power module connected

Milestone C: I/O complete, from RPi (M)

- [ ] Get user button status (turn on)
- [x] Get adjust potentiometer (power level)
- [x] Set fan power
- [x] Show text on display
- [ ] Turn status LEDs on/off

## Finito

- [ ] Displays windspeed color

Server datafetching

- [x] Define MQTT interface (J)
- [ ] Server fetches data using YR (J) 3
- [ ] Fetch data using scraped HTML page (J) 3

Monitoring

- [ ] Device reports heartbeat to server on MQTT (`fbp`)
- [ ] Server stores heartbeat events

Thurs 25.
Milestone D: System complete

- [ ] Hide startup rainbow, linux messages from screen
- [ ] Monitoring exists
- [ ] Windgust animation works
- [ ] Tested device poweroffs
- [ ] Tested device disconnect/reconnect
- [ ] Tested server downtime
- [ ] Tested full gust button
- [ ] Tested connectivity loss


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


