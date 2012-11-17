from .container import Container


__all__ = ["Universe"]


class Universe(Container):
    """
    Universe() -> obj
    
    This is the root object containing references to all necessary objects to
    piece together a complete and entire picture of the system.
    
    A Universe object may contain references to Area objects, but it does not
    have a parent/child relationship with them.
    
    Normally, it should be only be necessary to create one Universe object per
    running application.
    """
    
    # Private instance attributes:
    # 
    # _next_global_id: (int) the next ID to be assigned
    # _objects_by_global_id: (dict of Containable[int]) 
    #                        dictionary of ALL smart home objects by ID
    # _eventhandlers: (dict of set[tuple])
    #     Each value is a list of functions.
    #     The functions take a single parameter, which is the event.
    #     The tuple has as its first item the 
    
    def __init__(self):
        Container.__init__(self)
        self._initobjecttracking()
        self._initdevicetracking()
        self._initeventdispatching()
    
    ###########################
    # OBJECT TRACKING SECTION #
    ###########################
    
    def _initobjecttracking(self):
        self._next_global_id = 1  # 0 is reserved for future use
        self._objects_by_global_id = {}
        
        
    @property
    def objects_dict(self):
        """
        objects_dict: (dict of Containable[int]) items indexed by global ID
        """
        return self._objects_by_global_id.copy()
    
    @property
    def devices(self):
        """devices: (set) the devices existing in this universe"""
        return self._devices.copy()
    
    def object_with_global_id(self, global_id):
        """
        obj.object_with_global_id(global_id) -> (Containable)
        
        return the item with the specified global id
        
        global_id: (int) the global ID of the item in question 
        """
        return self._objects_by_global_id[global_id]
    
    def _registeritem(self, containable):
        """
        obj._registeritem(containable)
    
        assign a unique ID to containable without making it a root object or
        assigning to its _container attribute
    
        containable: (Containable) the containable object
        """
        containable._universe = self
        
        # assign global ID and store in dictionary
        assert isinstance(self._next_global_id, int)
        assert containable._global_id == None
        assert self._next_global_id not in self._objects_by_global_id.keys()
        containable._global_id = self._next_global_id
        self._objects_by_global_id[containable._global_id] = containable
        self._next_global_id += 1
    
    def _ejectitem(self, containable):
        """
        obj._ejectitem(containable)
        
        remove an item from the universe
        
        If it still has a container relationship, that will be unlinked.
        
        containable: (Containable) the thing to delete from the universe
        """
        assert containable.readytoquituniverse(), \
            "containable not ready to be removed; try removing the thing " \
            "that manages it, such as its device"
        self._removeitemfromuniverse(containable)
    
    def _removeitemfromuniverse(self, containable):
        assert isinstance(containable, Containable)
        containable.finalize()
        #if containable._container:
        #    containable._container.removeitem(containable)
        #    assert containable._container is None
        del self._objects_by_global_id[containable._global_id]
        containable._global_id = None
        containable._universe = None
    
    ###########################
    # DEVICE TRACKING SECTION #
    ###########################
    
    def _initdevicetracking(self):
        self._devices = set()
    
    def _registerdevice(self, device):
        assert isinstance(device, PhysicalDevice)
        self._devices.add(device)
    
    #############################
    # EVENT DISPATCHING SECTION #
    #############################
    
    def _initeventdispatching(self):
        self._eventhandlers = {}
        
    def _registereventhandler(self, dispatchtuple, function):
        """
        obj._registereventhandler(dispatchtuple, function)
        """
        self._eventhandlers.setdefault(dispatchtuple, set()).add(function)
    
    def _postevent(self, event):
        """
        obj._postevent(event)
        
        event: (Event)
        """
        for func in self._eventhandlers.get(event.dispatchtuple, []):
            func(event)


from .physicaldevice import PhysicalDevice