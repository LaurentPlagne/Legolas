#include "Legolas/BlockMatrix/Structures/Sparse/SparseBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseMultOperatorIgnoreEmptyLines.hxx"

namespace Legolas{

  SparseMultOperatorIgnoreEmptyLines::SparseMultOperatorIgnoreEmptyLines( void ){}
  SparseMultOperatorIgnoreEmptyLines::SparseMultOperatorIgnoreEmptyLines(const SparseMultOperatorIgnoreEmptyLines & source){}
  SparseMultOperatorIgnoreEmptyLines::~SparseMultOperatorIgnoreEmptyLines( void ){}
  
  void SparseMultOperatorIgnoreEmptyLines::addMult(const SparseVirtualBlockMatrix & A, const double & a,
				   const VirtualVector & X, VirtualVector & Y){
    
    for (int i=0 ; i< A.nrows() ; i++){
      for (int j=A.beginColInRow(i) ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	A.sparseGetElement(i,j).addMult(a,X.getElement(j),Y.getElement(i));
      }
    }
    
  }


  void SparseMultOperatorIgnoreEmptyLines::mult(const SparseVirtualBlockMatrix & A,
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
      if (j<A.endColInRow(i)){//This line is not empty
	A.sparseGetElement(i,j).mult(X.getElement(j),Y.getElement(i));
      }
      else{//Empty line
	//	INFOS("ICI");
	//Ignore Empty Lines: Only OK if Y.getElement(i) has already be set to Zero !
	assert(Y.getElement(i).dot(Y.getElement(i))<1.e-10);
	//	Y.getElement(i).scale(0.0);
      }
      j=A.nextColInRow(i,j);
      
      //remaining cols
      for ( ; j < A.endColInRow(i) ; j=A.nextColInRow(i,j) ){
	//	INFOS("j="<<j);

	A.sparseGetElement(i,j).addMult(1.0,X.getElement(j),Y.getElement(i));
      }

    }

    //    INFOS("Y="); Y.display();
    
  }
}
    
