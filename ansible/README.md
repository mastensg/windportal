hub
---

initial run as root:

    # add hub's host key to known_hosts
    ssh root@windportal.dept.no true

    ansible-playbook hub.yml -k -u root

after the first run:

    ansible-playbook hub.yml
