from .core import *
from .pstripandsensors import PStripAndSensors


def create():
    universe = Universe()
    
    building = Area(universe, "Merrill Engineering Building")
    universe.additem(building, "MEB", 1)
    
    floor = Area(universe)
    universe["MEB"].additem(floor, "level 3", 3)
    
    lab = Area(universe, "Senior Hardware Labratory")
    universe["MEB"]["level 3"].additem(lab, "Room 3143", 3143)
    
    # now the stuff IN the senior hardware lab
    
    vlsi = Area(universe, "VLSI test room")
    lab.additem(vlsi, "vlsi", 1)

    pstrip = PStripAndSensors(universe, "192.168.0.102", 80, 
                              "outlets, sprinklers, temp and lighting sensors")
    lab.additem(pstrip, "power strip and sensors", 2)
    
    lab.additem(pstrip.outlet1ison, "floor lamp", 3)
    lab.additem(pstrip.outlet2ison, "fan", 4)

    pstrip.outlet1ison.move(vlsi, new_local_id=1)
    pstrip.outlet2ison.move(universe["MEB"]["level 3"], "Light")
    
    return universe
