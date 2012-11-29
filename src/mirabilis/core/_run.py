import socket

import threading
from datetime import datetime, timedelta
import time
import pdb
import cPickle as cpickle  # ugly camel case!

from ._print import printlock, printfunc


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
        self._universe = universe
        self._schedule = {}  # dict of (next polling time)[device or function]
        self._devicethreads = {}
        # must have lock to modify _schedule or _devicethreads
        self._schedulelock = threading.Lock()

        self._serversocket = None
        self._serverport = 5348 #  ASCII codes for SH (Smart Home) in hex
        self._serverthread = threading.Thread(target=self._runserver, 
                                              name="server")
    
    def run(self):
        self._serverthread.start()
        
        with self._schedulelock:
            for device in self._universe.devices:
                self._schedule[device] = datetime.now()
        
        number = 1
        while True:
            with printlock:
                printfunc("main thread: running main loop", number)
            self._mainloopiteration(number)
            with printlock:
                printfunc("main thread: sleeping 10 seconds")
            time.sleep(10.0)
            number += 1
    
    def _updatedevicereschedule(self, device):
        with printlock:
            printfunc("in thread to update device", device)
        device.update_entities()
        with self._schedulelock:
            delta = timedelta(0, device.pollinginterval)
            self._schedule[device] = datetime.now() + delta
            del self._devicethreads[device]
        with printlock:
            printfunc("in thread done updating device", device)
            
    
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
        def receiveall(conn):
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
                data = receiveall(conn)
                response = self._processrequest(data)
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
        with self._universe.readlock:
            return cpickle.dumps(self._universe, 2)
        
    def _mainloopiteration(self, number):
        with printlock:
            printfunc("starting main loop iteration #{}".format(number))
            printfunc("schedule is {}".format(self._schedule))
            theadcount = len(self._devicethreads)
            printfunc("there are {} device threads".format(theadcount))
            for thread in self._devicethreads.values():
                printfunc("{}: {}".format(thread, 
                                          "alive" if thread.is_alive() 
                                          else "dead"))
        servicedevices = set()
        now = datetime.now()
        with printlock:
            printfunc("preparing to get schedule lock... ", end="")
        with self._schedulelock:
            print "got lock"
            with printlock:
                printfunc("got schedule lock")
            for device, time in self._schedule.items():
                if time - now <= timedelta(0):
                    with printlock:
                        printfunc("time for device", device, "to be updated")
                    del self._schedule[device]
                    servicedevices.add(device)
                    threadname = "thread for {}".format(device)
                    target = self._updatedevicereschedule
                    thread = threading.Thread(name=threadname,
                                              target=target,
                                              args=[device])
                    assert device not in self._devicethreads.keys()
                    self._devicethreads[device] = thread
            with printlock:
                printfunc("releasing schedule lock")
        for device in servicedevices:
            with printlock:
                printfunc("starting thread for device", device)
            self._devicethreads[device].start()
