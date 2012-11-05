from .core import PhysicalDevice, RStateEntity, RWStateEntity

class PowerStrip(PhysicalDevice):
    def __init__(self, universe, localname=None, local_id=None):
        PhysicalDevice.__init__(self, universe, localname, local_id)
        
        description = "the on/off state of an outlet "
        self.outlet1onoff = RWStateEntity(self._writestate)
        self.outlet2onoff = RWStateEntity(self._writestate)
        self.outlet3onoff = RWStateEntity(self._writestate)
        self.outlet4onoff = RWStateEntity(self._writestate)
        
        self.add_state_entity(self.outlet1onoff)
        self.add_state_entity(self.outlet2onoff)
        self.add_state_entity(self.outlet3onoff)
        self.add_state_entity(self.outlet4onoff)

    def _writestate(self, state_entity, newstate):
        fmtstr = "this is {}, changing state of {} to {!r}"
        print fmtstr.format(self, state_entity, newstate)