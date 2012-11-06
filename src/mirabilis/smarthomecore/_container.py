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


