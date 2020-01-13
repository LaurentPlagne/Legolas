#include <cstdlib>
#include <cmath>
#include <iostream>
#include "UTILITES.hxx" 

#include "Legolas/Vector/Vector.hxx"
#include "MyBandedMatrix.hxx" 

#include "Legolas/BlockMatrix/tst/LegolasTestSolver.hxx"

int main( int argc,  char *argv[] )
{
  
  {
    INFOS("START LEVEL 1");
    
    typedef double RealType;
    const int s1=10;
    
    Legolas::MultiVector<RealType,1>  Xref(s1);
    
    for (int i=0 ; i<s1 ; ++i) Xref[i]=RealType(i+1);
    INFOS("Xref="<<Xref);

    Legolas::MatrixShape<1> ms1(s1,s1);
    MyBandedMatrix<RealType> A(ms1);
    
    solveAndCheckResidual(A,Xref);

    
    INFOS("END LEVEL 1");
  }

  
}
