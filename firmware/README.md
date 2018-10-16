
## SSH config
To jump via the central cloud server.

    Host windhub
	    HostName hub.wind.mastensg.net
	    User jonnor
    
    Host wind0
     	User jonnor
    #	HostName  dlock-dev.local
	    ProxyCommand=ssh windhub nc localhost 2000

## Initalize device from base image

On first run you need to use default `pi` user. This should only be done on a private Ethernet connection.

    ansible-playbook display.yml -l display0 --user pi --ask-pass

Afterwards the SSH keys are in place

    ansible-playbook display.yml -l display0

## Make base image
To make something flashable of a known size (under 8GB) with a known hostname.

* Download Raspbian https://www.raspberrypi.org/downloads/raspbian/
* Flash onto SD card
* Mount root filesytem, change hostname and hosts to `display0`
* Enable workstation in /etc/avahi-daemon.conf
* Mount boot filesyste. Add empty `ssh` file to enable SSH
* Put SDCard into RPi, connect Ethernet cable to PC
* Boot RPi, check that you can SSH into it `ssh pi@display0.local`
* Check that automatic partition resizing was completed
* Turn of RPI, put SDcard into PC
* Resize root filesystem to 7G with `resize2fs`
* Resize root partition to 7.5G with `cfdisk`
* Make image using `dd if=/dev/mmcblk1 conv=sync,noerror bs=1M count=8k | gzip -c > display0-raspbian.img.gz`

## Flash base image


    gunzip -c display0-rasbian.img | sudo dd of=/dev/MYSCARD status=progress oflag=sync bs=1M
