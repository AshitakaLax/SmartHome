__all__ = ["Container"]


class Container(object):
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
    
    def __contains__(self, item):
        return item in self._dictofitemstokeys.keys()
    
    def __iter__(self):
        return iter(self._dictofitemstokeys.keys())
        
    def additem(self, item, newlocalname=None, new_local_id=None,
                      addkeys=frozenset()):
        """
        obj.additem(item[, newlocalname
                        [, new_local_id
                        [, addkeys]]])
        
        add an item to this holder
        
        item: (Containable) the thing to be added
        addkeys: (iterable) the keys--numeric, string, or other type--for 
                            accessing the item locally from its holder
        newlocalname: (str or <None>) a new local name to set on item
        new_local_id: (int or <None>) a new local ID to set on item
        """
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
                raise KeyError("the key {!r} is already in use".format(key))
            self._itemsbykeycache[key] = item
        item._container = self
    
    def removeitem(self, item):
        """
        obj.removeitem(item)
        
        remove an item from the holder
        
        item: (Containable) the thing to be removed
        """
        assert item in self
        for key in self._dictofitemstokeys[item]:
            del self._itemsbykeycache[key]
        del self._dictofitemstokeys[item]
        item._container = None
    
    def getitemkeys(self, item):
        """
        obj.getitemkeys(item) -> (set)
        
        get keys for the item specified
        
        item: (Containable) what to get the keys for
        """
        assert item in self._dictofitemstokeys.keys()
        return set(self._dictofitemstokeys[item])


from .smarthomeitem import SmartHomeItem
