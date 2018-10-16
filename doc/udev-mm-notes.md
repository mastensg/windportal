

## serial device internal on RPI

jonnor@display0:~ $ udevadm info -a --name /dev/ttyAMA0

Udevadm info starts with the device specified by the devpath and then
walks up the chain of parent devices. It prints for every device
found, all possible attributes in the udev rules key format.
A rule to match, can be composed by the attributes of the device
and the attributes from one single parent device.

  looking at device '/devices/platform/soc/3f201000.serial/tty/ttyAMA0':
    KERNEL=="ttyAMA0"
    SUBSYSTEM=="tty"
    DRIVER==""

  looking at parent device '/devices/platform/soc/3f201000.serial':
    KERNELS=="3f201000.serial"
    SUBSYSTEMS=="amba"
    DRIVERS=="uart-pl011"
    ATTRS{driver_override}=="(null)"
    ATTRS{id}=="00241011"
    ATTRS{irq0}=="87"

  looking at parent device '/devices/platform/soc':
    KERNELS=="soc"
    SUBSYSTEMS=="platform"
    DRIVERS==""
    ATTRS{driver_override}=="(null)"

  looking at parent device '/devices/platform':
    KERNELS=="platform"
    SUBSYSTEMS==""
    DRIVERS==""


    jonnor@display0:~ $ udevadm info --query=property /dev/ttyAMA0
    DEVLINKS=/dev/serial1
    DEVNAME=/dev/ttyAMA0
    DEVPATH=/devices/platform/soc/3f201000.serial/tty/ttyAMA0
    ID_MM_CANDIDATE=1
    MAJOR=204
    MINOR=64
    SUBSYSTEM=tty
    TAGS=:systemd:
    USEC_INITIALIZED=3899475


## Arduino Leonardo

jonnor@display0:~ $ udevadm monitor
monitor will print the received events for:
UDEV - the event which udev sends out after rule processing
KERNEL - the kernel uevent

KERNEL[785.479930] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3 (usb)
KERNEL[785.481021] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3/1-1.3:1.0 (usb)
KERNEL[785.481991] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3/1-1.3:1.1 (usb)
UDEV  [785.510745] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3 (usb)
UDEV  [785.521791] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3/1-1.3:1.1 (usb)
KERNEL[785.526125] add      /module/cdc_acm (module)
KERNEL[785.528409] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3/1-1.3:1.0/tty/ttyACM0 (tty)
KERNEL[785.528593] add      /bus/usb/drivers/cdc_acm (drivers)
UDEV  [785.528899] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3/1-1.3:1.0 (usb)
UDEV  [785.530816] add      /module/cdc_acm (module)
UDEV  [785.535533] add      /bus/usb/drivers/cdc_acm (drivers)
UDEV  [785.541592] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3/1-1.3:1.0/tty/ttyACM0 (tty)
^Cjonnor@display0:~ $ udevadm info -a --name /dev/ttyACM0 --query=all
P: /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3/1-1.3:1.0/tty/ttyACM0
N: ttyACM0
S: serial/by-id/usb-Arduino_LLC_Arduino_Leonardo-if00
S: serial/by-path/platform-3f980000.usb-usb-0:1.3:1.0
E: DEVLINKS=/dev/serial/by-id/usb-Arduino_LLC_Arduino_Leonardo-if00 /dev/serial/by-path/platform-3f980000.usb-usb-0:1.3:1.0
E: DEVNAME=/dev/ttyACM0
E: DEVPATH=/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3/1-1.3:1.0/tty/ttyACM0
E: ID_BUS=usb
E: ID_MM_CANDIDATE=1
E: ID_MODEL=Arduino_Leonardo
E: ID_MODEL_ENC=Arduino\x20Leonardo
E: ID_MODEL_FROM_DATABASE=Leonardo (CDC ACM, HID)
E: ID_MODEL_ID=8036
E: ID_PATH=platform-3f980000.usb-usb-0:1.3:1.0
E: ID_PATH_TAG=platform-3f980000_usb-usb-0_1_3_1_0
E: ID_REVISION=0100
E: ID_SERIAL=Arduino_LLC_Arduino_Leonardo
E: ID_TYPE=generic
E: ID_USB_CLASS_FROM_DATABASE=Miscellaneous Device
E: ID_USB_DRIVER=cdc_acm
E: ID_USB_INTERFACES=:020200:0a0000:
E: ID_USB_INTERFACE_NUM=00
E: ID_USB_PROTOCOL_FROM_DATABASE=Interface Association
E: ID_VENDOR=Arduino_LLC
E: ID_VENDOR_ENC=Arduino\x20LLC
E: ID_VENDOR_FROM_DATABASE=Arduino SA
E: ID_VENDOR_ID=2341
E: MAJOR=166
E: MINOR=0
E: SUBSYSTEM=tty
E: TAGS=:systemd:
E: USEC_INITIALIZED=785540456

