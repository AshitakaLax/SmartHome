import abc

__all__ = ["SmartHomeItemInterface"]

class SmartHomeItemInterface(object):
    """
    UniverseObjectInterface (abstract class)
    """
    
    @property
    @abc.abstractmethod
    def universe(self):
        """universe: (Universe) the universe where this object belongs"""
    
    @property
    @abc.abstractmethod
    def global_id(self):
        """
        global_id: (int or <None>) the global ID of this object in its universe
        """
