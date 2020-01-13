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
#include "Legolas/Matrix/MatrixStructures/Sparse/EmptySparseMatrixDefinition.hxx"

namespace Legolas{

  template <class REAL_TYPE>
  class SparseMatrixDefinition : public EmptySparseMatrixDefinition<RealDataDriver<REAL_TYPE> >{
  public:
    
    typedef EmptySparseMatrixDefinition<RealDataDriver<REAL_TYPE> > Base;
    //    typedef typename Base::BaseData Data;

    typedef Legolas::MatrixShape<1> Data;
    typedef typename Base::GetElement GetElement;
    
    typedef typename Base::template FilledRows<Data> FilledRows;
    typedef typename Base::template FilledCols<Data> FilledCols;
    
    static inline GetElement sparseGetElement(int i , int j, const Data & data) { return 0.0;}
  };
  
}


#endif
    

									     

