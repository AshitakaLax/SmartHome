from .smarthomeitem import SmartHomeItem
from .container import Container


__all__ = []

def _export(clsorfunc):
    __all__.append(clsorfunc.__name__)
    return clsorfunc


@_export
class Area(SmartHomeItem, Container):
    """
    Area(universe[, description[, localname[, local_id]]]) -> obj
    
    An Area object can hold other areas, physical devices, and state entities.
    
    It can also be directly contained in another area or as a root item in a
    universe.
    
    universe: (Universe) the universe where the object is located
    description: (str on <None>)
    localname: (str or <None>) the local name of the area in its container
                               -- optional (default: None)
    local_id: (int or <None>) the local ID number of the area in its container
                             -- optional (default: None)
    
    Public instance attributes:
    
    (inherited from class SmartHomeItem)
    """
    
    def __init__(self, universe, description=None, 
                       localname=None, local_id=None):
        SmartHomeItem.__init__(self, universe, description, 
                                     localname, local_id)
        Container.__init__(self)


