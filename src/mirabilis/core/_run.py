import socket

import threading
from datetime import datetime, timedelta
import time
import pdb
import cPickle as cpickle  # ugly camel case!
import re
import traceback

from ._print import printsync
from ._state_entity import *



class SocketOpeningError(Exception):
    pass


class _Cell(object):
    """
    _Cell([initialvalue]) -> obj
    
    initialvalue: (object) the first object reference to store 
                           -- optional (defaults to None)
    
    Instance attribute:
    
    value: (object) the stored value reference
    """
    
    def __init__(self, initialvalue=None):
        self.value = initialvalue



REQUEST_SEP = "\n"


class Runner(object):
    def __init__(self, universe):
        self._mainverbose = False
        self._deviceverbose = False
        
        self._checkinterval = 0.1
        
        self._universe = universe
        self._schedule = {}  # dict of (next polling time)[device or function]

        # must have lock to modify _schedule or _devicethreads
        self._devicethreads = {}
        self._schedulelock = threading.Lock()

        self._serversocket = None
        self._serverport = 5348 #  ASCII codes for SH (Smart Home) in hex
        self._serverthread = threading.Thread(target=self._runserver, 
                                              name="server")
    
    def run(self):
        self._serverthread.daemon = True
        self._serverthread.start()
        
        with self._schedulelock:
            for device in self._universe.devices:
                self._schedule[device] = datetime.now()
        
        number = 1
        while True:
            try:
                if self._mainverbose:
                    printsync("\nMAIN: running main loop #{}".format(number))
                self._mainloopiteration()
                if self._mainverbose:
                    msg = "MAIN: sleeping {} second(s)"
                    printsync(msg.format(self._checkinterval))
                time.sleep(self._checkinterval)
                number += 1
            except KeyboardInterrupt:
                print
                break
    
    def _updatedevicereschedule(self, device):
        if self._deviceverbose:
            msg = "\nDEVICE: {}: in thread to update device"
            printsync(msg.format(device))
        device.update_entities()
        if self._deviceverbose:
            msg = "DEVICE: {}: just finished updating device"
            printsync(msg.format(device))
        with self._schedulelock:
            delta = timedelta(0, device.pollinginterval)
            self._schedule[device] = datetime.now() + delta
            del self._devicethreads[device]
        if self._deviceverbose:
            printsync("DEVICE: {}: rescheduled device".format(device))
    
    def _runserver(self):
        while True:
            self._serve()
            time.sleep(0.1)
    
    def _prepserversocket(self):
        assert not self._serversocket
        
        HOST = None  # Symbolic name meaning all available interfaces
        for res in socket.getaddrinfo(HOST, 
                                      self._serverport, 
                                      socket.AF_UNSPEC,
                                      socket.SOCK_STREAM, 
                                      0, 
                                      socket.AI_PASSIVE):
            (af, socktype, proto, canonname, sa) = res
            s = None
            try:
                s = socket.socket(af, socktype, proto)
                s.bind(sa)
                s.listen(1)
            except socket.error:
                if s:
                    s.close()
            else:
                assert s
                self._serversocket = s
                break
    
        if not self._serversocket:
            raise SocketOpeningError("could not open socket")
        
    
    def _serve(self):
        if not self._serversocket:
            self._prepserversocket()
        
        alldata = _Cell('')
        def receive_all(conn):
            recv = None
            while recv != '' and REQUEST_SEP not in alldata.value:
                recv = conn.recv(1024)
                alldata.value += recv
            if REQUEST_SEP in alldata.value:
                (first, sep, rest) = alldata.value.partition(REQUEST_SEP)
                alldata.value = rest
                return first
            return alldata.value
        
        try:
            (conn, addr) = self._serversocket.accept()
            try:
                data = receive_all(conn)
                try:
                    response = self._processrequest(data)
                except:
                    print "ERROR processing request"
                    traceback.print_exc()
                    response = "FAILURE: error while processing"
                assert isinstance(response, str)
                conn.sendall(response)
            finally:
                conn.close()
        except:
            if self._serversocket:
                self._serversocket.close()
                self._serversocket = None
            raise
    
    def _processrequest(self, data):
        printsync("SERVER: got request:\n{!r}".format(data))
        while True: # used for goto
            if data == "dump_pickle":
                with self._universe.readlock:
                    ret = "SUCCESS: dumping pickle\n" + \
                        cpickle.dumps(self._universe, 2)
                printsync("SERVER: returning:\n<<<PICKLE DUMP>>>")
                return ret
            m = re.match("write_entity (\d+):(\d+):", data)
            if m:
                global_id = int(m.group(1))
                newvaluelen = int(m.group(2))
                newvalue = data[m.end():]
                if newvaluelen != len(newvalue):
                    ret = "FAILURE: new value did not match specified " \
                        "length (write_entity)"
                    break
                item = self._universe.object_with_global_id(global_id)
                if not isinstance(item, (WStateEntity, RWStateEntity)):
                    ret = "FAILURE: item is not writable (write_entity)"
                    break
                try:
                    item.write(newvalue)
                except:
                    print "FAILED in write:"
                    traceback.print_exc()
                    ret = "FAILURE: an error occured while performing the " \
                        "write (write_entity)"
                    break
                else:
                    msg = "SUCCESS: wrote {!r} to item {} at {!r}"
                    ret = msg.format(newvalue, item, item.path)
                    break
                assert False, "not reached"
            else:
                return "FAILURE: unrecognized command:\n" + data
        assert ret
        printsync("SERVER: returning:\n{!r}".format(ret))
        return ret
        
    def _mainloopiteration(self):
        if self._mainverbose:
            printsync("MAIN: schedule is {}".format(self._schedule))
            theadcount = len(self._devicethreads)
            printsync("MAIN: there are {} device threads".format(theadcount))
            for thread in self._devicethreads.values():
                printsync("MAIN: {}: {}".format(thread, 
                                          "alive" if thread.is_alive() 
                                          else "dead"))
        servicedevices = set()
        now = datetime.now()
        if self._mainverbose:
            printsync("MAIN: preparing to get schedule lock... ", end="")
        with self._schedulelock:
            if self._mainverbose:
                printsync("MAIN: got schedule lock")
            for device, time in self._schedule.items():
                if now - time >= timedelta(0):
                    if self._mainverbose:
                        printsync("MAIN: time for device", device, "to be updated")
                    del self._schedule[device]
                    servicedevices.add(device)
                    threadname = "thread for {}".format(device)
                    target = self._updatedevicereschedule
                    thread = threading.Thread(name=threadname,
                                              target=target,
                                              args=[device])
                    thread.daemon = True
                    assert device not in self._devicethreads.keys()
                    self._devicethreads[device] = thread
            if self._mainverbose:
                printsync("MAIN: releasing schedule lock")
        for device in servicedevices:
            if self._mainverbose:
                printsync("MAIN: starting thread for device", device)
            self._devicethreads[device].start()
