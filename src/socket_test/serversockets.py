import socket

import threading
import pdb

class _Cell(object):
    """
    Cell([initialvalue]) -> obj
    
    Python 2, unlike Python 3, does not allow a function to assign a new 
    object to a variable in an enclosing function, but it does allow 
    modifying the object referenced by a such a variable.
    
    This class creates cell objects, each of which stores a modifiable 
    reference to some object.
    
    initialvalue: (object) the first object reference to store 
                           -- optional (defaults to None)
    
    Instance attribute:
    
    value: (object) the stored value reference
    """
    
    def __init__(self, initialvalue=None):
        self.value = initialvalue


class SocketOpeningError(Exception):
    pass


REQ_SEP = "\n"


class HostConnectionThread(threading.Thread):
    """
    HostConnectionThread(port, processfunc[, threadname]) -> obj
    
    A thread to accept a socket connection on <port>, read from it, 
    process the data with <processfunc> and send the function's return value
    (must be a str) back on the socket.
    
    port: (int) the socket port number
    processfunc: (function) function taking one argument (str) and returning a
                            str
    
    After instantiating this class, call the method start() on the object to
    start the thread.
    """
    
    def __init__(self, port, processfunc, threadname="hostconnection"):
        threading.Thread.__init__(self, name=threadname)
        self._port = port
        self._processfunc = processfunc
        # defined by Thread; tells Thread to automatically quit this thread
        # when no other non-daemon threads remain
        self.daemon = True
    
    def run(self):
        HOST = None  # Symbolic name meaning all available interfaces
        sock = None
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
                sock = s
                break
    
        if not sock:
            raise SocketOpeningError("could not open socket")
        
        alldata = _Cell('')
        def receiveall(conn):
            recv = None
            while recv != '' and REQ_SEP not in alldata.value:
                recv = conn.recv(1024)
                alldata.value += recv
                conn.send(recv)
            if REQ_SEP in alldata.value:
                (first, sep, rest) = alldata.value.partition(REQ_SEP)
                alldata.value = rest
                return first
            return alldata.value
        
        try:
            (conn, addr) = sock.accept()
            print "Connected by", addr
            print conn, type(conn)
            try:
                data = receiveall(conn)
                response = self._processfunc(data)
                assert isinstance(response, str)
                conn.sendall(response)
            finally:
                conn.close()
        finally:
            sock.close()
            

def test():
    def echodata(data):
        print "got this:", repr(data)
        return data
    thread = HostConnectionThread(50001, echodata)
    thread.start()
    thread.join()
    #thread.run()
    

if __name__ == '__main__':
    import pdb
    #pdb.set_trace()
    test()
