
--------------------------------  2018-11-26  --------------------------------

2018-11-26 13:20:59 +0100	hos gpj

```
mastensg@display0:~ $ journalctl -u display-firmata -n 1000 -f
-- Logs begin at Thu 2016-11-03 17:16:43 UTC. --
Nov 20 12:17:05 display0 systemd[1]: Started display-firmata.service.
-- Reboot --
Nov 26 12:00:16 display0 systemd[1]: Started display-firmata.service.
-- Reboot --
Nov 26 12:05:06 display0 systemd[1]: Started display-firmata.service.
-- Reboot --
Nov 26 12:07:26 display0 systemd[1]: Started display-firmata.service.
Nov 26 12:08:53 display0 python[318]: Python Version 3.5.3 (default, Sep 27 2018, 17:25:39)
Nov 26 12:08:53 display0 python[318]: [GCC 6.3.0 20170516]
Nov 26 12:08:53 display0 python[318]: PyMata version 2.18  Copyright(C) 2013-18 Alan Yorinks    All rights reserved.
Nov 26 12:08:53 display0 python[318]: Opening Arduino Serial port /dev/ttyACM0
Nov 26 12:08:53 display0 python[318]: Please wait while Arduino is being detected. This can take up to 30 seconds ...
Nov 26 12:08:53 display0 python[318]: Board Auto Discovery Failed!, Shutting Down
Nov 26 12:08:53 display0 python[318]: Traceback (most recent call last):
Nov 26 12:08:53 display0 python[318]:   File "/opt/windmachine/firmware/wp_firmata.py", line 56, in <module>
Nov 26 12:08:53 display0 python[318]:     main()
Nov 26 12:08:53 display0 python[318]:   File "/opt/windmachine/firmware/wp_firmata.py", line 23, in main
Nov 26 12:08:53 display0 python[318]:     board.set_pin_mode(LED_PIN, board.OUTPUT, board.DIGITAL)
Nov 26 12:08:53 display0 python[318]:   File "/opt/windmachine/venv/lib/python3.5/site-packages/PyMata/pymata.py", line 802, in set_pin_mode
Nov 26 12:08:53 display0 python[318]:     self._command_handler.send_command(command)
Nov 26 12:08:53 display0 python[318]:   File "/opt/windmachine/venv/lib/python3.5/site-packages/PyMata/pymata_command_handler.py", line 656, in send_command
Nov 26 12:08:53 display0 python[318]:     self.pymata.transport.write(data)
Nov 26 12:08:53 display0 python[318]:   File "/opt/windmachine/venv/lib/python3.5/site-packages/PyMata/pymata_serial.py", line 108, in write
Nov 26 12:08:53 display0 python[318]:     self.arduino.write(bytes([ord(data)]))
Nov 26 12:08:53 display0 python[318]:   File "/opt/windmachine/venv/lib/python3.5/site-packages/serial/serialposix.py", line 531, in write
Nov 26 12:08:53 display0 python[318]:     raise portNotOpenError
Nov 26 12:08:53 display0 python[318]: serial.serialutil.SerialException: Attempting to use a port that is not open
Nov 26 12:08:53 display0 systemd[1]: display-firmata.service: Main process exited, code=exited, status=1/FAILURE
Nov 26 12:08:53 display0 systemd[1]: display-firmata.service: Unit entered failed state.
Nov 26 12:08:53 display0 systemd[1]: display-firmata.service: Failed with result 'exit-code'.
Nov 26 12:09:03 display0 systemd[1]: display-firmata.service: Service hold-off time over, scheduling restart.
Nov 26 12:09:03 display0 systemd[1]: Stopped display-firmata.service.
Nov 26 12:09:03 display0 systemd[1]: Started display-firmata.service.
-- Reboot --
Nov 26 12:10:27 display0 systemd[1]: Started display-firmata.service.
mastensg@display0:~ $ ps aux | grep firmata
display    320  104  1.4  55988 13484 ?        Rsl  12:11  11:28 /opt/windmachine/venv/bin/python /opt/windmachine/firmware/wp_firmata.py
mastensg  1549  0.0  0.0   4372   532 pts/0    S+   12:22   0:00 grep --color=auto firmata
mastensg@display0:~ $ sudo strace -p 320 2>&1 | head -10
strace: Process 320 attached
write(13, "\367", 1)                    = -1 EAGAIN (Resource temporarily unavailable)
write(13, "\367", 1)                    = -1 EAGAIN (Resource temporarily unavailable)
write(13, "\367", 1)                    = -1 EAGAIN (Resource temporarily unavailable)
write(13, "\367", 1)                    = -1 EAGAIN (Resource temporarily unavailable)
write(13, "\367", 1)                    = -1 EAGAIN (Resource temporarily unavailable)
write(13, "\367", 1)                    = -1 EAGAIN (Resource temporarily unavailable)
write(13, "\367", 1)                    = -1 EAGAIN (Resource temporarily unavailable)
write(13, "\367", 1)                    = -1 EAGAIN (Resource temporarily unavailable)
write(13, "\367", 1)                    = -1 EAGAIN (Resource temporarily unavailable)
mastensg@display0:~ $ sudo ls -l /proc/320/fd/13
lrwx------ 1 display display 64 Nov 26 12:10 /proc/320/fd/13 -> /dev/ttyACM0
mastensg@display0:~ $ ps aux | grep -i modem
root       310  0.0  0.8  53020  7804 ?        Ssl  12:11   0:00 /usr/sbin/ModemManager
mastensg  1658  0.0  0.0   4372   556 pts/0    S+   12:25   0:00 grep --color=auto -i modem
mastensg@display0:~ $ sudo ls -l /proc/310/fd
total 0
lr-x------ 1 root root 64 Nov 26 12:10 0 -> /dev/null
lrwx------ 1 root root 64 Nov 26 12:10 1 -> socket:[12313]
lrwx------ 1 root root 64 Nov 26 12:10 10 -> /dev/ttyUSB0
lrwx------ 1 root root 64 Nov 26 12:10 11 -> /dev/ttyUSB1
l-wx------ 1 root root 64 Nov 26 12:10 2 -> /dev/null
lrwx------ 1 root root 64 Nov 26 12:10 3 -> anon_inode:[eventfd]
lrwx------ 1 root root 64 Nov 26 12:10 4 -> anon_inode:[eventfd]
lrwx------ 1 root root 64 Nov 26 12:10 5 -> socket:[12401]
lrwx------ 1 root root 64 Nov 26 12:10 6 -> anon_inode:[eventfd]
lrwx------ 1 root root 64 Nov 26 12:10 7 -> socket:[11604]
lrwx------ 1 root root 64 Nov 26 12:10 8 -> socket:[13396]
l-wx------ 1 root root 64 Nov 26 12:10 9 -> /run/systemd/inhibit/1.ref
mastensg@display0:~ $ ls -l /dev/tty[A-Z]*
crw-rw---- 1 root dialout 166,  0 Nov 26 12:10 /dev/ttyACM0
crw--w---- 1 root tty     204, 64 Nov 26 12:10 /dev/ttyAMA0
crw------- 1 root root      5,  3 Nov 20 12:17 /dev/ttyprintk
crw-rw---- 1 root dialout   4, 64 Nov 20 12:17 /dev/ttyS0
crw-rw---- 1 root dialout 188,  0 Nov 26 12:26 /dev/ttyUSB0
crw-rw---- 1 root dialout 188,  1 Nov 26 12:10 /dev/ttyUSB1
mastensg@display0:~ $
```

