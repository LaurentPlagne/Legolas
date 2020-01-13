#pragma once
#include "EigenWrapper.hxx"

class TBBSSESolverOperatorBis{

  typedef EigenWrapper<RealType>::BlockArray BlockArray;
  
  static const int blockSize=EigenWrapper<RealType>::blockSize;

  typedef Legolas::MultiVector<RealType,1> V1D;
  typedef Legolas::MultiVector<RealType,2>  V2D;

  const V2D & DI_;
  const V2D & LI_;
  const V2D & UI_;
  
  const V2D & BI_;
  V2D & SI_;
  V2D & XI_;
  
  int s2_;

public:

  
  TBBSSESolverOperatorBis(const V2D & DI,const V2D & LI,const V2D & UI,
			  const V2D & BI,V2D & SI, V2D & XI):DI_(DI),LI_(LI),UI_(UI),
							     BI_(BI),SI_(SI),XI_(XI){
    s2_=XI_[0].size()/blockSize;;
  }

  
  inline void operator()(const my_tbb::blocked_range<int> & r) const {

    
    for (int b=r.begin() ; b!=r.end() ; b++){
      
      ConstEigenWrapper<RealType> D(DI_[b]);
      ConstEigenWrapper<RealType> U(UI_[b]);
      ConstEigenWrapper<RealType> L(LI_[b]);
      ConstEigenWrapper<RealType> B(BI_[b]);
      
      EigenWrapper<RealType> X(XI_[b]);
      EigenWrapper<RealType> S(SI_[b]);
      
      BlockArray s(D[0]);
      BlockArray sm1=s.inverse();

      X[0]=B[0]*sm1;
      
      for (int i=1 ; i < s2_ ; i++ ){
	
	S[i]=U[i-1]*sm1;
	s=D[i]-L[i]*S[i];	
	X[i]=B[i]-L[i]*X[i-1];
	sm1=s.inverse();
	X[i]*=sm1;
	
      }
      
      for (int i=(s2_-2) ; i >= 0 ; i-- ){
	X[i]-=S[i+1]*X[i+1];
      }

    }
  }
};
    
