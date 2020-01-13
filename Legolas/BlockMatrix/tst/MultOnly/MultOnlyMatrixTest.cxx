#include <cstdlib>
#include <cmath>
#include <iostream>
#include "UTILITES.hxx" 

#include "Legolas/Vector/Vector.hxx"
#include "MyMultOnlyMatrix.hxx" 
#include "Legolas/BlockMatrix/tst/LegolasTestSolver.hxx"
#include "Legolas/BlockMatrix/ConjugateGradientSolver.hxx"

int main( int argc,  char *argv[] )
{
  
  {
    INFOS("START LEVEL 1");
    
    typedef double RealType;
    const int s1=10;
    
    Legolas::MultiVector<RealType,1>  Xref(s1);
    
    for (int i=0 ; i<s1 ; ++i) Xref[i]=RealType(i+2);
    INFOS("Xref="<<Xref);

    Legolas::MatrixShape<1> ms1(s1,s1);
    MyMultOnlyMatrix<RealType> A(ms1,3.0);

    Legolas::MultiVector<RealType,1>  Y(s1);

    A.addMult(1.0,Xref,Y);

    INFOS("Y="<<Y);

    solveAndCheckResidual(A,Xref);

    //    A.setSolverPtr(new Legolas::ConjugateGradientSolver() );

    //    solveAndCheckResidual(A,Xref);

    A.displayLatex("A.tex");
    A.displaySVG("A.svg");



    INFOS("A.name="<<A.name());

    
    INFOS("END LEVEL 1");
  }

  
}
