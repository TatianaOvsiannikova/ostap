// ============================================================================
// Include files
// ============================================================================
// local
// ============================================================================
#include "Ostap/Funcs.h"
#include "Ostap/Formula.h"
#include "Ostap/Iterator.h"
#include "Ostap/StatusCode.h"
#include "Ostap/HFuncs.h"
#include "Ostap/FormulaVar.h"
// ============================================================================
// Root
// ============================================================================
#include "TTree.h"
#include "RooArgList.h"
#include "RooArgSet.h"
#include "RooAbsData.h"
// ============================================================================
//  Local
// ============================================================================
#include "Exception.h"
#include "local_utils.h"
// ============================================================================
/** @file 
 *  Implementation file for classes from namespace Ostap::Functions
 *  @date 2018-03-31 
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */
// ============================================================================
ClassImp(Ostap::Functions::FuncFormula)
ClassImp(Ostap::Functions::Func1D)
ClassImp(Ostap::Functions::Func2D)
ClassImp(Ostap::Functions::Func3D)
ClassImp(Ostap::Functions::FuncTH1)
ClassImp(Ostap::Functions::FuncTH2)
ClassImp(Ostap::Functions::FuncTH3)
// ============================================================================
/*  constructor from the formula expression 
 *  @param expression the  formula expression 
 *  @param tree       the tree 
 *  @param name       the name for the formula 
 */
// ============================================================================
Ostap::Functions::FuncFormula::FuncFormula
( const std::string& expression , 
  const TTree*       tree       ,
  const std::string& name       )
: Ostap::IFuncTree()
  , TObject      () 
  , m_tree       ( tree       ) 
  , m_formula    ( nullptr    )    
  , m_expression ( expression )  
  , m_name       ( name       )  
{
  if ( m_tree && !make_formula () )
  { throw Ostap::Exception ( "Invalid Formula '" + m_expression + "'" , 
                             "Ostap::Function::FuncFormula"           , 
                             Ostap::StatusCode(700)                   ) ; }
}
// ============================================================================
// copy
// ============================================================================
Ostap::Functions::FuncFormula::FuncFormula
( const Ostap::Functions::FuncFormula& right )  
  : Ostap::IFuncTree( right )
  , TObject         ( right ) 
  , m_tree          ( nullptr            )  // ATTENTION! 
  , m_formula       ( nullptr            )  // ATTENTION  
  , m_expression    ( right.m_expression )  
  , m_name          ( right.m_name       )  
{}
// ============================================================================
Ostap::Functions::FuncFormula*
Ostap::Functions::FuncFormula::Clone ( const char* /* newname */ ) const 
{ return new FuncFormula ( *this ) ; }
// ============================================================================
// destructor
// ============================================================================
Ostap::Functions::FuncFormula::~FuncFormula(){}
// ============================================================================
// notify 
// ============================================================================
Bool_t Ostap::Functions::FuncFormula::Notify () 
{ 
  m_formula.reset ( nullptr ) ;
  return ( m_formula &&  m_formula->ok() ) ? m_formula->Notify() : false ; 
}
// ============================================================================
// make formula  
// ============================================================================
bool Ostap::Functions::FuncFormula::make_formula () const
{
  m_formula.reset ( nullptr ) ;
  if ( nullptr == m_tree ) { return false ; }
  TTree* t  = const_cast<TTree*> ( m_tree ) ;
  m_formula = std::make_unique<Ostap::Formula> ( m_name , m_expression , t ) ; 
  return  ( m_formula && m_formula -> ok () ) ?  m_formula->Notify() : false ;  
}
// ============================================================================
//  evaluate the formula for  TTree
// ============================================================================
double Ostap::Functions::FuncFormula::operator() ( const TTree* tree ) const
{
  //
  if ( nullptr != tree && m_tree != tree )
  {
    m_tree = tree ;
    m_formula.reset( nullptr) ; 
  }
  //
  if ( m_formula &&  m_formula->ok() &&  m_formula->GetTree() != m_tree  )
  { m_formula.reset( nullptr) ; }
  //
  Ostap::Assert ( nullptr != m_tree                ,
                  "InvalidTree"                    ,
                  "Ostap::Function::FuncFormula"   , 
                  Ostap::StatusCode(701)           ) ;
  //
  if ( !m_formula || !m_formula->ok() ) { make_formula () ;}
  //
  Ostap::Assert ( m_formula && m_formula->ok()    ,
                  "Invalid Formula"               , 
                  "Ostap::Function::FuncFormula"  , 
                  Ostap::StatusCode(700)          ) ;  
  //
  return m_formula->evaluate() ;
}
// ===========================================================================
/* constructor from the formula expression 
 *  @param expression the formula expression 
 *  @param data       the data
 *  @param name       the name for the formula 
 */
