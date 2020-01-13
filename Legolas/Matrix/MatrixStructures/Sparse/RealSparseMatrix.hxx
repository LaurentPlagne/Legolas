#ifndef __LEGOLAS_REALSPARSEMATRIX_HXX__
#define __LEGOLAS_REALSPARSEMATRIX_HXX__

#include "Legolas/Matrix/RealElement/RealDataDriver.hxx"
#include "Legolas/Matrix/MatrixStructures/Sparse/EmptySparseMatrixDefinition.hxx"
#include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
#include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
#include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"

namespace Legolas{

  template <class REAL_TYPE>
  class __HELPER__{
  private:
    class SparseMatrixDefinition : public EmptySparseMatrixDefinition<RealDataDriver<REAL_TYPE> >{
    public:
      
      typedef EmptySparseMatrixDefinition<RealDataDriver<REAL_TYPE> > Base;
      typedef typename Base::BaseData Data;
      typedef typename Base::GetElement GetElement;
      
      typedef typename Base::template FilledRows<Data> FilledRows;
      typedef typename Base::template FilledCols<Data> FilledCols;
      
      static inline GetElement zero(const Data & data) { return 0.0;}
      static inline GetElement sparseGetElement(int i , int j, const Data & data) { return zero(data);}
    };

  
    typedef SparseMatrixDefinition              SMD;
    typedef InputMatrixOptions<Actual>          SMO;
    typedef RealElementInterface<REAL_TYPE>     MEI;
    

    //  public:

    typedef Legolas::GenericMatrixInterface<SMD,SMO,MEI> SparseMatrixInterface;    

  public:

    typedef typename GenericMatrixInterface<SMD,SMO,MEI>::Matrix SparseMatrix;
    typedef typename SMD::Data SparseData;
  };
  
  template <class REAL_TYPE>
  class RealSparseMatrix : public __HELPER__<REAL_TYPE>::SparseMatrix{
  private:
    typedef typename __HELPER__<REAL_TYPE>::SparseMatrix SparseBase;
    typedef typename __HELPER__<REAL_TYPE>::SparseData SparseData;
  public:

    RealSparseMatrix(int i, int j):SparseBase(SparseData(i,j)){
    }
  };


}

#endif
