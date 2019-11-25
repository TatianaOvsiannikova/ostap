// ============================================================================
#ifndef OSTAP_MOREROOFIT_H 
#define OSTAP_MOREROOFIT_H 1
// ============================================================================
// Include files
// ============================================================================
// ROOT/RooFit 
// ============================================================================
#include "RooAddition.h"
#include "RooRealProxy.h"
// ============================================================================
namespace Ostap 
{
  // ==========================================================================
  /** @namespace Ostap::MoreRooFit   Ostap/MoreRooFit.h
   *  Collection of small additios to RooFit 
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru 
   *  @date 2019-11-21
   */
  namespace MoreRooFit 
  {
    // ========================================================================
    /** @class Subtraction 
     *  A simple modification of class RooAddition
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru 
     *  @date 2019-11-21
     */ 
    class Subtraction : public RooAddition
    {
      // ========================================================================
      ClassDef(Ostap::MoreRooFit::Subtraction , 1 ) ;  // Difference of RooAbsReal objects
      // ========================================================================
    public:
      // ========================================================================
      Subtraction () ;
      /// constructor with list of variables 
      Subtraction ( const char*        name                   , 
                    const char*        title                  , 
                    const RooArgList&  vars                   , 
                    const Bool_t       takeOwnerShip = kFALSE ) ;
      /// construtor with two variables 
      Subtraction ( const char*        name                   , 
                    const char*        title                  , 
                    RooAbsReal&        a                      , 
                    RooAbsReal&        b                      , 
                    const Bool_t       takeOwnerShip = kFALSE ) ;
      // copy 
      Subtraction ( const Subtraction& right       , 
                    const char*        newname = 0 ) ;
      //
      virtual ~Subtraction() ;
      //
      Subtraction* clone ( const char* newname ) const override ;
      //
      Double_t analyticalIntegral ( Int_t code ,
                                    const char* rangeName = 0 ) const override ;
      Double_t evaluate           () const override ;    
      // ======================================================================
    }; // 
    // ==========================================================================
    /** @class Division
     *  Evaluate \f$ \frac{a}{b}\f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru 
     *  @date 2019-11-21
     */
    class Division : public RooAbsReal
    {
      // ========================================================================
      ClassDef(Ostap::MoreRooFit::Division , 1 ) ;  // ratio of RooAbsReal objects
      // ========================================================================
    public:
      // ======================================================================
      /// constructor with two variables 
      Division  ( const char*    name  , 
                  const char*    title , 
                  RooAbsReal&    a     , 
                  RooAbsReal&    b     ) ;
      // ======================================================================
      /// default constructor 
      Division  () =  default ;
      // ======================================================================
      // copy 
      Division ( const Division& right       , 
                 const char*     newname = 0 ) ;
      // ======================================================================
      // destructor 
      virtual ~Division() ;
      // ======================================================================
      // clone method 
      Division* clone ( const char* newname ) const override ;
      // ====================================================================== 
    protected:
      // ======================================================================
      // the actual evaluation of the result 
      Double_t evaluate () const override ;    
      // ======================================================================
    protected :
      // ======================================================================
      /// a-variable
      RooRealProxy m_A {} ; // a-variable
      /// b-variable
      RooRealProxy m_B {} ; /// b-variable
      // ======================================================================
    }; //
    // ========================================================================
    /** @class Fraction
     *  Evaluate \f$ \frac{a}{a+b}\f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru 
     *  @date 2019-11-21
     */
    class Fraction : public Division
    {
      // ======================================================================
      ClassDef(Ostap::MoreRooFit::Fraction , 1 ) ;  // Fraction 
      // ======================================================================
    public:
      // ======================================================================
      /// constructor with two variables 
      Fraction  ( const char*    name  , 
                  const char*    title , 
                  RooAbsReal&    a     , 
                  RooAbsReal&    b     ) ;
      // ======================================================================
      /// default constructor 
      Fraction  () =  default ;
      // ======================================================================
      // copy 
      Fraction ( const Fraction& right       , 
                 const char*     newname = 0 ) ;
      // ======================================================================
      // destructor 
      virtual ~Fraction() ;
      // ======================================================================
      // clone method 
      Fraction* clone ( const char* newname ) const override ;
      // 
    protected:
      // ======================================================================
      // the actual evaluation of the result 
      Double_t evaluate () const override ;    
      // ======================================================================
    }; //
    // ========================================================================
    /** @class RelDifference
     *  Evaluate \f$ \frac{a-b}{a+b}\f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru 
     *  @date 2019-11-21
     */
    class RelDifference : public Division
    {
      // ======================================================================
      ClassDef(Ostap::MoreRooFit::RelDifference , 1 ) ;  // Relative difference 
      // ======================================================================
    public:
      // ======================================================================
      /// constructor with two variables 
      RelDifference  ( const char* name  , 
                       const char* title , 
                       RooAbsReal& a     , 
                       RooAbsReal& b     ) ;
      // ======================================================================
      /// default constructor 
      RelDifference () =  default ;
      // ======================================================================
      // copy 
      RelDifference ( const RelDifference& right       , 
                      const char*          newname = 0 ) ;
      // ======================================================================
      // destructor 
      virtual ~RelDifference () ;
      // ======================================================================
      // clone method 
      RelDifference * clone ( const char* newname ) const override ;
      // ======================================================================
    protected:
      // ======================================================================
      // the actual evaluation of the result 
      Double_t evaluate () const override ;    
      // ======================================================================
    }; //
    // ========================================================================
  } //                                   The end of namespace Ostap::MoreRooFit  
  // ==========================================================================
} //                                                 The end of namespace Ostap
// ============================================================================
//                                                                      The END 
// ============================================================================
#endif // OSTAP_MOREROOFIT_H
// ============================================================================