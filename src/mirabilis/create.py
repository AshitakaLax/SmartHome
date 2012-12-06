from .core import *
from .pstripandsensors import PStripAndSensors
from .epic_cube import EpicCubeDevice

def create():
    universe = Universe()
    
    building = Area(universe, "Merrill Engineering Building")
    universe.additem(building, "MEB", 1)
    
    floor = Area(universe, "third floor")
    universe["MEB"].additem(floor, "level 3", 3)
    
    lab = Area(universe, "Senior Hardware Lab")
    universe["MEB"]["level 3"].additem(lab, "Room 3143", 3143)
    
    # now the stuff IN the senior hardware lab
    
    vlsi = Area(universe, "VLSI test room", 1)
    lab.additem(vlsi, "vlsi")
    
    # epic cube
    epic_cube = EpicCubeDevice(universe, "Levi's Epic Cube") #, container=lab)
    lab.additem(epic_cube, "Epic Cube Device")
    
    
    # power strip and sensors
    pstrip = PStripAndSensors(universe, "Todd's smart power strip")
    lab.additem(pstrip, "Power Strip and Sensors")
    
    lab.additem(pstrip.outlet1onoff, "outlet 1 ON-OFF")
    lab.additem(pstrip.outlet2onoff, "outlet 2 ON-OFF")
    lab.additem(pstrip.outlet3onoff, "outlet 3 ON-OFF")
    lab.additem(pstrip.outlet4onoff, "outlet 4 ON-OFF")
    
    lab.additem(pstrip.outlet1power, "outlet 1 power")
    lab.additem(pstrip.outlet2power, "outlet 2 power")
    lab.additem(pstrip.outlet3power, "outlet 3 power")
    lab.additem(pstrip.outlet4power, "outlet 4 power")
    
    vlsi.additem(pstrip.motionsensor, "motion sensor")
    vlsi.additem(pstrip.lightstatus, "light ON-OFF")
    
    return universe