## mitigation

* enable watchdog for `display-firmata` service

`WatchdogSec=30`

```
mastensg@display0:~ $ sudo systemctl restart display-firmata
Warning: display-firmata.service changed on disk. Run 'systemctl daemon-reload' to reload units.
mastensg@display0:~ $ sudo systemctl daemon-reload
mastensg@display0:~ $ sudo systemctl restart display-firmata
mastensg@display0:~ $ journalctl -f
-- Logs begin at Thu 2016-11-03 17:16:43 UTC. --
Nov 26 12:49:43 display0 sudo[2659]: pam_unix(sudo:session): session opened for user root by mastensg(uid=0)
Nov 26 12:49:43 display0 systemd[1]: Stopped display-firmata.service.
Nov 26 12:49:43 display0 systemd[1]: Started display-firmata.service.
Nov 26 12:49:44 display0 sudo[2659]: pam_unix(sudo:session): session closed for user root
Nov 26 12:49:44 display0 systemd[1]: Started display-status-led.service.
Nov 26 12:49:45 display0 systemd[1]: display-firmata.service: Watchdog timeout (limit 1s)!
Nov 26 12:49:45 display0 systemd[1]: display-firmata.service: Killing process 2666 (python) with signal SIGABRT.
Nov 26 12:49:45 display0 systemd[1]: display-firmata.service: Main process exited, code=killed, status=6/ABRT
Nov 26 12:49:45 display0 systemd[1]: display-firmata.service: Unit entered failed state.
Nov 26 12:49:45 display0 systemd[1]: display-firmata.service: Failed with result 'watchdog'.




Nov 26 12:49:54 display0 systemd[1]: Starting display-status-led.service...
Nov 26 12:49:55 display0 systemd[1]: display-firmata.service: Service hold-off time over, scheduling restart.
Nov 26 12:49:55 display0 systemd[1]: Stopped display-firmata.service.
Nov 26 12:49:55 display0 systemd[1]: Started display-firmata.service.
Nov 26 12:49:56 display0 systemd[1]: Started display-status-led.service.
Nov 26 12:49:56 display0 systemd[1]: display-firmata.service: Watchdog timeout (limit 1s)!
Nov 26 12:49:56 display0 systemd[1]: display-firmata.service: Killing process 2690 (python) with signal SIGABRT.
Nov 26 12:49:56 display0 systemd[1]: display-firmata.service: Main process exited, code=killed, status=6/ABRT
Nov 26 12:49:56 display0 systemd[1]: display-firmata.service: Unit entered failed state.
Nov 26 12:49:56 display0 systemd[1]: display-firmata.service: Failed with result 'watchdog'.








Nov 26 12:50:04 display0 systemd[1]: Starting display-status-led.service...
Nov 26 12:50:06 display0 systemd[1]: Started display-status-led.service.
Nov 26 12:50:06 display0 systemd[1]: display-firmata.service: Service hold-off time over, scheduling restart.
Nov 26 12:50:06 display0 systemd[1]: Stopped display-firmata.service.
Nov 26 12:50:06 display0 systemd[1]: Started display-firmata.service.
Nov 26 12:50:08 display0 systemd[1]: display-firmata.service: Watchdog timeout (limit 1s)!
Nov 26 12:50:08 display0 systemd[1]: display-firmata.service: Killing process 2729 (python) with signal SIGABRT.
Nov 26 12:50:08 display0 systemd[1]: display-firmata.service: Main process exited, code=killed, status=6/ABRT
Nov 26 12:50:08 display0 systemd[1]: display-firmata.service: Unit entered failed state.
Nov 26 12:50:08 display0 systemd[1]: display-firmata.service: Failed with result 'watchdog'.





^C
mastensg@display0:~ $
```