jonnor@display0:~ $ udevadm info -a --name /dev/ttyACM0


## Not connecting

Oct 16 18:10:15 display0 ModemManager[297]: <info>  Simple connect started...
Oct 16 18:10:15 display0 ModemManager[297]: <info>  Simple connect state (4/8): Wait to get fully enabled
Oct 16 18:10:15 display0 ModemManager[297]: <info>  Simple connect state (5/8): Register
Oct 16 18:10:15 display0 ModemManager[297]: <info>  Simple connect state (6/8): Bearer
Oct 16 18:10:15 display0 ModemManager[297]: <info>  Simple connect state (7/8): Connect
Oct 16 18:10:15 display0 ModemManager[297]: <info>  Modem /org/freedesktop/ModemManager1/Modem/1: state changed (registered -> connecting)
Oct 16 18:10:15 display0 ModemManager[297]: <warn>  (ttyUSB1): port attributes not fully set

Udevadm info starts with the device specified by the devpath and then
walks up the chain of parent devices. It prints for every device
found, all possible attributes in the udev rules key format.
A rule to match, can be composed by the attributes of the device
and the attributes from one single parent device.

  looking at device '/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3/1-1.3:1.0/tty/ttyACM0':
    KERNEL=="ttyACM0"
    SUBSYSTEM=="tty"
    DRIVER==""

  looking at parent device '/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3/1-1.3:1.0':
    KERNELS=="1-1.3:1.0"
    SUBSYSTEMS=="usb"
    DRIVERS=="cdc_acm"
    ATTRS{authorized}=="1"
    ATTRS{bAlternateSetting}==" 0"
    ATTRS{bInterfaceClass}=="02"
    ATTRS{bInterfaceNumber}=="00"
    ATTRS{bInterfaceProtocol}=="00"
    ATTRS{bInterfaceSubClass}=="02"
    ATTRS{bNumEndpoints}=="01"
    ATTRS{bmCapabilities}=="6"
    ATTRS{iad_bFirstInterface}=="00"
    ATTRS{iad_bFunctionClass}=="02"
    ATTRS{iad_bFunctionProtocol}=="01"
    ATTRS{iad_bFunctionSubClass}=="02"
    ATTRS{iad_bInterfaceCount}=="02"
    ATTRS{supports_autosuspend}=="1"

  looking at parent device '/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.3':
    KERNELS=="1-1.3"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{authorized}=="1"
    ATTRS{avoid_reset_quirk}=="0"
    ATTRS{bConfigurationValue}=="1"
    ATTRS{bDeviceClass}=="ef"
    ATTRS{bDeviceProtocol}=="01"
    ATTRS{bDeviceSubClass}=="02"
    ATTRS{bMaxPacketSize0}=="64"
    ATTRS{bMaxPower}=="500mA"
    ATTRS{bNumConfigurations}=="1"
    ATTRS{bNumInterfaces}==" 2"
    ATTRS{bcdDevice}=="0100"
    ATTRS{bmAttributes}=="a0"
    ATTRS{busnum}=="1"
    ATTRS{configuration}==""
    ATTRS{devnum}=="5"
    ATTRS{devpath}=="1.3"
    ATTRS{devspec}=="  (null)"
    ATTRS{idProduct}=="8036"
    ATTRS{idVendor}=="2341"
    ATTRS{ltm_capable}=="no"
    ATTRS{manufacturer}=="Arduino LLC"
    ATTRS{maxchild}=="0"
    ATTRS{product}=="Arduino Leonardo"
    ATTRS{quirks}=="0x0"
    ATTRS{removable}=="removable"
    ATTRS{speed}=="12"
    ATTRS{urbnum}=="276"
    ATTRS{version}==" 2.00"

  looking at parent device '/devices/platform/soc/3f980000.usb/usb1/1-1':
    KERNELS=="1-1"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{authorized}=="1"
    ATTRS{avoid_reset_quirk}=="0"
    ATTRS{bConfigurationValue}=="1"
    ATTRS{bDeviceClass}=="09"
    ATTRS{bDeviceProtocol}=="02"
    ATTRS{bDeviceSubClass}=="00"
    ATTRS{bMaxPacketSize0}=="64"
    ATTRS{bMaxPower}=="2mA"
    ATTRS{bNumConfigurations}=="1"
    ATTRS{bNumInterfaces}==" 1"
    ATTRS{bcdDevice}=="0bb3"
    ATTRS{bmAttributes}=="e0"
    ATTRS{busnum}=="1"
    ATTRS{configuration}==""
    ATTRS{devnum}=="2"
    ATTRS{devpath}=="1"
    ATTRS{idProduct}=="2514"
    ATTRS{idVendor}=="0424"
    ATTRS{ltm_capable}=="no"
    ATTRS{maxchild}=="4"
    ATTRS{quirks}=="0x0"
    ATTRS{removable}=="unknown"
    ATTRS{speed}=="480"
    ATTRS{urbnum}=="53"
    ATTRS{version}==" 2.00"

  looking at parent device '/devices/platform/soc/3f980000.usb/usb1':
    KERNELS=="usb1"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{authorized}=="1"
    ATTRS{authorized_default}=="1"
    ATTRS{avoid_reset_quirk}=="0"
    ATTRS{bConfigurationValue}=="1"
    ATTRS{bDeviceClass}=="09"
    ATTRS{bDeviceProtocol}=="01"
    ATTRS{bDeviceSubClass}=="00"
    ATTRS{bMaxPacketSize0}=="64"
    ATTRS{bMaxPower}=="0mA"
    ATTRS{bNumConfigurations}=="1"
    ATTRS{bNumInterfaces}==" 1"
    ATTRS{bcdDevice}=="0414"
    ATTRS{bmAttributes}=="e0"
    ATTRS{busnum}=="1"
    ATTRS{configuration}==""
    ATTRS{devnum}=="1"
    ATTRS{devpath}=="0"
    ATTRS{idProduct}=="0002"
    ATTRS{idVendor}=="1d6b"
    ATTRS{interface_authorized_default}=="1"
    ATTRS{ltm_capable}=="no"
    ATTRS{manufacturer}=="Linux 4.14.71-v7+ dwc_otg_hcd"
    ATTRS{maxchild}=="1"
    ATTRS{product}=="DWC OTG Controller"
    ATTRS{quirks}=="0x0"
    ATTRS{removable}=="unknown"
    ATTRS{serial}=="3f980000.usb"
    ATTRS{speed}=="480"
    ATTRS{urbnum}=="31"
    ATTRS{version}==" 2.00"

  looking at parent device '/devices/platform/soc/3f980000.usb':
    KERNELS=="3f980000.usb"
    SUBSYSTEMS=="platform"
    DRIVERS=="dwc_otg"
    ATTRS{busconnected}=="Bus Connected = 0x1"
    ATTRS{buspower}=="Bus Power = 0x1"
    ATTRS{bussuspend}=="Bus Suspend = 0x0"
    ATTRS{devspeed}=="Device Speed = 0x0"
    ATTRS{driver_override}=="(null)"
    ATTRS{enumspeed}=="Device Enumeration Speed = 0x1"
    ATTRS{fr_interval}=="Frame Interval = 0x1d4b"
    ATTRS{ggpio}=="GGPIO = 0x00000000"
    ATTRS{gnptxfsiz}=="GNPTXFSIZ = 0x01000306"
    ATTRS{gotgctl}=="GOTGCTL = 0x001c0000"
    ATTRS{gpvndctl}=="GPVNDCTL = 0x00000000"
    ATTRS{grxfsiz}=="GRXFSIZ = 0x00000306"
    ATTRS{gsnpsid}=="GSNPSID = 0x4f54280a"
    ATTRS{guid}=="GUID = 0x2708a000"
    ATTRS{gusbcfg}=="GUSBCFG = 0x20001700"
    ATTRS{hcd_frrem}=="HCD Dump Frame Remaining"
    ATTRS{hcddump}=="HCD Dump"
    ATTRS{hnp}=="HstNegScs = 0x0"
    ATTRS{hnpcapable}=="HNPCapable = 0x1"
    ATTRS{hprt0}=="HPRT0 = 0x00001005"
    ATTRS{hptxfsiz}=="HPTXFSIZ = 0x02000406"
    ATTRS{hsic_connect}=="HSIC Connect = 0x1"
    ATTRS{inv_sel_hsic}=="Invert Select HSIC = 0x0"
    ATTRS{mode}=="Mode = 0x1"
    ATTRS{mode_ch_tim_en}=="Mode Change Ready Timer Enable = 0x0"
    ATTRS{rd_reg_test}=="Time to read GNPTXFSIZ reg 10000000 times: 1770 msecs (177 jiffies)"
    ATTRS{regdump}=="Register Dump"
    ATTRS{regoffset}=="0xffffffff"
    ATTRS{regvalue}=="invalid offset"
    ATTRS{rem_wakeup_pwrdn}==""
    ATTRS{remote_wakeup}=="Remote Wakeup Sig = 0 Enabled = 0 LPM Remote Wakeup = 0"
    ATTRS{spramdump}=="SPRAM Dump"
    ATTRS{srp}=="SesReqScs = 0x0"
    ATTRS{srpcapable}=="SRPCapable = 0x1"
    ATTRS{wr_reg_test}=="Time to write GNPTXFSIZ reg 10000000 times: 320 msecs (32 jiffies)"

  looking at parent device '/devices/platform/soc':
    KERNELS=="soc"
    SUBSYSTEMS=="platform"
    DRIVERS==""
    ATTRS{driver_override}=="(null)"

  looking at parent device '/devices/platform':
    KERNELS=="platform"
    SUBSYSTEMS==""
    DRIVERS==""


