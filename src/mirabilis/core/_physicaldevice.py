import abc

from ._rename import renamemodule
from ._smarthomeitem import SmartHomeItem


__all__ = ["PhysicalDevice"]


# enforces that add_state_entity can only be called during execution of 
# __init__
@renamemodule
class PhysicalDeviceClass(abc.ABCMeta):
    """metaclass for class PhysicalDevice"""
    
    def __call__(cls, *args, **kwargs):
        instance = cls.__new__(cls, *args, **kwargs)
        instance.__init__(*args, **kwargs)
        instance._initialized = True
        return instance


@renamemodule
class PhysicalDevice(SmartHomeItem):
    """
    PhysicalDevice (abstract class)
    
    A device is something that can be directly communicated with, which may
    monitor and/or control one or more state entities.
    
    Public instance attributes:
    
    (inherited from class SmartHomeItem)
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
        assert isinstance(universe, Universe)
        SmartHomeItem.__init__(self, universe, localname, local_id)
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
        assert isinstance(state_entity, StateEntity)
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
        SmartHomeItem.finalize(self)
        
    def update_entities(self):
        """
        obj.update_entities()
        
        when the universe calls this method, the device must update the status
        of its entities
        """
        raise NotImplementedError()
    

from ._universe import Universe
from ._state_entity import StateEntity
