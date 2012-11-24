def renamemodule(clsorfunc):
    assert hasattr(clsorfunc, "__module__")
    clsorfunc.__module__ = __package__
    return clsorfunc