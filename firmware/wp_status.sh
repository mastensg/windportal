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
    lsusb -d 0000:0000 >/dev/null
}

# TODO(mastensg):
modem_link_is_up() {
    # ip link ?
    # ip address ?
    # mmcli ?

    false
}

hub_is_connected() {
    systemctl is-active --quiet ssh-call-home
}

all_services_are_running() {
    ! systemctl is-failed --quiet '*'
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

    printf "%-25s %s\n" "${status_name}" "${status}"
}

output_mqtt() {
    local status_name="${1}"
    local status="${2}"

    printf "%-25s %s\n" "${status_name}" "${status}"
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
