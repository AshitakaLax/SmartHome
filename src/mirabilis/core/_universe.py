import threading

from ._lock import Lock
from ._rename import renamemodule
from ._container import Container


__all__ = ["Universe"]


@renamemodule
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
    # _objects_by_global_id: (dict of SmartHomeItem[int]) 
    #                        dictionary of ALL smart home objects by ID
    # _eventhandlers: (dict of set[tuple])
    #     Each value is a list of functions.
    #     The functions take a single parameter, which is the event.
    #     The tuple has as its first item the 
    
    def __init__(self):
        Container.__init__(self)
        
        self._init_object_tracking()
        self._initdevicetracking()
        self._initeventdispatching()
    
        self._readerthreads = set()
        self._modifylock = threading.Condition()
        self._writerthread = None
    
    class _ReadLockContextManager(object):
        def __init__(self, universe):
            self._universe = universe
            
        def __enter__(self):
            self._universe._lockread()
        
        def __exit__(self, *args):
            self._universe._releaseread()
    
    class _WriteLockContextManager(object):
        def __init__(self, universe):
            self._universe = universe
        
        def __enter__(self):
            self._universe._lockwrite()
        
        def __exit__(self, *args):
            self._universe._releasewrite()
    
    @property
    def readlock(self):
        """
        readlock: (context manager) use this with a "with" statement
        """
        return Universe._ReadLockContextManager(self)
    
    @property
    def writelock(self):
        """
        writelock: (context manager) use this with a "with" statement
        """
        return Universe._WriteLockContextManager(self)
    
    # locking
    def _lockread(self):
        with self._modifylock:
            while self.writerthread:
                self._modifylock.wait()
            self._readerthreads.add(threading.current_thread())
    
    def _releaseread(self):
        with self._modifylock:
            assert not self._writerthread
            assert threading.current_thread() in self._readerthreads
            self._readerthreads.remove(threading.current_thread())
            self._modifylock.notify_all()
    
    def _lockwrite(self):
        with self._modifylock:
            while self._readerthreads:
                self._modifylock.wait()
            self._writerthread = threading.current_thread()
    
    def _releasewrite(self):
        with self._modifylock:
            assert not self._readerthreads
            assert self._writerthread == threading.current_thread()
            self._writerthread = None
            self._modifylock.notify_all()
            
    ###########################
    # OBJECT TRACKING SECTION #
    ###########################
    
    def _init_object_tracking(self):
        self._next_global_id = 1  # 0 is reserved for future use
        self._objects_by_global_id = {}
        
        
    @property
    def objectsdict(self):
        """
        objectsdict: (dict of SmartHomeItem[int]) items indexed by global ID
        
        (read only)
        """
        return self._objects_by_global_id.copy()
    
    @property
    def devices(self):
        """
        devices: (set) the devices existing in this universe
        
        (read only)
        """
        return self._devices.copy()
    
    def object_with_global_id(self, global_id):
        """
        obj.object_with_global_id(global_id) -> (SmartHomeItem)
        
        return the item with the specified global id
        
        global_id: (int) the global ID of the item in question 
        """
        return self._objects_by_global_id[global_id]
    
    def _registeritem(self, containable):
        """
        obj._registeritem(containable)
    
        assign a unique ID to containable without making it a root object or
        assigning to its _container attribute
    
        containable: (SmartHomeItem) the containable object
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
        
        containable: (SmartHomeItem) the thing to delete from the universe
        """
        assert containable.readytoquituniverse(), \
            "containable not ready to be removed; try removing the thing " \
            "that manages it, such as its device"
        self._removeitemfromuniverse(containable)
    
    def _removeitemfromuniverse(self, containable):
        assert isinstance(containable, SmartHomeItem)
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
        self._event_threads = []
        
    def registereventhandler(self, dispatchtuple, function):
        """
        obj.registereventhandler(dispatchtuple, function)
        """
        self._eventhandlers.setdefault(dispatchtuple, set()).add(function)
    
    def postevent(self, event):
        """
        obj.postevent(event)
        
        event: (Event)
        """
        for func in self._eventhandlers.get(event.dispatchtuple, []):
            threadnum = len(self._event_threads) + 1
            thread = threading.Thread(None, 
                                      func, 
                                      "eventhandler{}".format(threadnum), 
                                      (event,))
            self._event_threads.append(thread)
            thread.start()
    
    # for pickle serialization, remove unpicklable objects
    def __getstate__(self):
        odict = self.__dict__.copy()
        del odict["_event_threads"]
        return odict
    
    # for pickle serialization, remove unpicklable objects
    def __setstate__(self, odict):
        self.__dict__.update(odict)
        self._event_threads = None
    

from ._physicaldevice import PhysicalDevice
