#ifndef __LEGOLAS_SPARSESCALARMATRIXINTERFACE_HXX__
#define __LEGOLAS_SPARSESCALARMATRIXINTERFACE_HXX__

#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/VirtualSolver.hxx"
#include "Legolas/BlockMatrix/VirtualMultOperator.hxx"
#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/BlockProfile.hxx"

namespace Legolas{
  
  template <class SPARSE_MATRIX_CONTAINER, class MATRIX_OPTIONS>
  class SparseScalarMatrixInterface: public SPARSE_MATRIX_CONTAINER {
  public:
    typedef typename SPARSE_MATRIX_CONTAINER::RealType RealType;
    //    typedef typename SPARSE_MATRIX_CONTAINER::Container Container;
    typedef SPARSE_MATRIX_CONTAINER Container;
  private:
    typedef Container Base;
    typedef Legolas::MultiVector<RealType,1> V1D;
    
  public:

    virtual ~SparseScalarMatrixInterface( void ){}

    typedef Legolas::MatrixShape<1> Shape;

    inline void initializeOperatorPtr( void ){
      typedef typename MATRIX_OPTIONS::Solver::template Engine<SparseScalarMatrixInterface> Solver;
      this->setVirtualSolverPtr(new Solver());
      typedef typename MATRIX_OPTIONS::MultOperator:: template Engine<SparseScalarMatrixInterface> MultOperator;
      this->setVirtualMultOperatorPtr(new MultOperator());
    }      
    
    inline SparseScalarMatrixInterface(const SparseScalarMatrixInterface & source):Base(source){
      this->setVirtualSolverPtr(source.virtualSolver()->clone());
      this->setVirtualSolverPtr(source.virtualMultOperatorPtr()->clone());
    }
    inline SparseScalarMatrixInterface(const VirtualMatrixShape & virtualMatrixShape):Base(virtualMatrixShape){
      //      INFOS("ICI");
      this->initializeOperatorPtr();
    }
    
    inline SparseScalarMatrixInterface(const Container & container):Base(container){
      //      INFOS("ICI");
      this->initializeOperatorPtr();
    }
    
    void iterateOverElements(Legolas::MatrixStream & matrixStream) const{
      
      const SparseScalarMatrixInterface & block=*this;
      
      const int nr=block.nrows();
      const int nc=block.ncols();

      assert(nr!=0);
      assert(nc!=0);

      matrixStream.blockInitialize(nr,nc);

      std::vector<bool> emptycol(nc,true);
      std::vector< std::vector<bool> > emptyness(nr,emptycol);
      
      for (int i=block.beginRow() ; i < block.endRow() ; i++ ){
	for (int j=block.beginColInRow(i) ; j < block.endColInRow(i) ; j=block.nextColInRow(i,j) ){
	  emptyness[i][j]=false;
	}
      }
      
      for (int i=0 ; i < nr ; i++){     
	matrixStream.rowInitialize(i);
	for (int j=0 ; j < nc ; j++){

	  matrixStream.colInRowInitialize(i,j);
	  if (!emptyness[i][j]){
	    matrixStream.createNonEmptyBlock(i,j);
	    matrixStream.dumpElement(i,j,block.sparseGetElement(i,j));
	  }
	  else{
	    matrixStream.createEmptyBlock(i,j);
	  }
	  matrixStream.colInRowFinalize(i,j);
	}
	matrixStream.rowFinalize(i);
      }
      matrixStream.blockFinalize();
    }

    
    void collectStatistics(Profile & profile) const{

      const SparseScalarMatrixInterface & A=*this;
      //      const SparseVirtualBlockMatrix & A=*this;
      
      if (A.virtualMultOperator()){
	if (A.virtualMultOperator()->counter()>0){
	  profile[A.level()][A.name()].operationProfiles[A.virtualMultOperator()->name()].duration+=A.virtualMultOperator()->duration();
	  profile[A.level()][A.name()].operationProfiles[A.virtualMultOperator()->name()].counter+=A.virtualMultOperator()->counter();
	}
      }

      if (A.virtualSolver()){
	if (A.virtualSolver()->counter()>0){
	  profile[A.level()][A.name()].operationProfiles[A.virtualSolver()->name()].duration+=A.virtualSolver()->duration();
	  profile[A.level()][A.name()].operationProfiles[A.virtualSolver()->name()].counter+=A.virtualSolver()->counter();
	}
      }

    }

    int getPrecisionInBytePerRealElement( void ) const{
      return sizeof(RealType);
    }

    virtual std::string name( void ) const{
      return "SparseScalarMatrix";
    }


  };
  
}

#endif

    
    
    
    

  
