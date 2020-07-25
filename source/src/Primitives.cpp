// ============================================================================
// Include files 
// ============================================================================
// Ostap
// ============================================================================
#include "Ostap/Primitives.h"
// ============================================================================
// Local
// ============================================================================
#include "Exception.h"
#include "local_math.h"
// ============================================================================
/** @file  
 *  Implementaiton file for functions from the file   Ostap/Primitives.h 
 *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
 *  @date 2020-07-25
 */
// ============================================================================
Ostap::Math::Moebius::Moebius
( const double a ,
  const double b ,
  const double c ,
  const double d )
  : m_a ( a ) 
  , m_b ( b ) 
  , m_c ( c ) 
  , m_d ( d )
{
  Ostap::Assert ( !s_zero ( m_a * m_d - m_b * m_c )    ,
                  "invalid parameters!"  , 
                  "Ostap::Math::Moebius" ) ;
}
// ======================================================================



// ============================================================================
//                                                                      The END 
// ============================================================================
