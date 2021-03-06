from ._rename import renamemodule
from ._smarthomeitem import SmartHomeItem


__all__ = ["Container"]


@renamemodule
class Container(object):
    """
    Container (abstract class)
 
    a class for things that hold other items
    """

    # Private instance attributes:
    # 
    # _dictofitemstokeys: (dict of frozenset[SmartHomeItem])
    #                     this stores a cache of the keys into _itemsbykey,
    #                     indexed by the value; ALL items associated with 
    #                     this holder MUST be stored as keys into this 
    #                     dictionary, even if their "local keys" are an empty
    #                     set
    # _itemsbykeycache: (dict of SmartHomeItem[object]) 
    #                   may pair every item with an object which need only be 
    #                   unique within the Container instance; items without 
    #                   any keys will be omitted from this dictionary
        
    # must call this __init__ method to initialize the variables
    def __init__(self):
        self._dictofitemstokeys = {}
        self._itemsbykeycache = {}
    
    def __getitem__(self, index):
        assert not isinstance(index, slice)
        return self._itemsbykeycache[index]
    
    def __contains__(self, item):
        return item in self._dictofitemstokeys.iterkeys()
    
    def __iter__(self):
        return iter(self.items)
    
    @property
    def universe(self):
        """
        universe: (Universe) the universe of this container, which may be 
                             itself
        """
        raise NotImplementedError()
    
    @property
    def childkeys(self):
        """
        childkeys: (iterator of set) the child keys of the container
        """
        return self._dictofitemstokeys.itervalues()
    
    @property
    def items(self):
        """
        items: (iterator of SmartHomeItem)
        """
        return self._dictofitemstokeys.iterkeys()
    
    @property
    def itemskeys(self):
        """
        itemskeys: (iterator of tuple of (SmartHomeItem, set))
        """
        return self._dictofitemstokeys.iteritems()
    
    def additem(self, item, newlocalname=None, new_local_id=None,
                      addkeys=frozenset()):
        """
        obj.additem(item[, newlocalname
                        [, new_local_id
                        [, addkeys]]])
        
        add an item to this holder
        
        item: (SmartHomeItem) the thing to be added
        addkeys: (iterable) the keys--numeric, string, or other type--for 
                            accessing the item locally from its holder
        newlocalname: (str or <None>) a new local name to set on item
        new_local_id: (int or <None>) a new local ID to set on item
        """
        with self.universe.writelock:
            assert not item._container
            assert isinstance(item, SmartHomeItem)
            if newlocalname is not None:
                item.localname = newlocalname
            if new_local_id is not None:
                item.local_id = new_local_id
            keys = {x for x in [item.localname, item.local_id]
                      if x is not None}
            keys = frozenset(keys.union(addkeys))
            self._dictofitemstokeys[item] = keys
            for key in keys:
                if key in self._itemsbykeycache.keys():
                    raise \
                        KeyError("the key {!r} is already in use".format(key))
                self._itemsbykeycache[key] = item
            item._container = self
    
    def removeitem(self, item):
        """
        obj.removeitem(item)
        
        remove an item from the holder
        
        item: (SmartHomeItem) the thing to be removed
        """
        with self.universe.writelock:
            assert item in self
            for key in self._dictofitemstokeys[item]:
                del self._itemsbykeycache[key]
            del self._dictofitemstokeys[item]
            item._container = None
    
    def getitemkeys(self, item):
        """
        obj.getitemkeys(item) -> (set)
        
        get keys for the item specified
        
        item: (SmartHomeItem) what to get the keys for
        """
        with self.universe.readlock:
            assert item in self._dictofitemstokeys.keys()
            return set(self._dictofitemstokeys[item])
