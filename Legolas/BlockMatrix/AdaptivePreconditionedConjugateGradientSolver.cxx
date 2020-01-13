#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/AdaptivePreconditionedConjugateGradientSolver.hxx"

namespace Legolas{

  AdaptivePreconditionedConjugateGradientSolver::AdaptivePreconditionedConjugateGradientSolver( void ):RPtr_(0),
												       DPtr_(0),
												       PPtr_(0),
												       ZPtr_(0),
												       BPtr_(0),
												       preconditioner_(0),
												       counter_(0)
  {}
  
  AdaptivePreconditionedConjugateGradientSolver::AdaptivePreconditionedConjugateGradientSolver(Legolas::Matrix * preconditioner):RPtr_(0),
																 DPtr_(0),
																 PPtr_(0),
																 ZPtr_(0),
																 BPtr_(0),
																 preconditioner_(preconditioner),
																 counter_(0)
  {
    //    INFOS("preconditioner_="<<preconditioner_);
  }
  AdaptivePreconditionedConjugateGradientSolver::AdaptivePreconditionedConjugateGradientSolver(const AdaptivePreconditionedConjugateGradientSolver & source):RPtr_(0),
																			     DPtr_(0),
																			     PPtr_(0),
																			     ZPtr_(0),
																			     BPtr_(0),
																			     preconditioner_(0),
																			     counter_(0)
  {}
  
  AdaptivePreconditionedConjugateGradientSolver::~AdaptivePreconditionedConjugateGradientSolver( void ){
    if (RPtr_){delete RPtr_;RPtr_=0;}
    if (DPtr_){delete DPtr_;DPtr_=0;}
    if (PPtr_){delete PPtr_;PPtr_=0;}
    if (ZPtr_){delete ZPtr_;ZPtr_=0;}
    if (BPtr_){delete BPtr_;BPtr_=0;}
    if (preconditioner_){ delete preconditioner_ ; preconditioner_=0 ;}
  }
  
  std::string AdaptivePreconditionedConjugateGradientSolver::name( void ) const {
    assert(preconditioner_);
    std::string result("AdaptivePreconditionedConjugateGradientSolver");
    result+=" (";
    result+=preconditioner_->name();
    result+=")";
    return result;
  }

  bool AdaptivePreconditionedConjugateGradientSolver::restart(const VirtualVector & B,const VirtualVector & R){
    bool result=true;

    if (this->counter_==0){
      VirtualVector::getClone(BPtr_,B);
    }
    else{
      VirtualVector & Bold=(*BPtr_);
      double bobo=Bold.dot(Bold);
      double bb=B.dot(B);
      if ( (std::abs(bobo-bb)/bb < 0.01) && counter_<11){
	result=false;
      }
      else{
	Bold.copy(B);
	counter_=0;
      }
    }

    if (result){
      INFOS("restart");
    }
    else{
      INFOS("no restart");
    }

    counter_++;
    return result;
  }

  //From Quarteroni (p.50)
  void AdaptivePreconditionedConjugateGradientSolver::solve(const Matrix & A, const VirtualVector & B, VirtualVector & X){
    
    if (preconditioner_==0){
      INFOS("There is no preconditioner");
      throw std::runtime_error("There is no preconditioner");
    }
    
    this->iterationControler().initialize(A,X);
    
    VirtualVector & R=VirtualVector::getClone(RPtr_,B);
    
    //R=B-AX;
    A.addMult(-1.0,X,R);

    VirtualVector & Z=VirtualVector::getClone(ZPtr_,R);
    preconditioner_->solve(R,Z);

    if (this->restart(B,R)){
      VirtualVector::getClone(PPtr_,Z);
    }
    
    VirtualVector & P=(*PPtr_);


    VirtualVector & D=VirtualVector::getClone(DPtr_,Z);

    double delta=Z.dot(R);
    
    do {
      //D=A*P
      A.mult(P,D);
      const double alpha=delta/D.dot(P);
      
      //X+=alpha*P;
      X.plusAssign(alpha,P);
      //R-=alpha*D;
      R.plusAssign(-1.0*alpha,D);
      
      const double deltaOld=delta;
      
      preconditioner_->solve(R,Z);
      delta=Z.dot(R);

      const double beta=delta/deltaOld;
      //P=beta*P+R
      P.scaleAndPlusAssign(beta,1.0,Z);
      
    }while(!this->iterationControler().end(X));

    
  }
  
  
}
    
