/**
 * project  DESCARTES
 *
 * @file     ProductCompoundMatrixTest.cxx
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

# include "DMatrixDefinition.hxx"
# include "RMatrixDefinition.hxx"
# include "AMatrixDefinition.hxx"

# include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
# include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
# include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
# include "Legolas/Matrix/MatrixStructures/QuadMatrix/QuadMatrixInterface.hxx"
# include "Legolas/Matrix/MatrixVectorOperations.hxx"

# include "Legolas/Algorithm/NonPreconditionedLinearConjugateGradientAlgorithm.hxx"
# include "Legolas/Algorithm/NonPreconditionedLinearBiConjugateGradientAlgorithm.hxx"

# include "Legolas/Vector/Vector.hxx"

using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{

  MESSAGE("debut du code");
  
  typedef double RealType;

  // don't change this size ! 
  const int size=3;

  // Vectors X,B,Residual
  Legolas::Vector<RealType> X(size),B(size),Residual(size);
  
  B=1.0;

  // Matrix D

  typedef InputMatrixOptions<Legolas::Actual> DMatrixOptions ;
  typedef GenericMatrixInterface<DMatrixDefinition<RealType>,DMatrixOptions,RealElementInterface<RealType> > DMatrixInterface;
  
  Legolas::MatrixShape<1> ms(size,size);
  DMatrixInterface::Matrix D(ms);
 
  Legolas::displayLatex(D,"D.tex");
    //  D.display("D");

  X=B/D;
  INFOS("X="<<X);
  Residual=D*X;
  Residual-=B;
  INFOS("Residual="<<Residual);
  INFOS("Residual R=AX-B  norme="<<(Legolas::dot(Residual,Residual)/Legolas::dot(X,X)));
  
  // Matrix R
  
  typedef InputMatrixOptions<Legolas::Actual> RMatrixOptions ;
  typedef GenericMatrixInterface<RMatrixDefinition<RealType>,RMatrixOptions,RealElementInterface<RealType> > RMatrixInterface;
  RMatrixInterface::Matrix R(ms);
  Legolas::displayLatex(R,"R.tex");

  // Matrix A

  typedef InputMatrixOptions<Legolas::Actual> AMatrixOptions ;
  typedef GenericMatrixInterface<AMatrixDefinition<RealType>,AMatrixOptions,RealElementInterface<RealType> > AMatrixInterface;
  AMatrixInterface::Matrix A(ms);
  Legolas::displayLatex(A,"A.tex");


  X=0.0;
  Legolas::solve(A,X,B);
  INFOS("X="<<X);
  Residual=A*X;
  Residual-=B;
  INFOS("Residual="<<Residual);
  INFOS("Residual R=AX-B  norme="<<(Legolas::dot(Residual,Residual)/Legolas::dot(X,X)));


  // QuadMatrix composition

  {
  
    //    typedef QuadMatrixInterface<DMatrixInterface,RMatrixInterface,RMatrixInterface,DMatrixInterface,Legolas::NonPreconditionedLinearConjugateGradientAlgorithm> QMI;
    typedef QuadMatrixInterface<DMatrixInterface,RMatrixInterface,RMatrixInterface,DMatrixInterface,Legolas::NonPreconditionedLinearBiConjugateGradientAlgorithm> QMI;
    
    
    typedef QMI::DataDriver TOTO;

    

    QMI::DataDriver::Data data(ms,ms,ms,ms);
    QMI::Matrix quadMatrix(data);

//     INFOS("quadMatrix.getMaxIteration()="<<quadMatrix.getMaxIteration());


    Legolas::displayLatex(quadMatrix,"QuadMatrix.tex");
    //    Legolas::displayHTML(quadMatrix,"QuadMatrix.html");

    typedef Legolas::MultiVector<double,2> Vector2D;    
    
    Legolas::MatrixShape<2>::Shape xShape(2,size);


    Vector2D  X(xShape);
    Vector2D  Xref(xShape);

    Xref=1.0;

    INFOS("Xref="<<Xref);

    Vector2D  Y(Xref);

    Y=quadMatrix*Xref;

    INFOS("Y="<<Y);

    //    X=Y/quadMatrix;
    //    Legolas::solve(quadMatrix,X,Y);

    //    INFOS("X="<<X);
    //    INFOS("Legolas::norm2(X-Xref)="<<Legolas::norm2(X-Xref));




    


  }
  

}
