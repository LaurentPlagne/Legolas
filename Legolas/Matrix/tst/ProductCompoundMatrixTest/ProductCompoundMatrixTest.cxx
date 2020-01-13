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
# include "MultiProductCompoundMatrixDefinition.hxx"

# include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
# include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
# include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
# include "Legolas/Matrix/MatrixStructures/ProductCompound/ProductCompoundMatrixInterface.hxx"
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

  //  X=B/D;

  Legolas::solve(D,X,B);

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
  //  Legolas::solve(A,X,B);
  INFOS("X="<<X);
  Residual=A*X;
  Residual-=B;
  INFOS("Residual="<<Residual);
  INFOS("Residual R=AX-B  norme="<<(Legolas::dot(Residual,Residual)/Legolas::dot(X,X)));


  // ProductCompoundMatrix composition

  {
  
    typedef ProductCompoundMatrixInterface<DMatrixInterface,RMatrixInterface> PCMI;
    
    //ProductCompoundMatrixInterface::Data data(ms,ms);
    PCMI::DataDriver::Data data(ms,ms);
    PCMI::Matrix PC(data);

    INFOS("A.getMaxIteration()="<<PC.getMaxIteration());


    //    A.nrows();

    Legolas::displayLatex(PC,"ProductCompound.tex");
    //    Legolas::displayHTML(PC,"ProductCompound.html");

    Residual=PC*X;
    Residual-=B;
    INFOS("Bis Residual="<<Residual);
    INFOS("Bis Residual R=AX-B  norme="<<(Legolas::dot(Residual,Residual)/Legolas::dot(X,X)));    

    X=0.0;

    Legolas::solve(PC,X,B);

    //    X=B/PC;
    INFOS("X="<<X);
    Residual=PC*X;
    Residual-=B;
    INFOS("ter Residual="<<Residual);
    INFOS("ter Residual R=AX-B  norme="<<(Legolas::dot(Residual,Residual)/Legolas::dot(X,X)));    

    INFOS("PC.getStructureName()="<<PC.getStructureName());

    // Test d'une matrice d'elements de type ProductCompoundMatrix :
    
    typedef InputMatrixOptions<Legolas::Virtual> MO ;
    typedef GenericMatrixInterface<MultiProductCompoundMatrixDefinition<PCMI>,MO,PCMI> MultiProductCompoundMatrixInterface;
    
    const int diagSize=2;

    Legolas::MatrixShape<2>::Shape rowShape(diagSize,size);
    Legolas::MatrixShape<2>::Shape colShape(diagSize,size);
    Legolas::MatrixShape<2> m2s(rowShape,colShape);
    MultiProductCompoundMatrixInterface::Matrix MPC(m2s);

    Legolas::displayLatex(MPC,"MultiProductCompound.tex");
    //    Legolas::displayHTML(MPC,"MultiProductCompound.html");
    //    Legolas::displayPNG(MPC,"MultiProductCompound.png");

    // Test d'une matrice de type ProductCompoundMatrix L*R avec R de type ProductCompoundMatrix

    typedef ProductCompoundMatrixInterface<PCMI,RMatrixInterface> PCMI2;

    PCMI2::Matrix::LMatrixInterface::MatrixDefinition::Data ldata(ms,ms);
    PCMI2::Matrix::RMatrixInterface::MatrixDefinition::Data rdata(ms);
    
    PCMI2::DataDriver::Data data2(ldata,rdata);

    PCMI2::Matrix PC2(data2);

    X=0.0;

    Legolas::solve(PC2,X,B);

    //    X=B/PC2;
    INFOS("X="<<X);
    Residual=PC2*X;
    Residual-=B;
    INFOS("4 Residual="<<Residual);
    INFOS("4 Residual R=AX-B  norme="<<(Legolas::dot(Residual,Residual)/Legolas::dot(X,X)));    

    INFOS("PC2.getStructureName()="<<PC2.getStructureName());
    
    //    Legolas::displayLatex(PC2,"ProductCompound.tex");
//     Legolas::displayHTML(PC2,"ProductCompound.html");

    




  }
    

  

}
