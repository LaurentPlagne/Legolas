#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/ConjugateGradientSolver.hxx"

namespace Legolas{
  
  ConjugateGradientSolver::ConjugateGradientSolver( void ):RPtr_(0),DPtr_(0),PPtr_(0){}
  ConjugateGradientSolver::ConjugateGradientSolver(const ConjugateGradientSolver & source):RPtr_(0),DPtr_(0),PPtr_(0){
  }
  ConjugateGradientSolver::~ConjugateGradientSolver( void ){
    if (RPtr_){delete RPtr_;RPtr_=0;}
    if (DPtr_){delete DPtr_;DPtr_=0;}
    if (PPtr_){delete PPtr_;PPtr_=0;}
  }

  //From Quarteroni
  void ConjugateGradientSolver::solve(const Matrix & A, const VirtualVector & B, VirtualVector & X){

    this->iterationControler().initialize(A,X);
    
    VirtualVector & R=VirtualVector::getClone(RPtr_,B);
    
    //R=B-AX;
    A.addMult(-1.0,X,R);

    VirtualVector & P=VirtualVector::getClone(PPtr_,R);
    VirtualVector & D=VirtualVector::getClone(DPtr_,R);
    
    double delta=R.dot(R);

    do {
      //D=A*P
      A.mult(P,D);
      const double alpha=delta/D.dot(P);
      
      //X+=alpha*P;
      X.plusAssign(alpha,P);
      //R-=alpha*D;
      R.plusAssign(-1.0*alpha,D);
      
      const double deltaOld=delta;
      delta=R.dot(R);

      const double beta=delta/deltaOld;
      //P=beta*P+R
      P.scaleAndPlusAssign(beta,1.0,R);
      
    }while(!this->iterationControler().end(X));

    
  }
  
  
}
    
