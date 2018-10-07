
## Initalize device from base image

    ansible-playbook display-base.yml --extra-vars "hosts=display0 user=pi" --ask-pass

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


    gunzip -c display0.img | sudo dd of=/dev/MYSCARD status=progress oflag=sync bs=1M