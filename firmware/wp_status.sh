#!/bin/sh -eu

# Read the status of different subsystems, and output them to one of some
# output types.
#
# Examples:
#
#   ./wp_status.sh          # print to standard output
#   ./wp_status.sh led      # turn status leds on or off
#   ./wp_status.sh mqtt     # send mqtt messages

##############################################################################

# Status readings

arduino_is_attached() {
    lsusb -d 2341:8036 >/dev/null
}

# TODO(mastensg): VID & PID for modem
modem_is_attached() {
    lsusb -d 12d1:1506 >/dev/null
}

modem_link_is_up() {
    mmcli -m 0 | grep '  state:' | grep -q "'connected'$"
}

hub_is_connected() {
    systemctl is-active --quiet ssh-call-home
}

all_services_are_running() {
    ! systemctl is-failed --quiet '*'
}

##############################################################################

# LED control

LED_SLEEP_SECONDS="0.1"

led_enable() {
	local pin="${1}"

	if [ ! -e "/sys/class/gpio/gpio${pin}" ]
	then
		echo "${pin}" > /sys/class/gpio/export
		sleep "${LED_SLEEP_SECONDS}"
	fi

	echo "out" > "/sys/class/gpio/gpio${pin}/direction"
	sleep "${LED_SLEEP_SECONDS}"
}

led_off() {
	local pin="${1}"

	echo "0" > "/sys/class/gpio/gpio${pin}/value"
	sleep "${LED_SLEEP_SECONDS}"
}

led_on() {
	local pin="${1}"

	echo "1" > "/sys/class/gpio/gpio${pin}/value"
	sleep "${LED_SLEEP_SECONDS}"
}

##############################################################################

# Status outputs

output_text() {
    local status_name="${1}"
    local status="${2}"

    printf "%-25s %s\n" "${status_name}" "${status}"
}

output_led() {
    local status_name="${1}"
    local status="${2}"

    # -     Fan connected
    # -     Arduino power
    # -     Raspberry Pi power
    # G10   Modem attached
    # G9    Modem link up
    # G11   Arduino attached
    # G5    Hub connected
    # G6    All services running

    case "${status_name}" in
        ("arduino_is_attached")
            local led_pin="10"
            ;;
        ("modem_is_attached")
            local led_pin="9"
            ;;
        ("modem_link_is_up")
            local led_pin="11"
            ;;
        ("hub_is_connected")
            local led_pin="5"
            ;;
        ("all_services_are_running")
            local led_pin="6"
            ;;
    esac

    led_enable "${led_pin}"

    case "${status}" in
        ("true")
            led_on "${led_pin}"
            ;;
        ("false")
            led_off "${led_pin}"
            ;;
    esac
}

output_mqtt() {
    local status_name="${1}"
    local status="${2}"

    true
}

##############################################################################

main() {
    local output=${1:-text}

    for status_name in \
        arduino_is_attached \
        modem_is_attached \
        modem_link_is_up \
        hub_is_connected \
        all_services_are_running
    do
        local status="false"
        ${status_name} && status="true"

        "output_${output}" "${status_name}" "${status}"
    done
}

main "${@}"
