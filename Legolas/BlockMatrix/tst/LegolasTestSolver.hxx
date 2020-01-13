#ifndef __LEGOLAS_TEST_SOLVER_HXX__
#define __LEGOLAS_TEST_SOLVER_HXX__

#include "UTILITES.hxx" 

template <class MATRIX, class VECTOR>
void solveAndCheckResidual(const MATRIX & A, const VECTOR & Xref){
  VECTOR B(Xref.shape());
  VECTOR X(Xref.shape());
  
  //B=A*X
  B=0.0;
  A.addMult(1.0,Xref,B);
  INFOS("B="<<B);
  X=0.0;
  std::cout << "Solving A*X=B with B=A*Xref" << std::endl;
  A.solve(B,X);
  INFOS("X="<<X);
  // INFOS("Xref="<<Xref);

  // for (size_t i=0 ; i<X.flatSize() ; i++){
  //   INFOS("X("<<i<<")="<<X(i));
  //   INFOS("Xref("<<i<<")="<<Xref(i));
  // }


  //  std::cout <<"X="<<X<< std::endl;
  std::cout <<"Residual : Legolas::norm2(X-Xref)="<<Legolas::norm2(X-Xref)<< std::endl;

  std::cout <<"External Iteration Number="<<A.iterationNumber()<< std::endl;
  std::cout <<"||X-Xold||/||X||="<<A.relativeDifference()<< std::endl;

  // if (Legolas::norm2(X-Xref)>1.e-6){
  //   INFOS("ERROR (Legolas::norm2(X-Xref)="<<(Legolas::norm2(X-Xref)));
  //   throw std::runtime_error("Legolas::norm2(X-Xref)>1.e-6");
  // }

  A.displayProfile();
}

#endif
  
  
  
  
