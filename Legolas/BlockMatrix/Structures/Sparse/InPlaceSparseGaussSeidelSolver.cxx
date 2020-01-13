#include "Legolas/BlockMatrix/Structures/Sparse/SparseBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/InPlaceSparseGaussSeidelSolver.hxx"
#include <set>

namespace Legolas{

  InPlaceSparseGaussSeidelSolver::InPlaceSparseGaussSeidelSolver( void ){}
  InPlaceSparseGaussSeidelSolver::InPlaceSparseGaussSeidelSolver(const InPlaceSparseGaussSeidelSolver & source){
  }
  InPlaceSparseGaussSeidelSolver::~InPlaceSparseGaussSeidelSolver( void ){
  }
  
  void InPlaceSparseGaussSeidelSolver::solve(const SparseVirtualBlockMatrix & A, const VirtualVector & Bc, VirtualVector & X){

    VirtualVector & B=const_cast<VirtualVector &>(Bc);

    //    INFOS("Entering InPlaceSparseGaussSeidelSolver::solve for the matrix :"<<A.name());
    
    for (int i=A.beginRow() ; i < A.endRow() ; i++ ){

      for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	if (i!=j){
	  A.sparseGetElement(i,j).addMult(-1.0,X.getElement(j),B.getElement(i));
	}
      }

      A.sparseGetElement(i,i).solve(B.getElement(i),X.getElement(i));
      
    }
    
  }


  void InPlaceSparseGaussSeidelSolver::transposeSolve(const SparseVirtualBlockMatrix & A, const VirtualVector & Bc, VirtualVector & X){

    //    INFOS("ICI");
    VirtualVector & B=const_cast<VirtualVector &>(Bc);

    std::vector< std::set<int> > transposeNonZeroElements(A.ncols());
    for (int i=A.endRow()-1 ; i >= A.beginRow() ; i-- ){
      for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	transposeNonZeroElements[j].insert(i);
      }
    }

    //    INFOS("Entering InPlaceSparseGaussSeidelSolver::solve for the matrix :"<<A.name());
    
    //    for (int i=A.endRow()-1 ; i >= A.beginRow() ; i-- ){
    for (int i=transposeNonZeroElements.size()-1 ; i>=0 ; i--){

      //      for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
      auto iter=transposeNonZeroElements[i].begin();
      for ( ; iter!=transposeNonZeroElements[i].end(); iter++ ){
	const int j=*iter;
      //      for (int j=transposeNonZeroElements[i].begin() ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	if (i!=j){
	  A.sparseGetElement(j,i).transposeAddMult(-1.0,X.getElement(j),B.getElement(i));
	}
      }

      //      INFOS("i="<<i);
      A.sparseGetElement(i,i).transposeSolve(B.getElement(i),X.getElement(i));
      //      INFOS("i="<<i);
      
    }
    
  }
  
  
}
    
