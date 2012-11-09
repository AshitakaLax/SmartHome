from .core import PhysicalDevice, RStateEntity, RWStateEntity

import threading
import httplib
import re

class PStripAndSensors(PhysicalDevice):
    def __init__(self, universe, hostaddress, port, 
                       localname=None, local_id=None):
        PhysicalDevice.__init__(self, universe, localname, local_id)
        self.hostaddress = hostaddress
        self.port = port
        self._querythread = None
        
        self.outlet1ison = RWStateEntity(self, self._writestate)
        self.outlet2ison = RWStateEntity(self, self._writestate)
        self.outlet3ison = RWStateEntity(self, self._writestate)
        self.outlet4ison = RWStateEntity(self, self._writestate)
        
        self.outlet1power = RStateEntity(self)
        self.outlet2power = RStateEntity(self)
        self.outlet3power = RStateEntity(self)
        self.outlet4power = RStateEntity(self)
        
        self.motionsensor = RStateEntity(self)
        self.lasertripwire = RStateEntity(self)
        self.tempfahrenheit = RStateEntity(self)
        self.sprinklersonoff = RStateEntity(self)
        
        self._pollingthread = None

    def _writestate(self, state_entity, newstate):
        
    
    def update_entities(self):
        # code for polling the powerstrip's status and updating its entities
        if not self._querythread:
            self._querythread = threading.Thread(target=self._performupdate)
            self._querythread.start()
    
    def _performupdate(self):
        response = self._contactdevice()
        self._parseandupdate(response)
        self._querythread = None
        print "Update performed."
    
    def _contactdevice(self):
        #return type(self)._testsample
        
        connection = httplib.HTTPConnection(self.hostaddress, self.port)
        connection.request("GET", "/")
        response = connection.getresponse().read()
        assert response
        connection.close()
        return response

    _testsample = """\
<html><a href=?LED0>Led 1: on</a> <a href=?LED1>Led 2: off</a> <a href=?LED2>Led 3: off</a> <a href=?LED3>Led 4: off</a> <html>Temperature Kelvin: 302.25 T	Temperature Celsius 26.60 *C
Temperature Fehrenhite 79.87 *F
        
</center><center>Sprinkler Detection: Sprinkler System is on        
Testing Current: 
22 -2 0 0

</html></center>"""
        
    def _parseandupdate(self, response):
        outletonoffregex = r"(?<=<a href=\?LED{0}>Led {1}: )(on|off)(?=</a>)"

        def updateoutletonoffstate(outletnumber, entity):
            regex = outletonoffregex.format(outletnumber - 1, outletnumber)
            match = re.search(regex, response)
            assert match
            entity.update(match.group() == "on")
        
        updateoutletonoffstate(1, self.outlet1ison)
        updateoutletonoffstate(2, self.outlet2ison)
        updateoutletonoffstate(3, self.outlet3ison)
        updateoutletonoffstate(4, self.outlet4ison)
        
        outlet_power_regex = r"(?m)(?<=Testing Current: \n)(?:-?\d+| )+$"
        match = re.search(outlet_power_regex, response)
        assert match
        line = match.group().split()
        
        def updateoutletpower(outletnumber, entity):
            entity.update(line[outletnumber - 1] + " A")
        
        updateoutletpower(1, self.outlet1power)
        updateoutletpower(2, self.outlet2power)
        updateoutletpower(3, self.outlet3power)
        updateoutletpower(4, self.outlet4power)
        
        temperatureregex = r"(?<=Temperature Fehrenhite )\d+(?:\.\d*)?(?= \*F)"
        match = re.search(temperatureregex, response)
        assert match
        self.tempfahrenheit.update(float(match.group()))
        
        sprinkler_regex = r"(?<=Sprinkler System is )(on|off)"
        match = re.search(sprinkler_regex, response)
        assert match
        self.sprinklersonoff.update(match.group() == "on")
        