"""
module core
"""

import abc
from weakref import WeakValueDictionary, ref


__all__ = ["ItemHolder", "Containable", "Universe", "Area", "PhysicalDevice", 
           "StateEntity", "RStateEntity", "WStateEntity", "RWStateEntity",
           "UniverseObject"]


class UniverseObject(object):
    """
    UniverseObject (abstract class)
    
    an item that has a global_id and exists in a universe
    """
    
    def __init__(self, universe):
        self._universe = None  # written to by Universe object
        self._global_id = None  # written to by Universe object    
        
        if universe:
            universe._registeritem(self)
    
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
        global_id: (int or <None>) the universe-wide ID for this containable 
                                   object
        
        read-only property
        """
        return self._global_id

    def readytoleaveuniverse(self):
        """
        obj.readytoleaveuniverse() -> (bool) 
        
        whether or not the object can be ejected from the universe at this time
        """
        return True
    
    def finalize(self):
        """
        obj.finalize()
        
        called before ejecting an item from the unvirse
        
        THIS METHOD SHOULD ONLY BE CALLED BY METHODS OF class Universe!
        """
        # don't assume this method will always be called; it might be 
        # optimized away
        assert self.readytoleaveuniverse()
    
    def leaveuniverse(self):
        """
        obj.leaveuniverse()
        
        remove the item from the universe
        """
        self.universe._ejectitem(self)
    

class ItemHolder(object):
    """
    ItemHolder (abstract class)
 
    a class for things that hold other items
    """

    # Private instance attributes:
    # 
    # _dictofitemstokeys: (dict of frozenset[Containable])
    #                     this stores a cache of the keys into _itemsbykey,
    #                     indexed by the value; ALL items associated with 
    #                     this holder MUST be stored as keys into this 
    #                     dictionary, even if their "local keys" are an empty
    #                     set
    # _itemsbykeycache: (dict of Containable[object]) 
    #                   may pair every item with an object which need only be 
    #                   unique within the _ItemHolder instance; items without 
    #                   any keys will be omitted from this dictionary
        
    # must call this __init__ method to initialize the variables
    def __init__(self):
        self._dictofitemstokeys = {}
        self._itemsbykeycache = {}
    
    def __getitem__(self, index):
        assert not isinstance(index, slice)
        return self._itemsbykeycache[index]
    
    def __contains__(self, containable):
        return containable in self._dictofitemstokeys.keys()
    
    def __iter__(self):
        return iter(self._dictofitemstokeys.keys())
        
    def additem(self, containable, addkeys=frozenset(),
                newlocalname=None, new_local_id=None): # intended as kw-only 
        """
        obj.additem(containable[, keys]
                    [, newlocalname=<value>][, new_local_id=<value>])
        
        add an item to this holder
        
        containable: (Containable) the thing to be added
        addkeys: (iterable) the keys--numeric, string, or other type--for 
                            accessing the containable locally from its holder
        newlocalname: (str or <None>) a new local name to set on containable
        new_local_id: (int or <None>) a new local ID to set on containable
        """
        assert not containable._container
        if newlocalname is not None:
            containable.localname = newlocalname
        if new_local_id is not None:
            containable.local_id = new_local_id
        keys = {x for x in [containable.localname, containable.local_id]
                  if x is not None}
        keys = frozenset(keys.union(addkeys))
        self._dictofitemstokeys[containable] = keys
        for key in keys:
            assert key not in self._itemsbykeycache.keys()
            self._itemsbykeycache[key] = containable
        containable._container = self
    
    def removeitem(self, containable):
        """
        obj.removeitem(containable)
        
        remove an item from the holder
        
        containable: (Containable) the thing to be removed
        """
        for key in self._dictofitemstokeys[containable]:
            del self._itemsbykeycache[key]
        del self._dictofitemstokeys[containable]
        containable._container = None
    
    def getitemkeys(self, containable):
        """
        obj.getitemkeys(containable) -> (set)
        
        get keys for the containable specified
        
        containable: (Containable) what to get the keys for
        """
        assert containable in self._dictofitemstokeys.keys()
        return set(self._dictofitemstokeys[containable])


class Universe(ItemHolder):
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
    
    def __init__(self):
        ItemHolder.__init__(self)
        self._next_global_id = 1  # 0 is reserved for future use
        self._objects_by_global_id = {}
        self._devices = set()
        
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
    
    def objects_with_global_id(self, global_id):
        """
        obj.object_with_global_id(global_id) -> (Containable)
        
        return the item with the specified global id
        
        global_id: (int) the global ID of the item in question 
        """
        return self._objects_by_global_id[global_id]
    
    # obj._registeritem(containable)
    #
    # assign a unique ID to containable without making it a root object or
    # assigning to its _container attribute
    #
    # containable: (Containable) the containable object
    def _registeritem(self, containable):
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
        assert containable.readytoleaveuniverse(), \
            "containable not ready to be removed; try removing the thing " \
            "that manages it, such as its device"
        self.__removeitemfromuniverse(containable)
    
    def __removeitemfromuniverse(self, containable):
        assert isinstance(containable, Containable)
        containable.finalize()
        #if containable._container:
        #    containable._container.removeitem(containable)
        #    assert containable._container is None
        del self._objects_by_global_id[containable._global_id]
        containable._global_id = None
        containable._universe = None
    
    def _registerdevice(self, device):
        assert isinstance(device, PhysicalDevice)
        self._devices.add(device)


class Containable(UniverseObject):
    """
    class Containable (abstract)
    
    root class for Area, PhysicalDevice, and StateEntity
    
    Instances of this class are objects that can be located inside the 
    boundaries of an ItemHolder object.
    
    Public instance attributes:
    
    localname: (str or <None>) the preferred local name of the object in its 
                               container
    local_id: (int or <None>) the preferred local ID of the object in its 
                              container
    """
    
    def __init__(self, universe, localname=None, local_id=None):
        UniverseObject.__init__(self, universe)
        self._container = None  # written to by ItemHolder objects
        self.localname = localname
        self.local_id = local_id
    
    def __repr__(self):
        fmtstr = "<{0.__class__.__module__}.{0.__class__.__name__} " \
                 "gloabl_id:{0.global_id}>"
        return fmtstr.format(self)
    
    @property
    def path(self):
        c = self
        parents = []
        while isinstance(c, Containable):
            parents.append(c.localname or c.local_id or "<unnamed>")
            c = c.container
        if isinstance(c, Universe):
            c = "/"
        elif c is None:
            c = "<floating>/"
        else:
            raise AssertionError() 
        return c + "/".join(x for x in reversed(parents))
    
    @property
    def container(self):
        """
        container: (ItemHolder or <None>) whatever is containing this object
        
        read-only property
        """
        return self._container
    
    @property
    def keys(self):
        """
        keys: (set) the set of keys used to locally identify this item
        
        read-only property
        """
        assert self.container, "the item is not in any ItemHolder object"
        return self.container.getitemkeys(self)
    
    def updatekeys(self, newkeys):
        """
        obj.updatekeys(newkeys)
        
        Reset the keys of this containable object through its container.
        newkeys will be automatically supplemented with the name and local_id 
        properties if they are not None.
        
        newkeys: (iterable) the new keys to use, in addition to localname and 
                            local_id
        """
        container = self.container
        assert container, "the item is not in any ItemHolder object"
        container.removeitem(self)
        newkeys = set(newkeys)
        if self.name is not None:
            newkeys.add(self.name)
        if self.local_id is not None:
            newkeys.add(self.local_id)
        container.additem(self, newkeys)
    
    def readytoleaveuniverse(self):
        """
        obj.readytoleaveuniverse() -> (bool)
        
        return whether the containable is ready to be removed from the universe
        """
        return not self._container and \
            UniverseObject.readytoleaveuniverse(self)
    
    def removefromcontainer(self):
        """
        obj.removefromconainer()
        
        removes the containable from its current container
        
        raises an exception if it's not in any container
        """
        assert self.container
        self.container.removeitem(self)
    
    def move(self, newholder):
        """
        obj.move(newholder)
        
        moves the containable to a new container
        """
        if self.container:
            self.removefromcontainer()
        newholder.additem(self)
        

class Area(Containable, ItemHolder):
    """
    Area(universe[, localname[, local_id]]) -> obj
    
    An Area object can hold other areas, physical devices, and state entities.
    
    It can also be directly contained in another area or as a root item in a
    universe.
    
    universe: (Universe) the universe where the object is located
    localname: (str or <None>) the local name of the area in its container
                               -- optional (default: None)
    local_id: (int or <None>) the local ID number of the area in its container
                             -- optional (default: None)
    
    Public instance attributes:
    
    (inherited from class Containable)
    """
    
    def __init__(self, universe, localname=None, local_id=None):
        Containable.__init__(self, universe, localname, local_id)
        ItemHolder.__init__(self)


# enforces that add_state_entity can only be called during execution of 
# __init__
class PhysicalDeviceClass(abc.ABCMeta):
    """metaclass for class PhysicalDevice"""
    
    def __call__(cls, *args, **kwargs):
        instance = cls.__new__(cls, *args, **kwargs)
        instance.__init__(*args, **kwargs)
        instance._initialized = True
        return instance


class PhysicalDevice(Containable):
    """
    PhysicalDevice (abstract class)
    
    A device is something that can be directly communicated with, which may
    monitor and/or control one or more state entities.
    
    Public instance attributes:
    
    (inherited from class Containable)
    """
    
    __metaclass__ = PhysicalDeviceClass
    
    def __init__(self, universe, localname=None, local_id=None):
        """
        obj.__init__(universe[, localname[, local_id]])
        
        universe: (Universe) the universe where the object is located
        localname: (str or <None>) the local name of the device within its
                                   container
        local_id: (int or <None>) the local ID number of the device within its 
                                  container
        """
        Containable.__init__(self, universe, localname, local_id)
        self._initialized = False  # also written to by metaclass
        self._state_entities = set()
        self.universe._registerdevice(self)
    
    def add_state_entity(self, state_entity):
        """
        obj.add_state_entity(state_entity)
        
        This method can only be called during execution of the __init__ method
        and should only be called by subclasses of class PhysicalDevice.
        """
        assert not self._initialized, \
            "this method can only be called during execution of __init__()"
        self._state_entities.add(state_entity)
        state_entity._device = self
        self.universe._registeritem(state_entity)
    
    def finalize(self):
        """
        obj.finalize()
        
        prepare to be removed from the universe
        """
        for state_entity in self._state_entities:
            state_entity._device = None
            self.universe._removeitemfromuniverse(state_entity)
        self._state_entities = set()
        Containable.finalize(self)
        
    
    ##@abc.abstractmethod
    def update_entities(self):
        """
        obj.update_entities()
        
        this method tells the device to update the status of its entities
        """
        raise NotImplementedError()
    
    # obj._writeablestatechanged(w_state_entity)
    # 
    # this method should be called from WStateEntity to propogate the changed
    # from the state entity to the actual device
    # 
    # w_state_entity: (WStateEntity) the entity with the changed state
    ##@abc.abstractmethod
    def _writablestatechanged(self, w_state_entity):
        raise NotImplementedError()
    

class StateEntity(Containable):
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
        Containable.__init__(self, None, localname, local_id)  # no universe
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
    
    def readytoleaveuniverse(self):
        """
        obj.readytoleaveuniverse() -> (bool)
        
        return whether the object is ready to be ejected from the universe
        """
        return not self._device and Containable.readytoleaveuniverse(self)


class RStateEntityBase(object):
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
        
        newstate: the newest value to be recorded
        """
        self._state = newstate
    
    def read(self):
        """
        obj.read() -> most current state value
        """
        return self._state
    
    