## 4G modem

jonnor@display0:~ $ udevadm monitor
monitor will print the received events for:
UDEV - the event which udev sends out after rule processing
KERNEL - the kernel uevent


KERNEL[890.422815] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2 (usb)
KERNEL[890.485129] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0 (usb)
KERNEL[890.486846] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/host0 (scsi)
KERNEL[890.487033] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/host0/scsi_host/host0 (scsi_host)
UDEV  [890.525196] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2 (usb)

UDEV  [890.625184] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0 (usb)
UDEV  [890.628709] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/host0 (scsi)
UDEV  [890.632169] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/host0/scsi_host/host0 (scsi_host)
KERNEL[891.221433] remove   /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/host0/scsi_host/host0 (scsi_host)
KERNEL[891.222620] remove   /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/host0 (scsi)
UDEV  [891.223584] remove   /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/host0/scsi_host/host0 (scsi_host)
UDEV  [891.224100] remove   /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/host0 (scsi)
KERNEL[891.534158] remove   /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0 (usb)
KERNEL[891.535558] remove   /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2 (usb)
UDEV  [891.538819] remove   /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0 (usb)
UDEV  [891.541997] remove   /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2 (usb)
KERNEL[892.112520] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2 (usb)
KERNEL[892.175345] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0 (usb)
KERNEL[892.175591] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.1 (usb)
KERNEL[892.176247] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.2 (usb)
KERNEL[892.177231] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3 (usb)
KERNEL[892.178595] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0 (scsi)
KERNEL[892.178760] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/scsi_host/host0 (scsi_host)
KERNEL[892.179371] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4 (usb)
KERNEL[892.184697] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1 (scsi)
KERNEL[892.184878] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/scsi_host/host1 (scsi_host)
UDEV  [892.257261] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2 (usb)
UDEV  [892.274844] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4 (usb)
KERNEL[892.278015] add      /module/usbserial (module)
KERNEL[892.278256] add      /bus/usb-serial (bus)
KERNEL[892.278371] add      /bus/usb/drivers/usbserial (drivers)
KERNEL[892.278503] add      /bus/usb/drivers/usbserial_generic (drivers)
KERNEL[892.278627] add      /bus/usb-serial/drivers/generic (drivers)
KERNEL[892.281855] add      /module/cdc_ncm (module)
KERNEL[892.282149] add      /bus/usb/drivers/cdc_ncm (drivers)
KERNEL[892.283416] add      /module/usb_wwan (module)
KERNEL[892.285813] add      /module/cdc_wdm (module)
KERNEL[892.286000] add      /bus/usb/drivers/cdc_wdm (drivers)
UDEV  [892.295627] add      /bus/usb-serial (bus)
UDEV  [892.296264] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1 (scsi)
UDEV  [892.296591] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3 (usb)
UDEV  [892.297199] add      /module/usbserial (module)
KERNEL[892.298153] add      /module/huawei_cdc_ncm (module)
UDEV  [892.303886] add      /module/cdc_ncm (module)
UDEV  [892.306884] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/scsi_host/host1 (scsi_host)
KERNEL[892.307621] add      /module/option (module)
KERNEL[892.307770] add      /bus/usb/drivers/option (drivers)
UDEV  [892.308091] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0 (scsi)
KERNEL[892.308664] add      /bus/usb-serial/drivers/option1 (drivers)
UDEV  [892.312162] add      /bus/usb/drivers/usbserial_generic (drivers)
UDEV  [892.313157] add      /module/cdc_wdm (module)
UDEV  [892.317561] add      /module/huawei_cdc_ncm (module)
UDEV  [892.318239] add      /bus/usb/drivers/cdc_wdm (drivers)
UDEV  [892.318459] add      /bus/usb-serial/drivers/generic (drivers)
UDEV  [892.320194] add      /bus/usb/drivers/cdc_ncm (drivers)
UDEV  [892.321433] add      /module/usb_wwan (module)
UDEV  [892.325736] add      /bus/usb/drivers/option (drivers)
UDEV  [892.326747] add      /bus/usb-serial/drivers/option1 (drivers)
UDEV  [892.328542] add      /bus/usb/drivers/usbserial (drivers)
UDEV  [892.330215] add      /module/option (module)
UDEV  [892.334218] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/scsi_host/host0 (scsi_host)
KERNEL[892.412795] add      /class/usbmisc (class)
KERNEL[892.412936] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.2/usbmisc/cdc-wdm0 (usbmisc)
KERNEL[892.413414] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.2/net/wwan0 (net)
KERNEL[892.413513] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.2/net/wwan0/queues/rx-0 (queues)
KERNEL[892.413563] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.2/net/wwan0/queues/tx-0 (queues)
KERNEL[892.413617] add      /bus/usb/drivers/huawei_cdc_ncm (drivers)
UDEV  [892.413917] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.2 (usb)
KERNEL[892.414575] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/ttyUSB0 (usb-serial)
KERNEL[892.414701] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/ttyUSB0/tty/ttyUSB0 (tty)
KERNEL[892.414769] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.1/ttyUSB1 (usb-serial)
KERNEL[892.415367] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.1/ttyUSB1/tty/ttyUSB1 (tty)
UDEV  [892.415521] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.1 (usb)
UDEV  [892.416177] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0 (usb)
UDEV  [892.416449] add      /class/usbmisc (class)
UDEV  [892.420099] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/ttyUSB0 (usb-serial)
UDEV  [892.421726] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.1/ttyUSB1 (usb-serial)
UDEV  [892.423616] add      /bus/usb/drivers/huawei_cdc_ncm (drivers)
UDEV  [892.428848] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.2/usbmisc/cdc-wdm0 (usbmisc)
UDEV  [892.461268] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/ttyUSB0/tty/ttyUSB0 (tty)
UDEV  [892.465756] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.1/ttyUSB1/tty/ttyUSB1 (tty)
UDEV  [892.476228] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.2/net/wwan0 (net)
UDEV  [892.483090] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.2/net/wwan0/queues/tx-0 (queues)
UDEV  [892.483813] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.2/net/wwan0/queues/rx-0 (queues)
KERNEL[893.201550] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0 (scsi)
KERNEL[893.201794] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0/0:0:0:0 (scsi)
KERNEL[893.201964] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0/0:0:0:0/scsi_device/0:0:0:0 (scsi_device)
KERNEL[893.203686] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0 (scsi)
KERNEL[893.203866] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0 (scsi)
KERNEL[893.204012] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0/scsi_disk/1:0:0:0 (scsi_disk)
KERNEL[893.204223] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0/scsi_device/1:0:0:0 (scsi_device)
KERNEL[893.206090] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0/0:0:0:0/bsg/0:0:0:0 (bsg)
KERNEL[893.206771] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0/bsg/1:0:0:0 (bsg)
KERNEL[893.207913] add      /devices/virtual/bdi/8:0 (bdi)
KERNEL[893.208562] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0/block/sda (block)
KERNEL[893.215062] change   /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0/block/sda (block)
UDEV  [893.215710] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0 (scsi)
UDEV  [893.216120] add      /devices/virtual/bdi/8:0 (bdi)
UDEV  [893.221402] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0 (scsi)
KERNEL[893.227185] add      /module/sg (module)
KERNEL[893.227357] add      /class/scsi_generic (class)
KERNEL[893.229634] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0/0:0:0:0/scsi_generic/sg0 (scsi_generic)
KERNEL[893.230695] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0/scsi_generic/sg1 (scsi_generic)
UDEV  [893.231045] add      /module/sg (module)
UDEV  [893.232640] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0 (scsi)
UDEV  [893.238934] add      /class/scsi_generic (class)
UDEV  [893.243572] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0/scsi_device/1:0:0:0 (scsi_device)
KERNEL[893.244850] add      /module/cdrom (module)
UDEV  [893.249239] add      /module/cdrom (module)
KERNEL[893.249423] add      /module/sr_mod (module)
UDEV  [893.250084] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0/scsi_disk/1:0:0:0 (scsi_disk)
KERNEL[893.251542] add      /devices/virtual/bdi/11:0 (bdi)
KERNEL[893.251867] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0/0:0:0:0/block/sr0 (block)
KERNEL[893.252266] add      /bus/scsi/drivers/sr (drivers)
UDEV  [893.252574] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0/0:0:0:0 (scsi)
KERNEL[893.253322] change   /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0/0:0:0:0/block/sr0 (block)
UDEV  [893.254731] add      /module/sr_mod (module)
UDEV  [893.256853] add      /devices/virtual/bdi/11:0 (bdi)
UDEV  [893.261433] add      /bus/scsi/drivers/sr (drivers)
UDEV  [893.261548] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0/0:0:0:0/scsi_device/0:0:0:0 (scsi_device)
UDEV  [893.262439] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0/bsg/1:0:0:0 (bsg)
UDEV  [893.264472] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0/scsi_generic/sg1 (scsi_generic)
UDEV  [893.265433] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0/0:0:0:0/bsg/0:0:0:0 (bsg)
UDEV  [893.268924] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0/0:0:0:0/scsi_generic/sg0 (scsi_generic)
UDEV  [893.271819] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0/block/sda (block)
UDEV  [893.280708] change   /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.4/host1/target1:0:0/1:0:0:0/block/sda (block)
UDEV  [893.650868] add      /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0/0:0:0:0/block/sr0 (block)
UDEV  [893.748424] change   /devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.3/host0/target0:0:0/0:0:0:0/block/sr0 (block)


