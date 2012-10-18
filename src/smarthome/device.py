
class PhysicalDevice(object):
    def __init__(self, name):
        self.name = name

  
class NetworkDevice(PhysicalDevice):
    def __init__(self, name, address):
        super().__init__(name)
        self.address = address


NOT_UPDATED = object()  # sentinel


class StatefulEntity(object):
    """
    StatefulEntity(device) -> obj
    
    device: (PhysicalDevice) the device where the stateful entity exists
    """
    def __init__(self, device):
        assert isinstance(device, PhysicalDevice)
        self.device = device
        self.__state = NOT_UPDATED
    
    def update(self, newstate):
        """
        obj.update(newstate)
        
        newstate: the newest value to be recorded
        """
        self.__state = newstate
    
    def get(self):
        """
        obj.get() -> most current state value
        """
        return self.__state
    
    
class StateMutableEntity(StatefulEntity):
    def set(self, newvalue):
        """
        obj.set(newvalue)
        
        try to set a new value of newvalue on something
        """
        raise NotImplemented
    