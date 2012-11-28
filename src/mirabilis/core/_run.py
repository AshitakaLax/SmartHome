import socket

import threading
import time
import datetime
import pdb


class SocketOpeningError(Exception):
    pass


class _Cell(object):
    """
    Cell([initialvalue]) -> obj
    
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
        self.universe = universe
        self.schedule = {}  # dict of (device or function)[next polling time]
        self.serversocket = None
        self.serverthread = Thread(target=self.runserver, name="server")
    
    def run(self):
        #timers = {}
        #for d in self.devices:
        #    timer[d] = threading.Timer(d.pollinginterval,
        #                               d.update_entities)
        #
        #for t in timers.itervalues():
        #    t.start()
        self.serverthread.start()
        self.mainloop()
    
    def runserver(self):
        while True:
            self.serve()
            time.sleep(0.1)
    
    def prepserversocket(self):
        assert not self.serversocket
        
        HOST = None  # Symbolic name meaning all available interfaces
        for res in socket.getaddrinfo(HOST, 
                                      self._port, 
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
                self.serversocket = s
                break
    
        if not self.serversocket:
            raise SocketOpeningError("could not open socket")
        
    
    def serve(self):
        if not self.serversocket:
            self.prepserversocket()
        
        alldata = _Cell('')
        def receiveall(conn):
            recv = None
            while recv != '' and REQUEST_SEP not in alldata.value:
                recv = conn.recv(1024)
                print "recv was", repr(recv)
                alldata.value += recv
                conn.send(recv)
            if REQUEST_SEP in alldata.value:
                (first, sep, rest) = alldata.value.partition(REQUEST_SEP)
                alldata.value = rest
                print "returning", repr(first)
                return first
            return alldata.value
        
        try:
            (conn, addr) = self.serversocket.accept()
            try:
                data = receiveall(conn)
                response = self.processrequest(data)
                assert isinstance(response, str)
                conn.sendall(response)
            finally:
                conn.close()
        except:
            self.serversocket.close()
            self.serversocket = None
            raise
    
    def processrequest(self, data):
        raise NotImplementedError()
        
    def mainloop(mainloopinfo):
         # 2 things to do:
         # check schedule and start threads to poll devices
         # read socket and response to commands