jonnor@display0:~ $ udevadm info -a --name /dev/ttyUSB0

Udevadm info starts with the device specified by the devpath and then
walks up the chain of parent devices. It prints for every device
found, all possible attributes in the udev rules key format.
A rule to match, can be composed by the attributes of the device
and the attributes from one single parent device.

  looking at device '/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/ttyUSB0/tty/ttyUSB0':
    KERNEL=="ttyUSB0"
    SUBSYSTEM=="tty"
    DRIVER==""

  looking at parent device '/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0/ttyUSB0':
    KERNELS=="ttyUSB0"
    SUBSYSTEMS=="usb-serial"
    DRIVERS=="option1"
    ATTRS{port_number}=="0"

  looking at parent device '/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2/1-1.2:1.0':
    KERNELS=="1-1.2:1.0"
    SUBSYSTEMS=="usb"
    DRIVERS=="option"
    ATTRS{authorized}=="1"
    ATTRS{bAlternateSetting}==" 0"
    ATTRS{bInterfaceClass}=="ff"
    ATTRS{bInterfaceNumber}=="00"
    ATTRS{bInterfaceProtocol}=="12"
    ATTRS{bInterfaceSubClass}=="02"
    ATTRS{bNumEndpoints}=="02"
    ATTRS{supports_autosuspend}=="1"

  looking at parent device '/devices/platform/soc/3f980000.usb/usb1/1-1/1-1.2':
    KERNELS=="1-1.2"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{authorized}=="1"
    ATTRS{avoid_reset_quirk}=="1"
    ATTRS{bConfigurationValue}=="1"
    ATTRS{bDeviceClass}=="00"
    ATTRS{bDeviceProtocol}=="00"
    ATTRS{bDeviceSubClass}=="00"
    ATTRS{bMaxPacketSize0}=="64"
    ATTRS{bMaxPower}=="2mA"
    ATTRS{bNumConfigurations}=="1"
    ATTRS{bNumInterfaces}==" 5"
    ATTRS{bcdDevice}=="0102"
    ATTRS{bmAttributes}=="80"
    ATTRS{busnum}=="1"
    ATTRS{configuration}==""
    ATTRS{devnum}=="7"
    ATTRS{devpath}=="1.2"
    ATTRS{devspec}=="  (null)"
    ATTRS{idProduct}=="1506"
    ATTRS{idVendor}=="12d1"
    ATTRS{ltm_capable}=="no"
    ATTRS{manufacturer}=="HUAWEI_MOBILE"
    ATTRS{maxchild}=="0"
    ATTRS{product}=="HUAWEI_MOBILE"
    ATTRS{quirks}=="0x10"
    ATTRS{removable}=="removable"
    ATTRS{speed}=="480"
    ATTRS{urbnum}=="2056"
    ATTRS{version}==" 2.10"

  looking at parent device '/devices/platform/soc/3f980000.usb/usb1/1-1':
    KERNELS=="1-1"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{authorized}=="1"
    ATTRS{avoid_reset_quirk}=="0"
    ATTRS{bConfigurationValue}=="1"
    ATTRS{bDeviceClass}=="09"
    ATTRS{bDeviceProtocol}=="02"
    ATTRS{bDeviceSubClass}=="00"
    ATTRS{bMaxPacketSize0}=="64"
    ATTRS{bMaxPower}=="2mA"
    ATTRS{bNumConfigurations}=="1"
    ATTRS{bNumInterfaces}==" 1"
    ATTRS{bcdDevice}=="0bb3"
    ATTRS{bmAttributes}=="e0"
    ATTRS{busnum}=="1"
    ATTRS{configuration}==""
    ATTRS{devnum}=="2"
    ATTRS{devpath}=="1"
    ATTRS{idProduct}=="2514"
    ATTRS{idVendor}=="0424"
    ATTRS{ltm_capable}=="no"
    ATTRS{maxchild}=="4"
    ATTRS{quirks}=="0x0"
    ATTRS{removable}=="unknown"
    ATTRS{speed}=="480"
    ATTRS{urbnum}=="85"
    ATTRS{version}==" 2.00"

  looking at parent device '/devices/platform/soc/3f980000.usb/usb1':
    KERNELS=="usb1"
    SUBSYSTEMS=="usb"
    DRIVERS=="usb"
    ATTRS{authorized}=="1"
    ATTRS{authorized_default}=="1"
    ATTRS{avoid_reset_quirk}=="0"
    ATTRS{bConfigurationValue}=="1"
    ATTRS{bDeviceClass}=="09"
    ATTRS{bDeviceProtocol}=="01"
    ATTRS{bDeviceSubClass}=="00"
    ATTRS{bMaxPacketSize0}=="64"
    ATTRS{bMaxPower}=="0mA"
    ATTRS{bNumConfigurations}=="1"
    ATTRS{bNumInterfaces}==" 1"
    ATTRS{bcdDevice}=="0414"
    ATTRS{bmAttributes}=="e0"
    ATTRS{busnum}=="1"
    ATTRS{configuration}==""
    ATTRS{devnum}=="1"
    ATTRS{devpath}=="0"
    ATTRS{idProduct}=="0002"
    ATTRS{idVendor}=="1d6b"
    ATTRS{interface_authorized_default}=="1"
    ATTRS{ltm_capable}=="no"
    ATTRS{manufacturer}=="Linux 4.14.71-v7+ dwc_otg_hcd"
    ATTRS{maxchild}=="1"
    ATTRS{product}=="DWC OTG Controller"
    ATTRS{quirks}=="0x0"
    ATTRS{removable}=="unknown"
    ATTRS{serial}=="3f980000.usb"
    ATTRS{speed}=="480"
    ATTRS{urbnum}=="31"
    ATTRS{version}==" 2.00"

  looking at parent device '/devices/platform/soc/3f980000.usb':
    KERNELS=="3f980000.usb"
    SUBSYSTEMS=="platform"
    DRIVERS=="dwc_otg"
    ATTRS{busconnected}=="Bus Connected = 0x1"
    ATTRS{buspower}=="Bus Power = 0x1"
    ATTRS{bussuspend}=="Bus Suspend = 0x0"
    ATTRS{devspeed}=="Device Speed = 0x0"
    ATTRS{driver_override}=="(null)"
    ATTRS{enumspeed}=="Device Enumeration Speed = 0x1"
    ATTRS{fr_interval}=="Frame Interval = 0x1d4b"
    ATTRS{ggpio}=="GGPIO = 0x00000000"
    ATTRS{gnptxfsiz}=="GNPTXFSIZ = 0x01000306"
    ATTRS{gotgctl}=="GOTGCTL = 0x001c0000"
    ATTRS{gpvndctl}=="GPVNDCTL = 0x00000000"
    ATTRS{grxfsiz}=="GRXFSIZ = 0x00000306"
    ATTRS{gsnpsid}=="GSNPSID = 0x4f54280a"
    ATTRS{guid}=="GUID = 0x2708a000"
    ATTRS{gusbcfg}=="GUSBCFG = 0x20001700"
    ATTRS{hcd_frrem}=="HCD Dump Frame Remaining"
    ATTRS{hcddump}=="HCD Dump"
    ATTRS{hnp}=="HstNegScs = 0x0"
    ATTRS{hnpcapable}=="HNPCapable = 0x1"
    ATTRS{hprt0}=="HPRT0 = 0x00001005"
    ATTRS{hptxfsiz}=="HPTXFSIZ = 0x02000406"
    ATTRS{hsic_connect}=="HSIC Connect = 0x1"
    ATTRS{inv_sel_hsic}=="Invert Select HSIC = 0x0"
    ATTRS{mode}=="Mode = 0x1"
    ATTRS{mode_ch_tim_en}=="Mode Change Ready Timer Enable = 0x0"
    ATTRS{rd_reg_test}=="Time to read GNPTXFSIZ reg 10000000 times: 1720 msecs (172 jiffies)"
    ATTRS{regdump}=="Register Dump"
    ATTRS{regoffset}=="0xffffffff"
    ATTRS{regvalue}=="invalid offset"
    ATTRS{rem_wakeup_pwrdn}==""
    ATTRS{remote_wakeup}=="Remote Wakeup Sig = 0 Enabled = 0 LPM Remote Wakeup = 0"
    ATTRS{spramdump}=="SPRAM Dump"
    ATTRS{srp}=="SesReqScs = 0x0"
    ATTRS{srpcapable}=="SRPCapable = 0x1"
    ATTRS{wr_reg_test}=="Time to write GNPTXFSIZ reg 10000000 times: 310 msecs (31 jiffies)"

  looking at parent device '/devices/platform/soc':
    KERNELS=="soc"
    SUBSYSTEMS=="platform"
    DRIVERS==""
    ATTRS{driver_override}=="(null)"

  looking at parent device '/devices/platform':
    KERNELS=="platform"
    SUBSYSTEMS==""
    DRIVERS==""


