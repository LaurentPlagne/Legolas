/**
 * project  DESCARTES
 *
 * @file     DenseMatrixTest.cxx
 *
 * @author Laurent PLAGNE
 * @date   september 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
# include <cstdlib>
# include <cmath>
# include <iostream>

# include "UTILITES.hxx" 
# include "Legolas/Vector/Vector.hxx"

# include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
# include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
# include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
# include "Legolas/Matrix/MatrixVectorOperations.hxx"
# include "Legolas/Matrix/PolymorphicMatrix.hxx"
# include "Legolas/Matrix/PolymorphicMatrixInterface.hxx"

# include "DenseMatrixDefinition.hxx"
# include "DiagonalMatrixDefinition.hxx"
# include "PolyMatrixDefinition.hxx"

#include <assert.h>

using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{
  const int size=3;

//   // ********* Matrix construction : A ***********
//   typedef InputMatrixOptions<Actual>    MO ;
//   typedef RealElementInterface<double>  EI ;

//   typedef GenericMatrixInterface<DenseMatrixDefinition,MO,EI> DenseMatrixInterface;

//   Legolas::MatrixShape<1> ms(size,size);
//   DenseMatrixInterface::Matrix A(ms);

//   A(0,0)=5.0; A(0,1)=2.0; A(0,2)=3.0;
//   A(1,0)=3.0; A(1,1)=4.0; A(1,2)=2.0;
//   A(2,0)=1.0; A(2,1)=2.0; A(2,2)=4.0;


//   Legolas::displayLatex(A,"A.tex");
 
//   {
//     // ********* Vector construction : X,B,R ***********

//     Legolas::Vector<double> X(size),B(size),R(size);
  
//     X=0.0;   R=0.0;
  
//     B[0]=18.0;
//     B[1]=17.0;
//     B[2]=17.0;

//     cout << "B      "<< B << endl ;

//     // ********* Linear solver test : AX=B ***********

//     Legolas::solve(A,X,B);
//     cout << "Legolas::solve(A,X,B)  "<< X << endl ;
    
//     INFOS("R="<<R);
//     R+=A*X;

//     cout << "R=A*X  "<< R << endl ;
    
//     R-=B;
//     cout << "R=A*X-B"<< R << endl ;
    
//     double residual=Legolas::dot(R,R);
//     INFOS("residual="<<residual);
//     assert(residual<1.e-15);
//   }

  
  Legolas::MatrixShape<2>::Shape rs2(2,size);
  Legolas::MatrixShape<2>::Shape cs2(2,size);
  Legolas::MatrixShape<2> ms2(rs2,cs2);
  
  typedef Legolas::PolymorphicMatrixInterface<1,double> PMI;

  
  //  typedef InputMatrixOptions<Actual,MyFakeInverse>    MO2 ;
  typedef InputMatrixOptions<Actual>    MO2 ;

  typedef Legolas::GenericMatrixInterface<PolyMatrixDefinition,MO2,PMI> GMI2;
  
  GMI2::Matrix poly2(ms2);

  Legolas::displayLatex(poly2,"poly2.tex");
  Legolas::displayPNG(poly2,"poly2.png",2);

  
  {
    // ********* Vector construction : X,B,R ***********

    Legolas::MultiVector<double,2> X(rs2),B(rs2),R(rs2);
  
    X=0.0;   R=0.0; B=1.0;

    INFOS("X="<<X);
    INFOS("B="<<X);

    Legolas::solve(poly2,X,B);

    INFOS("X="<<X);

    R=poly2*X;
    R-=B;

    INFOS("R="<<R);
  }
  
}

