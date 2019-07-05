#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
## @file ostap/utils/cidict.py
#  Case insensitive dictionary
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2019-07-03
# =============================================================================
"""Case insensitive dictionary
"""
# =============================================================================
__version__ = "$Revision$"
__author__  = "Vanya BELYAEV Ivan.Belyaev@itep.ru"
__date__    = "2013-02-10"
# =============================================================================
__all__     = (
    'cidict'   , ## case-insensitive dictionary
    )
# =============================================================================
from sys import version_info as python_version
if python_version.major >= 3  : from collections.abc import  MutableMapping
else                          : from collections     import  MutableMapping
# =============================================================================
## @class cidict
#  Case-insensitive dictionary
#  @code
#  d = cidict ( a = 1 , A = 2 , b = 1 , B = 2 )
#  @endcode
#  Other key transformations are also possible:
#  @code
#  d = cidict ( a_1 = 1 , a1 = 2 ,
#               A_1 = 3 , a___1 = 4 ,
#               transform = lambda k : k.lower().replace('_','') )
#  @endcode 
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2019-07-03
class cidict(MutableMapping) :
    """Case-insensitive dictionary
    >>> d = cidict ( a = 1 , A = 2 , b = 1 , B = 2 )

    Other key transformations are also possible:
    >>> d = cidict ( a_1 = 1 , a1 = 2 ,
    ...              A_1 = 3 , a___1 = 4 ,
    ...              transform = lambda k : k.lower().replace('_','') )
    """
    
    def __init__ ( self                           ,
                   dct       = {}                 ,
                   transform = lambda k : k.lower ,
                   **kwargs ) :

        self.__transform = transform 
        self.__store     = {}
        
        dtmp = dict ( dct )
        for k in dtmp :
            kk = self.the_key( k )
            self.__store [ kk ] = dtmp [ k ]
            
        for k in kwargs  :
            
            kk = self.the_key( k )
            self.__store [ kk ] = kwargs [ k ]

    def the_key ( self , key ) :
        return self.__transform ( key )
    
    def __getitem__ ( self , key ) :
        return self.__store[ self.the_key ( key )  ]
    
    def __setitem__ ( self , key , value ) :
        self.__store[ self.the_key ( key )  ] = value
    
    def __delitem__ ( self , key ) :
        del self.__store[ self.the_key ( key ) ] 

    def __iter__ ( self ) : return iter ( self.__store ) 
    def __len__  ( self ) : return len  ( self.__store )
    def __repr__ ( self ) : return repr ( self.__store ) 
    def __str__  ( self ) : return str  ( self.__store ) 


# =============================================================================
if '__main__' == __name__ :
    
    from   ostap.logger.logger import getLogger
    logger = getLogger( 'ostap.utils.cidict' )
    
    from ostap.utils.docme import docme
    docme ( __name__ , logger = logger )

# =============================================================================
##                                                                      The END 
# =============================================================================

