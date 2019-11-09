// ============================================================================
#ifndef OSTAP_DATAFRAME_H 
#define OSTAP_DATAFRAME_H 1
// ============================================================================
// Include files
// ============================================================================
// ROOT
// ============================================================================
#include "RVersion.h"   // ROOT 
#include "RtypesCore.h" // ROOT 
// ============================================================================
// ROOT::ROOT
// ============================================================================
/** @file Ostap/DataFrame.h
 *  the   first attemps to use ROOT (R,T)DataFrame
 */
// ============================================================================
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,14,0)
// ============================================================================
namespace ROOT  { class         RDataFrame            ; 
  namespace RDF { template <class T> class RResultPtr ; } 
}
// ============================================================================
namespace Ostap { 
  // ==========================================================================
  /** @typename DataFrame 
   *  the actual type of data-frame
   *  @see ROOT::RDataFrame 
   */
  typedef ROOT::RDataFrame                 DataFrame ;
  // ==========================================================================
  /** @typename DFCount
   *  the result type of RDataFrame::Count 
   *  @see ROOT::RDataFrame 
   */
  typedef ROOT::RDF::RResultPtr<ULong64_t> DFCount   ;
  // ==========================================================================
} //                                                  The END of namepace Ostap
// ============================================================================
#else 
// ============================================================================
namespace ROOT  { namespace Experimental { 
    class TDataFrame ;
    namespace TDF { template <class T> class TResultProxy ; } 
  } }
// ============================================================================
namespace Ostap { 
  // ==========================================================================
  /** @typename DataFrame 
   *  the actual type of data-frame
   *  @see ROOT::Experimental::TDataFrame; 
   */
  typedef ROOT::Experimental::TDataFrame DataFrame ; 
  // ==========================================================================
  /** @typename DFCount
   *  the result type of RDataFrame::Count 
   *  @see ROOT::RDataFrame 
   */
  typedef ROOT::Experimental::TDF::TResultProxy<unsigned int > DFCount ;  
  // ==========================================================================  
} //                                                 The end of namespace Ostap
// ============================================================================
#endif
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // OSTAP_DATAFRAME_H
// ============================================================================