// ===========================================================================
Ostap::Functions::FuncRooFormula::FuncRooFormula 
( const std::string& expression , 
  const RooAbsData*  data       ,
  const std::string& name       ) 
  : Ostap::IFuncData () 
  , m_data       ( data       )
  , m_formula    ( nullptr    )
  , m_expression ( expression ) 
  , m_name       ( name       )
{
  if ( m_data && !make_formula() )
  { throw Ostap::Exception ( "Invalid Formula '" + m_expression + "'" , 
                             "Ostap::Function::FuncRooFormula"        , 
                             Ostap::StatusCode(706)                   ) ; }
}
// ============================================================================
// copy
// ============================================================================
Ostap::Functions::FuncRooFormula::FuncRooFormula
( const Ostap::Functions::FuncRooFormula& right )  
  : Ostap::IFuncData ( right )
  , m_data       ( nullptr            ) // ATTENTION! 
  , m_formula    ( nullptr            ) // ATTENTION!
  , m_expression ( right.m_expression ) 
  , m_name       ( right.m_name       )
{}
// ============================================================================
// destructor
// ============================================================================
Ostap::Functions::FuncRooFormula::~FuncRooFormula(){}
// ============================================================================
// make formula  
// ============================================================================
bool Ostap::Functions::FuncRooFormula::make_formula () const
{
  m_formula.reset ( nullptr ) ;
  if  ( !m_data ) { return false ; }
  //
  const RooArgSet* varset  = m_data->get() ;
  if (  nullptr == varset ) 
  { throw Ostap::Exception ( "Invalid RooArgSet", 
                             "Ostap::Function::FuncRooFormula"  , 
                             Ostap::StatusCode(705)             ) ; }
  RooArgList varlst ;
  Ostap::Utils::Iterator iter ( *varset ) ;
  while ( RooAbsArg* a = iter.static_next<RooAbsArg>() ) { varlst.add ( *a ) ; }
  //
  m_formula = std::make_unique<Ostap::FormulaVar> ( m_name , m_expression , varlst , false ) ;
  //
  return m_formula && m_formula -> ok () ;
}
// ============================================================================
//  evaluate the formula for  Data
// ============================================================================
double Ostap::Functions::FuncRooFormula::operator() ( const RooAbsData* data ) const
{
  //
  if ( nullptr != data  && data != m_data )
  { 
    m_data   = data ;
    m_formula.reset ( nullptr ) ;
  }  
  //
  Ostap::Assert ( nullptr != m_data                  ,  
                  "Invalid RooAbsData"               , 
                  "Ostap::Function::FuncRooFormula"  , 
                  Ostap::StatusCode(709)             ) ; 
  //
  if ( !m_formula || !m_formula->ok() ) { make_formula () ; }
  //
  Ostap::Assert  ( m_formula && m_formula->ok()      , 
                   "Invalid RooFormula"              , 
                   "Ostap::Function::FuncRooFormula" , 
                   Ostap::StatusCode(708)            ) ; 
  //
  return m_formula->getVal() ;
}
// ============================================================================
// copy constructor 
// ============================================================================
Ostap::Functions::Func1D::Func1D
( const Ostap::Functions::Func1D&  right ) 
  : Ostap::IFuncTree ( right            ) 
  , TObject          ( right            ) 
  , m_fun            ( right.m_fun      )
  , m_xvar_exp       ( right.m_xvar_exp ) 
  , m_xvar           ( nullptr )
  , m_tree           ( nullptr ) 
{}
// ===========================================================================
// clone :
// ===========================================================================
Ostap::Functions::Func1D* 
Ostap::Functions::Func1D::Clone ( const char* /* newname */ ) const
{ return new Func1D ( *this ) ; }
// ===========================================================================
// notify 
// ============================================================================
Bool_t Ostap::Functions::Func1D::Notify () 
{  
  /// attention! here  we delete the variable instead of notify/reset 
  m_xvar.reset ( nullptr ) ;
  return ( m_xvar &&  m_xvar->ok() ) ? m_xvar->Notify() : false ; 
}
// ============================================================================
// make the formula
// ============================================================================
bool Ostap::Functions::Func1D::make_xvar() const
{
  m_xvar.reset ( nullptr ) ;
  if ( nullptr == m_tree ) { return false ; }
  m_xvar.reset ( nullptr ) ;
  TTree* t = const_cast<TTree*> ( m_tree ) ; 
  m_xvar   = std::make_unique<Ostap::Formula> ( m_xvar_exp , t ) ;
  if ( m_tree && m_xvar && m_xvar->ok() ) { m_xvar->Notify() ; }
  return m_xvar && m_xvar->ok () ;
}
// ============================================================================
//  evaluate the function for  TTree
// ============================================================================
double Ostap::Functions::Func1D::operator() ( const TTree* tree ) const
{
  //
  // the tree 
  if ( tree != m_tree )
  { 
    m_tree = tree  ;
    m_xvar.reset ( nullptr ) ;
  }
  //
  Ostap::Assert ( nullptr != m_tree , 
                  "Invalid Tree"    , 
                  "Ostap::Function::Func1D" ) ;
  //
  // check consistency
  if ( m_xvar && ( m_xvar -> GetTree() != m_tree ) ) { m_xvar.reset ( nullptr ) ; }
  //
  // the  axis 
  if ( !m_xvar || !m_xvar->ok() ) { make_xvar()  ; }
  Ostap::Assert ( m_xvar && m_xvar->ok()                 , 
                  "Invalid Formula '" + m_xvar_exp + "'" , 
                  "Ostap::Function::Func1D"             ) ;
  //
  // agree? 
  Ostap::Assert ( m_tree == m_xvar->GetTree()            , 
                  "mismatch in tree"                     ,
                  "Ostap::Function::Func1D"             ) ;
  //
  const double xvar = m_xvar->evaluate() ;
  //
  return m_fun ( xvar ) ;
}
// ============================================================================
// copy constructor 
// ============================================================================
Ostap::Functions::Func2D::Func2D
( const Ostap::Functions::Func2D&  right ) 
  : Ostap::IFuncTree ( right            ) 
  , TObject          ( right            ) 
  , m_fun            ( right.m_fun      )
  , m_xvar_exp       ( right.m_xvar_exp ) 
  , m_yvar_exp       ( right.m_yvar_exp ) 
  , m_xvar           ( nullptr )
  , m_yvar           ( nullptr )
  , m_tree           ( nullptr ) 
{}
// ===========================================================================
// clone :
// ===========================================================================
Ostap::Functions::Func2D* 
Ostap::Functions::Func2D::Clone ( const char* /* newname */ ) const
{ return new Func2D ( *this ) ; }
// ===========================================================================
// notify 
// ============================================================================
Bool_t Ostap::Functions::Func2D::Notify () 
{  
  m_xvar.reset ( nullptr ) ;
  m_yvar.reset ( nullptr ) ;
  //
  const bool b1 = ( m_xvar && m_xvar->ok() ) ? m_xvar->Notify() : false ; 
  const bool b2 = ( m_yvar && m_yvar->ok() ) ? m_yvar->Notify() : false ; 
  //
  return b1 && b2 ;
}
// ============================================================================
// make the formula
// ============================================================================
bool Ostap::Functions::Func2D::make_xvar() const
{
  m_xvar.reset ( nullptr ) ;
  if ( nullptr == m_tree ) { return false ; }
  m_xvar.reset ( nullptr ) ;
  TTree* t = const_cast<TTree*> ( m_tree ) ; 
  m_xvar   = std::make_unique<Ostap::Formula> ( m_xvar_exp , t ) ;
  if ( m_tree && m_xvar && m_xvar->ok() ) { m_xvar->Notify() ; }
  return m_xvar && m_xvar->ok () ;
}
// ============================================================================
// make the formula
// ============================================================================
bool Ostap::Functions::Func2D::make_yvar() const
{
  m_yvar.reset ( nullptr ) ;
  if ( nullptr == m_tree ) { return false ; }
  m_yvar.reset ( nullptr ) ;
  TTree* t = const_cast<TTree*> ( m_tree ) ; 
  m_yvar   = std::make_unique<Ostap::Formula> ( m_yvar_exp , t ) ;
  if ( m_tree && m_yvar && m_yvar->ok() ) { m_yvar->Notify() ; }
  return m_yvar && m_yvar->ok () ;
}
// ============================================================================
//  evaluate the function for  TTree
// ============================================================================
double Ostap::Functions::Func2D::operator() ( const TTree* tree ) const
{
  //
  // the tree 
  if ( tree != m_tree )
  { 
    m_tree = tree  ;
    m_xvar.reset ( nullptr ) ;
    m_yvar.reset ( nullptr ) ;
  }
  //
  Ostap::Assert ( nullptr != m_tree , 
                  "Invalid Tree"    , 
                  "Ostap::Function::Func2D" ) ;
  //
  // check consistency
  if ( m_xvar && ( m_xvar -> GetTree() != m_tree ) ) { m_xvar.reset ( nullptr ) ; }
  if ( m_yvar && ( m_yvar -> GetTree() != m_tree ) ) { m_yvar.reset ( nullptr ) ; }
  //
  // the  axis 
  if ( !m_xvar || !m_xvar->ok() ) { make_xvar()  ; }
  Ostap::Assert ( m_xvar && m_xvar->ok()                 , 
                  "Invalid Formula '" + m_xvar_exp + "'" , 
                  "Ostap::Function::Func2D"             ) ;
  // the  axis 
  if ( !m_yvar || !m_yvar->ok() ) { make_yvar()  ; }
  Ostap::Assert ( m_yvar && m_yvar->ok()                 , 
                  "Invalid Formula '" + m_yvar_exp + "'" , 
                  "Ostap::Function::Func2D"             ) ;
  //
  // agree? 
  Ostap::Assert ( m_tree == m_xvar->GetTree() && 
                  m_tree == m_yvar->GetTree() ,
                  "mismatch in tree"                     ,
                  "Ostap::Function::Func2D"             ) ;
  //
  const double xvar = m_xvar->evaluate() ;
  const double yvar = m_yvar->evaluate() ;
  //
  return m_fun ( xvar , yvar ) ;
}
// ============================================================================

