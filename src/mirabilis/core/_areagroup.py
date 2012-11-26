from ._universe import Universe
from ._smarthomeitem import SmartHomeItem
from ._container import Container
from ._rename import renamemodule


__all__ = ["Area", "Group"]


@renamemodule
class Group(SmartHomeItem, Container):
    """
    Group([universe[, description[, localname[, local_id[, container]]]]]) 
        -> obj
    
    A Group object can only hold other groups and state entities.
    
    It can also be directly contained in another container.
    
    universe: (Universe or <None>) the universe where the object is located
    description: (str on <None>) a description of the group
    localname: (str or <None>) the local name of the group in its container
                               -- optional (default: None)
    local_id: (int or <None>) the local ID number of the group in its 
                              container -- optional (default: None)
    container: (Container or <None>) where to add the Group instance after 
                                     creating it
    
    Either universe or container must be specified.
    
    Public instance attributes:
    
    (inherited from class SmartHomeItem)
    """
    
    def __init__(self, universe=None, description=None, 
                       localname=None, local_id=None, container=None):
        SmartHomeItem.__init__(self, universe, description, 
                                     localname, local_id, container)
        Container.__init__(self)
    
    def additem(self, item, *args, **kwargs):
        assert isinstance(item, (Group, StateEntity), \
            "A Group instance can only hold other groups and state entities"
        return Container.additem(item, *args, **kwargs)


@renamemodule
class Area(SmartHomeItem, Container):
    """
    Area([universe[, description[, localname[, local_id[, container]]]]]) 
        -> obj
    
    An Area object can hold other instances of Area, Group, PhysicalDevices, 
    and StateEntity.
    
    It can also be directly contained in another area (but not in a group) 
    or as a root item in its universe.
    
    universe: (Universe) the universe where the object is located
    description: (str on <None>) a description of the group
    localname: (str or <None>) the local name of the group in its container
                               -- optional (default: None)
    local_id: (int or <None>) the local ID number of the group in its 
                              container -- optional (default: None)
    container: (Universe or Area or <None>) where to add the Area after
                                            creating it
    
    Either universe or container must be specified.
    
    Public instance attributes:
    
    (inherited from class SmartHomeItem)
    """
    
    def __init__(self, universe=None, description=None, 
                       localname=None, local_id=None, container=None):
        assert isinstance(container, (Universe, Area))
        SmartHomeItem.__init__(self, universe, description, 
                                     localname, local_id, container)
        Container.__init__(self)


from ._state_entity import StateEntity