from .core import PhysicalDevice, RStateEntity, RWStateEntity

class PStripAndSensors(PhysicalDevice):
    def __init__(self, universe, localname=None, local_id=None):
        PhysicalDevice.__init__(self, universe, localname, local_id)
        description = "the on/off state of outlet #{}"
        self.outlet1onoff = RWStateEntity(self._writestate, 
                                          description.format(1))
        self.outlet2onoff = RWStateEntity(self._writestate, 
                                          description.format(2))
        self.outlet3onoff = RWStateEntity(self._writestate, 
                                          description.format(3))
        self.outlet4onoff = RWStateEntity(self._writestate, 
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
        self.lasertripwire = RStateEntity("laser trip wire state")
        self.add_state_entity(self.motionsensor)
        self.add_state_entity(self.lasertripwire)
        
        self._pollingthread = None

    def _writestate(self, state_entity, newstate):
        fmtstr = "this is {}, changing state of {} to {!r}"
        print fmtstr.format(self, state_entity, newstate)
    
    def update_entities(self):
        # code for polling the powerstrip's status and updating its entities
        pass