// ============================================================================
// Func3D 
// ============================================================================
// copy constructor 
// ============================================================================
Ostap::Functions::Func3D::Func3D
( const Ostap::Functions::Func3D&  right ) 
  : Ostap::IFuncTree ( right            ) 
  , TObject          ( right            ) 
  , m_fun            ( right.m_fun      )
  , m_xvar_exp       ( right.m_xvar_exp ) 
  , m_yvar_exp       ( right.m_yvar_exp ) 
  , m_zvar_exp       ( right.m_zvar_exp ) 
  , m_xvar           ( nullptr )
  , m_yvar           ( nullptr )
  , m_zvar           ( nullptr )
  , m_tree           ( nullptr ) 
{}
// ===========================================================================
// clone :
// ===========================================================================
Ostap::Functions::Func3D* 
Ostap::Functions::Func3D::Clone ( const char* /* newname */ ) const
{ return new Func3D ( *this ) ; }
// ===========================================================================
// notify 
// ============================================================================
Bool_t Ostap::Functions::Func3D::Notify () 
{  
  m_xvar.reset ( nullptr ) ;
  m_yvar.reset ( nullptr ) ;
  m_zvar.reset ( nullptr ) ;
  //
  const bool b1 = ( m_xvar && m_xvar->ok() ) ? m_xvar->Notify() : false ; 
  const bool b2 = ( m_yvar && m_yvar->ok() ) ? m_yvar->Notify() : false ; 
  const bool b3 = ( m_zvar && m_zvar->ok() ) ? m_zvar->Notify() : false ; 
  //
  return b1 && b2 && b3 ;
}
// ============================================================================
// make the formula
// ============================================================================
bool Ostap::Functions::Func3D::make_xvar() const
{
  m_xvar.reset ( nullptr ) ;
  if ( nullptr == m_tree ) { return false ; }
  m_xvar.reset ( nullptr ) ;
  TTree* t = const_cast<TTree*> ( m_tree ) ; 
  m_xvar   = std::make_unique<Ostap::Formula> ( m_xvar_exp , t ) ;
  if ( m_tree && m_xvar && m_xvar->ok() ) { m_xvar->Notify() ; }
  return m_xvar && m_xvar->ok () ;
}
// ============================================================================
// make the formula
// ============================================================================
bool Ostap::Functions::Func3D::make_yvar() const
{
  m_yvar.reset ( nullptr ) ;
  if ( nullptr == m_tree ) { return false ; }
  m_yvar.reset ( nullptr ) ;
  TTree* t = const_cast<TTree*> ( m_tree ) ; 
  m_yvar   = std::make_unique<Ostap::Formula> ( m_yvar_exp , t ) ;
  if ( m_tree && m_yvar && m_yvar->ok() ) { m_yvar->Notify() ; }
  return m_yvar && m_yvar->ok () ;
}
// ============================================================================
// make the formula
// ============================================================================
bool Ostap::Functions::Func3D::make_zvar() const
{
  m_zvar.reset ( nullptr ) ;
  if ( nullptr == m_tree ) { return false ; }
  m_zvar.reset ( nullptr ) ;
  TTree* t = const_cast<TTree*> ( m_tree ) ; 
  m_zvar   = std::make_unique<Ostap::Formula> ( m_zvar_exp , t ) ;
  if ( m_tree && m_zvar && m_zvar->ok() ) { m_zvar->Notify() ; }
  return m_zvar && m_zvar->ok () ;
}
// ============================================================================
//  evaluate the function for  TTree
// ============================================================================
double Ostap::Functions::Func3D::operator() ( const TTree* tree ) const
{
  //
  // the tree 
  if ( tree != m_tree )
  { 
    m_tree = tree  ;
    m_xvar.reset ( nullptr ) ;
    m_yvar.reset ( nullptr ) ;
    m_zvar.reset ( nullptr ) ;
  }
  //
  Ostap::Assert ( nullptr != m_tree , 
                  "Invalid Tree"    , 
                  "Ostap::Function::Func3D" ) ;
  //
  // check consistency
  if ( m_xvar && ( m_xvar -> GetTree() != m_tree ) ) { m_xvar.reset ( nullptr ) ; }
  if ( m_yvar && ( m_yvar -> GetTree() != m_tree ) ) { m_yvar.reset ( nullptr ) ; }
  if ( m_zvar && ( m_zvar -> GetTree() != m_tree ) ) { m_zvar.reset ( nullptr ) ; }
  //
  // the  axis 
  if ( !m_xvar || !m_xvar->ok() ) { make_xvar()  ; }
  Ostap::Assert ( m_xvar && m_xvar->ok()                 , 
                  "Invalid Formula '" + m_xvar_exp + "'" , 
                  "Ostap::Function::Func2D"             ) ;
  // the  axis 
  if ( !m_yvar || !m_yvar->ok() ) { make_yvar()  ; }
  Ostap::Assert ( m_yvar && m_yvar->ok()                 , 
                  "Invalid Formula '" + m_yvar_exp + "'" , 
                  "Ostap::Function::Func2D"             ) ;
  // the  axis 
  if ( !m_zvar || !m_zvar->ok() ) { make_zvar()  ; }
  Ostap::Assert ( m_zvar && m_zvar->ok()                 , 
                  "Invalid Formula '" + m_zvar_exp + "'" , 
                  "Ostap::Function::Func3D"             ) ;
  //
  // agree? 
  Ostap::Assert ( m_tree == m_xvar->GetTree() && 
                  m_tree == m_yvar->GetTree() && 
                  m_tree == m_zvar->GetTree()  , 
                  "mismatch in tree"                     ,
                  "Ostap::Function::Func2D"             ) ;
  //
  const double xvar = m_xvar->evaluate() ;
  const double yvar = m_yvar->evaluate() ;
  const double zvar = m_zvar->evaluate() ;
  //
  return m_fun ( xvar , yvar , zvar ) ;
}



