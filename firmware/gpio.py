## GPIO using generic Linux sysfs 

def file_path(number: int):
    return '/sys/class/gpio/gpio{}/value'.format(number)

def setup_pin(number : int, direction):
    """Enable and configure a single GPIO pin.

    Idempotent, safe to call multiple times."""

    export_path = '/sys/class/gpio/export'
    direction_path = '/sys/class/gpio/gpio{}/direction'.format(number)

    # Export GPIO
    if not os.path.exists(direction_path):
        with open(export_path, 'w') as exportfile:
            exportfile.write(str(number))
        # wait for export done, otherwise direction change fails EACCESS
        time.sleep(0.1)
    
    # Set direction
    with open(direction_path, 'w') as directionfile:
        directionfile.write(direction)


def read_boolean(path : str) -> bool:
    with open(path) as f:
        s = f.read().strip() 
        return s == '1'

def write_boolean(path : str, on : bool):
    s = "1" if on else "0"
    with open(path, 'w') as f:
        f.write(s)

class GPIO:
    def __init__(self, inputs={}, outputs={}):
        self.pinmap = pinmap

        for name, pin in inputs.items():
            setup_pin(pin, 'in')

        for name, pin in output.items():
            setup_pin(pin, 'out')

    def pin_path(self, name: str) -> str:
        return file_path(self.pinmap[name][1])

    def get(self, name : str) -> bool:
        return read_boolean(self.pin_path(name))
    def set(self, name : str, on : bool):
        return write_boolean(self.pin_path(name), on)

    def get_all(self):
        return { name: self.get(name) for name in self.inputs.keys() }
    def set_all(self, mapping):
        for name, value in mapping.items():
            self.set(name, value)

