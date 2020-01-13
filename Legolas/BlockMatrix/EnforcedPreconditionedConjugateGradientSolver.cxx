#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/EnforcedPreconditionedConjugateGradientSolver.hxx"

namespace Legolas{

  EnforcedPreconditionedConjugateGradientSolver::EnforcedPreconditionedConjugateGradientSolver( void ):RPtr_(0),
												       DPtr_(0),
												       PPtr_(0),
												       ZPtr_(0),
												       BPtr_(0),
												       preconditioner_(0),
												       counter_(0)
  {}
  
  EnforcedPreconditionedConjugateGradientSolver::EnforcedPreconditionedConjugateGradientSolver(Legolas::Matrix * preconditioner):RPtr_(0),
																 DPtr_(0),
																 PPtr_(0),
																 ZPtr_(0),
																 BPtr_(0),
																 preconditioner_(preconditioner),
																 counter_(0)
  {
    //    INFOS("preconditioner_="<<preconditioner_);
  }
  EnforcedPreconditionedConjugateGradientSolver::EnforcedPreconditionedConjugateGradientSolver(const EnforcedPreconditionedConjugateGradientSolver & source):RPtr_(0),
																			     DPtr_(0),
																			     PPtr_(0),
																			     ZPtr_(0),
																			     BPtr_(0),
																			     preconditioner_(0),
																			     counter_(0)
  {}
  
  EnforcedPreconditionedConjugateGradientSolver::~EnforcedPreconditionedConjugateGradientSolver( void ){
    if (RPtr_){delete RPtr_;RPtr_=0;}
    if (DPtr_){delete DPtr_;DPtr_=0;}
    if (PPtr_){delete PPtr_;PPtr_=0;}
    if (ZPtr_){delete PPtr_;PPtr_=0;}
    if (BPtr_){delete PPtr_;PPtr_=0;}
    if (preconditioner_){ delete preconditioner_ ; preconditioner_=0 ;}
  }
  
  std::string EnforcedPreconditionedConjugateGradientSolver::name( void ) const {
    assert(preconditioner_);
    std::string result("EnforcedPreconditionedConjugateGradientSolver");
    result+=" (";
    result+=preconditioner_->name();
    result+=")";
    return result;
  }

  bool EnforcedPreconditionedConjugateGradientSolver::goon(const VirtualVector & R){
    bool cont=false;

    if (this->counter_==0){
      VirtualVector::getClone(BPtr_,R);
    }
    else{
      VirtualVector & Rold=(*BPtr_);

      double roro=Rold.dot(Rold);
      double rr=R.dot(R);

      if (rr<roro){
	cont=false;
	Rold.copy(R);
      }
    }
    
    counter_++;

    INFOS("cont="<<cont);

    return cont;
  }

  
  //From Quarteroni (p.50)
  void EnforcedPreconditionedConjugateGradientSolver::solve(const Matrix & A, const VirtualVector & B, VirtualVector & X){
    
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

    VirtualVector & P=VirtualVector::getClone(PPtr_,Z);

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
      
    }while(goon(R));

    
  }
  
  
}
    
