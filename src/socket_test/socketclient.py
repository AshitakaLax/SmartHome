# Echo client program
import socket
import sys
import time

def test():
    HOST = "127.0.0.1"    # The remote host
    PORT = 50001              # The same port as used by the server
    s = None
    for res in socket.getaddrinfo(HOST, PORT, socket.AF_UNSPEC, 
                                              socket.SOCK_STREAM):
        print repr(res)
        (af, socktype, proto, canonname, sa) = res
        try:
            s = socket.socket(af, socktype, proto)
        except socket.error:
            s = None
            continue
        try:
            s.connect(sa)
            break
        except socket.error:
            s.close()
            s = None
    if s is None:
        print "could not open socket"
        return 1
    s.sendall("Hello, world")
    data = s.recv(1024)
    print "Received", repr(data)
    #print "sleeping..."
    #time.sleep(15)
    s.close()
    return 0

if __name__ == "__main__":
    test()