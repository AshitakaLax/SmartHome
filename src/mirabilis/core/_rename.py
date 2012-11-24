import abc  # bug workaround: have to import something to get __package__
del abc


__all__ = []


def renamemodule(clsorfunc):
    assert hasattr(clsorfunc, "__module__")
    assert __package__
    clsorfunc.__module__ = __package__
    return clsorfunc