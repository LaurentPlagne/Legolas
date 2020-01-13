#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/Structures/SumCompound/SumCompoundSolver.hxx"
#include "Legolas/BlockMatrix/Structures/SumCompound/SumCompoundMatrix.hxx"

namespace Legolas{
  
  SumCompoundSolver::SumCompoundSolver( void ):SPtr_(0){}
  SumCompoundSolver::SumCompoundSolver(const SumCompoundSolver & source):SPtr_(0){
    INFOS("SumCompoundSolver ctor");
  }
  SumCompoundSolver::~SumCompoundSolver( void ){
    if (SPtr_){delete SPtr_;SPtr_=0;}
  }

  //(L+R)*X=B iteratively solved as L*X^{n+1}=B-R*X^{n}
  void SumCompoundSolver::solve(const Matrix & Av, const VirtualVector & B, VirtualVector & X){
    
    //    INFOS("Entering SumCompoundSolver::solve for the matrix :"<<Av.name());

    const SumCompoundMatrix & A=static_cast<const SumCompoundMatrix &>(Av);

    //    INFOS("ICI"); X.display();
    
    this->iterationControler().initialize(A,X);
    
    const Matrix & L=A.leftMatrix();
    const Matrix & R=A.rightMatrix();
    
    VirtualVector & S=VirtualVector::getClone(SPtr_,B);

    //    INFOS("ICI"); B.display(); 
    //    INFOS("ICI"); S.display(); 

    X86Timer    chronos,chronosPart;
    chronos.start();	
    
    do {
      chronosPart.start();
      if (this->iterationControler().iterationNumber()!=0) S.copy(B) ;
      chronosPart.stop();
      chronosPart.storeResult("ZZZZ SumCompoundSolver S.copy(B)");

      

      chronosPart.start();
      R.addMult(-1.0,X,S);
      chronosPart.stop();
      chronosPart.storeResult("ZZZZ SumCompoundSolver  R.addMult(-1.0,X,S)");

      //      INFOS("ICI"); S.display(); 
      //      INFOS("avant L solve");
      chronosPart.start();
      L.solve(S,X);
      chronosPart.stop();
      //      INFOS("apres L solve");
      chronosPart.storeResult("ZZZZ SumCompoundSolver LL.solve(S,X)");
      //      INFOS("ICI"); X.display();

      
      //      INFOS("X="<<(static_cast<const Legolas::MultiVector<double,1> &>(X)));

      

    } while(!this->iterationControler().end(X));

    chronos.stop();
    chronos.storeResult("ZZZZ SumCompoundSolver");
    
  }
  
  
}
    
