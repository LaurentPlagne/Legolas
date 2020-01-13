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
#ifndef __LEGOLAS_SPARSEMATRIXDEFINITION2D_HXX__
#define __LEGOLAS_SPARSEMATRIXDEFINITION2D_HXX__

#include "Legolas/Matrix/RealElement/RealDataDriver.hxx"
#include "Legolas/Matrix/MatrixStructures/Sparse/EmptySparseMatrixDefinition.hxx"
#include "SparseMatrixDefinition.hxx"

namespace Legolas{

  template <class REAL_TYPE>
  class SparseMatrixDefinition2D : public EmptySparseMatrixDefinition< SparseMatrixDefinition<REAL_TYPE> >{
  public:

    typedef SparseMatrixDefinition<REAL_TYPE>    SubMatrixDefinition;
    typedef typename SubMatrixDefinition::Data   ElementData;
    typedef typename SubMatrixDefinition::Data   GetElement;

    typedef EmptySparseMatrixDefinition< SparseMatrixDefinition<REAL_TYPE> > Base;
    typedef typename Base::BaseData                                          BaseData;

    class Data : public BaseData{
    public:
    
      Data(int nrowsOut, int ncolsOut, int nrowsIn, int ncolsIn):BaseData(nrowsOut,ncolsOut),
								 elementData_(nrowsIn,ncolsIn){
      }
    
      const ElementData & elementData( void ) const { return elementData_ ;}

    private:
      ElementData elementData_;
    };
    
    typedef typename Base::template FilledRows<Data> FilledRows;
    typedef typename Base::template FilledCols<Data> FilledCols;

    // La sous matrice est une matrice nulle par defautg
    static inline GetElement zero(const Data & data) { return data.elementData();}
    static inline GetElement sparseGetElement(int i, int j, const Data & data) { return zero(data);}


  };

}

#endif
    

									     

