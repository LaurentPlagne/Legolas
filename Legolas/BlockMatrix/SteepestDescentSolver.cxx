#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/SteepestDescentSolver.hxx"

namespace Legolas{
  
  SteepestDescentSolver::SteepestDescentSolver( void ):RPtr_(0),DPtr_(0){}
  SteepestDescentSolver::SteepestDescentSolver(const SteepestDescentSolver & source):RPtr_(0),DPtr_(0){
  }
  SteepestDescentSolver::~SteepestDescentSolver( void ){
    if (RPtr_){delete RPtr_;RPtr_=0;}
    if (DPtr_){delete DPtr_;DPtr_=0;}
  }

  //From Quarteroni
  void SteepestDescentSolver::solve(const Matrix & A, const VirtualVector & B, VirtualVector & X){

    this->iterationControler().initialize(A,X);
    
    VirtualVector & R=VirtualVector::getClone(RPtr_,B);
    
    //R=B-AX;
    A.addMult(-1.0,X,R);

    VirtualVector & D=VirtualVector::getClone(DPtr_,R);
    
    double delta=R.dot(R);

    do {
      //D=A*R
      A.mult(R,D);
      //alpha=(R,R)/(R,AR)
      const double alpha=delta/D.dot(R);
      
      //X+=alpha*P;
      X.plusAssign(alpha,R);
      //R-=alpha*D;
      R.plusAssign(-1.0*alpha,D);

      delta=R.dot(R);
      
    }while(!this->iterationControler().end(X));

    
  }
  
  
}
    
