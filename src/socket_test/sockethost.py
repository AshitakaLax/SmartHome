# Echo server program
import socket
import sys

def test():
    HOST = None               # Symbolic name meaning all available interfaces
    PORT = 50001              # Arbitrary non-privileged port
    s = None
    for res in socket.getaddrinfo(HOST, PORT, socket.AF_UNSPEC,
                                  socket.SOCK_STREAM, 0, socket.AI_PASSIVE):
        print repr(res)
        (af, socktype, proto, canonname, sa) = res
        try:
            s = socket.socket(af, socktype, proto)
        except socket.error:
            s = None
            continue
        try:
            s.bind(sa)
            s.listen(1)
            break
        except socket.error:
            s.close()
            s = None
    if s is None:
        print "could not open socket"
        return 1
    (conn, addr) = s.accept()
    print "Connected by", addr
    while True:
        data = conn.recv(1024)
        if not data: 
            break
        conn.send(data)
    conn.close()
    return 0
    
if __name__ == "__main__":
    test()