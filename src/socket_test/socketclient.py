# Echo client program
import socket
import sys
import time

errs = []
def test():
    #HOST = "127.0.0.1"    # The remote host
    #HOST = "155.99.163.104"
    HOST = "155.98.69.177"
    PORT = 50001              # The same port as used by the server
    sock = None
    for res in socket.getaddrinfo(HOST, PORT, socket.AF_UNSPEC, 
                                              socket.SOCK_STREAM):
        print repr(res)
        (af, socktype, proto, canonname, sa) = res
        s = None
        try:
            s = socket.socket(af, socktype, proto)
            s.connect(sa)
        except socket.error as e:
            print e
            if s:
                s.close()
        else:
            sock = s
            break
            
    if sock is None:
        print "could not open socket"
        return 1
    sock.sendall("Hello, world")
    data = s.recv(1024)
    print "Received", repr(data)
    #print "sleeping..."
    #time.sleep(15)
    sock.close()
    return 0

if __name__ == "__main__":
    test()
