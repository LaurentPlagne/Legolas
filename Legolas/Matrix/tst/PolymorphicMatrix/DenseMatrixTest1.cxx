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

# include "DenseMatrixDefinition.hxx"
# include "DiagonalMatrixDefinition.hxx"

# include "PolymorphicMatrix.hxx"
# include "PolymorphicMatrixInterface.hxx"
# include "PolyMatrixDefinition.hxx"

#include <assert.h>

using namespace std; using namespace Legolas;
using namespace Legolas;

//   class MyFakeInverse{
//   public :
    
//     template <class ASSIGN_MODE>
//     class Engine{
//     public:

//       template <class SPARSE_MATRIX,class VECTOR, class VECTOR_INOUT>
//       static inline void apply(const SPARSE_MATRIX & A, 
// 			       const VECTOR & B,
// 			       VECTOR_INOUT & X)
//       {

// 	//	A.sparseGetElement(0,0).matrixVectorProduct(B[0],X[0],0);
// 	Legolas::AnyMatrixVectorMultExpression expr(A.sparseGetElement(0,0),B[0]);

// 	X[0]=expr;
	
// 	//	X[0]=A.sparseGetElement(0,0)*B[0];
//       }
//     };
//   };

int main( int argc,  char *argv[] )
{
  const int size=3;

  // ********* Matrix construction : A ***********
  typedef InputMatrixOptions<Actual>    MO ;
  typedef RealElementInterface<double>  EI ;

  typedef GenericMatrixInterface<DenseMatrixDefinition,MO,EI> DenseMatrixInterface;

  Legolas::MatrixShape<1> ms(size,size);
  DenseMatrixInterface::Matrix A(ms);

  A(0,0)=5.0; A(0,1)=2.0; A(0,2)=3.0;
  A(1,0)=3.0; A(1,1)=4.0; A(1,2)=2.0;
  A(2,0)=1.0; A(2,1)=2.0; A(2,2)=4.0;

  Legolas::displayLatex(A,"A.tex");
  
  //  std::shared_ptr<Legolas::AnyMatrix> elementPtr(new DenseMatrixInterface::Matrix(A));
  //  Legolas::PolymorphicMatrix AA(elementPtr);
  Legolas::PolymorphicMatrix AA(A);

  Legolas::displayLatex(AA,"A.tex");

  // ********* Matrix construction : D ***********

  DenseMatrixInterface::Matrix D(ms);

  D(0,0)=5.0; D(0,1)=2.0; D(0,2)=3.0;
  D(1,0)=3.0; D(1,1)=4.0; D(1,2)=2.0;
  D(2,0)=1.0; D(2,1)=2.0; D(2,2)=4.0;

  Legolas::displayLatex(D,"D.tex");

  // ********* Matrix construction : B ***********

  typedef GenericMatrixInterface<DiagonalMatrixDefinition,MO,EI> DiagonalMatrixInterface;

  DiagonalMatrixInterface::Matrix B(ms);

  B.diagonalGetElement(0)=5.0;
  B.diagonalGetElement(1)=5.0;
  B.diagonalGetElement(2)=5.0;
  
  Legolas::displayLatex(B,"B.tex");

  // ********* Matrix construction : C ***********

  DiagonalMatrixInterface::Matrix C(ms);

  C.diagonalGetElement(0)=-5.0;
  C.diagonalGetElement(1)=-5.0;
  C.diagonalGetElement(2)=-5.0;
  
  Legolas::displayLatex(C,"C.tex");

  {
    // ********* Vector construction : X,B,R ***********

    Legolas::Vector<double> X(size),B(size),R(size);
  
    X=0.0;   R=0.0;
  
    B[0]=18.0;
    B[1]=17.0;
    B[2]=17.0;

    cout << "B      "<< B << endl ;

    // ********* Linear solver test : AX=B ***********

    //  Legolas::solve(A,X,B); // solves AX=B
    Legolas::solve(A,X,B); // solves AX=B
    cout << "Legolas::solve(A,X,B)  "<< X << endl ;
    
    //  AnyMatrixVectorMultExpression expr(AA*X);
    INFOS("R="<<R);
    //    R+=AA*X;
    R+=AA*X;
    //  AA.matrixVectorProduct(X,R,0);
    //  R=A*X;
    cout << "R=A*X  "<< R << endl ;
    
    R-=B;
    cout << "R=A*X-B"<< R << endl ;
    
    double residual=Legolas::dot(R,R);
    INFOS("residual="<<residual);
    assert(residual<1.e-15);
  }

  Legolas::MatrixShape<2>::Shape rs2(size,2);
  Legolas::MatrixShape<2>::Shape cs2(size,2);
  Legolas::MatrixShape<2> ms2(rs2,cs2);
  
  typedef Legolas::PolymorphicMatrixInterface<1,double> PMI;



  
  //  typedef InputMatrixOptions<Actual,MyFakeInverse>    MO2 ;
  typedef InputMatrixOptions<Actual>    MO2 ;

  typedef Legolas::GenericMatrixInterface<PolyMatrixDefinition,MO2,PMI> GMI2;
  
  GMI2::Matrix poly2(ms2);

  {
    // ********* Vector construction : X,B,R ***********

    Legolas::Vector<double> X(size),B(size),R(size);
  
    X=1.0;   R=0.0;
    INFOS("X="<<X);
    INFOS("R="<<R);
    R+=poly2(0,0)*X;
    INFOS("R="<<R);
  }



}
