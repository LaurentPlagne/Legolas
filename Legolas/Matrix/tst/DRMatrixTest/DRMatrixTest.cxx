/**
 * project  DESCARTES
 *
 * @file     DRMatrixTest.cxx
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
# include "MultiDRMatrixDefinition.hxx"

# include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
# include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
# include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
# include "Legolas/Matrix/MatrixStructures/DRMatrix/DRGenericMatrixInterface.hxx"
# include "Legolas/Matrix/MatrixVectorOperations.hxx"

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

  // DRMatrix composition

  {
  
    typedef DRGenericMatrixInterface<DMatrixInterface,RMatrixInterface> DRMatrixInterface;
    
    //DRMatrixInterface::Data data(ms,ms);
    DRMatrixInterface::DataDriver::Data data(ms,ms);
    DRMatrixInterface::Matrix A(data);

    INFOS("A.getMaxIteration()="<<A.getMaxIteration());


    //    A.nrows();

    Legolas::displayLatex(A,"DR.tex");
    //    Legolas::displayHTML(A,"DR.html");

    DMatrixInterface::ConstView D(A.D());
    RMatrixInterface::ConstView R(A.R());



    //    D.display("D");
    //    R.display("R");


    X=0.0;
    Legolas::solve(A,X,B);
    INFOS("X="<<X);    
    Residual=A*X;
    Residual-=B;
    
    INFOS("Residual R=AX-B  norme="<<(Legolas::dot(Residual,Residual)/Legolas::dot(X,X)));

    X=0.0;
    A.setEpsilon()=1.e-6;
    Legolas::solve(A,X,B);
    INFOS("X="<<X);    
    Residual=A*X;
    Residual-=B;
    

    INFOS("A.getStructureName()="<<A.getStructureName());

   //  // Test d'une matrice d'elements de type DRMatrix :

//     typedef InputMatrixOptions<Legolas::Virtual> MO ;
//     typedef GenericMatrixInterface<MultiDRMatrixDefinition<DRMatrixInterface>,MO,DRMatrixInterface> MDRMatrixInterface;

//     const int diagSize=2;

//     Legolas::MatrixShape<2>::Shape rowShape(diagSize,size);
//     Legolas::MatrixShape<2>::Shape colShape(diagSize,size);
//     Legolas::MatrixShape<2> m2s(rowShape,colShape);
//     MDRMatrixInterface::Matrix AA(m2s);

//     Legolas::displayLatex(AA,"MDR.tex");
//     Legolas::displayHTML(AA,"MDR.html");

  }
    

  

}
