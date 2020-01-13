#ifndef __LEGOLAS_BOOTSPROFILESPARSEGAUSSSEIDEL_HXX__
#define __LEGOLAS_BOOTSPROFILESPARSEGAUSSSEIDEL_HXX__

#include "Legolas/Algorithm/IterationControl.hxx"
#include "UTILITES.hxx"
#include "Legolas/Algorithm/DefaultLinearSystemIterationControl.hxx"
#include "Legolas/Algorithm/NoLinearSystemAcceleration.hxx"
#include "Legolas/Algorithm/ComputeLinearSystemResidual.hxx"
#include "X86Timer.hxx"

#include "Legolas/Algorithm/LinearSolver.hxx"
#include "Legolas/Vector/Expression/VectorOperations.hxx"

namespace Legolas{
  
  class SparseBootsProfileGaussSeidel{
  public :
    
    template <class TA, class TX, class TB>
    class Engine : public LinearSolver<TA,TX,TB> {
    private:
    public:

      Engine():LinearSolver<TA,TX,TB>(){}
      
      typedef LinearSolver<TA,TX,TB> Base;

      typedef typename Base::VectorPoolPtr   VectorPoolPtr;
      typedef typename Base::VectorProvider  VectorProvider;
      typedef typename Base::ElementProvider ElementProvider;


      Engine(const TA & A, TX & X, const TB & B ):LinearSolver<TA,TX,TB>(A,X,B){}
      Engine(const TA & A, TX & X, const TB & B, const VectorPoolPtr & vectorPoolPtr):LinearSolver<TA,TX,TB>(A,X,B,vectorPoolPtr){}

      inline void solve(const TA & A, TX & X, const TB & B){


	typedef DefaultLinearSystemIterationControl<NoLinearSystemAcceleration<TA,TB,TX> > IterationControl;

	const int level=A.getLevel();
	std::string stringBase="SPGS Boots Profile (L=";
	stringBase+=stringConvert(level);
	stringBase+=") ";

	std::string stringOne=stringBase+"One element";	
	std::string stringInit=stringBase+"Init";	
	std::string stringBi=stringBase+"s=B[i]";	
	std::string stringSolve=stringBase+"X[i]=s/aii";	
	std::string stringAccumulate=stringBase+"s-=aij";	

	X86Timer    chronometre;
	chronometre.start();	


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
	//LP: The aim is to determine ic a row index defined by
	//LP:  forall i1<ic :
	//LP:    forall i2>i1 A(g1,g2)=0.
	//LP: With this definition, the GS algorithm can only iterate other the
	//LP: row's indexes i>ic.
	
	

	int ic=-1;
	bool found=false;
	
	for (int i=0 ; i < size ; i++ ){
	  int jmax=0;          
	  typename FilledRows::FilledCols cols=rows.filledColsInRow(i);
	  for (int j=cols.begin() ; j < cols.end() ; cols.next(j) ){
	    jmax=j;
	  }
	  if ( (!found) && (jmax<=i) ){
	    ic=i;
	  }
	  else{
	    found=true;
	  }
	}
	
	//	INFOS("ic="<<ic);

	
	for (int i=0 ; i <= ic ; i++ ){
	    
	  chronometre.start();	
	  accumulator_=B[i];
	  chronometre.stop();
	  chronometre.storeResult(stringBi);
	  
	  typename FilledRows::FilledCols cols=rows.filledColsInRow(i);
	  
	  chronometre.start();	
	  for (int j=cols.begin() ; j < cols.end() ; cols.next(j) ){
	    if (i!=j) accumulator_-=A.sparseGetElement(i,j)*X[j];
	  }
	  
	  chronometre.stop();
	  chronometre.storeResult(stringAccumulate);

	  
	  for (int ig=0 ; ig<A.getMaxIteration() ; ig++){
	    chronometre.start();	
	    this->blockSolve(A.sparseGetElement(i,i),X[i],accumulator_,i);
	    chronometre.stop();
	    chronometre.storeResult(stringSolve);
	  }
	  
	}
	
	  
	
	do {
	  
	  for (int i=ic+1 ; i < size ; i++ ){
	    
	    chronometre.start();	
	    accumulator_=B[i];
	    chronometre.stop();
	    chronometre.storeResult(stringBi);
	    
	    typename FilledRows::FilledCols cols=rows.filledColsInRow(i);
	    
	    chronometre.start();	
	    for (int j=cols.begin() ; j < cols.end() ; cols.next(j) ){
	      if (i!=j) accumulator_-=A.sparseGetElement(i,j)*X[j];
	    }
	    
	    chronometre.stop();
	    chronometre.storeResult(stringAccumulate);
	    
	    chronometre.start();	
	    this->blockSolve(A.sparseGetElement(i,i),X[i],accumulator_,i);
	    
	    chronometre.stop();
	    chronometre.storeResult(stringSolve);
	    
	    }  
	  
	}while(!iter.end(X));
	
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
