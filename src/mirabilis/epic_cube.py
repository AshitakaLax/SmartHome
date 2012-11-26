from .core import (PhysicalDevice, 
                   RStateEntity, 
                   RWStateEntity, 
                   WStateEntity, 
                   Group)
                    


# these variables must always be iterable
_DAMPER_NUMS = range(16)
_TEMP_SENSOR_NUMS = range(64)
_SPRINKLER_NUMS = range(6)
_FAN_NUMS = [0, 2, 3, 4]


class EpicCube(PhysicalDevice):
    """
    EpicCube([universe[, localname[, local_id[, container]]]]) -> obj
    
    universe: (Universe or <None>)
    localname: (str)
    local_id: (int)
    container: (Universe or Area or <None>)
    
    instance variables:
    
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
        
        self.dampers = {}
        for number in _DAMPER_NUMS:
            description = "EpicCube damper #{}".format(number)
            entity = RWStateEntity(self._writedamperstate, description)
            entity.__dampernumber = number
            self.dampers[number] = entity
            self.add_state_entity(entity)
        
        self.hvacstatus = RStateEntity("EpicCube HVAC status")
        self.add_state_entity(self.hvacstatus)
        
        self.hvac_command = WStateEntity(self._write_hvac_command,
                                         "EpicCube HVAC command sender")
        self.add_state_entity(self.hvac_command)
        
        self.garage = WStateEntity(self._writegaragestate, 
                                   "EpicCube garage opener")
        self.add_state_entity(self.garage)

        self.tempsensors = {}
        for number in _TEMP_SENSOR_NUMS:
            description = "EpicCube temperature sensor #{}".format(number)
            entity = RStateEntity(description)
            entity.__sensornumber = number
            self.tempsensors[number] = entity
            self.add_state_entity(entity)
        
        self.sprinklers = {}
        for number in _SPRINKLER_NUMS:
            description = "EpicCube sprinkler #{}".format(number)
            entity = WStateEntity(self._writesprinklerstate, description)
            entity.__sprinklernumber = number
            self.sprinklers[number] = entity
            self.add_state_entity(entity)
        
        self.fans = {}
        for number in _FAN_NUMS:
            description = "EpicCube fan #{}".format(number)
            entity = RWStateEntity(self._writefanstate, description)
            entity.__fan_number = number
            self.fans[number] = entity
            self.add_state_entity(entity)
        
    def setupincontainer(self, container):
        self.move(container, newlocalname="EpicCube device")
        maingroup = Group(container=container, localname="EpicCube")
        #maingroup = container
        dampergroup = Group(container=maingroup, localname="dampers")
        for id, damper in self.dampers.items():
            damper.move(dampergroup, new_local_id=id)
        self.hvacstatus.move(maingroup, newlocalname="HVAC status")
        self.hvac_command.move(maingroup, newlocalname="HVAC command")
        self.garage.move(maingroup, newlocalname="garage")
        tempsensorgroup = Group(container=maingroup, localname="temp sensors")
        for id, sensor in self.tempsensors.items():
            sensor.move(tempsensorgroup, new_local_id=id)
        sprinklergroup = Group(container=maingroup, localname="sprinklers")
        for id, sprinkler in self.sprinklers.items():
            sprinkler.move(sprinklergroup, new_local_id=id)
        fangroup = Group(container=maingroup, localname="fans")
        for id, fan in self.fans.items():
            fan.move(fangroup, new_local_id=id)
        
    def _send(self, command):
        raise NotImplementedError()
    
    # may return empty string
    def _receive(self):
        raise NotImplementedError()
        
    def _sendreceive(self, value):
        self._send(value)
        return self._receive()
    
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
        self._updatedampers()
        self._updatehvacstatus()
        # garage not updated
        self._updatetempsensors()
        self._updatefans()
    
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