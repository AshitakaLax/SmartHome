import abc

from ._containable import Containable

# enforces that add_state_entity can only be called during execution of 
# __init__
class PhysicalDeviceClass(abc.ABCMeta):
    """metaclass for class PhysicalDevice"""
    
    def __call__(cls, *args, **kwargs):
        instance = cls.__new__(cls, *args, **kwargs)
        instance.__init__(*args, **kwargs)
        instance._initialized = True
        return instance


class PhysicalDevice(Containable):
    """
    PhysicalDevice (abstract class)
    
    A device is something that can be directly communicated with, which may
    monitor and/or control one or more state entities.
    
    Public instance attributes:
    
    (inherited from class Containable)
    """
    
    __metaclass__ = PhysicalDeviceClass
    
    def __init__(self, universe, localname=None, local_id=None):
        """
        obj.__init__(universe[, localname[, local_id]])
        
        universe: (Universe) the universe where the object is located
        localname: (str or <None>) the local name of the device within its
                                   container
        local_id: (int or <None>) the local ID number of the device within its 
                                  container
        """
        Containable.__init__(self, universe, localname, local_id)
        self._initialized = False  # also written to by metaclass
        self._state_entities = set()
        self.universe._registerdevice(self)
    
    def add_state_entity(self, state_entity):
        """
        obj.add_state_entity(state_entity)
        
        This method can only be called during execution of the __init__ method
        and should only be called by subclasses of class PhysicalDevice.
        """
        assert not self._initialized, \
            "this method can only be called during execution of __init__()"
        self._state_entities.add(state_entity)
        state_entity._device = self
        self.universe._registeritem(state_entity)
    
    def finalize(self):
        """
        obj.finalize()
        
        prepare to be removed from the universe
        """
        for state_entity in self._state_entities:
            state_entity._device = None
            self.universe._removeitemfromuniverse(state_entity)
        self._state_entities = set()
        Containable.finalize(self)
        
    def update_entities(self):
        """
        obj.update_entities()
        
        this method tells the device to update the status of its entities
        """
        raise NotImplementedError()
    
    def _writablestatechanged(self, w_state_entity):
        raise NotImplementedError()

