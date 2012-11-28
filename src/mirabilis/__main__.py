def main():
    from .create import create
    
    import cPickle

    universe = create()
    
    from pprint import pprint
    
    doprint = False
    
    lab = universe["MEB"]["level 3"]["Room 3143"]
    if doprint:
        print "lab:"
        print lab
    
        print "\nlist(lab):"
        pprint(list(lab))

        print "\npprint(universe.objectdict):"
        pprint(universe.objectsdict)

        print "\nlist(universe):"
        print list(universe)
    
        print "\nuniverse[\"MEB\"][3][3143][\"power strip and sensors\"]:"
        print universe["MEB"][3][3143]["power strip and sensors"]

        print "\nfor x in universe.objectsdict.values():"
        for x in universe.objectsdict.values():
            print repr(x.path) + "\t\t" + str(x)
    
    filename = "./universe.pickle"
    with open(filename, "wb") as file:
        cPickle.dump(universe, file, 2)
        
    print "Successfully dumped to", repr(filename) + "."


def main2():
    from .create import create
    
    universe = create()
    
    universe.run()


if __name__ == "__main__":
    #main()
    main2()