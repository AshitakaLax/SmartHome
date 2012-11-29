#!/usr/bin/env python2.7
import os
import sys

if __name__ == "__main__":
    newpath = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    #print "newpath is", repr(newpath)
    sys.path.insert(0, newpath)
    #print "tweaked path is as follows:"
    #print repr(sys.path)
    
    os.environ.setdefault("DJANGO_SETTINGS_MODULE", "ladybug.settings")
    
    from django.core.management import execute_from_command_line

    execute_from_command_line(sys.argv)
