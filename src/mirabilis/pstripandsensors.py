import re
import pdb
from urllib import urlopen
import traceback
import time

from .core import PhysicalDevice, RStateEntity, RWStateEntity, BoundMethod
from .core import printsync, printlock

_verbose = True

class PStripAndSensors(PhysicalDevice):
    def __init__(self, universe, localname=None, local_id=None):
        PhysicalDevice.__init__(self, universe, localname, local_id)
        description = "the on/off state of outlet #{}"
        self.outlet1onoff = \
            RWStateEntity(BoundMethod(self, "_writeoutletonoffstate"), 
                          description.format(1))
        self.outlet2onoff = \
            RWStateEntity(BoundMethod(self, "_writeoutletonoffstate"), 
                          description.format(2))
        self.outlet3onoff = \
            RWStateEntity(BoundMethod(self, "_writeoutletonoffstate"), 
                          description.format(3))
        self.outlet4onoff = RWStateEntity(BoundMethod(self, "_writeoutletonoffstate"), 
                                          description.format(4))
        self.add_state_entity(self.outlet1onoff)
        self.add_state_entity(self.outlet2onoff)
        self.add_state_entity(self.outlet3onoff)
        self.add_state_entity(self.outlet4onoff)
        
        description = "the power draw of outlet #{}"
        self.outlet1power = RStateEntity(description.format(1))
        self.outlet2power = RStateEntity(description.format(2))
        self.outlet3power = RStateEntity(description.format(3))
        self.outlet4power = RStateEntity(description.format(4))
        self.add_state_entity(self.outlet1power)
        self.add_state_entity(self.outlet2power)
        self.add_state_entity(self.outlet3power)
        self.add_state_entity(self.outlet4power)
        
        self.motionsensor = RStateEntity("motion sensing state")
        #self.lasertripwire = RStateEntity("laser trip wire state")
        self.lightstatus = RStateEntity("light state")
        
        self.add_state_entity(self.motionsensor)
        #self.add_state_entity(self.lasertripwire)
        self.add_state_entity(self.lightstatus)
    
    def _writeoutletonoffstate(self, state_entity, newstate):
        fmtstr = "this is {}, changing state of {} to {!r}"
        if _verbose:
            printsync(fmtstr.format(self, state_entity, newstate))
        assert state_entity.read() in ["ON", "OFF"]
        assert newstate in ["ON", "OFF"]
        if state_entity.read() != newstate:
            try:
                number = repr(int(state_entity.description[-1]) - 1)
                thing = urlopen("http://192.168.1.102/?LED{}".format(number))
            except IOError:
                msg = "PStripAndSensors._writeoutletonoffstate: error"
                with printlock:
                    print "DEVICE: {}: {}".format(self, msg)
                    traceback.print_exc()
                    print "DEVICE: {}: trying again...".format(self)
                time.sleep(5)
            thing.close()
            state_entity.update(newstate)
                
    
    def update_entities(self):
        # code for polling the powerstrip's status and updating its entities
        if _verbose:
            printsync("PStripAndSensors.update_entities: "
                      "starting to update device", self)
        while True:
            try:
                thing = urlopen("http://192.168.1.102/")
            except IOError:
                msg = "PStripAndSensors.update_entities: error"
                with printlock:
                    print "DEVICE: {}: {}".format(self, msg)
                    traceback.print_exc()
                    print "DEVICE: {}: trying again...".format(self)
                time.sleep(5)
            else:
                break
        data = thing.read()
        thing.close()
        matches = list(re.finditer("Led \d: (on|off)", data))
        #pdb.set_trace()
        assert len(matches) == 4
        self.outlet1onoff.update(matches[0].group(1).upper())
        self.outlet2onoff.update(matches[1].group(1).upper())
        self.outlet3onoff.update(matches[2].group(1).upper())
        self.outlet4onoff.update(matches[3].group(1).upper())
        
        pattern = r"Socket Power Consumption:\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)"
        match = re.search(pattern, data)
        if not match:
            print "could not locate power consumption; data is:", data
        self.outlet1power.update(match.group(1))
        self.outlet2power.update(match.group(2))
        self.outlet3power.update(match.group(3))
        self.outlet4power.update(match.group(4))
        
        #status = re.search("Trip Wire Status: (0|1)", data).group(1)
        #self.lasertripwire.update("TRIPPED" if status == "1" else "RESET")
        status = re.search("Motion Sensor Status: (0|1)", data).group(1)
        self.motionsensor.update("MOTION" if status == "1" else "STILL")
        
        status = re.search("Digital line for turning on lights: (0|1)", 
                           data).group(1)
        self.lightstatus.update(status)
    
    @property
    def pollinginterval(self):
        """"
        pollinginterval: (float)
        
        see documentation for PhysicalDevice.pollinginterval
        """
        return 10.0
    
    @property
    def shouldpollafterwrite(self):
        """"
        shouldpollafterwrite: (bool)
        
        see documentation for PhysicalDevice.shouldpollafterwrite
        """
        return True