#ifndef __LEGOLAS_MULTILEVELSPARSEMATRIXINTERFACE_HXX__
#define __LEGOLAS_MULTILEVELSPARSEMATRIXINTERFACE_HXX__

#include "Legolas/Matrix/MatrixStructures/Sparse/MultiLevelSparseMatrixDefinition.hxx"
#include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
#include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
#include "Legolas/Matrix/RealElement/RealElementInterface.hxx"

namespace Legolas{
  
  template <class REAL_TYPE, int level>
  class MultiLevelSparseMatrixInterface{
  public:

    typedef MultiLevelSparseMatrixDefinition<REAL_TYPE,level>   MatrixDefinition;
    typedef InputMatrixOptions<Legolas::Actual>                 MatrixOptions;
    typedef MultiLevelSparseMatrixInterface<REAL_TYPE,level-1>  MatrixElementInterface;
    
  private:
    typedef GenericMatrixInterface<MatrixDefinition,MatrixOptions,MatrixElementInterface> GMI;
  public :

    typedef typename GMI::DataDriver DataDriver;
    typedef typename GMI::Matrix     Matrix;
    typedef typename GMI::Copy       Copy;
    typedef typename GMI::View       View;
    typedef typename GMI::ConstView  ConstView;
    typedef typename GMI::Solver     Solver;


  };

  
  template <class REAL_TYPE>
  class MultiLevelSparseMatrixInterface<REAL_TYPE,1>{
  public:
    
    typedef MultiLevelSparseMatrixDefinition<REAL_TYPE,1>   MatrixDefinition;
    typedef InputMatrixOptions<Legolas::Actual>             MatrixOptions;
    typedef RealElementInterface<REAL_TYPE>                 MatrixElementInterface;  
    
  private:
    typedef GenericMatrixInterface<MatrixDefinition,MatrixOptions,MatrixElementInterface> GMI;
  public :

    typedef typename GMI::DataDriver DataDriver;
    typedef typename GMI::Matrix     Matrix;
    typedef typename GMI::Copy       Copy;
    typedef typename GMI::View       View;
    typedef typename GMI::ConstView  ConstView;
    typedef typename GMI::Solver     Solver;
  };
};

#endif
    
    
    