jonnor@display0:~ $ ls -la /lib/udev/rules.d/*mm-*
-rw-r--r-- 1 root root   484 Nov 16  2016 /lib/udev/rules.d/77-mm-cinterion-port-types.rules
-rw-r--r-- 1 root root   646 Nov 16  2016 /lib/udev/rules.d/77-mm-dell-port-types.rules
-rw-r--r-- 1 root root  6910 Nov 16  2016 /lib/udev/rules.d/77-mm-ericsson-mbm.rules
-rw-r--r-- 1 root root   577 Nov 16  2016 /lib/udev/rules.d/77-mm-haier-port-types.rules
-rw-r--r-- 1 root root  1734 Nov 16  2016 /lib/udev/rules.d/77-mm-huawei-net-port-types.rules
-rw-r--r-- 1 root root 13187 Nov 16  2016 /lib/udev/rules.d/77-mm-longcheer-port-types.rules
-rw-r--r-- 1 root root  2869 Nov 16  2016 /lib/udev/rules.d/77-mm-mtk-port-types.rules
-rw-r--r-- 1 root root  2024 Nov 16  2016 /lib/udev/rules.d/77-mm-nokia-port-types.rules
-rw-r--r-- 1 root root   383 Nov 16  2016 /lib/udev/rules.d/77-mm-pcmcia-device-blacklist.rules
-rw-r--r-- 1 root root   514 Nov 16  2016 /lib/udev/rules.d/77-mm-platform-serial-whitelist.rules
-rw-r--r-- 1 root root  3155 Nov 16  2016 /lib/udev/rules.d/77-mm-qdl-device-blacklist.rules
-rw-r--r-- 1 root root  1840 Nov 16  2016 /lib/udev/rules.d/77-mm-simtech-port-types.rules
-rw-r--r-- 1 root root  3268 Nov 16  2016 /lib/udev/rules.d/77-mm-telit-port-types.rules
-rw-r--r-- 1 root root  7245 Nov 16  2016 /lib/udev/rules.d/77-mm-usb-device-blacklist.rules
-rw-r--r-- 1 root root  2452 Nov 16  2016 /lib/udev/rules.d/77-mm-usb-serial-adapters-greylist.rules
-rw-r--r-- 1 root root  3666 Nov 16  2016 /lib/udev/rules.d/77-mm-x22x-port-types.rules
-rw-r--r-- 1 root root 14350 Nov 16  2016 /lib/udev/rules.d/77-mm-zte-port-types.rules
-rw-r--r-- 1 root root   972 Nov 16  2016 /lib/udev/rules.d/80-mm-candidate.rules


