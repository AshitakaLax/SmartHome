import abc

from ._interfaces import SmartHomeItemInterface
from ._smarthomeitem import SmartHomeItem


__all__ = []

def _export(thing):
    __all__.append(thing.__name__)
    return thing


@_export
class StateEntity(SmartHomeItem):
    """
    StateEntity (abstract class)

    An entity is some kind of thing controlled by or associated with a device

    Public instance attributes:
    
    description: (str or <None>) a description of the state entity
    """

    def __init__(self, description=None, localname=None, local_id=None):
        """
        obj.__init__([description[, localname[, local_id]]]) -> obj
    
        description: (str or <None>) a description of the state entity
        localname: (str or <None>) the name of the entity 
                                   -- optional (defaults to None)
        local_id: (int or <None>) the local ID for the entity -- optional
                                 (defaults to None)
        """
        SmartHomeItem.__init__(self, None, localname, local_id)  # no universe
        self.description = description
        self._device = None  # written to by PhysicalDevice objects
    
    @property
    def device(self):
        """
        device: (PhysicalDevice or <None>) the device associated with this 
                                           entity
        
        read-only property
        """
        return self._device
    
    def readytoquituniverse(self):
        """
        obj.readytoquituniverse() -> (bool)
        
        return whether the object is ready to be ejected from the universe
        """
        return not self._device and SmartHomeItem.readytoquituniverse(self)    


class RStateEntityBase(SmartHomeItemInterface):
    """
    RStateEntity (abstract class)
    
    a readable state entity
    """
    
    UNINITIALIZED = object()
    
    def __init__(self):
        self._state = RStateEntity.UNINITIALIZED
    
    def _update(self, newstate):
        """
        obj._update(newstate)
        
        record a new value observed by the device
        
        newstate: the newest value to be recorded
        """
        (laststate, self._state) = (self._state, newstate)
        if newstate != laststate:
            event = StateChangedEvent(self, newstate, laststate)
            self.universe._postevent(event)
    
    def read(self):
        """
        obj.read() -> most current state value
        """
        return self._state
    
    
class WStateEntityBase(SmartHomeItemInterface):
    """
    WStateEntityBase (abstract class)
    
    a writable state entity
    """
    
    __metaclass__ = abc.ABCMeta
    
    NEVER_WRITTEN = object()
    
    def __init__(self, writerfunc):
        """
        obj.__init__(writerfunc)
        
        writerfunc: (function) the function, accepting as an argument this 
                               state entity and the new value, that will assume
                               responsibility for carrying out a state setting 
                               operation
        """
        self._writerfunc = writerfunc
        self._lastwrite = WStateEntityBase.NEVER_WRITTEN
    
    class NoPriorWritesError(Exception):
        pass
    
    @property
    def lastwrite(self):
        if self._lastwrite is WStateEntityBase._NEVER_WRITTEN:
            errmsg = "can't access this property until a value has been " \
                     "written using write()"
            raise WStateEntityBase.NoPriorWritesError(errmsg)
        return self._lastwrite
    
    def write(self, newvalue):
        """
        obj.write(newvalue)
        
        try to set a new value of newvalue on something
        """
        self._writerfunc(self, newvalue)
        (oldwrite, self._lastwrite) = (self._lastwrite, newvalue)
        # post event, regardless of whether old and new writes are equal values
        self.universe._postevent(ValueWrittenEvent(self, newvalue, oldwrite))
    
    def rewrite(self):
        """
        obj.rewrite()
        
        re-write the last value written another time
        """
        self.write(self.lastwrite)


@_export
class RStateEntity(StateEntity, RStateEntityBase):
    """
    RStateEntity([description[, localname[, local_id]]]) -> obj
    
    A readable state entity.
    
    description: (str or <None>) a description of the state entity
    localname: (str or <None>) the name of the entity 
                               -- optional (defaults to None)
    local_id: (int or <None>) the local ID for the entity -- optional
                             (defaults to None)
    """
    
    def __init__(self, description=None, localname=None, local_id=None):
        StateEntity.__init__(self, description, localname, local_id)
        RStateEntityBase.__init__(self)


@_export
class WStateEntity(StateEntity, WStateEntityBase):
    """
    WStateEntity(writerfunc[, description[, localname[, local_id]]]) -> obj
    
    A writable state entity.
    
    writerfunc: (function) the function to call with the entity when a change
                           needs to be written out; it takes as arguments the
                           state entity first and the value to set second
    description: (str or <None>) a description of the state entity
    localname: (str or <None>) the name of the entity 
                               -- optional (defaults to None)
    local_id: (int or <None>) the local ID for the entity -- optional
                             (defaults to None)
    """
    
    def __init__(self, writerfunc, description=None, 
                 localname=None, local_id=None):
        StateEntity.__init__(self, description, localname, local_id)
        WStateEntityBase.__init__(self, writerfunc)


@_export
class RWStateEntity(StateEntity, RStateEntityBase, WStateEntityBase):
    """
    RWStateEntity(writerfunc[, description[, localname[, local_id]]]) -> obj
    
    A readable and writable state entity.
    
    writerfunc: (function) the function to call with the entity when a change
                           needs to be written out; it takes as arguments the
                           state entity first and the value to set second
    description: (str or <None>) a description of the state entity
    localname: (str or <None>) the name of the entity 
                               -- optional (defaults to None)
    local_id: (int or <None>) the local ID for the entity -- optional
                             (defaults to None)
    """
    
    def __init__(self, writerfunc, description=None, 
                 localname=None, local_id=None):
        StateEntity.__init__(self, description, localname, local_id)
        RStateEntityBase.__init__(self)
        WStateEntityBase.__init__(self, writerfunc)
