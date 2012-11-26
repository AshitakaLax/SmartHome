from ._rename import renamemodule
from ._interfaces import SmartHomeItemInterface


__all__ = ["SmartHomeItem"]


@renamemodule
class SmartHomeItem(SmartHomeItemInterface):
    """
    SmartHomeItem (abstract class)
    
    root class for Area, PhysicalDevice, and StateEntity
    
    Instances of this class are objects that can be located inside the 
    boundaries of a Container object.
    
    Public instance attributes:
    
    description: (str or <None>) a description of this item
    localname: (str or <None>) the preferred local name of the object in its 
                               container
    local_id: (int or <None>) the preferred local ID of the object in its 
                              container
    """
    
    # private attributes:
    # 
    # _universe: (Universe or <None>) the universe containing this object
    # _global_id: (int or <None>) the global ID for this object
    # _container: (Container or <None>) the container of this object 
    
    def __init__(self, universe=None, description=None,
                       localname=None, local_id=None, container=None):
        """
        obj.__init__(universe
                     [, description
                     [, localname
                     [, local_id
                     [, container]]]])
        
        universe: (Universe or <None>) the universe where this is being created
        description: (str or <None>) a description of the object -- 
                                     optional (default: None)
        localname: (str or <None>) the preferred local name of the object in 
                                   its container
        local_id: (int or <None>) the preferred local ID of the object in its 
                                  container
        container: (Area or Group) where to add the item after creating it;
                                   if universe is None, then universe will be
                                   taken from container.universe
        """
        self._universe = None  # written to by Universe object
        self._global_id = None  # written to by Universe object
        self._container = None  # written to by Container objects
        self.description = description
        self.localname = localname
        self.local_id = local_id
        
        
        assert isinstance(universe, (Universe, type(None)))
        if container and not universe:
            if isinstance(container, SmartHomeItem):
                universe = container.universe
            elif isinstance(container, Universe):
                universe = container
            else:
                raise AssertionError("not reached")
        if universe:
            universe._registeritem(self)
            if container:
                assert isinstance(container, (Area, Group))
        if container:
            container.additem(self, localname=localname, local_id=local_id)
                
    def __repr__(self):
        fmtstr = "<{0.__class__.__module__}.{0.__class__.__name__} " \
                 "gloabl_id:{0.global_id} description:{0.description!r}>"
        return fmtstr.format(self)
    
    @property
    def universe(self):
        """
        universe: (Universe) the universe for this object
        
        read-only property
        """
        return self._universe

    @property
    def global_id(self):
        """
        global_id: (int or <None>) the universe-wide ID for this item 
        
        read-only property
        """
        return self._global_id

    @property
    def path(self):
        """
        path: (str) a slash-separated path of the names leading up to this
                    item
        
        read-only property
        """
        c = self
        parents = []
        while isinstance(c, SmartHomeItem):
            parents.append(c.localname or c.local_id or "<unnamed>")
            c = c.container
        if isinstance(c, Universe):
            front = "/"
        elif c is None:
            front = "<floating>/"
        else:
            raise AssertionError()  # not reached
        return front + "/".join(x for x in reversed(parents))
    
    @property
    def container(self):
        """
        container: (Container or <None>) whatever is containing this object
        
        read-only property
        """
        return self._container
    
    @property
    def keys(self):
        """
        keys: (set) the set of keys used to locally identify this item
        
        read-only property
        """
        assert self.container, "the item is not in any Container object"
        return self.container.getitemkeys(self)
    
    def updatekeys(self, newkeys):
        """
        obj.updatekeys(newkeys)
        
        Reset the keys of this item through its container.
        newkeys will be automatically supplemented with the name and local_id 
        properties if they are not None.
        
        newkeys: (iterable) the new keys to use, in addition to localname and 
                            local_id
        """
        container = self.container
        assert container, "the item is not in any Container object"
        container.removeitem(self)
        newkeys = set(newkeys)
        if self.name is not None:
            newkeys.add(self.name)
        if self.local_id is not None:
            newkeys.add(self.local_id)
        container.additem(self, newkeys)
    
    def readytoquituniverse(self):
        """
        obj.readytoquituniverse() -> (bool)
        
        return whether the item is ready to be removed from the universe
        """
        return not self._container
    
    def finalize(self):
        """
        obj.finalize()
        
        called before ejecting an item from the unvirse
        
        THIS METHOD SHOULD ONLY BE CALLED BY METHODS OF CLASS Universe!
        """
        # don't assume this method will always be called; it might be 
        # optimized away
        assert self.readytoquituniverse()
    
    def quituniverse(self):
        """
        obj.quituniverse()
        
        remove the item from the universe
        """
        self.universe._ejectitem(self)
    
    def removefromcontainer(self):
        """
        obj.removefromconainer()
        
        removes the item from its current container
        
        raises an exception if it's not in any container
        """
        assert self.container
        self.container.removeitem(self)
    
    def move(self, newholder, newlocalname=None, new_local_id=None):
        """
        obj.move(newholder)
        
        moves the item to a new container
        
        newholder: (Container or <None>)
        """
        assert isinstance(newholder, (Container, type(None)))
        if self.container:
            self.removefromcontainer()
        if newholder:
            newholder.additem(self, newlocalname=newlocalname, 
                                    new_local_id=new_local_id)


from ._areagroup import Area, Group
from ._universe import Universe