class WStateEntityBase(object):
    """
    WStateEntityBase (abstract class)
    
    a writable state entity
    """
    
    __metaclass__ = abc.ABCMeta
    
    _NEVER_WRITTEN = object()
    
    def __init__(self, writerfunc):
        """
        obj.__init__(writerfunc)
        
        writerfunc: (function) the function, accepting as an argument this 
                               state entity and the new value, that will assume
                               responsibility for carrying out a state setting 
                               operation
        """
        self._writerfunc = writerfunc
        self._lastvaluewritten = WStateEntityBase._NEVER_WRITTEN
    
    class NoPriorWritesError(Exception):
        pass
    
    @property
    def lastvaluewritten(self):
        if self._lastvaluewritten is WStateEntityBase._NEVER_WRITTEN:
            errmsg = "can't access this property until a value has been " \
                     "written using write()"
            raise WStateEntityBase.NoPriorWritesError(errmsg)
        return self._lastvaluewritten
            
    
    def write(self, newvalue):
        """
        obj.write(newvalue)
        
        try to set a new value of newvalue on something
        """
        self._writerfunc(self, newvalue)


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


class RWStateEntity(StateEntity, RStateEntityBase, WStateEntityBase):
    """
    StateEntity([description[, localname[, local_id]]]) -> obj
    
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
