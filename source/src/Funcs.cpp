// ============================================================================
// Include files
// ============================================================================
// local
// ============================================================================
#include "Ostap/Funcs.h"
#include "Ostap/Formula.h"
#include "Ostap/Iterator.h"
#include "Ostap/StatusCode.h"
// ============================================================================
// Root
// ============================================================================
#include "RooFormulaVar.h"
#include "RooArgList.h"
#include "RooArgSet.h"
#include "RooAbsData.h"
// ============================================================================
//  Local
// ============================================================================
#include "Exception.h"
// ============================================================================
/** @file 
 *  Implementation file for classes from namespace Ostap::Functions
 *  @date 2018-03-31 
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 */
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
  , m_tree       ( tree    ) 
  , m_formula    ( nullptr )  
  , m_expression ( name    )  
{
  /// create the formula
  if ( m_tree ) 
  { 
    TTree* t = const_cast<TTree*> ( m_tree ) ;
    m_formula = std::make_unique<Ostap::Formula> ( m_name , m_expression , t ) ;
    if ( !m_formula || !m_formula->ok() ) 
    { throw Ostap::Exception ( "Invalid Formula '" + m_expression + "'" , 
                               "Ostap::Function::FuncFormula"           , 
                               Ostap::StatusCode(700)                   ) ; }
  }
}
// ============================================================================
//  evaluate the formula for  TTree
// ============================================================================
double Ostap::Functions::FuncFormula::evaluate ( const TTree* tree ) const
{
  //
  if ( nullptr != tree  && tree != m_tree )
  { 
    m_tree    = tree  ;
    m_formula.reset ( nullptr ) ;
    //
    TTree* t = const_cast<TTree*> ( m_tree ) ; 
    m_formula = std::make_unique<Ostap::Formula> ( m_name , m_expression , t ) ;
  }
  //
  if ( nullptr == m_tree ) 
  { throw Ostap::Exception ( "Invalid Tree", 
                             "Ostap::Function::FuncFormula"           , 
                             Ostap::StatusCode(701)                   ) ; }
  if ( !m_formula || !m_formula->ok() ) 
  { throw Ostap::Exception ( "Invalid Formula '" + m_expression + "'" , 
                             "Ostap::Function::FuncFormula"           , 
                             Ostap::StatusCode(702)                   ) ; }
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
  if ( m_data ) 
  {
    const RooArgSet* varset  = m_data->get() ;
    if (  nullptr == varset ) 
    { throw Ostap::Exception ( "Invalid RooArgSet", 
                               "Ostap::Function::FuncRooFormula"  , 
                               Ostap::StatusCode(705)             ) ; }
    RooArgList varlst ;
    Ostap::Utils::Iterator iter ( *varset ) ;
    while ( RooAbsArg* a = iter.static_next<RooAbsArg>() ) { varlst.add ( *a ) ; }
    //
    m_formula = std::make_unique<RooFormulaVar> 
      ( m_name .c_str () , m_expression.c_str () , varlst ) ;
    //
    if ( !m_formula || !m_formula->ok() ) 
    { throw Ostap::Exception ( "Invalid Formula '" + m_expression + "'" , 
                               "Ostap::Function::FuncRooFormula"        , 
                               Ostap::StatusCode(706)                   ) ; }
  }  
}
// ============================================================================
//  evaluate the formula for  Data
// ============================================================================
double Ostap::Functions::FuncRooFormula::evaluate ( const RooAbsData* data ) const
{
  //
  if ( nullptr != data  && data != m_data )
  { 
    m_data   = data ;
    m_formula.reset ( nullptr ) ;
    //
    const RooArgSet* varset  = m_data->get() ;
    if (  nullptr == varset ) 
    { throw Ostap::Exception ( "Invalid RooArgSet", 
                               "Ostap::Function::FuncRooFormula"  , 
                               Ostap::StatusCode(707)             ) ; }
    RooArgList varlst ;
    Ostap::Utils::Iterator iter ( *varset ) ;
    while ( RooAbsArg* a = iter.static_next<RooAbsArg>() ) { varlst.add ( *a ) ; }
    //
    m_formula = std::make_unique<RooFormulaVar> 
      ( m_name .c_str () , m_expression.c_str () , varlst ) ;
    //
    if ( !m_formula || !m_formula->ok() ) 
    { throw Ostap::Exception ( "Invalid RooFormula '" + m_expression + "'" , 
                               "Ostap::Function::FuncRooFormula"        , 
                               Ostap::StatusCode(708)                   ) ; }
  }
  //
  if ( nullptr == m_data ) 
  { throw Ostap::Exception ( "Invalid RooAbsData", 
                             "Ostap::Function::FuncRooFormula"        , 
                             Ostap::StatusCode(709)                   ) ; }
  if ( !m_formula || !m_formula->ok() ) 
  { throw Ostap::Exception ( "Invalid RooFormula '" + m_expression + "'" , 
                             "Ostap::Function::FuncRooFormula"           , 
                             Ostap::StatusCode(710)                   ) ; }
  //
  return m_formula->getVal() ;
}


// ============================================================================
// The END 
// ============================================================================