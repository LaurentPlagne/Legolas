#include "Legolas/BlockMatrix/Structures/Sparse/SparseBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseMultOperator.hxx"
#include <set>

namespace Legolas{

  SparseMultOperator::SparseMultOperator( void ){}
  SparseMultOperator::SparseMultOperator(const SparseMultOperator & source){}
  SparseMultOperator::~SparseMultOperator( void ){}
  
  void SparseMultOperator::addMult(const SparseVirtualBlockMatrix & A, const double & a,
				   const VirtualVector & X, VirtualVector & Y){
    
    for (int i=0 ; i< A.nrows() ; i++){
      for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	A.sparseGetElement(i,j).addMult(a,X.getElement(j),Y.getElement(i));
      }
    }
    
  }

  void SparseMultOperator::mult(const SparseVirtualBlockMatrix & A,
				const VirtualVector & X, VirtualVector & Y){

    // INFOS("X="); X.display();

    // INFOS("A.nrows()="<<A.nrows());
    // INFOS("A.ncols()="<<A.ncols());
    // INFOS("X.size()="<<X.size());
    // INFOS("Y.size()="<<Y.size());
    
    //    INFOS("A.name()="<<A.name());
    //    INFOS("X="); X.display();
    
    for (int i=0 ; i< A.nrows() ; i++){
      //first col
      int j=A.beginColInRow(i);
      //      INFOS("j="<<j<<" A.endColInRow(i)="<<A.endColInRow(i));
      if (j<A.endColInRow(i)){//This line is not empty
	A.sparseGetElement(i,j).mult(X.getElement(j),Y.getElement(i));
      }
      else{//Empty line
	//	INFOS("ICI");
	Y.getElement(i).scale(0.0);
      }
      j=A.nextColInRow(i,j);
      
      //remaining cols
      for ( ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	A.sparseGetElement(i,j).addMult(1.0,X.getElement(j),Y.getElement(i));
      }

    }
  }

  void SparseMultOperator::transposeAddMult(const SparseVirtualBlockMatrix & A, const double & a,
					    const VirtualVector & X, VirtualVector & Y){

  
    for (int i=0 ; i< A.nrows() ; i++){
      //      INFOS("i="<<i);
      for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	//    	INFOS("j="<<j);
    	//	A.sparseGetElement(i,j).addMult(a,X.getElement(j),Y.getElement(i));}
    	A.sparseGetElement(i,j).transposeAddMult(a,X.getElement(i),Y.getElement(j));
      }
    }
    
  }
  
  
  void SparseMultOperator::transposeMult(const SparseVirtualBlockMatrix & A,
					 const VirtualVector & X, VirtualVector & Y){
    
    std::vector< std::set<int> > transposeNonZeroElements(A.ncols());
    for (int i=A.endRow()-1 ; i >= A.beginRow() ; i-- ){
      for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
    	transposeNonZeroElements[j].insert(i);
      }
    }

    for (uint i=0 ; i<transposeNonZeroElements.size() ; i++){
      auto iter=transposeNonZeroElements[i].begin();

      if (iter!=transposeNonZeroElements[i].end()){//This line is not empty
	const int j=*iter;
	//	INFOS("j="<<j);
	A.sparseGetElement(j,i).transposeMult(X.getElement(j),Y.getElement(i));
	iter++;
	for ( ; iter!=transposeNonZeroElements[i].end(); iter++ ){
	  const int j=*iter;
	  A.sparseGetElement(j,i).transposeAddMult(1.0,X.getElement(j),Y.getElement(i));
	}
      }
      else{//this line is empty
	Y.getElement(i).scale(0.0);
      }
    }
  
  }

}
    
