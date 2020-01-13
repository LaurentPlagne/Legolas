#include "Legolas/BlockMatrix/Structures/Sparse/SparseBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseJacobiSolver.hxx"

namespace Legolas{

  SparseJacobiSolver::SparseJacobiSolver( void ):accumulatorPtr_(0),xnm1Ptr_(0){}
  SparseJacobiSolver::SparseJacobiSolver(const SparseJacobiSolver & source):accumulatorPtr_(){
  }
  SparseJacobiSolver::~SparseJacobiSolver( void ){
    if (accumulatorPtr_){
      delete accumulatorPtr_;
      accumulatorPtr_=0;
    }
    if (xnm1Ptr_){
      delete xnm1Ptr_;
      xnm1Ptr_=0;
    }
  }
  
  void SparseJacobiSolver::solve(const SparseVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X){

    INFOS("Entering SparseJacobiSolver::solve for the matrix :"<<A.name());

    this->iterationControler().initialize(A,X);

    const VirtualVector & B0=B.getElement(0);

    //    const VirtualVector & Xv=X;

    VirtualVector & accumulator=VirtualVector::getClone(accumulatorPtr_,B0);

    VirtualVector & Xnm1=VirtualVector::getClone(xnm1Ptr_,X);

    //    INFOS("ICI"); B.display(); 
    //    INFOS("ICI"); accumulator.display(); 

    do {


      for (int i=A.beginRow() ; i < A.endRow() ; i++ ){
	
	
	accumulator.copy(B.getElement(i));

	//	INFOS("accumulator"); accumulator.display();
	
	for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	  if (i!=j){
 	     //	    INFOS("j="<<j);
	    A.sparseGetElement(i,j).addMult(-1.0,Xnm1.getElement(j),accumulator);
	  }
	}

	//	INFOS("accumulator"); accumulator.display();

	
	A.sparseGetElement(i,i).solve(accumulator,X.getElement(i));

	

      }

      Xnm1.copy(X);


    }while(!this->iterationControler().end(X));

    INFOS("Sparse Jacobi Algoritm has converged within "<<this->iterationControler().iterationNumber()<<" iterations");
      
	


  }
  
  
}
    
