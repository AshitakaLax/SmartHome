import socket

import threading
import time
import datetime
import pdb
import cPickle as cpickle  # ugly camel case!


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
        self._schedule = {}  # dict of (device or function)[next polling time]
        self._serversocket = None
        self._serverport = 5348 #  ASCII codes for SH (Smart Home) in hex
        self._serverthread = threading.Thread(target=self._runserver, 
                                              name="server")
    
    def run(self):
        #timers = {}
        #for d in self.devices:
        #    timer[d] = threading.Timer(d.pollinginterval,
        #                               d.update_entities)
        #
        #for t in timers.itervalues():
        #    t.start()
        self._serverthread.start()
        self._mainloop()
    
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
            print res
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
                print "recv was", repr(recv)
                alldata.value += recv
            if REQUEST_SEP in alldata.value:
                (first, sep, rest) = alldata.value.partition(REQUEST_SEP)
                alldata.value = rest
                print "returning", repr(first)
                return first
            return alldata.value
        
        try:
            (conn, addr) = self._serversocket.accept()
            try:
                data = receiveall(conn)
                response = self._processrequest(data)
                assert isinstance(response, str)
                print "response is", repr(response)
                conn.sendall(response)
            finally:
                conn.close()
        except:
            if self._serversocket:
                self._serversocket.close()
                self._serversocket = None
            raise
    
    def _processrequest(self, data):
        print "_processrequest; data is", repr(data)
        with self._universe.readlock:
            return cpickle.dumps(self._universe, 2)
        
    def _mainloop(mainloopinfo):
        time.sleep(10000)
         # 2 things to do:
         # check schedule and start threads to poll devices
         # read socket and response to commands