// ======================================================================
/*  constructor from the histogram 
 *  @param histo         (INPUT) the histogram 
 *  @param xvar          (INPUT) the expression/variable 
 *  @param tree          (INPUT) the tree 
 *  @param interpolation (INPUT) interpolation type 
 *  @param edges         (INPUT) special tretament of edges?
 *  @param extrapolate   (INPUT) use extrapolation?
 *  @param density       (INPUT) use  density?
 */
// ===========================================================================
Ostap::Functions::FuncTH1::FuncTH1
( const TH1&           histo         , 
  const std::string&   xvar          , 
  const TTree*         tree          ,
  const Ostap::Math::HistoInterpolation::Type tx , 
  const bool           edges         ,
  const bool           extrapolate   , 
  const bool           density       )
  : Func1D ( Ostap::Math::Histo1D ( histo , tx , edges , extrapolate , density ) , 
             xvar , 
             tree ) 
{}
// ============================================================================
/*  constructor from the histogram 
 *  @param histo         (INPUT) the historgam 
 *  @param xvar          (INPUT) the expression/variable 
 *  @param tree          (INPUT) the tree 
 */
// ============================================================================
Ostap::Functions::FuncTH1::FuncTH1
( const Ostap::Math::Histo1D& histo , 
  const std::string&          xvar  , 
  const TTree*                tree  ) 
  : Func1D ( histo , xvar , tree ) 
{}
// ============================================================================
// copy constructor 
// ============================================================================
Ostap::Functions::FuncTH1::FuncTH1
( const Ostap::Functions::FuncTH1&  right ) 
  : Func1D ( right )
{}
// ===========================================================================
// clone :
// ===========================================================================
Ostap::Functions::FuncTH1* 
Ostap::Functions::FuncTH1::Clone ( const char* /* newname */ ) const
{ return new FuncTH1 ( *this ) ; }
// ============================================================================

