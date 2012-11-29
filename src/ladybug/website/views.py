# Create your views here.

from mako.template import Template
from mako.lookup import TemplateLookup

from mako_django import render, render_to_response, get_template

from django.template import Context, RequestContext

from django.http import (HttpResponse, 
                         HttpResponseRedirect, 
                         HttpResponseForbidden)

import cPickle as cpickle  # ugly camelcase
import socket


def connect(command):
    HOST = "127.0.0.1"    # The remote host
    #HOST = "155.99.163.104"
    #HOST = "155.98.69.177"
    PORT = 5348              # The same port as used by the server
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
        raise RuntimeError("could not open socket")
    sock.sendall(command + "\n")
    data = ''
    recvd = True
    while recvd:
        recvd = s.recv(1024)
        data += recvd
    print "Received:"
    print
    print repr(data)
    #print "sleeping..."
    #time.sleep(15)
    sock.close()
    return data


def index(request):
    raise NotImplementedError("index not implemented")
    

def view(request):
    pickledata = connect("dump_pickle")
    universe = cpickle.loads(pickledata)
    #return HttpResponse(repr(universe))
    return render_to_response("/base/basic.mako", 
                              {"universe": universe},
                              RequestContext(request))
                              


def change(request):
    raise NotImplementedError("change not implemented")
