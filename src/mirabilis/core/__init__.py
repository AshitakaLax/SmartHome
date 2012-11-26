"""
package mirabilis.core

This package defines the core objects needed on which to build an automation 
ecosystem.

The package is meant to be used as though it were a single module.

All submodules in this package begin with an underscore and are intended for
internal use only--do not import them.
"""

_imports = ["_smarthomeitem",
            "_container", 
            "_universe", 
            "_areagroup",
            "_physicaldevice",
            "_events",
            "_state_entity"]

__all__ = []


def _importstuff():
    from importlib import import_module
    for modulename in _imports:
        module = import_module("." + modulename, __package__)
        del globals()[modulename]
        for name in module.__all__:
            thing = getattr(module, name)
            #if hasattr(thing, "__module__"):
            #    thing.__module__ = __package__
            globals()[name] = thing
            __all__.append(name)


_importstuff()
