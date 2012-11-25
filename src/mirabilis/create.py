from .core import *
from .pstripandsensors import PStripAndSensors
from .epic_cube import EpicCube

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
    
    # power strip and sensors
    pstrip = PStripAndSensors(universe, "Todd's smart power strip")
    lab.additem(pstrip, "power strip and sensors", 2)
    
    lab.additem(pstrip.outlet1onoff, "floor lamp", 3)
    lab.additem(pstrip.outlet2onoff, "fan", 4)

    pstrip.outlet1onoff.move(vlsi, new_local_id=1)
    pstrip.outlet3onoff.move(universe["MEB"]["level 3"], "Light")
    
    # epic cube
    ecube = EpicCube(universe, "Levi's Epic Cube")
    
    return universe
