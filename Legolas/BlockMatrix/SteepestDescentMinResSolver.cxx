#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/SteepestDescentMinResSolver.hxx"

namespace Legolas{
  
  SteepestDescentMinResSolver::SteepestDescentMinResSolver( void ):RPtr_(0),DPtr_(0){}
  SteepestDescentMinResSolver::SteepestDescentMinResSolver(const SteepestDescentMinResSolver & source):RPtr_(0),DPtr_(0){
  }
  SteepestDescentMinResSolver::~SteepestDescentMinResSolver( void ){
    if (RPtr_){delete RPtr_;RPtr_=0;}
    if (DPtr_){delete DPtr_;DPtr_=0;}
  }

  //From Quarteroni
  void SteepestDescentMinResSolver::solve(const Matrix & A, const VirtualVector & B, VirtualVector & X){

    this->iterationControler().initialize(A,X);
    
    VirtualVector & R=VirtualVector::getClone(RPtr_,B);
    
    //R=B-AX;
    A.addMult(-1.0,X,R);

    VirtualVector & D=VirtualVector::getClone(DPtr_,R);
    
    do {
      //D=A*R
      A.mult(R,D);
      //alpha=(R,D)/(D,D)
      const double alpha=D.dot(R)/D.dot(D);
      
      //X+=alpha*P;
      X.plusAssign(alpha,R);
      //R-=alpha*D;
      R.plusAssign(-1.0*alpha,D);

    }while(!this->iterationControler().end(X));

    
  }
  
  
}
    
