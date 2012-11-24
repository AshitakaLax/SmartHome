_imports = ["smarthomeitem",
            "container", 
            "universe", 
            "area",
            "physicaldevice",
            "events",
            "state_entity"]

__all__ = []

def _importstuff():
    from importlib import import_module
    for modulename in _imports:
        print(__package__)
        module = import_module("._core." + modulename, __package__)
        for name in module.__all__:
            thing = getattr(module, name)
            if hasattr(thing, "__module__"):
                thing.__module__ = __package__ + "." + __name__
            globals()[name] = thing
            __all__.append(name)

_importstuff()