// ======================================================================
/*  constructor from the histogram 
 *  @param histo         (INPUT) the historgam 
 *  @param xvar          (INPUT) the expression/variable 
 *  @param tree          (INPUT) the tree 
 *  @param interpolation (INPUT) interpolation type 
 *  @param edges         (INPUT) special tretament of edges?
 *  @param extrapolate   (INPUT) use extrapolation?
 *  @param density       (INPUT) use  density?
 */
// ======================================================================
Ostap::Functions::FuncTH2::FuncTH2 
( const TH2&           histo                     , 
  const std::string&   xvar                      , 
  const std::string&   yvar                      , 
  const TTree*         tree                      ,
  const Ostap::Math::HistoInterpolation::Type tx ,
  const Ostap::Math::HistoInterpolation::Type ty , 
  const bool           edges                     ,
  const bool           extrapolate               , 
  const bool           density                   )
  : Func2D ( Ostap::Math::Histo2D ( histo , tx , ty , edges , extrapolate , density ) , 
             xvar ,   
             yvar , 
             tree )
{}
// ======================================================================
/*  constructor from the histogram 
 *  @param histo         (INPUT) the historgam 
 *  @param xvar          (INPUT) the expression/variable 
 *  @param tree          (INPUT) the tree 
 */
// ======================================================================
Ostap::Functions::FuncTH2::FuncTH2 
( const Ostap::Math::Histo2D& histo , 
  const std::string&          xvar  , 
  const std::string&          yvar  , 
  const TTree*                tree  ) 
  : Func2D ( histo , xvar , yvar , tree )
{}
// ============================================================================
// copy constructor 
// ============================================================================
Ostap::Functions::FuncTH2::FuncTH2
( const Ostap::Functions::FuncTH2&  right ) 
  : Func2D ( right ) 
{}
// ===========================================================================
// clone :
// ===========================================================================
Ostap::Functions::FuncTH2* 
Ostap::Functions::FuncTH2::Clone ( const char* /* newname */ ) const
{ return new FuncTH2 ( *this ) ; }
// ===========================================================================


// ======================================================================
/*  constructor from the histogram 
 *  @param histo         (INPUT) the historgam 
 *  @param xvar          (INPUT) the expression/variable 
 *  @param tree          (INPUT) the tree 
 *  @param interpolation (INPUT) interpolation type 
 *  @param edges         (INPUT) special tretament of edges?
 *  @param extrapolate   (INPUT) use extrapolation?
 *  @param density       (INPUT) use  density?
 */
// ======================================================================
Ostap::Functions::FuncTH3::FuncTH3
( const TH3&           histo                     , 
  const std::string&   xvar                      , 
  const std::string&   yvar                      , 
  const std::string&   zvar                      , 
  const TTree*         tree                      ,
  const Ostap::Math::HistoInterpolation::Type tx ,
  const Ostap::Math::HistoInterpolation::Type ty , 
  const Ostap::Math::HistoInterpolation::Type tz , 
  const bool           edges                     ,
  const bool           extrapolate               , 
  const bool           density                   )
  : Func3D ( Ostap::Math::Histo3D ( histo , tx , ty , tz , edges , extrapolate , density ) , 
             xvar , 
             yvar , 
             zvar , 
             tree )
{}
// ======================================================================
/*  constructor from the histogram 
 *  @param histo         (INPUT) the historgam 
 *  @param xvar          (INPUT) the expression/variable 
 *  @param tree          (INPUT) the tree 
 */
// ======================================================================
Ostap::Functions::FuncTH3::FuncTH3
( const Ostap::Math::Histo3D& histo , 
  const std::string&          xvar  , 
  const std::string&          yvar  , 
  const std::string&          zvar  , 
  const TTree*                tree  )
  : Func3D ( histo , xvar , yvar , zvar , tree )
{}
// ============================================================================
// copy constructor 
// ============================================================================
Ostap::Functions::FuncTH3::FuncTH3
( const Ostap::Functions::FuncTH3&  right ) 
  : Func3D ( right  )
{}
// ===========================================================================
// clone :
// ============================================================================
Ostap::Functions::FuncTH3* 
Ostap::Functions::FuncTH3::Clone ( const char* /* newname */ ) const
{ return new FuncTH3 ( *this ) ; }
// ============================================================================







// ======================================================================
// RooAbsData functions
// ======================================================================


