from .core import *
from pprint import pprint

universe = Universe()

meb = Area(universe, "MEB", 1)
universe.additem(meb)

level3 = Area(universe, "Level 3", 3)
meb.additem(level3)

senhardlab = Area(universe, "Senior Hardware Lab", 3143)
level3.additem(senhardlab)

smallroom = Area(universe, "Small Room")
senhardlab.additem(smallroom)


class SmartOutlet(PhysicalDevice):
    def __init__(self, universe, localname=None, local_id=None):
        PhysicalDevice.__init__(self, universe, localname, local_id)
        
        def donothing(*args):
            print "donothing() called with args", args
        
        self.outlet1onoff = RWStateEntity(donothing)
        self.outlet2onoff = RWStateEntity(donothing)
        self.outlet3onoff = RWStateEntity(donothing)
        self.outlet4onoff = RWStateEntity(donothing)
        
        self.add_state_entity(self.outlet1onoff)
        self.add_state_entity(self.outlet2onoff)
        self.add_state_entity(self.outlet3onoff)
        self.add_state_entity(self.outlet4onoff)


outlet = SmartOutlet(universe, "smart outlet")
senhardlab.additem(outlet)

senhardlab.additem(outlet.outlet1onoff, newlocalname="Lamp", new_local_id=1)

senhardlab.additem(outlet.outlet2onoff, newlocalname="Fan", new_local_id=2)

outlet.outlet1onoff.move(smallroom)
outlet.outlet3onoff.move(level3)

print senhardlab
print list(senhardlab)

pprint(universe._objects_by_global_id)

print list(universe)

print universe["MEB"][3][3143]["smart outlet"]

print
for x in universe.objects_dict.values():
    print x, repr(x.path)