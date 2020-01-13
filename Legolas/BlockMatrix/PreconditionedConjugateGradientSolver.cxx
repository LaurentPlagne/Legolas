#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/PreconditionedConjugateGradientSolver.hxx"

namespace Legolas{

  PreconditionedConjugateGradientSolver::PreconditionedConjugateGradientSolver( void ):RPtr_(0),
										       DPtr_(0),
										       PPtr_(0),
										       ZPtr_(0),
										       preconditioner_(0){}
  
  PreconditionedConjugateGradientSolver::PreconditionedConjugateGradientSolver(Legolas::Matrix * Preconditioner):RPtr_(0),
														 DPtr_(0),
														 PPtr_(0),
														 ZPtr_(0),
														 preconditioner_(Preconditioner){
    //    INFOS("preconditioner_="<<preconditioner_);
  }
  PreconditionedConjugateGradientSolver::PreconditionedConjugateGradientSolver(const PreconditionedConjugateGradientSolver & source):RPtr_(0),
																     DPtr_(0),
																     PPtr_(0),
																     ZPtr_(0),
																     preconditioner_(0){}
  
  PreconditionedConjugateGradientSolver::~PreconditionedConjugateGradientSolver( void ){
    if (RPtr_){delete RPtr_;RPtr_=0;}
    if (DPtr_){delete DPtr_;DPtr_=0;}
    if (PPtr_){delete PPtr_;PPtr_=0;}
    if (ZPtr_){delete PPtr_;PPtr_=0;}
    if (preconditioner_){ delete preconditioner_ ; preconditioner_=0 ;}
  }

  std::string PreconditionedConjugateGradientSolver::name( void ) const {
    assert(preconditioner_);
    std::string result("PreconditionedConjugateGradientSolver");
    result+=" (";
    result+=preconditioner_->name();
    result+=")";
    return result;
  }


  
  //From Quarteroni (p.50)
  void PreconditionedConjugateGradientSolver::solve(const Matrix & A, const VirtualVector & B, VirtualVector & X){
    
    //    INFOS("preconditioner_="<<preconditioner_);

    if (preconditioner_==0){
      INFOS("There is no Preconditioner");
      throw std::runtime_error("There is no preconditioner");
    }
    //    const Matrix & Prec=(*preconditioner_);
    
    this->iterationControler().initialize(A,X);
    
    VirtualVector & R=VirtualVector::getClone(RPtr_,B);
    
    //R=B-AX;
    A.addMult(-1.0,X,R);

    VirtualVector & Z=VirtualVector::getClone(ZPtr_,R);
    //    INFOS("ICI");
    //    INFOS("R.dot(R)="<<R.dot(R));

    preconditioner_->solve(R,Z);

    //    INFOS("Z.dot(Z)="<<Z.dot(Z));


    VirtualVector & P=VirtualVector::getClone(PPtr_,Z);

    //    INFOS("P.dot(P)="<<P.dot(P));

    //    P.copy(Z);

    VirtualVector & D=VirtualVector::getClone(DPtr_,Z);


    //    INFOS("LA");

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
    
