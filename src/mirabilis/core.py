"""
module core
"""

import abc
from datetime import datetime
from weakref import WeakValueDictionary, ref


__all__ = ["ItemHolder", "Containable", "Universe", "Area", "PhysicalDevice", 
           "StateEntity", "RStateEntity", "WStateEntity", "RWStateEntity",
           "UniverseObject"]


class UniverseObjectInterface(object):
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
    
    
class UniverseObject(UniverseObjectInterface):
    """
    UniverseObject (abstract class)
    
    an item that has a global ID and exists in a universe
    """
    
    # private attributes:
    # 
    # _universe: (Universe or <None>) the universe containing this object
    # _global_id: (int or <None>) the global ID for this object
    
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

    def readytoquituniverse(self):
        """
        obj.readytoquituniverse() -> (bool) 
        
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
        assert self.readytoquituniverse()
    
    def quituniverse(self):
        """
        obj.quituniverse()
        
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
        
    def additem(self, containable, newlocalname=None, new_local_id=None,
                      addkeys=frozenset()):
        """
        obj.additem(containable[, newlocalname
                               [, new_local_id
                               [, addkeys]]])
        
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
            if key in self._itemsbykeycache.keys():
                raise KeyError("the key {!r} is already in use".format(key))
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


class Universe(ItemHolder, _EventDispatcher):
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
        ItemHolder.__init__(self)
        _EventDispatcher.__init__(self)
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


class Containable(UniverseObject):
    """
    Containable (abstract class)
    
    root class for Area, PhysicalDevice, and StateEntity
    
    Instances of this class are objects that can be located inside the 
    boundaries of an ItemHolder object.
    
    Public instance attributes:
    
    description: (str or <None>) a description of this item
    localname: (str or <None>) the preferred local name of the object in its 
                               container
    local_id: (int or <None>) the preferred local ID of the object in its 
                              container
    """
    
    def __init__(self, universe, description=None, 
                       localname=None, local_id=None):
        """
        obj.__init__(universe[, description[, localname[, local_id]]])
        
        universe: (Universe) the universe where this is being created
        description: (str or <None>) a description of the object -- 
                                     optional (default: None)
        localname: (str or <None>) the preferred local name of the object in 
                                   its container
        local_id: (int or <None>) the preferred local ID of the object in its 
                                  container
        """
        UniverseObject.__init__(self, universe)
        self._container = None  # written to by ItemHolder objects
        self.description = description
        self.localname = localname
        self.local_id = local_id
    
    def __repr__(self):
        fmtstr = "<{0.__class__.__module__}.{0.__class__.__name__} " \
                 "gloabl_id:{0.global_id}>"
        return fmtstr.format(self)
    
    @property
    def path(self):
        """
        path: (str) a slash-separated path of the names leading up to this
                    item
        """
        c = self
        parents = []
        while isinstance(c, Containable):
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
    
    def readytoquituniverse(self):
        """
        obj.readytoquituniverse() -> (bool)
        
        return whether the containable is ready to be removed from the universe
        """
        return not self._container and \
            UniverseObject.readytoquituniverse(self)
    
    def removefromcontainer(self):
        """
        obj.removefromconainer()
        
        removes the containable from its current container
        
        raises an exception if it's not in any container
        """
        assert self.container
        self.container.removeitem(self)
    
    def move(self, newholder, newlocalname=None, new_local_id=None):
        """
        obj.move(newholder)
        
        moves the containable to a new container
        """
        if self.container:
            self.removefromcontainer()
        newholder.additem(self, newlocalname=newlocalname, 
                                new_local_id=new_local_id)
        

class Area(Containable, ItemHolder):
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
    
    (inherited from class Containable)
    """
    
    def __init__(self, universe, description=None, 
                       localname=None, local_id=None):
        Containable.__init__(self, universe, description, localname, local_id)
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
        
    def update_entities(self):
        """
        obj.update_entities()
        
        this method tells the device to update the status of its entities
        """
        raise NotImplementedError()
    
    def _writablestatechanged(self, w_state_entity):
        raise NotImplementedError()


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
        return (cls, entity)


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
    
    def readytoquituniverse(self):
        """
        obj.readytoquituniverse() -> (bool)
        
        return whether the object is ready to be ejected from the universe
        """
        return not self._device and Containable.readytoquituniverse(self)    


class RStateEntityBase(UniverseObjectInterface):
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
        
        record a new value observed by the device
        
        newstate: the newest value to be recorded
        """
        (laststate, self._state) = (self._state, newstate)
        if newstate != laststate:
            event = StateChangedEvent(self, newstate, laststate)
            self.universe._postevent(event)
    
    def read(self):
        """
        obj.read() -> most current state value
        """
        return self._state
    
    
class WStateEntityBase(UniverseObjectInterface):
    """
    WStateEntityBase (abstract class)
    
    a writable state entity
    """
    
    __metaclass__ = abc.ABCMeta
    
    NEVER_WRITTEN = object()
    
    def __init__(self, writerfunc):
        """
        obj.__init__(writerfunc)
        
        writerfunc: (function) the function, accepting as an argument this 
                               state entity and the new value, that will assume
                               responsibility for carrying out a state setting 
                               operation
        """
        self._writerfunc = writerfunc
        self._lastwrite = WStateEntityBase.NEVER_WRITTEN
    
    class NoPriorWritesError(Exception):
        pass
    
    @property
    def lastwrite(self):
        if self._lastwrite is WStateEntityBase._NEVER_WRITTEN:
            errmsg = "can't access this property until a value has been " \
                     "written using write()"
            raise WStateEntityBase.NoPriorWritesError(errmsg)
        return self._lastwrite
    
    def write(self, newvalue):
        """
        obj.write(newvalue)
        
        try to set a new value of newvalue on something
        """
        self._writerfunc(self, newvalue)
        (oldwrite, self._lastwrite) = (self._lastwrite, newvalue)
        # post event, regardless of whether old and new writes are equal values
        self.universe._postevent(ValueWrittenEvent(self, newvalue, oldwrite))
    
    def rewrite(self):
        """
        obj.rewrite()
        
        re-write the last value written another time
        """
        self.write(self.lastwrite)


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
    