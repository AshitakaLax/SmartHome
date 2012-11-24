import abc


__all__ = []

def _export(clsorfunc):
    from _renamemodules import dorename
    if dorename:
        clsorfunc.__module__ = __package__
    __all__.append(clsorfunc.__name__)
    return clsorfunc


@_export
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