`WatchdogSec=30`

```
Nov 26 12:51:00 display0 systemd[1]: Started display-status-led.service.
Nov 26 12:51:24 display0 systemd[1]: Starting display-status-led.service...
Nov 26 12:51:26 display0 systemd[1]: Started display-status-led.service.
Nov 26 12:51:28 display0 systemd[1]: display-firmata.service: Watchdog timeout (limit 30s)!
Nov 26 12:51:28 display0 systemd[1]: display-firmata.service: Killing process 2903 (python) with signal SIGABRT.
Nov 26 12:51:28 display0 systemd[1]: display-firmata.service: Main process exited, code=killed, status=6/ABRT
Nov 26 12:51:28 display0 systemd[1]: display-firmata.service: Unit entered failed state.
Nov 26 12:51:28 display0 systemd[1]: display-firmata.service: Failed with result 'watchdog'.
Nov 26 12:51:34 display0 systemd[1]: Starting display-status-led.service...
Nov 26 12:51:36 display0 systemd[1]: Started display-status-led.service.
Nov 26 12:51:38 display0 systemd[1]: display-firmata.service: Service hold-off time over, scheduling restart.
Nov 26 12:51:38 display0 systemd[1]: Stopped display-firmata.service.
Nov 26 12:51:38 display0 systemd[1]: Started display-firmata.service.
Nov 26 12:51:39 display0 kernel: cdc_acm 1-1.1.2.4:1.0: failed to set dtr/rts
Nov 26 12:51:40 display0 kernel: cdc_acm 1-1.1.2.4:1.0: failed to set dtr/rts
Nov 26 12:51:54 display0 systemd[1]: Starting display-status-led.service...
Nov 26 12:51:56 display0 systemd[1]: Started display-status-led.service.
Nov 26 12:52:04 display0 systemd[1]: Starting display-status-led.service...
Nov 26 12:52:06 display0 systemd[1]: Started display-status-led.service.
Nov 26 12:52:08 display0 systemd[1]: display-firmata.service: Watchdog timeout (limit 30s)!
Nov 26 12:52:08 display0 systemd[1]: display-firmata.service: Killing process 2983 (python) with signal SIGABRT.
Nov 26 12:52:08 display0 systemd[1]: display-firmata.service: Main process exited, code=killed, status=6/ABRT
Nov 26 12:52:08 display0 systemd[1]: display-firmata.service: Unit entered failed state.
Nov 26 12:52:08 display0 systemd[1]: display-firmata.service: Failed with result 'watchdog'.
Nov 26 12:52:14 display0 systemd[1]: Starting display-status-led.service...
Nov 26 12:52:16 display0 systemd[1]: Started display-status-led.service.
Nov 26 12:52:19 display0 systemd[1]: display-firmata.service: Service hold-off time over, scheduling restart.
Nov 26 12:52:19 display0 systemd[1]: Stopped display-firmata.service.
Nov 26 12:52:19 display0 systemd[1]: Started display-firmata.service.
Nov 26 12:52:19 display0 systemd[1]: Starting display-status-led.service...
Nov 26 12:52:20 display0 kernel: cdc_acm 1-1.1.2.4:1.0: failed to set dtr/rts
Nov 26 12:52:21 display0 systemd[1]: Started display-status-led.service.
```

