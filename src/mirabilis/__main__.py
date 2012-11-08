from .create import create

universe = create()

from pprint import pprint

lab = universe["MEB"]["level 3"]["Room 3143"]
print lab
print list(lab)

pprint(universe.objects_dict)

print list(universe)

universe["MEB"][3][3143]["power strip and sensors"].update_entities()

print universe["MEB"][3][3143]["power strip and sensors"]

print
for x in universe.objects_dict.values():
    print x, repr(x.path)
