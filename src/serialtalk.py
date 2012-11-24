import subprocess

#with open("/dev/cu.usbmodem12341", "w") as serial:
#    #print(serial)
#    #print(serial.read())
#    serial.write(raw_input("goahead"))
#    #print(serial.read())


ret = subprocess.check_output(["cat", "/dev/cu.usbmodem12341"])

print ret
print "done"