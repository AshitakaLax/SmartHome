from .universe import createstuff

universe = createstuff()

from pprint import pprint

lab = universe["MEB"]["level 3"]["Room 3143"]
print lab
print list(lab)

pprint(universe.objects_dict)

print list(universe)

print universe["MEB"][3][3143]["power strip"]

print
for x in universe.objects_dict.values():
    print x, repr(x.path)