import sys

from ._rename import renamemodule
from ._lock import Lock

__all__ = ["printfunc", "printlock"]


printlock = Lock()

@renamemodule
def printfunc(*args, **kwargs):
    assert printlock.islocked
    sep = kwargs.get("sep")
    sep = sep if sep is not None else " "
    end = kwargs.get("end")
    end = end if end is not None else "\n"
    sys.stdout.write(sep.join(map(str, args)) + end)
    sys.stdout.flush()