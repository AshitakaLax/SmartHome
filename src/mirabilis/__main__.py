def main():
    from .create import create

    universe = create()

    from pprint import pprint

    lab = universe["MEB"]["level 3"]["Room 3143"]
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


if __name__ == "__main__":
    main()