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
import time



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
_sendreceiveverbose = False


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
            self.maingroup = Group(container=container,
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
                dampergroup.additem(damper, 
                                    "damper #{}".format(number_str), 
                                    number)
        
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
                                    description="group for the temperature " 
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
                                   description="group for the sprinklers of "
                                               "the Epic Cube device",
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
                
        self._lock = Lock()
        
        try:
            self._serial = Serial(port="/dev/cu.usbmodem12341", 
                                  baudrate=38400, 
                                  timeout=0)
            #self._send("-help")
            if _sendreceiveverbose:
                printsync("Epic Cube: sleeping 1 second")
            time.sleep(1)
            data = [""]
            if _sendreceiveverbose:
                printsync("Epic Cube: doing extra receive for start up")
            self._receive()
        except:
            printsync("\a\a\a")
            printsync("WARNING: Initialization of serial communication for "
                      "Epic Cube failed.")
            traceback.print_exc()
            printsync("\ncontinuing...\n")
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
        if _sendreceiveverbose:
            printsync("Epic Cube: sent", repr(command + "\n"))
        time.sleep(0.01)        
        self._lowlevelreceive()
        self._serial.write("\n")
    
    # code for talking to the Epic Cube goes here
    # self._lock should be locked before calling this method
    def _lowlevelreceive(self):
        assert self._lock.islocked
        data = ""
        part = self._serial.read(256)
        while part:
            data += part
            part = self._serial.read(256)
        data = data.strip().strip(">").strip()
        if _sendreceiveverbose:
            printsync("Epic Cube: received", repr(data))
        return data
        
    def _send(self, command, delay=0.1):
        with self._lock:  # automatically acquire and release the lock
            self._lowlevelsend(command)
            time.sleep(delay)
            self._lowlevelreceive()
    
    # may return empty string
    def _receive(self):
        with self._lock:  # automatically acquire and release the lock
            return self._lowlevelreceive()
        
    def _sendreceive(self, value, delay=0.1):
        with self._lock:  # automatically acquire and release the lock
            self._lowlevelsend(value)
            time.sleep(delay)
            return self._lowlevelreceive()
    
    def _updatedampers(self):
        if _verbose:
            printsync("Epic Cube: skipping updating dampers")
        #for dampernum in sorted(self.dampers.keys()):
        #    assert 0 <= dampernum < 100
        #    response = self._sendreceive("Damper?{:02}".format(dampernum))
        #    if response in tuple("01"):
        #        status = "OPEN" if response == "0" else "CLOSED"
        #        self.dampers[dampernum].update(status)
        #    else:
        #        text = "got {!r} back from damper controller".format(response)
        #        raise AssertionError(text)
    
    def _updatehvacstatus(self):
        response = self._sendreceive("HVAC?", 3)
        if response == "1":
            status = "1 (furnace controller blower on)"
        elif response == "2":
            status = "2 (furnace controller blower and heating on)"
        elif response == "3":
            status = "3 (furnace controller blower and AC on)"
        elif response == "4":
            status = "4 (server controller blower on)"
        elif response == "5":
            status = "5 (server controller blower on)"
        elif response == "6":
            status = "6 (server controller blower on)"
        elif response == "7":
            status = "7 (all off)"
        else:
            raise AssertionError("got strange result in _updatehvacstatus: " +
                                 repr(response))
        self.hvacstatus.update(status)
    
    def _updatetempsensors(self):
        if _verbose:
            printsync("Epic Cube: skipping updating temperature sensors")
        #for sensornum in sorted(self.tempsensors.keys()):
        #    tempsensor = self.tempsensors[sensornum]
        #    assert 0 <= sensornum < 100
        #    response = self._sendreceive("Temp{:02}".format(sensornum), )
        #    response = int(response)
        #    if not 0 <= response <= 1024:
        #        pass
        #        #printsync("Epic Cube: WARNING: temp value {} not between "
        #        #          ">= 0 and < 1024".format(response))
        #    self.tempsensors[sensornum].update(repr(response))
    
    def _updatefans(self):
        fan_nums = sorted(self.fans.keys())
        response = self._sendreceive("AllFAN").split(",")
        assert len(response) == len(fan_nums)
        for responsepart in response:
            fan_number, onoff, speed = map(int, responsepart.split(":"))
            if fan_number == 1:
                fan_number = 0
            if onoff == 0:
                self.fans[fan_number].update("OFF")
            else:
                speed = int(speed)
                assert 0 <= speed <= 9
                self.fans[fan_number].update(repr(speed + 1))
        
    def update_entities(self):
        if not self._serial:
            if _verbose:
                printsync("in EpicCube.update_entities; bypassing update code")
            return
        if _verbose:
            msg = "EpicCube.update_entities: preparing to update..."
            printsync("DEVICE: {}: {}".format(self, msg))
        
        self._updatedampers()
        self._updatehvacstatus()
        # garage not updated
        self._updatetempsensors()
        self._updatefans()
        
        if _verbose:
            msg = "EpicCube.update_entities: update completed"
            printsync("DEVICE: {}: {}".format(self, msg))
    
    def _writedamperstate(self, state_entity, newstate):
        number = state_entity.__dampernumber
        assert newstate in ["OPEN", "CLOSED"]
        newstatebin = "0" if newstate == "OPEN" else "1"
        self._send("Damper{:02}{}".format(number, newstatebin), 10.0)
        #state_entity.update(newstate)
    
    def _write_hvac_command(self, state_entity, newstate):
        assert 1 <= int(newstate) <= 5
        self._send("HVAC{}".format(newstate))
    
    def _writegaragestate(self, state_entity, newstate):
        if _verbose:
            printsync("Epic Cube: toggling garage door")
        self._send("Garage")
    
    def _writesprinklerstate(self, state_entity, newstate):
        if newstate == "ON":
            newstate = "1"
        elif newstate == "OFF":
            newstate = "0"
        assert newstate in tuple("01")
        #assert 0 <= sprinklernumber <= 9
        self._send("Sprinkler{}{}".format(state_entity.__sprinklernumber,
                                          newstate))
    
    def _writefanstate(self, state_entity, newstate):
        fan_number = state_entity.__fan_number
        onoff = "0" if newstate == "OFF" else "1"
        speed = "0" if newstate == "OFF" else repr(int(newstate) - 1)
        self._send("Fan{}{}{}".format(fan_number, onoff, speed))
        state_entity.update(newstate)
        printsync("writing fan state to", newstate)
    
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
