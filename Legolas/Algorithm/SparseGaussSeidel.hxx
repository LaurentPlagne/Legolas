#ifndef __LEGOLAS_SPARSEGAUSSSEIDEL_HXX__
#define __LEGOLAS_SPARSEGAUSSSEIDEL_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "UTILITES.hxx"
#include "Legolas/Algorithm/DefaultLinearSystemIterationControl.hxx"
#include "Legolas/Algorithm/NoLinearSystemAcceleration.hxx"
#include "Legolas/Algorithm/ComputeLinearSystemResidual.hxx"
#include "X86Timer.hxx"

#include "Legolas/Algorithm/LinearSolver.hxx"
#include "Legolas/Vector/Expression/VectorOperations.hxx"

namespace Legolas{
  
  class SparseGaussSeidel{
  public :

    template <class TA, class TX, class TB>
    class Engine : public LinearSolver<TA,TX,TB> {
    private:
      //      typename TB::Element accumulator_;
    public:


      //      Engine():LinearSolver<TA,TX,TB>(),accumulator_(){}

      Engine():LinearSolver<TA,TX,TB>(){}
      
      //      Engine(const TA & A, TX & X, const TB & B ):LinearSolver<TA,TX,TB>(A,X,B),accumulator_(B[0]){}

      typedef LinearSolver<TA,TX,TB> Base;

      typedef typename Base::VectorPoolPtr   VectorPoolPtr;
      typedef typename Base::VectorProvider  VectorProvider;
      typedef typename Base::ElementProvider ElementProvider;


      Engine(const TA & A, TX & X, const TB & B ):LinearSolver<TA,TX,TB>(A,X,B){}
      Engine(const TA & A, TX & X, const TB & B, const VectorPoolPtr & vectorPoolPtr):LinearSolver<TA,TX,TB>(A,X,B,vectorPoolPtr){}

      //      typename TB::Element & accumulator( void ) { return accumulator_ ;}


      inline void solve(const TA & A, TX & X, const TB & B){


	//	TB * BPtr=const_cast<TB *>(&Bc);
	//	TB & B=(*BPtr);

// 	TX & toto=*(this->aquireVectorPtr(X.shape()));
// 	toto=1.0;
// 	INFOS("toto="<<toto);
// 	this->releaseVectorPtr(&toto);

// 	const typename TX::Element & X0=X[0];

// 	typename TX::Element & titi=*(this->aquireElementPtr(X0));
// 	titi=1.0;
// 	INFOS("titi="<<titi);
// 	this->releaseElementPtr(&titi);

	typedef DefaultLinearSystemIterationControl<NoLinearSystemAcceleration<TA,TB,TX> > IterationControl;

	const int level=A.getLevel();
	std::string stringBase="SPGS (L=";
	stringBase+=stringConvert(level);
	stringBase+=") ";

	std::string stringOne=stringBase+"One element";	
	std::string stringInit=stringBase+"Init";	
	std::string stringBi=stringBase+"s=B[i]";	
	std::string stringSolve=stringBase+"X[i]=s/aii";	
	std::string stringAccumulate=stringBase+"s-=aij";	

	X86Timer    chronometre;
	chronometre.start();	

	if (A.nrows()==1){
	  this->blockSolve(A.sparseGetElement(0,0),X[0],B[0]);
	  chronometre.stop();
	  chronometre.storeResult(stringOne);
	}
	else{

	
	  chronometre.start();	
	  IterationControl iter(A,B,X);

	
	  int size=B.size();
	  
	  typedef typename TA::FilledRows FilledRows;
	  FilledRows rows(A);

	  //	  typename TX::Element & accumulator_=*(this->aquireElementPtr(B[0]));

	  ElementProvider ep=this->getElementProvider();
	  typename ElementProvider::ReturnType accumulator_=ep.getItem(Legolas::getShape(B[0]));
	  
	  MESSAGE("Direct Sparse GaussSeidel");

	  chronometre.stop();
	  chronometre.storeResult(stringInit);

	  //	  bool first=true;
	  
	  do {
	    
	    for (int i=0 ; i < size ; i++ ){
	      
	      chronometre.start();	
	      accumulator_=B[i];
	      chronometre.stop();
	      chronometre.storeResult(stringBi);
	      
	      typename FilledRows::FilledCols cols=rows.filledColsInRow(i);

 	      chronometre.start();	
	      for (int j=cols.begin() ; j < cols.end() ; cols.next(j) ){
		//		if (j==cols.begin()) accumulator_=B[i];
		if (i!=j) accumulator_-=A.sparseGetElement(i,j)*X[j];
	      }
	      
	      
	 //      int j=i;
// 	      cols.next(j);
	      
// 	      chronometre.start();	
	      
// 	      for ( ; j < cols.end() ; cols.next(j) ) accumulator_-=A.sparseGetElement(i,j)*X[j];


//	      for ( ; j < cols.end() ; cols.next(j) ) accumulator_-=A.sparseGetElement(i,j)*X[j];
//	      for (int j=cols.begin() ; j < i ; cols.next(j) ) accumulator_-=A.sparseGetElement(i,j)*X[j];

	      //	      for ( ; j < cols.end() ; cols.next(j) ) B[i]-=A.sparseGetElement(i,j)*X[j];
	      //	      for (int j=cols.begin() ; j < i ; cols.next(j) ) B[i]-=A.sparseGetElement(i,j)*X[j];
	      
	      
	      chronometre.stop();
	      chronometre.storeResult(stringAccumulate);
	      
	      chronometre.start();	
	      this->blockSolve(A.sparseGetElement(i,i),X[i],accumulator_,i);
	      //	      this->blockSolve(A.sparseGetElement(i,i),X[i],B[i],i);
	      
	      chronometre.stop();
	      chronometre.storeResult(stringSolve);
	      
	    }  
	    
	  }while(!iter.end(X));

	  //	  this->releaseElementPtr(&accumulator_);
	}

	  
	//	ComputeLinearSystemResidual::apply(A,X,B);
	
      }


