import sys

from ._rename import renamemodule
from ._lock import Lock

__all__ = ["printsync"]


_printlock = Lock()

@renamemodule
def printsync(*args, **kwargs):
    with _printlock:
        sep = kwargs.get("sep")
        sep = sep if sep is not None else " "
        end = kwargs.get("end")
        end = end if end is not None else "\n"
        sys.stdout.write(sep.join(map(str, args)) + end)
        sys.stdout.flush()