// ======================================================================
// copy constructor
// ======================================================================
Ostap::Functions::FuncRoo1D::FuncRoo1D 
( const Ostap::Functions::FuncRoo1D& right ) 
  : Ostap::IFuncData (  right ) 
  , m_fun      ( right.m_fun      ) 
  , m_xvar_exp ( right.m_xvar_exp )
  , m_xvar     ( nullptr )   
  , m_data     ( nullptr ) 
{}
// ============================================================================
// make formula 
// ============================================================================
bool Ostap::Functions::FuncRoo1D::make_xvar () const 
{
  m_xvar.reset ( nullptr ) ;
  if ( nullptr == m_data ) { return false ; }
  m_xvar.reset ( nullptr ) ;
  // 
  const RooArgSet* varset  = m_data->get() ;
  if (  nullptr == varset ) 
  { throw Ostap::Exception ( "Invalid RooArgSet", 
                             "Ostap::Function::FuncRoo1D"  , 
                             Ostap::StatusCode(705)        ) ; }
  //
  RooArgList varlst ;
  Ostap::Utils::Iterator iter ( *varset ) ;
  while ( RooAbsArg* a = iter.static_next<RooAbsArg>() ) { varlst.add ( *a ) ; }
  //
  m_xvar = std::make_unique<Ostap::FormulaVar> ( m_xvar_exp , varlst , false ) ;
  //
  return m_xvar && m_xvar -> ok () ;
}
// ============================================================================
//  evaluate the formula for RooAbsData
// ============================================================================
double Ostap::Functions::FuncRoo1D::operator() 
  ( const RooAbsData* data ) const
{
  //
  if ( nullptr != data  && data != m_data )
  { 
    m_data   = data ;
    m_xvar.reset ( nullptr ) ;
  }  
  //
  Ostap::Assert ( nullptr != m_data              ,  
                  "Invalid RooAbsData"           , 
                  "Ostap::Function::FuncRoo1D"   , 
                  Ostap::StatusCode(709)         ) ; 
  //
  if ( !m_xvar || !m_xvar->ok() ) { make_xvar () ; }
  //
  Ostap::Assert  ( m_xvar && m_xvar->ok()        , 
                   "Invalid RooFormula"          , 
                   "Ostap::Function::FuncRoo1D"  , 
                   Ostap::StatusCode(708)        ) ; 
  //
  const double x = m_xvar->getVal() ;
  //
  return m_fun ( x ) ;
}
// ======================================================================

// ======================================================================
// copy constructor
// ======================================================================
Ostap::Functions::FuncRoo2D::FuncRoo2D 
( const Ostap::Functions::FuncRoo2D& right ) 
  : Ostap::IFuncData (  right ) 
  , m_fun      ( right.m_fun      ) 
  , m_xvar_exp ( right.m_xvar_exp )
  , m_yvar_exp ( right.m_yvar_exp )
  , m_xvar     ( nullptr )   
  , m_yvar     ( nullptr )   
  , m_data     ( nullptr ) 
{}
// ============================================================================
// make formula 
// ============================================================================
bool Ostap::Functions::FuncRoo2D::make_xvar () const 
{
  m_xvar.reset ( nullptr ) ;
  if ( nullptr == m_data ) { return false ; }
  m_xvar.reset ( nullptr ) ;
  // 
  const RooArgSet* varset  = m_data->get() ;
  if (  nullptr == varset ) 
  { throw Ostap::Exception ( "Invalid RooArgSet", 
                             "Ostap::Function::FuncRoo2D"  , 
                             Ostap::StatusCode(705)        ) ; }
  //
  RooArgList varlst ;
  Ostap::Utils::Iterator iter ( *varset ) ;
  while ( RooAbsArg* a = iter.static_next<RooAbsArg>() ) { varlst.add ( *a ) ; }
  //
  m_xvar = std::make_unique<Ostap::FormulaVar> ( m_xvar_exp , varlst , false ) ;
  //
  return m_xvar && m_xvar -> ok () ;
}
// ============================================================================
// make formula 
// ============================================================================
bool Ostap::Functions::FuncRoo2D::make_yvar () const 
{
  m_yvar.reset ( nullptr ) ;
  if ( nullptr == m_data ) { return false ; }
  m_yvar.reset ( nullptr ) ;
  // 
  const RooArgSet* varset  = m_data->get() ;
  if (  nullptr == varset ) 
  { throw Ostap::Exception ( "Invalid RooArgSet", 
                             "Ostap::Function::FuncRoo2D"  , 
                             Ostap::StatusCode(705)        ) ; }
  //
  RooArgList varlst ;
  Ostap::Utils::Iterator iter ( *varset ) ;
  while ( RooAbsArg* a = iter.static_next<RooAbsArg>() ) { varlst.add ( *a ) ; }
  //
  m_yvar = std::make_unique<Ostap::FormulaVar> ( m_yvar_exp , varlst , false ) ;
  //
  return m_yvar && m_yvar -> ok () ;
}
// ============================================================================
//  evaluate the formula for RooAbsData
// ============================================================================
double Ostap::Functions::FuncRoo2D::operator() 
  ( const RooAbsData* data ) const
{
  //
  if ( nullptr != data  && data != m_data )
  { 
    m_data   = data ;
    m_xvar.reset ( nullptr ) ;
    m_yvar.reset ( nullptr ) ;
  }  
  //
  Ostap::Assert ( nullptr != m_data              ,  
                  "Invalid RooAbsData"           , 
                  "Ostap::Function::FuncRoo2D"   , 
                  Ostap::StatusCode(709)         ) ; 
  //
  if ( !m_xvar || !m_xvar->ok() ) { make_xvar () ; }
  Ostap::Assert  ( m_xvar && m_xvar->ok()        , 
                   "Invalid RooFormula"          , 
                   "Ostap::Function::FuncRoo2D"  , 
                   Ostap::StatusCode(708)        ) ; 
  //
  if ( !m_yvar || !m_yvar->ok() ) { make_yvar () ; }
  Ostap::Assert  ( m_yvar && m_yvar->ok()        , 
                   "Invalid RooFormula"          , 
                   "Ostap::Function::FuncRoo2D"  , 
                   Ostap::StatusCode(708)        ) ; 
  //
  const double x = m_xvar->getVal() ;
  const double y = m_yvar->getVal() ;
  //
  return m_fun ( x , y ) ;
}



