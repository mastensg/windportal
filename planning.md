
Key questions

- [x] OK with FOSS (Sjur)
- [x] Fan control interface. Analog or modbus 
- [ ] LCD display, dial indicator or 7-segment. ! may need more hardware
- [ ] Decide ownership domain,server,SIM (dep: Sjur)

Risk moments

- [ ] Interfacing fan,display
- [ ] Data communication device,server,datasource
- [ ] In-field failures

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

Milestone FANCONTROL:

- [x] Tested control of fan from software (via Firmata, analog/PWM) 

## TODO

Milestone DISPLAY:

- [ ] Test servocontrol for dial type display. Incl a simple hand. (M)
- [ ] ... pending decision ...
- [ ] Test if 7-segment module bright enough (M)

RPI ready

- [x] RPI automatically sets up reverse SSH (J)
- [ ] Setup/test 3G modem on RPI (J)
- [ ] Test/fix Firmata at same time as ModemManager, ttyACMx. (J)
Hints about udev mechanisms. `ID_MM_CANDIDATE` and `ID_MM_PLATFORM_DRIVER_PROBE`
https://askubuntu.com/questions/740584/enabling-serial-network-devices-with-modemmanager
- [ ] Device can fetch current wind data from server (J)

Server datafetching

- [ ] Define MQTT interface (J)
- [ ] Server fetches data using YR (J) 3
- [ ] Fetch data using scraped HTML page (J) 3

Monitoring

- [ ] Device reports heartbeat to server on MQTT (`fbp`)
- [ ] Server stores heartbeat events

Windgust animation

- [ ] First version. Should depend on (average) windspeed
- [ ] (optional) Try to make fan changes faster, over Modbus

Mon 8
Milestone B: All electronics assembled (M)

- [ ] Optocoupler for fancontrol. Non-inverting
- [ ] RPi powered and running
- [ ] Arduino with daughterboard
- [ ] LEDs in control panel
- [ ] Potmeter in control panel
- [ ] Test button in control panel
- [ ] User button plugged in
- [ ] Display connected
- [ ] Fan power module connected

Fri 12 - Mon 16
Milestone C: I/O complete, from RPi (M)

- [ ] Turn status LEDs on/off
- [ ] Get user button status (turn on)
- [ ] Get adjust potentiometer (power level)
- [ ] Get test button stats (full power)
- [ ] Set fan power
- [ ] Show text on display

Thurs 25.
Milestone D: System complete

- [ ] Monitoring exists
- [ ] Windgust animation works
- [ ] Tested device poweroffs
- [ ] Tested device disconnect/reconnect
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
    03.10.2018,jon,2,firmware
    07.10.2018,jon,4,firmware
    09.10.2018,jon,1,planning
    09.10.2018,jon,4,io






