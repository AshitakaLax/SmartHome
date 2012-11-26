from ._smarthomeitem import SmartHomeItem
from ._container import Container
from ._rename import renamemodule


__all__ = ["Area", "Group"]


@renamemodule
class Group(SmartHomeItem, Container):
    """
    Group(universe[, description[, localname[, local_id]]]) -> obj
    
    A Group object can only hold other groups and state entities.
    
    It can also be directly contained in another container.
    
    universe: (Universe) the universe where the object is located
    description: (str on <None>) a description of the group
    localname: (str or <None>) the local name of the group in its container
                               -- optional (default: None)
    local_id: (int or <None>) the local ID number of the group in its 
                              container -- optional (default: None)
    
    Public instance attributes:
    
    (inherited from class SmartHomeItem)
    """
    
    def __init__(self, universe, description=None, 
                       localname=None, local_id=None):
        SmartHomeItem.__init__(self, universe, description, 
                                     localname, local_id)
        Container.__init__(self)
    
    def additem(self, item, *args, **kwargs):
        assert isinstance(item, (Group, StateEntity)), \
            "Group instance can only hold other groups and state entities"
        return Container.additem(self, item, *args, **kwargs)
            


@renamemodule
class Area(SmartHomeItem, Container):
    """
    Area(universe[, description[, localname[, local_id]]]) -> obj
    
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
    
    Public instance attributes:
    
    (inherited from class SmartHomeItem)
    """
    
    def __init__(self, universe, description=None, 
                       localname=None, local_id=None):
        SmartHomeItem.__init__(self, universe, description, 
                                     localname, local_id)
        Container.__init__(self)


from ._state_entity import StateEntity