#include "Legolas/BlockMatrix/Structures/Sparse/SparseBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseBootsProfileGaussSeidelSolver.hxx"

namespace Legolas{

  SparseBootsProfileGaussSeidelSolver::SparseBootsProfileGaussSeidelSolver( void ):accumulatorPtr_(0){}
  SparseBootsProfileGaussSeidelSolver::SparseBootsProfileGaussSeidelSolver(const SparseBootsProfileGaussSeidelSolver & source):accumulatorPtr_(){
  }
  SparseBootsProfileGaussSeidelSolver::~SparseBootsProfileGaussSeidelSolver( void ){
    if (accumulatorPtr_){
      delete accumulatorPtr_;
      accumulatorPtr_=0;
    }
  }
  
  void SparseBootsProfileGaussSeidelSolver::solve(const SparseVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X){
    
    MESSAGE("Entering SparseBootsProfileGaussSeidelSolver::solve for the matrix :"<<A.name());
    //LP: The aim is to determine ic a row index defined by
    //LP:  forall i1<ic :
    //LP:    forall i2>i1 A(g1,g2)=0.
    //LP: With this definition, the GS algorithm can only iterate other the
    //LP: row's indexes i>ic.
    
    this->iterationControler().initialize(A,X);

    
    const VirtualVector & B0=B.getElement(0);
    VirtualVector & accumulator=VirtualVector::getClone(accumulatorPtr_,B0);


    int ic=-1;
    bool found=false;
    
    for (int i=A.beginRow() ; i < A.endRow() ; i++ ){
      int jmax=0;          
      for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	jmax=j;
      }
      if ( (!found) && (jmax<=i) ){
	ic=i;
      }
      else{
	found=true;
      }
    }
    
    //    INFOS("ic="<<ic);

    for (int i=A.beginRow() ; i <= ic ; i++ ){
      accumulator.copy(B.getElement(i));
      
      for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	if (i!=j){
	  A.sparseGetElement(i,j).addMult(-1.0,X.getElement(j),accumulator);
	}
      }
       
      //      A.sparseGetElement(i,i).modifyEpsilon(this->iterationControler().epsilon());
      
      for (int ig=0 ; ig<this->iterationControler().maxIteration() ; ig++){
	A.sparseGetElement(i,i).solve(accumulator,X.getElement(i));
      }

    }

    do {

      for (int i=A.beginRow() ; i < A.endRow() ; i++ ){
	if (i>ic){

	  accumulator.copy(B.getElement(i));

	  for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	    if (i!=j){
	      A.sparseGetElement(i,j).addMult(-1.0,X.getElement(j),accumulator);
	    }
	  }
	  
	  
	  //	  A.sparseGetElement(i,i).modifyEpsilon(this->iterationControler().epsilon());
	  A.sparseGetElement(i,i).solve(accumulator,X.getElement(i));
	}
      }
      
    }while(!this->iterationControler().end(X));
  }
  
  
}
    
