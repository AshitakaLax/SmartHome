from .core import PhysicalDevice, RStateEntity, RWStateEntity

class PStripAndSensors(PhysicalDevice):
    def __init__(self, universe, localname=None, local_id=None):
        PhysicalDevice.__init__(self, universe, localname, local_id)
        #description = "the on/off state of an outlet "
        self.outlet1onoff = RWStateEntity(self._writestate)
        self.outlet2onoff = RWStateEntity(self._writestate)
        self.outlet3onoff = RWStateEntity(self._writestate)
        self.outlet4onoff = RWStateEntity(self._writestate)
        self.add_state_entity(self.outlet1onoff)
        self.add_state_entity(self.outlet2onoff)
        self.add_state_entity(self.outlet3onoff)
        self.add_state_entity(self.outlet4onoff)
        
        self.outlet1power = RStateEntity()
        self.outlet2power = RStateEntity()
        self.outlet3power = RStateEntity()
        self.outlet4power = RStateEntity()
        self.add_state_entity(self.outlet1power)
        self.add_state_entity(self.outlet2power)
        self.add_state_entity(self.outlet3power)
        self.add_state_entity(self.outlet4power)
        
        self.motionsensor = RStateEntity()
        self.lasertripwire = RStateEntity()
        self.add_state_entity(self.motionsensor)
        self.add_state_entity(self.lasertripwire)
        
        self._pollingthread = None

    def _writestate(self, state_entity, newstate):
        fmtstr = "this is {}, changing state of {} to {!r}"
        print fmtstr.format(self, state_entity, newstate)
    
    def update_entities(self):
        # code for polling the powerstrip's status and updating its entities
        pass