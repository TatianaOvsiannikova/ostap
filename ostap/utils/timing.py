#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
# @file
# Set of useful utilities
# =============================================================================
"""Set of useful utilisties for timing
"""
# =============================================================================
__all__     = (
    'timing'         , ## context manager to count time 
    'timer'          , ## ditto
    'Timer'          , ## context manager to count time 
   )
# =============================================================================
from   ostap.logger.logger import getLogger
if '__main__' ==  __name__ : logger = getLogger ( 'ostap.utils.timing' )
else                       : logger = getLogger ( __name__             )
_logger_t = type ( logger )
del getLogger
# =============================================================================
from timeit import  default_timer as _timer 
# ==============================================================================
## @class Timer
#  Simple context manager to measure the time 
#  @code
#  with Timer() :
#     ... whatever action is here 
#     ... at the exit it prints the time 
#  @endcode
# Or:
#  @code
#  with Timer() as t :
#     ... whatever action is here 
#     ... at the exit it prints the clock counts 
#  print t.delta
#  @endcode
#  Print to specified logger :
#  @code
#  logger = ...
#  wtih Timer ( logger = logger ) : ... 
#  @endcode 
#  Print to specified logger level  :
#  @code
#  logger = ...
#  wtih Timer ( logger = logger.error  ) : ... 
#  @endcode
#  no print at all
#  @code
#  logger = ...
#  wtih Timer ( logger = lambda *s : '' ) : ... 
#  @endcode
class Timer(object):
    """Simple context manager to measure the time
    
    >>> with Timer() :
    ...  whatever action is
    at the exit it prints the time 
    
    Or:
    
    >>> with Timer() as t :
    ...  whatever action is
    at the exit it prints the clock counts 
    
    >>> print t.delta

    - Print to specified logger :
    >>> logger = ...
    >>> with Timer ( logger = logger ) : ... 

    - Print to specified logger level  :
    >>> logger = ...
    >>> with Timer ( logger = logger.error  ) : ...
    
    - no print at all
    >>> logger = ...
    >>> with Timer ( logger = lambda *s : '' ) : ... 
    """
    __logger = logger.info
    ##
    def __init__  ( self                          ,
                    name   = ''                   ,
                    logger = None                 ,
                    format = 'Timing %-18s %.3fs' ,
                    start  = ''                   ) :        
        self.name   = name
        
        if   logger and isinstance ( logger , _logger_t ) :
            self.logger = logger.info
        elif logger and callable   ( logger ) :
            self.logger = logger
        else :
            self.logger = self.__logger 

        self.format        = format
        self.start_message = start 
        
    def __enter__ ( self ) :
        self.start = _timer ()
        if self.start_message :
            self.logger ( self.start_message )
        return self
    
    def __exit__  ( self, *_ ) :
        self.stop  = _timer ()
        self.delta = self.stop - self.start         
        try :
            message = self.format       % ( self.name , self.delta ) 
        except TypeError :
            message = 'Timing %-18s %s' % ( self.name , self.delta )

        self.logger ( message )
            
# =============================================================================
## Simple context manager to measure the time
#
#  @code
#  with timer () :
#     ... whatever action is here 
#     ... at the exit it prints the time
#  @endcode
#
# Or: 
#
#  @code
#  with timer () as t :
#     ... whatever action is here 
#     ... at the exit it prints the clock counts 
#  print t.delta 
#  @endcode
def timing ( name = '' , logger = None , format = 'Timing %-18s %.3fs' , **kwargs  ) :
    """Simple context manager to measure the clock counts 
    
    >>> with timing () :
    ...   whatever action is here
    at the exit it prints the clock counts 
    
    >>> with timing () as c :
    ...   whatever action is here 
    at the exit it prints the clock counts 
    
    >>> print c.delta
    """
    return Timer ( name , logger , format , **kwargs ) 

## ditto 
timer = timing   # ditto

# =============================================================================
if '__main__' == __name__ :
    
    with timer ( logger = logger ) :
        from ostap.utils.docme import docme
        docme ( __name__ , logger = logger )
        
# =============================================================================
# The END 
# =============================================================================