### disable output buffering, to make the journal useful

`ExecStart=/opt/windmachine/venv/bin/python /opt/windmachine/firmware/wp_firmata.py`

into

`ExecStart=/opt/windmachine/venv/bin/python -u /opt/windmachine/firmware/wp_firmata.py`

### watchdog timeout

```
TimeoutStartSec=20
WatchdogSec=2
```

too short watchdog timeout:

```
Nov 26 13:30:28 display0 systemd[1]: display-firmata.service: Service hold-off time over, scheduling restart.
Nov 26 13:30:28 display0 systemd[1]: Stopped display-firmata.service.
Nov 26 13:30:28 display0 systemd[1]: Started display-firmata.service.
Nov 26 13:30:28 display0 python[10508]: Python Version 3.5.3 (default, Sep 27 2018, 17:25:39)
Nov 26 13:30:28 display0 python[10508]: [GCC 6.3.0 20170516]
Nov 26 13:30:28 display0 python[10508]: PyMata version 2.18  Copyright(C) 2013-18 Alan Yorinks    All rights reserved.
Nov 26 13:30:28 display0 python[10508]: Opening Arduino Serial port /dev/ttyACM0
Nov 26 13:30:30 display0 systemd[1]: display-firmata.service: Watchdog timeout (limit 2s)!
Nov 26 13:30:30 display0 systemd[1]: display-firmata.service: Killing process 10508 (python) with signal SIGABRT.
Nov 26 13:30:30 display0 systemd[1]: display-firmata.service: Main process exited, code=killed, status=6/ABRT
Nov 26 13:30:30 display0 systemd[1]: display-firmata.service: Unit entered failed state.
Nov 26 13:30:30 display0 systemd[1]: display-firmata.service: Failed with result 'watchdog'.
```

```
TimeoutStartSec=10
WatchdogSec=10
```

looks good.