// ======================================================================
// copy constructor
// ======================================================================
Ostap::Functions::FuncRoo3D::FuncRoo3D 
( const Ostap::Functions::FuncRoo3D& right ) 
  : Ostap::IFuncData (  right ) 
  , m_fun      ( right.m_fun      ) 
  , m_xvar_exp ( right.m_xvar_exp )
  , m_yvar_exp ( right.m_yvar_exp )
  , m_zvar_exp ( right.m_zvar_exp )
  , m_xvar     ( nullptr )   
  , m_yvar     ( nullptr )   
  , m_zvar     ( nullptr )   
  , m_data     ( nullptr ) 
{}
// ============================================================================
// make formula 
// ============================================================================
bool Ostap::Functions::FuncRoo3D::make_xvar () const 
{
  m_xvar.reset ( nullptr ) ;
  if ( nullptr == m_data ) { return false ; }
  m_xvar.reset ( nullptr ) ;
  // 
  const RooArgSet* varset  = m_data->get() ;
  if (  nullptr == varset ) 
  { throw Ostap::Exception ( "Invalid RooArgSet", 
                             "Ostap::Function::FuncRoo3D"  , 
                             Ostap::StatusCode(705)        ) ; }
  //
  RooArgList varlst ;
  Ostap::Utils::Iterator iter ( *varset ) ;
  while ( RooAbsArg* a = iter.static_next<RooAbsArg>() ) { varlst.add ( *a ) ; }
  //
  m_xvar = std::make_unique<Ostap::FormulaVar> ( m_xvar_exp , varlst , false ) ;
  //
  return m_xvar && m_xvar -> ok () ;
}
// ============================================================================
// make formula 
// ============================================================================
bool Ostap::Functions::FuncRoo3D::make_yvar () const 
{
  m_yvar.reset ( nullptr ) ;
  if ( nullptr == m_data ) { return false ; }
  m_yvar.reset ( nullptr ) ;
  // 
  const RooArgSet* varset  = m_data->get() ;
  if (  nullptr == varset ) 
  { throw Ostap::Exception ( "Invalid RooArgSet", 
                             "Ostap::Function::FuncRoo3D"  , 
                             Ostap::StatusCode(705)        ) ; }
  //
  RooArgList varlst ;
  Ostap::Utils::Iterator iter ( *varset ) ;
  while ( RooAbsArg* a = iter.static_next<RooAbsArg>() ) { varlst.add ( *a ) ; }
  //
  m_yvar = std::make_unique<Ostap::FormulaVar> ( m_yvar_exp , varlst , false ) ;
  //
  return m_yvar && m_yvar -> ok () ;
}
// ============================================================================
// make formula 
// ============================================================================
bool Ostap::Functions::FuncRoo3D::make_zvar () const 
{
  m_zvar.reset ( nullptr ) ;
  if ( nullptr == m_data ) { return false ; }
  m_zvar.reset ( nullptr ) ;
  // 
  const RooArgSet* varset  = m_data->get() ;
  if (  nullptr == varset ) 
  { throw Ostap::Exception ( "Invalid RooArgSet", 
                             "Ostap::Function::FuncRoo3D"  , 
                             Ostap::StatusCode(705)        ) ; }
  //
  RooArgList varlst ;
  Ostap::Utils::Iterator iter ( *varset ) ;
  while ( RooAbsArg* a = iter.static_next<RooAbsArg>() ) { varlst.add ( *a ) ; }
  //
  m_zvar = std::make_unique<Ostap::FormulaVar> ( m_zvar_exp , varlst , false ) ;
  //
  return m_zvar && m_zvar -> ok () ;
}
// ============================================================================
//  evaluate the formula for RooAbsData
// ============================================================================
double Ostap::Functions::FuncRoo3D::operator() 
  ( const RooAbsData* data ) const
{
  //
  if ( nullptr != data  && data != m_data )
  { 
    m_data   = data ;
    m_xvar.reset ( nullptr ) ;
    m_yvar.reset ( nullptr ) ;
    m_zvar.reset ( nullptr ) ;
  }  
  //
  Ostap::Assert ( nullptr != m_data              ,  
                  "Invalid RooAbsData"           , 
                  "Ostap::Function::FuncRoo2D"   , 
                  Ostap::StatusCode(709)         ) ; 
  //
  if ( !m_xvar || !m_xvar->ok() ) { make_xvar () ; }
  Ostap::Assert  ( m_xvar && m_xvar->ok()        , 
                   "Invalid RooFormula"          , 
                   "Ostap::Function::FuncRoo2D"  , 
                   Ostap::StatusCode(708)        ) ; 
  //
  if ( !m_yvar || !m_yvar->ok() ) { make_yvar () ; }
  Ostap::Assert  ( m_yvar && m_yvar->ok()        , 
                   "Invalid RooFormula"          , 
                   "Ostap::Function::FuncRoo2D"  , 
                   Ostap::StatusCode(708)        ) ; 
  //
  if ( !m_zvar || !m_zvar->ok() ) { make_zvar () ; }
  Ostap::Assert  ( m_zvar && m_zvar->ok()        , 
                   "Invalid RooFormula"          , 
                   "Ostap::Function::FuncRoo2D"  , 
                   Ostap::StatusCode(708)        ) ; 
  //
  const double x = m_xvar->getVal() ;
  const double y = m_yvar->getVal() ;
  const double z = m_zvar->getVal() ;
  //
  return m_fun ( x , y , z ) ;
}



 



