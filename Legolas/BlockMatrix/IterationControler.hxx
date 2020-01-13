#ifndef __LEGOLAS_ITERATION_CONTROLER_HXX__
#define __LEGOLAS_ITERATION_CONTROLER_HXX__

#include "Legolas/Vector/Vector.hxx"


namespace Legolas{

  class IterationControler{
    VirtualVector * XoldPtr_;
    int iterationNumber_;
    int maxIteration_;
    mutable double epsilon_;
    bool fixedIterationNumber_;
    double relativeDifference_;
    
  public:

    inline int iterationNumber( void ) const { return iterationNumber_ ; }
    inline int & getIterationNumber( void ) { return iterationNumber_ ; }
    inline double relativeDifference( void ) const { return relativeDifference_;}

    const int & maxIteration( void ) const {return maxIteration_;}
    int & maxIteration( void ) {return maxIteration_;}
    const double & epsilon( void ) const {return epsilon_;}
    double & epsilon( void ) {return epsilon_;}

    void modifyEpsilon( double epsilon ) const { epsilon_=epsilon ;}

    const bool & fixedIterationNumber( void ) const {return fixedIterationNumber_;}
    bool & fixedIterationNumber( void ) {return fixedIterationNumber_;}

    inline IterationControler( void ):XoldPtr_(0),
				      iterationNumber_(0),
				      maxIteration_(100),
				      epsilon_(1.e-12),
				      fixedIterationNumber_(false),
				      relativeDifference_(-1.0)
    {
    }

    inline IterationControler(const IterationControler & ic):XoldPtr_(ic.XoldPtr_->clone()),
							     iterationNumber_(ic.iterationNumber_),
							     maxIteration_(ic.maxIteration_),
							     epsilon_(ic.epsilon_),
							     fixedIterationNumber_(ic.fixedIterationNumber_),
							     relativeDifference_(ic.relativeDifference_)
    {
    }

    
    inline ~IterationControler( void ){
      if (XoldPtr_){
	delete XoldPtr_;
	XoldPtr_=0;
      }
    }
    
      

    template <class MATRIX>
    inline void initialize(const MATRIX & A, const VirtualVector & Xold){
      
      iterationNumber_=0;
      maxIteration_=A.maxIteration();
      epsilon_=A.epsilon();
      fixedIterationNumber_=A.fixedIterationNumber();
      
      if (maxIteration_<=1) fixedIterationNumber_=true;
      
      if (!fixedIterationNumber_){
	//Ensure Xold_ ==Xold
	if (!XoldPtr_){ 
	  XoldPtr_=Xold.clone(); 
	}
	else{
	  if (!XoldPtr_->sameShape(Xold)){
	    INFOS("TEMPORARY RESIZE...");
	    delete XoldPtr_;
	    XoldPtr_=Xold.clone();
	  }
	  else{
	    XoldPtr_->copy(Xold);
	  }
	}
      }
    }

    inline  bool end(const VirtualVector & X){
      bool result=false;
      iterationNumber_++;
      if (iterationNumber_>=maxIteration_){
	result=true;
	//	INFOS("iterationNumber>=maxIteration"<< maxIteration_);
      }
      else{
	if (!fixedIterationNumber_){
	  //returns relativeDiff=||X-Xold|| and Copy Xold=X
	  relativeDifference_=XoldPtr_->relativeDiffAndCopy(X);
	  if (relativeDifference_<epsilon_){ result=true;
	    //	    INFOS("relativeDifference="<<relativeDifference_<<" (iterationNumber="<<iterationNumber_<<")");
	  }
	}
      }
      return result;
    }
  };

}

#endif
	
	
	
      
	  
	  
				 

      
      
