// ============================================================================
// Include files 
// ============================================================================
//  STD&STL
// ============================================================================
// Ostap
// ============================================================================
#include "Ostap/Moments.h"
// ============================================================================
// Local
// ============================================================================
#include  "local_math.h"
// ============================================================================
/** @file 
 *  Implementation file for classes from the file Ostap/Moments.h
 *  @date 2020-06-07 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */
// ============================================================================
/*  @var s_INVALID_CENTRAL_MOMENT ; 
 *  the invalid value of the central moment 
 */ 
// ============================================================================
const double Ostap::Math::Moments::s_INVALID_MOMENT = -1 * s_INFINITY ;
// ===========================================================================
// virtual destructor
// ===========================================================================
Ostap::Math::Moment::~Moment(){}
// ===========================================================================


// int test()
// {
//   Ostap::Math::Moment_<5> m1 ;
//   for ( unsigned i = 0 ; i < 100 ; ++i ) { m1 += i ; }
//   //
//   m1.M(0) ;
//   m1.M(1) ;
//   m1.M(2) ;
//   Ostap::Math::Moment_<5> m2 ;
//   for ( unsigned i = 0 ; i < 100 ; ++i ) { m2 += i ; }
//   //
//   m2.M(0) ;
//   m2.M(1) ;
//   m2.M(2) ;
//   Ostap::Math::Moment_<2> m4 ;
//   for ( unsigned i = 0 ; i < 100 ; ++i ) { m4 += i ; }
//   const auto m3  = m1 + m2 ;
//   const auto a1 = Ostap::Math::Moments::central_moment<2> ( m3 ) ;
//   const auto a2 = Ostap::Math::Moments::central_moment<2> ( m4 ) ;
//   return 0 ;
// }


// ============================================================================
//                                                                      The END 
// ============================================================================
