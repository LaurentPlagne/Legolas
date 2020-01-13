/**
 * project  DESCARTES
 *
 * @file     PoissonEquation2D.cxx
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

# include "RXYMatrixDefinition.hxx"
# include "RYMatrixDefinition.hxx"
# include "DXYMatrixDefinition.hxx"
# include "DYMatrixDefinition.hxx"

# include "Legolas/Matrix/MatrixStructures/DRMatrix/DRGenericMatrixInterface.hxx"


#include "X86Timer.hxx"
#include "display.hxx"
#include "SourceFunctor.hxx"

using namespace std; using namespace Legolas;

int main( int argc,  char *argv[] )
{
  //  typedef Legolas::Vector<double> Vector1D;
  typedef Legolas::MultiVector<double,1> Vector1D;
  
  // ************* Physical data ***********************

  // Mesh : the 1D mesh is a regular partition of [0,1];
  
  const SizeType meshSize=100;
  
  Vector1D mesh;
  mesh.resize(meshSize);
  double stepSize=1.0/double(meshSize-1);
  for (SizeType i = 0 ; i < mesh.size() ; i++) mesh[i]=double(i)*stepSize;
  
  double pi=2.0*acos(0.0); const double omega=4.0*pi;

  SourceFunctor<double> myFunction(omega);

  // ********* Vector construction : U,S,R ***********

  // size of the unknown solution = meshSize - 2 degrees of freedom for the boundary conditions
  const SizeType size=meshSize-2;

  typedef Legolas::MultiVector<double,2> Vector2D;

  Vector2D::Shape shape(size,size);

  Vector2D U(shape); 
  
//   U.resize(size); 
//   for (SizeType i=0 ; i < size ; i++){
//     U[i].resize(size);
//   }

  U=1.0;
    U=0.0;
  
  Vector2D S(U),R(U);

  INFOS("Legolas::norm2(U)="<<norm2(U));
  INFOS("Legolas::norm2(S)="<<norm2(S));
  INFOS("Legolas::norm2(R)="<<norm2(R));
  //  exit(0);
  
  for (SizeType i=0 ; i < size ; i++){
    for (SizeType j=0 ; j < size ; j++){
      const double x=mesh[i+1];
      const double y=mesh[j+1];

      S[i][j]=myFunction(x,y)*stepSize*stepSize;
    }
  }

  // Boundary conditions 
  
  // j=0 et j=N
  for (SizeType i=0 ; i < size ; i++){
    const double x=mesh[i+1];
    const double ui0=myFunction.exact(x,mesh[0]);
    const double uin=myFunction.exact(x,mesh[mesh.size()-1]);
    S[i][0]-=ui0;
    S[i][size-1]-=uin;
  }

  for (SizeType j=0 ; j < size ; j++){
    const double y=mesh[j+1];
    const double u0j=myFunction.exact(mesh[0],y);
    const double unj=myFunction.exact(mesh[mesh.size()-1],y);
    S[0][j]-=u0j;
    S[size-1][j]-=unj;
  }
  
  // *****************  Matrix Construction ***********************

  //  typedef Virtual StorageMode;
  typedef Actual StorageMode;

  // ********* Matrix construction : DYMatrixInterface ***********

  typedef DYMatrixDefinition                             DYMD;
  typedef InputMatrixOptions<StorageMode>                DYMO;
  typedef RealElementInterface<double>                   DYEI ;
  typedef GenericMatrixInterface<DYMD,DYMO,DYEI>         DYMatrixInterface;

  // ********* Matrix construction : DXYMatrixInterface ***********
  typedef DXYMatrixDefinition                                      DXYMD;
  typedef InputMatrixOptions<StorageMode>                          DXYMO;
  typedef GenericMatrixInterface<DXYMD,DXYMO,DYMatrixInterface>    DXYMatrixInterface;

  { // test DXY interface
    const SizeType smallSize=3;
    DXYMatrixDefinition::Data init(smallSize);
    DXYMatrixInterface::Matrix DXY(init);

    Legolas::displayLatex(DXY,"DXY.tex",0);
    //    display2D(DXY);
  }

  // ********* Matrix construction : RYMatrixInterface ***********
  
  typedef RYMatrixDefinition                        RYMD;
  typedef InputMatrixOptions<StorageMode>           RYMO;
  typedef RealElementInterface<double>              RYEI ;
  typedef GenericMatrixInterface<RYMD,RYMO,RYEI>    RYMatrixInterface;
  
  // ********* Matrix construction : RXYMatrixInterface ***********
  
  typedef RXYMatrixDefinition                                      RXYMD;
  typedef InputMatrixOptions<StorageMode>                          RXYMO;
  typedef GenericMatrixInterface<RXYMD,RXYMO,RYMatrixInterface>    RXYMatrixInterface;

  { // test RXY interface
    const SizeType smallSize=3;
    RXYMatrixDefinition::Data init(smallSize);
    RXYMatrixInterface::Matrix RXY(init);

    Legolas::displayLatex(RXY,"RXY.tex",0);
    //    display2D(RXY);
  }
  // ********* Matrix construction : LXY=DXY+RXY ***********
  typedef DRGenericMatrixInterface< DXYMatrixInterface, RXYMatrixInterface > LXYMatrixInterface;


  { // test RXY interface
    const SizeType smallSize=3;
    //LXYMatrixInterface::Data init(smallSize,smallSize);
    LXYMatrixInterface::DataDriver::Data init(smallSize,smallSize);
    LXYMatrixInterface::Matrix LXY(init);
    Legolas::displayLatex(LXY,"LXY.tex",0);
    //    Legolas::displayHTML(LXY,"LXY.html",0);
  }

  //LXYMatrixInterface::Data init(size,size);
  LXYMatrixInterface::DataDriver::Data init(size,size);
  LXYMatrixInterface::Matrix LXY(init);

  //  Legolas::displayLatex(LXY,"LXY.tex",0);

  // ********* Linear solver test : AU=S ***********
  

  X86Timer chronos;
  chronos.start();
  //Legolas::solve(LXY,U,S);
  U=S/LXY; // solves LXYU=S
  chronos.stop();
  INFOS("ICI");
  chronos.storeResult("U=S/LXY");

  R=LXY*U;
  R-=S;

  cout << "Residual="<< (Legolas::dot(R,R)) << endl ;
  INFOS("ICI");
  ofstream outfile ("result.dat",ios::out) ;

  for (SizeType i=1 ; i < mesh.size()-1 ; i++){
    for (SizeType j=1 ; j < mesh.size()-1 ; j++){

      double xi= mesh[i];
      double yj= mesh[j];
      double uij=U[i-1][j-1];
      double eij=myFunction.exact(xi,yj);

      outfile << xi << " " << yj << " " << uij << " " << eij << " " << uij-eij<< " " << endl;
    }
  }
    
  outfile.close();

}
