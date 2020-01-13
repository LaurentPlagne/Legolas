/**
 * project  DESCARTES
 *
 * @file     SparseMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_SPARSEMATRIXDEFINITION_HXX__
#define __LEGOLAS_SPARSEMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/RealElement/RealDataDriver.hxx"
#include "Legolas/Matrix/MatrixStructures/Sparse/ListedFilledRows.hxx"
#include "Legolas/Matrix/MatrixStructures/Sparse/ListedFilledCols.hxx"
#include "Legolas/Matrix/Helper/DefaultMatrixDefinition.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
//#include "Legolas/Matrix/MatrixStructures/Sparse/ListedFilledCols.hxx"

template <class REAL_TYPE>
class SparseMatrixDefinition : public Legolas::DefaultMatrixDefinition<REAL_TYPE>{
public:
  
  typedef Legolas::Sparse            MatrixStructure;
  typedef REAL_TYPE                  RealType;
  typedef REAL_TYPE                  GetElement;
  
  struct Data : public Legolas::MatrixShape<1>{
    
    Data( const Legolas::MatrixShape<1> & ms):Legolas::MatrixShape<1>(ms){}
    
    std::list< std::pair<int,int> > getFilledElementList( void ) const{
      std::list< std::pair<int,int> > result;
      result.push_back( std::pair<int,int>(0,0) );
      result.push_back( std::pair<int,int>(0,1) );
      result.push_back( std::pair<int,int>(1,0) );
      result.push_back( std::pair<int,int>(1,1) );
      result.push_back( std::pair<int,int>(2,2) );
      result.push_back( std::pair<int,int>(2,3) );
      result.push_back( std::pair<int,int>(3,2) );
      result.push_back( std::pair<int,int>(3,3) );
      result.push_back( std::pair<int,int>(2,0) );
      result.push_back( std::pair<int,int>(3,1) );
      return result;
    }
  };
  
  typedef Legolas::ListedFilledRows FilledRows;
  typedef Legolas::ListedFilledCols FilledCols;

  
  static int getMaxIteration( const Data & data ){ return 100 ; }
  static double getEpsilon( const Data & data ){ return 1.e-9 ; }
  
  static inline GetElement sparseGetElement(int i , int j, const Data & data) {

    GetElement result=0;

    RealType a=11.1148;
    RealType b=-3.64756;
    RealType c=-3.81972;
    

    if ( (i==0) && (j==0) ) result=a;
    if ( (i==0) && (j==1) ) result=b;
    if ( (i==1) && (j==0) ) result=b;
    if ( (i==1) && (j==1) ) result=a;
    if ( (i==2) && (j==2) ) result=a;
    if ( (i==2) && (j==3) ) result=b;
    if ( (i==3) && (j==2) ) result=b;
    if ( (i==3) && (j==3) ) result=a;
    if ( (i==2) && (j==0) ) result=c;
    if ( (i==3) && (j==1) ) result=c;

    return result;
  }

};


#endif
    

									     

