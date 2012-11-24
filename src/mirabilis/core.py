_imports = ["_smarthomeitem",
            "_container", 
            "_universe", 
            "_area",
            "_physicaldevice",
            "_events",
            "_state_entity"]

__all__ = []

def _importstuff():
    from importlib import import_module
    for modulename in _imports:
        module = import_module("." + modulename, __package__)
        for name in module.__all__:
            globals()[name] = getattr(module, name)
            __all__.append(name)

_importstuff()