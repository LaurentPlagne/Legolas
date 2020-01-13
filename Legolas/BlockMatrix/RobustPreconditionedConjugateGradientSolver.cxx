#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/RobustPreconditionedConjugateGradientSolver.hxx"

namespace Legolas{

  RobustPreconditionedConjugateGradientSolver::RobustPreconditionedConjugateGradientSolver( void ):RPtr_(0),
												       DPtr_(0),
												       PPtr_(0),
												       ZPtr_(0),
												       BPtr_(0),
												       preconditioner_(0),
												       counter_(0)
  {}
  
  RobustPreconditionedConjugateGradientSolver::RobustPreconditionedConjugateGradientSolver(Legolas::Matrix * preconditioner):RPtr_(0),
																 DPtr_(0),
																 PPtr_(0),
																 ZPtr_(0),
																 BPtr_(0),
																 preconditioner_(preconditioner),
																 counter_(0)
  {
    //    INFOS("preconditioner_="<<preconditioner_);
  }
  RobustPreconditionedConjugateGradientSolver::RobustPreconditionedConjugateGradientSolver(const RobustPreconditionedConjugateGradientSolver & source):RPtr_(0),
																			     DPtr_(0),
																			     PPtr_(0),
																			     ZPtr_(0),
																			     BPtr_(0),
																			     preconditioner_(0),
																			     counter_(0)
  {}
  
  RobustPreconditionedConjugateGradientSolver::~RobustPreconditionedConjugateGradientSolver( void ){
    INFOS("ici");
    if (RPtr_){delete RPtr_;RPtr_=0;}
    INFOS("ici");
    if (DPtr_){delete DPtr_;DPtr_=0;}
    INFOS("ici");
    if (PPtr_){delete PPtr_;PPtr_=0;}
    INFOS("ici");
    if (ZPtr_){delete ZPtr_;ZPtr_=0;}
    INFOS("ici");
    if (preconditioner_){ delete preconditioner_ ; preconditioner_=0 ;}
    INFOS("ici");
  }
  
  std::string RobustPreconditionedConjugateGradientSolver::name( void ) const {
    assert(preconditioner_);
    std::string result("RobustPreconditionedConjugateGradientSolver");
    result+=" (";
    result+=preconditioner_->name();
    result+=")";
    return result;
  }

  bool RobustPreconditionedConjugateGradientSolver::restart(const VirtualVector & B,const VirtualVector & R){
    bool result=true;

    if (this->counter_==0){
      rmax_=0.01;
    }
    else{
      //      VirtualVector & Bold=(*BPtr_);
      //      double bobo=Bold.dot(Bold);
      double bb=B.dot(B);
      
      double rr=R.dot(R);
      
      //      if ( (abs(bobo-bb)/bb < 0.01) && counter_<11){
      if ( ( rr/bb < rmax_) && (counter_<11) ){
	result=false;
	rmax_=rr/bb;
      }
      else{
	//	Bold.copy(B);
	counter_=0;
      }
    }

    if (result) this->iterationControler().maxIteration()=4;

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
  void RobustPreconditionedConjugateGradientSolver::solve(const Matrix & A, const VirtualVector & B, VirtualVector & X){

    int itmax=this->iterationControler().maxIteration();
    
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

    this->iterationControler().getIterationNumber()=itmax;
    
  }
  
  
}
    