      inline void transposeSolve(const TA & A, TX & X, const TB & B){

	typedef DefaultLinearSystemIterationControl<NoLinearSystemAcceleration<TA,TB,TX> > IterationControl;
	
	X86Timer    chronometre;
	chronometre.start();	

	if (A.nrows()==1){
	  this->blockTransposeSolve(A.sparseGetElement(0,0),X[0],B[0],0);
	  chronometre.stop();
	  chronometre.storeResult("SPMInvGS One element");
	}
	else{

	
	  IterationControl iter(A,B,X);
	  
	  chronometre.stop();
	  chronometre.storeResult("SPMInv Init");
	  

	  ElementProvider ep=this->getElementProvider();
	  typename ElementProvider::ReturnType accumulator_=ep.getItem(Legolas::getShape(B[0]));
	  //	  typename TX::Element & accumulator_=*(this->aquireElementPtr(B[0]));
      
	  int size=B.size();
	  
	  typedef typename TA::FilledCols FilledCols;
	  FilledCols cols(A);
	  
	  do {
	    
	    for (int i=size-1 ; i > -1 ; i-- ){
	      
	      chronometre.start();
	      accumulator_=B[i];
	      chronometre.stop();
	      chronometre.storeResult("SPMInv s=B[i]");
	      
	      typename FilledCols::FilledRows rows=cols.filledRowsInCol(i);
	      
	      chronometre.start();	


 	      for (int j=rows.begin() ; j < rows.end() ; rows.next(j) ){
		if (i!=j) accumulator_-=transpose(A.sparseGetElement(j,i))*X[j];
 	      }

	      chronometre.stop();
	      chronometre.storeResult("SPMInv s-=aijxj");
	  
	      chronometre.start();	
	      this->blockTransposeSolve(A.sparseGetElement(i,i),X[i],accumulator_,i);

	      chronometre.stop();
	      chronometre.storeResult("SPMInv X[i]=s/aii");
	    
	    }  

	  }while(!iter.end(X));
	  
	  //	  this->releaseElementPtr(&accumulator_);
	  //	ComputeLinearSystemResidual::apply(A,X,B);
	
	}
      }

    };
    
  };

}


#endif		
