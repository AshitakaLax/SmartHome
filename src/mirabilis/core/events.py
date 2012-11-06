from datetime import datetime


__all__ = []

def _export(thing):
    __all__.append(thing.__name__)
    return thing


@_export
class Event(object):
    """
    Event (abstract class)
    
    Instance attribute:
    
    timestamp: (datetime) the time the event occurred
    """
    def __init__(self, timestamp=None):
        """
        Event.__init__(self[, timestamp])
        
        timestamp: (datetime) when the event happened
        """
        self.timestamp = timestamp or datetime.now()
    
    @property
    def dispatchtuple(self):
        """
        dispatchtuple: (tuple) the tuple that should be used for dispatching 
                               the event
        """
        return (type(self),)
    
    @classmethod
    def makedispatchtuple(cls):
        """
        cls.makedispatchtuple() -> (tuple)
        
        use this to make a dispatch tuple
        """
        

@_export
class StateEntityEvent(Event):
    """
    StateEntityEvent (abstract class)
    
    Instance attribute:
    
    (inherits instance attributes of Event)

    entity: (StateEntity) the entity involved in the event
    """
    def __init__(self, entity, timestamp=None):
        """
        Event.__init__(self[, timestamp])
        
        entity: (StateEntity) the entity involved in the event
        timestamp: (datetime) when the event happened
        """
        Event.__init__(self, timestamp)
        assert isinstance(entity, StateEntity)
        self.entity = entity
    
    @property
    def dispatchtuple(self):
        return (type(self), self.entity)
    
    @classmethod
    def makedispatchtuple(cls, entity):
        """
        cls.makedispatchtuple(entity) -> (tuple)
        
        use this to make a dispatch tuple for an event
        
        entity: (StateEntity) the entity for which the event took place
        """
        assert isinstance(entity, StateEntity)
        return (cls, entity)


@_export
class StateChangedEvent(StateEntityEvent):
    """
    StateChangedEvent(entity, newstate, oldstate[, timestamp]) -> obj
    
    entity: (StateEntity) the entity involved in the event
    newstate: (object) the new state of the entity
    oldstate: (object) the old state of the entity
    timestamp: (datetime) when the event happened
    
    Instance attributes:
    
    (inherits instance attributes of StateEntityEvent)
    
    newstate: (object) the new state of the entity
    oldstate: (object) the previous state of the entity
    """
    def __init__(self, entity, newstate, oldstate, timestamp=None):
        StateEntityEvent.__init__(self, entity, timestamp)
        self.newstate = newstate
        self.oldstate = oldstate


@_export
class ValueWrittenEvent(StateEntityEvent):
    """
    StateChangedEvent(entity, newstate, oldstate[, timestamp]) -> obj
    
    entity: (StateEntity) the entity involved in the event
    value: (object) the value written to the entity
    lastvaluewrite: (object) last value written to the entity
    timestamp: (datetime) when the event happened
    
    Instance attributes:
    
    (inherits instance attributes of StateEntityEvent)
    
    value: (object) the new state of the entity
    lastvaluewrite: (object) the previous state of the entity
    """
    def __init__(self, entity, value, lastvaluewrite=None, timestamp=None):
        StateEntityEvent.__init__(self, entity, timestamp)
        self.value = value
        self.lastvaluewrite = lastvaluewrite


from .state_entity import StateEntity