// ======================================================================
/*  constructor from the histogram 
 *  @param histo         (INPUT) the histogram 
 *  @param xvar          (INPUT) the expression/variable 
 *  @param tree          (INPUT) the tree 
 *  @param interpolation (INPUT) interpolation type 
 *  @param edges         (INPUT) special tretament of edges?
 *  @param extrapolate   (INPUT) use extrapolation?
 *  @param density       (INPUT) use  density?
 */
// ===========================================================================
Ostap::Functions::FuncRooTH1::FuncRooTH1
( const TH1&           histo         , 
  const std::string&   xvar          , 
  const RooAbsData*    data          ,
  const Ostap::Math::HistoInterpolation::Type tx , 
  const bool           edges         ,
  const bool           extrapolate   , 
  const bool           density       )
  : FuncRoo1D ( Ostap::Math::Histo1D ( histo , tx , edges , extrapolate , density ) , 
                xvar , 
                data )
{}
// ============================================================================
/*  constructor from the histogram 
 *  @param histo         (INPUT) the historgam 
 *  @param xvar          (INPUT) the expression/variable 
 *  @param tree          (INPUT) the tree 
 */
// ============================================================================
Ostap::Functions::FuncRooTH1::FuncRooTH1
( const Ostap::Math::Histo1D& histo , 
  const std::string&          xvar  , 
  const RooAbsData*           data  )
  : FuncRoo1D ( histo , xvar , data )
{}
// ============================================================================
/*  constructor from the histogram 
 *  @param histo         (INPUT) the histogram 
 *  @param xvar          (INPUT) the expression/variable 
 *  @param yvar          (INPUT) the expression/variable 
 *  @param tree          (INPUT) the tree 
 *  @param interpolation (INPUT) interpolation type 
 *  @param edges         (INPUT) special tretament of edges?
 *  @param extrapolate   (INPUT) use extrapolation?
 *  @param density       (INPUT) use  density?
 */
// ===========================================================================
Ostap::Functions::FuncRooTH2::FuncRooTH2
( const TH2&           histo         , 
  const std::string&   xvar          , 
  const std::string&   yvar          , 
  const RooAbsData*    data          ,
  const Ostap::Math::HistoInterpolation::Type tx , 
  const Ostap::Math::HistoInterpolation::Type ty , 
  const bool           edges         ,
  const bool           extrapolate   , 
  const bool           density       )
  : FuncRoo2D ( Ostap::Math::Histo2D ( histo , tx , ty , edges , extrapolate , density ) , 
                xvar , 
                yvar , 
                data ) 
{}
// ============================================================================
/*  constructor from the histogram 
 *  @param histo         (INPUT) the historgam 
 *  @param xvar          (INPUT) the expression/variable 
 *  @param tree          (INPUT) the tree 
 */
// ============================================================================
Ostap::Functions::FuncRooTH2::FuncRooTH2
( const Ostap::Math::Histo2D& histo , 
  const std::string&          xvar  , 
  const std::string&          yvar  , 
  const RooAbsData*           data  )
  : FuncRoo2D ( histo , xvar , yvar , data )
{}
// ======================================================================
/*  constructor from the histogram 
 *  @param histo         (INPUT) the histogram 
 *  @param xvar          (INPUT) the expression/variable 
 *  @param yvar          (INPUT) the expression/variable 
 *  @param zvar          (INPUT) the expression/variable 
 *  @param tree          (INPUT) the tree 
 *  @param interpolation (INPUT) interpolation type 
 *  @param edges         (INPUT) special tretament of edges?
 *  @param extrapolate   (INPUT) use extrapolation?
 *  @param density       (INPUT) use  density?
 */
// ===========================================================================
Ostap::Functions::FuncRooTH3::FuncRooTH3
( const TH3&           histo         , 
  const std::string&   xvar          , 
  const std::string&   yvar          , 
  const std::string&   zvar          , 
  const RooAbsData*    data          ,
  const Ostap::Math::HistoInterpolation::Type tx , 
  const Ostap::Math::HistoInterpolation::Type ty , 
  const Ostap::Math::HistoInterpolation::Type tz , 
  const bool           edges         ,
  const bool           extrapolate   , 
  const bool           density       )
  : FuncRoo3D (  Ostap::Math::Histo3D ( histo , tx , ty , tz , edges , extrapolate , density ) , 
                 xvar , 
                 yvar , 
                 zvar , 
                 data ) 
{}
// ============================================================================
/*  constructor from the histogram 
 *  @param histo         (INPUT) the historgam 
 *  @param xvar          (INPUT) the expression/variable 
 *  @param tree          (INPUT) the tree 
 */
// ============================================================================
Ostap::Functions::FuncRooTH3::FuncRooTH3
( const Ostap::Math::Histo3D& histo , 
  const std::string&          xvar  , 
  const std::string&          yvar  , 
  const std::string&          zvar  , 
  const RooAbsData*           data  )
  : FuncRoo3D ( histo , xvar , yvar , zvar , data )
{}
// ============================================================================
//                                                                      The END 
// ============================================================================
