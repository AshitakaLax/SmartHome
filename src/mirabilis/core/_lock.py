from ._rename import renamemodule
import threading

__all__ = ["Lock"]


@renamemodule
class Lock(object):
    """
    Lock() -> obj
    
    Create a lock similar to that returned by threading.Lock().
    
    In lieu of an acquire function with a boolean argument telling whether to
    block, these Lock objects provide an islocked property that tells whether
    calling acquire, which takes no arguments, would block.
    
    This class enforces that release() be called by the same thread that called
    acquire().
    
    A Lock object may be used with a "with" statement like so:
    
        with <lockobject>:
            <code>
        # atumotically calls acquire() at the start of the block and release() 
        # at the end (even if an exception occurs during the body)
    
    The Lock object may also be explicitly released like so:
    
        <lockobject>.acquire()
        <code>
        .
        .
        .
        <lockobject>.release()
    """
    
    def __init__(self):
        self._lock = threading.Lock()
        self._lockingthread = None
        self._finalizing = False
    
    def __del__(self):
        """
        obj.__del__()
        
        releases the lock prior to being deleted
        """
        if self.islocked:
            self._finalizing = True
            self._islocked = False
            self._lockingthread = None
            self._lock.release()
    
    @property
    def islocked(self):
        """
        obj.islocked -> (bool) 
        
        A return value of True means calling acquire() will block.  Also a 
        return value of False means calling release will raise an error.
        """
        return self._lockingthread is not None
    
    def acquire(self):
        """
        obj.acquire() -> (Lock)
        
        Acquire the lock for the current thread and return the same Lock 
        object.
        """
        assert not self._finalizing, "can't acquire Lock during finalization"
        if self.islocked and self._lockingthread == threading.current_thread():
            raise RuntimeError("thread trying to acquire lock multiple times")
        self._lock.acquire(True)  # block
        self._lockingthread = threading.current_thread()
        return self
    
    def release(self):
        """
        obj.release()
        
        Release the lock, which must have been acquried by the same thread
        calling release().
        """
        assert self._lockingthread == threading.current_thread()
        self._lockingthread = None
        self._lock.release()
    
    def __enter__(self):
        """
        obj.__enter__() -> (Lock)
        
        calls obj.acquire() and returns obj
        """
        self.acquire()
        return self
    
    def __exit__(self, exc_type, exc_value, traceback):
        """
        obj.__exit__(exc_type, exc_value, traceback)
        
        calls obj.release()
        
        exc_type: (type or <None>) the exception type, a subclass of 
                                   BaseException
        exc_value: (BaseException or <None>) the exception value
        traceback: (traceback or <None>) the traceback of the exception
        """
        self.release()
