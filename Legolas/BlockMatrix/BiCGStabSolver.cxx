#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/BiCGStabSolver.hxx"

namespace Legolas{
  
  BiCGStabSolver::BiCGStabSolver( void ):RkPtr_(0),RtkPtr_(0),PkPtr_(0),PtkPtr_(0),QkPtr_(0),QtkPtr_(0){}
  BiCGStabSolver::BiCGStabSolver(const BiCGStabSolver & source):RkPtr_(0),RtkPtr_(0),PkPtr_(0),PtkPtr_(0),QkPtr_(0),QtkPtr_(0){
  }
  BiCGStabSolver::~BiCGStabSolver( void ){
    if (RkPtr_){delete RkPtr_;RkPtr_=0;}
    if (RtkPtr_){delete RtkPtr_;RtkPtr_=0;}

    if (PkPtr_){delete PkPtr_;PkPtr_=0;}
    if (PtkPtr_){delete PtkPtr_;PtkPtr_=0;}

    if (QkPtr_){delete QkPtr_;QkPtr_=0;}
    if (QtkPtr_){delete QtkPtr_;QtkPtr_=0;}

  }

  void BiCGStabSolver::solve(const Matrix & A, const VirtualVector & B, VirtualVector & X){

    this->iterationControler().initialize(A,X);

    VirtualVector & Rk=VirtualVector::getClone(RkPtr_,B);
    VirtualVector & Rtk=VirtualVector::getClone(RtkPtr_,B);

    VirtualVector & Pk=VirtualVector::getClone(PkPtr_,B);
    VirtualVector & Ptk=VirtualVector::getClone(PtkPtr_,B);

    VirtualVector & Qk=VirtualVector::getClone(QkPtr_,X);
    VirtualVector & Qtk=VirtualVector::getClone(QtkPtr_,X);
    
    Rk.copy(B);//rk=B
    A.addMult(-1.0,X,Rk);//rk-=A*X;
    Rtk.copy(Rk);//rtk=rk;
    
    Pk.copy(Rk);//pk=rk;
    Ptk.copy(Rtk);//ptk=rtk;
    
    Qk.copy(X);//qk=X;
    Qtk.copy(X);//qtk=X;
    
    
    double rhoKm1=0.0;
    
    do {
      
      const double rhoK=Rk.dot(Rtk);//Legolas::dot(rk,rtk);
      
      if (this->iterationControler().iterationNumber()!=0){
	const double betaK=rhoK/rhoKm1;
	
	Pk.scale(betaK);//pk*=betaK;
	Pk.plusAssign(1.0,Rk);//pk+=rk;
	Ptk.scale(betaK);//ptk*=betaK;
	Ptk.plusAssign(1.0,Rtk);//ptk+=rtk;
      }
      
      A.mult(Pk,Qk);//qk=A*pk;
      A.mult(Ptk,Qtk);//qtk=transpose(A)*ptk;
      
      const double alpha=rhoK/Ptk.dot(Qk);//Legolas::dot(ptk,qk);
      
      X.plusAssign(alpha,Pk);//      X+=alpha*pk;
      Rk.plusAssign(-alpha,Qk);//rk-=alpha*qk;
      Rtk.plusAssign(-alpha,Qtk);//rtk-=alpha*qtk;
      
      rhoKm1=rhoK;
      
      
    }while(!(this->iterationControler().end(X)));
    
    
    INFOS("Linear BiConjugate Gradient Algoritm has converged within "<<this->iterationControler().iterationNumber()<<" iterations");
    
  }
  
  
  
}
    
