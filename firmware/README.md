
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

## Flash base image

    gunzip -c display0-raspbian.img | sudo dd of=/dev/MYSCARD status=progress oflag=sync bs=1M

## Make base image
To make something flashable of a known size (under 4GB) with a known hostname.

* Download Raspbian https://www.raspberrypi.org/downloads/raspbian/
* Flash onto SD card `unzip -p raspbian_lite_latest 2018-10-09-raspbian-stretch-lite.img | dd of=/dev/MYCARD bs=1M`
* Mount `/` filesytem, change hostname and hosts to `display0`
* Enable workstation in `/etc/avahi-daemon.conf`: `publish-workstation=yes`
* Mount `/boot` filesyste. Add empty `ssh` file to enable SSH
* Add `enable_uart=1` and `dtoverlay=pi3-disable-bt` to `/boot/config.txt` for serial console
* Put SDCard into RPi, connect Ethernet cable to PC
* Boot RPi, check that you can SSH into it `ssh pi@display0.local`
* Check that automatic partition resizing was completed
* Turn of RPI, put SDcard into PC
* Resize root filesystem to 3000M with `resize2fs`
* Resize root partition to 3100M with `cfdisk`
* Make image using `dd if=/dev/mmcblk1 conv=sync,noerror bs=1M count=8k | gzip -c > display0-raspbian.img.gz`

## Environment variables

Some programs require variables to be set to work.

    MSGFLO_BROKER={{display_broker_url}}
    DISPLAY_PREFIX=display/{{ansible_hostname}}
    WP_FONT_PATH=/opt/windmachine/equinor_fonts/Equinor-Medium.otf
