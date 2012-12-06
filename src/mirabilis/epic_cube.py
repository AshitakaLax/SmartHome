from .core import (PhysicalDevice, 
                   RStateEntity, 
                   RWStateEntity, 
                   WStateEntity, 
                   Group,
                   BoundMethod,
                   Lock,
                   printsync)

from serial import Serial # pyserial
import traceback


# these variables must always be iterable
_DAMPER_NUMS = range(16)
_DAMPER_DIGITS = 2
_TEMP_SENSOR_NUMS = range(64)
_TEMP_SENSOR_DIGITS = 2
_SPRINKLER_NUMS = range(6)
_SPRINKLER_DIGITS = 1
_FAN_NUMS = [0, 2, 3, 4]
_FAN_DIGITS = 1


_verbose = False

class EpicCubeDevice(PhysicalDevice):
    """
    EpicCubeDevice([universe[, localname[, local_id[, container]]]]) -> obj
    
    universe: (Universe or <None>)
    localname: (str)
    local_id: (int)
    container: (Universe or Area or <None>)
    
    instance variables:
    
    maingroup: (Group) a group containing subgroups for each category of thing
    dampers: (dict of RWStateEntity[int])
    hvacstatus: (RStateEntity)
    hvac_command: (WStateEntity)
    garage: (WStateEntity)
    tempsensors: (dict of RStateEntity[int])
    sprinklers: (dict of WStateEntity[int])
    fans: (dict of RWStateEntity[int])
    """
    
    def __init__(self, universe=None, localname=None, local_id=None, 
                       container=None):
        PhysicalDevice.__init__(self, universe, localname, local_id, container)
        
        Method = BoundMethod
        if container:
            maingroupdescription = "the group holding the components and " \
                "subgroups of the Epic Cube device"
            self.maingroup = Group(universe, 
                                   description=maingroupdescription,
                                   localname="Epic Cube")
        else:
            self.maingroup = None
        
        dampergroupdescription = "group holding the damper state entities " \
            "for the Epic Cube device"
        if container:
            dampergroup = Group(container=self.maingroup, 
                                description=dampergroupdescription,
                                localname="dampers")
        self.dampers = {}
        for number in _DAMPER_NUMS:
            number_str = format(number, "0{}".format(_DAMPER_DIGITS))
            description = "Epic Cube damper #{}".format(number_str)
            damper = RWStateEntity(Method(self, "_writedamperstate"), 
                                   description)
            damper.__dampernumber = number
            self.dampers[number] = damper
            self.add_state_entity(damper)
            if container:
                dampergroup.additem(damper, "damper #{}".format(number_str), number)
        
        self.hvacstatus = RStateEntity("HVAC status for the Epic Cube", 
                                       "HVAC status")
        self.add_state_entity(self.hvacstatus)
        if container:
            self.maingroup.additem(self.hvacstatus)
        
        self.hvac_command = WStateEntity(Method(self, "_write_hvac_command"),
                                         "HVAC command sender for the Epic "
                                             "Cube",
                                         "HVAC command")
        self.add_state_entity(self.hvac_command)
        if container:
            self.maingroup.additem(self.hvac_command)
        
        self.garage = WStateEntity(Method(self, "_writegaragestate"), 
                                   "garage opener/closer for the Epic Cube",
                                   "garage")
        self.add_state_entity(self.garage)
        if container:
            self.maingroup.additem(self.garage)
            
        if container:
            tempsensorgroup = Group(container=self.maingroup, 
                                    description="group for the temperature " \
                                        "sensors of the Epic Cube device",
                                    localname="temperature sensors")
        self.tempsensors = {}
        for number in _TEMP_SENSOR_NUMS:
            number_str = format(number, "0{}".format(_TEMP_SENSOR_DIGITS))
            description = "Epic Cube temperature sensor #{}".format(number_str)
            tempsensor = RStateEntity(description)
            tempsensor.__sensornumber = number
            self.tempsensors[number] = tempsensor
            self.add_state_entity(tempsensor)
            if container:
                tempsensorgroup.additem(tempsensor, 
                                        "sensor #{}".format(number_str),
                                        number)
        
        if container:
            sprinklergroup = Group(container=self.maingroup, 
                                   description="group for the sprinklers of the " \
                                        "Epic Cube device",
                                   localname="sprinklers")
        self.sprinklers = {}
        for number in _SPRINKLER_NUMS:
            number_str = format(number, "0{}".format(_SPRINKLER_DIGITS))
            description = "Epic Cube sprinkler #{}".format(number_str)
            sprinkler = WStateEntity(Method(self, "_writesprinklerstate"), 
                                     description)
            sprinkler.__sprinklernumber = number
            self.sprinklers[number] = sprinkler
            self.add_state_entity(sprinkler)
            if container:
                sprinklergroup.additem(sprinkler, 
                                       "sprinkler #{}".format(number_str),
                                       number)
        
        if container:
            fangroup = Group(container=self.maingroup, 
                             description="group for the fans of the Epic Cube " \
                                 "device",
                             localname="fans")
        self.fans = {}
        for number in _FAN_NUMS:
            number_str = format(number, "0{}".format(_FAN_DIGITS))
            description = "Epic Cube fan #{}".format(number_str)
            fan = RWStateEntity(Method(self, "_writefanstate"), description)
            fan.__fan_number = number
            self.fans[number] = fan
            self.add_state_entity(fan)
            if container:
                fangroup.additem(fan, "fan #{}".format(number_str), number)
        
        if container:
            container.additem(self.maingroup)
        
        self._lock = Lock()
        
        try:
            self._serial = Serial(port="/dev/cu.usbmodem12341", 
                                  baudrate=38400, 
                                  timeout=0)
        except:
            print "\a\a\a" 
            print "WARNING: Initialization of serial communication for Epic Cube failed."
            traceback.print_exc()
            print "\ncontinuing...\n"
            self._serial = None
    
    def __getstate__(self):
        odict = self.__dict__.copy()
        del odict["_lock"]
        return odict
        
    def __setstate__(self, odict):
        self.__dict__.update(odict)
        self._lock = None
        
    # code for talking to the Epic Cube goes here
    # self._lock should be locked before calling this method
    def _lowlevelsend(self, command):
        command = command.strip()
        assert self._lock.islocked
        self._serial.write(command)
        self._serial.write("\n")
        print "Epic Cube: sent", repr(command)
    
    # code for talking to the Epic Cube goes here
    # self._lock should be locked before calling this method
    def _lowlevelreceive(self):
        assert self._lock.islocked
        data = ""
        part = self._serial.read(256)
        while part:
            data += part
            part = self._serial.read(256)
        print "Epic Cube: received", repr(data)
        return data
        
    def _send(self, command):
        with self._lock:  # automatically acquire and release the lock
            self._lowlevelsend(command)
    
    # may return empty string
    def _receive(self):
        with self._lock:  # automatically acquire and release the lock
            return self._lowlevelreceive()
        
    def _sendreceive(self, value):
        with self._lock:  # automatically acquire and release the lock
            self._lowlevelsend(value)
            return self._lowlevelreceive()
    
    def _updatedampers(self):
        for dampernum in sorted(self.dampers.keys()):
            assert 0 <= dampernum < 100
            response = self._sendreceive("Damper?{:02}".format(dampernum))
            if response in "01":
                self.dampers[dampernum].update(response)
            else:
                text = "got {!r} back from damper controller".format(response)
                raise AssertionError(text)
    
    def _updatehvacstatus(self):
        response = self._sendreceive("HVAC?")
        if response in "1234567":
            self.hvac.update(response)
        else:
            text = "got {!r} back from HVAC controller".format(response)
            raise AssertionError(text)
    
    def _updatetempsensors(self):
        for sensornum in sorted(self.tempsensors.keys()):
            tempsensor = self.tempsensors[sensornum]
            assert 0 <= sensornum < 100
            response = self._sendreceive("TEMP{:02}".format(sensornum))
            response = int(response)
            assert 0 <= response <= 1024
            self.tempsensors[sensornum].update(response)
    
    def _updatefans(self):
        fan_nums = sorted(self.fans.keys())
        response = self._sendreceive("AllFAN").split(",")
        assert len(response) == len(fan_nums)
        for responsepart in response:
            fan_number, onoff, speed = map(int, responsepart.split(":"))
            if onoff == "0":
                fans[fan_number].update(-1)
            else:
                speed = int(speed)
                assert 0 <= speed <= 9
                fans[fan_number].update(speed)
        
    def update_entities(self):
        if not self._serial:
            if _verbose:
                printsync("in EpicCube.update_entities; bypassing update code")
            return
        msg = "EpicCube.update_entities: preparing to update..."
        printsync("DEVICE: {}: {}".format(self, msg))
        
        self._updatedampers()
        self._updatehvacstatus()
        # garage not updated
        self._updatetempsensors()
        self._updatefans()
        
        msg = "EpicCube.update_entities: update completed"
        printsync("DEVICE: {}: {}".format(self, msg))
    
    def _writedamperstate(self, state_entity, newstate):
        number = state_entity.__dampernumber
        assert newstate in "01"
        self._send("Damper{:02}{}".format(number, newstate))
    
    def _write_hvac_command(self, state_entity, newstate):
        assert 1 <= int(newstate) <= 5
        self._send("HVAC{}".format(newstate))
    
    def _writegaragestate(self, state_entity, newstate):
        self._send("Garage")
    
    def _writesprinklerstate(self, state_entity, newstate):
        assert newstate in tuple("01")
        assert 0 <= sprinklernumber <= 9
        self._send("Sprinkler{}{}".format(state_entity.__sprinklernumber,
                                          newstate))
    
    def _writefanstate(self, state_entity, newstate):
        fan_number = state_entity.__fan_number
        onoff = "0" if newstate == "-1" else "1"
        speed = "0" if newstate == "-1" else newstate
        self._write("Fan{}{}{}".format(fan_number, onoff, speed))
    
    @property
    def pollinginterval(self):
        """"
        pollinginterval: (float)
        
        see documentation for PhysicalDevice.pollinginterval
        """
        return 5.0
    
    @property
    def shouldpollafterwrite(self):
        """"
        shouldpollafterwrite: (bool)
        
        see documentation for PhysicalDevice.shouldpollafterwrite
        """
        return True
