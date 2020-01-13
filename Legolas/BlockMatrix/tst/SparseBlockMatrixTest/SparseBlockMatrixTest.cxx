#include <cstdlib>
#include <cmath>
#include <iostream>
#include "UTILITES.hxx" 

#include "Legolas/Vector/Vector.hxx"
#include "MyDiagonalMatrix.hxx" 
#include "MySparseBlockMatrix.hxx" 
#include "Legolas/BlockMatrix/tst/LegolasTestSolver.hxx"
#include "Legolas/BlockMatrix/ConjugateGradientSolver.hxx"
#include "Legolas/BlockMatrix/BiCGStabSolver.hxx"

int main( int argc,  char *argv[] )
{
  
 //  Level 1
  if (1==1)
  {
    
    typedef double RealType;
    
    const int s1=10;
    
    Legolas::MultiVector<RealType,1>  Xref(s1);
    
    for (int i=0 ; i<s1 ; ++i) Xref[i]=RealType(i+1);
    
    INFOS("Xref="<<Xref);

    Legolas::MatrixShape<1> ms1(s1,s1);
    MyDiagonalMatrix<RealType> A(ms1,2.0);

    //    A.fixedIterationNumber()=true;
    INFOS(" A.fixedIterationNumber()="<< A.fixedIterationNumber());

    solveAndCheckResidual(A,Xref);

    A.displayLatex("A1.tex");

    INFOS("END LEVEL 1");
  }
  
  
  //Level 2


  {

    INFOS("DEBUT LEVEL 2");

    typedef double RealType;
    
    const int s1=5;
    const int s2=100;
    
    Legolas::MultiVector<RealType,2>::Shape  shape(s1,s2);
    Legolas::MultiVector<RealType,2>  Xref(shape);
    Xref=1.0;
    
    Legolas::MatrixShape<2>::Shape rowShape(s1,s2);
    Legolas::MatrixShape<2> ms2(rowShape,rowShape);
    MySparseBlockMatrix  A(ms2);

    solveAndCheckResidual(A,Xref);

    {

      INFOS("En utilisant un gradient conjugué :");
      A.setVirtualSolverPtr(new Legolas::ConjugateGradientSolver());
      A.maxIteration()=1000;
      A.fixedIterationNumber()=false;
      solveAndCheckResidual(A,Xref);

    }

    {

      Xref=-1.0;

      INFOS("En utilisant un BiCGStab :");
      A.setVirtualSolverPtr(new Legolas::BiCGStabSolver());
      A.maxIteration()=1000;
      A.fixedIterationNumber()=false;
      solveAndCheckResidual(A,Xref);

    }

    
    A.displayLatex("A2.tex");
    A.displaySVG("A2.svg");

    INFOS("END LEVEL 2");
    
  }
  
  
}
