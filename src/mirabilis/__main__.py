def main():
    from .create import create

    universe = create()

    from pprint import pprint

    lab = universe["MEB"]["level 3"]["Room 3143"]
    print lab
    print list(lab)

    pprint(universe.objectsdict)

    print list(universe)

    print universe["MEB"][3][3143]["power strip and sensors"]

    print
    for x in universe.objectsdict.values():
        print x, repr(x.path)


if __name__ == "__main__":
    main()