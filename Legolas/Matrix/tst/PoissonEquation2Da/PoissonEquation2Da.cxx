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

# include "YMatrixDefinition.hxx"
# include "XYMatrixDefinition.hxx"

#include "X86Timer.hxx"

#include "display.hxx"
#include "SourceFunctor.hxx"

#include "Legolas/Algorithm/SparseGaussSeidel.hxx"

//#include "../SparseMatrixTest/SParseMatrixDefinition.hxx"

using namespace std; using namespace Legolas;

int main( SizeType argc,  char *argv[] )
{
  //  typedef Legolas::Vector<double> Vector1D;
  typedef Legolas::MultiVector<double,1> Vector1D;
  
  // ************* Physical data ***********************

  // Mesh : the 1D mesh is a regular partition of [0,1];
  const SizeType meshSize=6;
  
  Vector1D mesh;
  mesh.resize(meshSize);
  double stepSize=1.0/double(meshSize-1);
  for (SizeType i = 0 ; i < mesh.size() ; i++) mesh[i]=double(i)*stepSize;

  double pi=2.0*acos(0.0);  
  const double omega=4.0*pi;

  SourceFunctor<double> myFunction(omega);

  // ********* Vector construction : U,S,R ***********

  // size of the unknown solution = meshSize - 2 degrees of freedom for the boundary conditions
  const SizeType size=meshSize-2;

  typedef Legolas::MultiVector<double,2> Vector2D;

  Vector2D::Shape shape(size,size);

  Vector2D U(shape); 
  
  //  U.resize(size); 
  //  for (SizeType i=0 ; i < size ; i++){
  //    U[i].resize(size);
  //  }
  U=0.0;

  //  U[0]=1.0;
  INFOS("ICI");
  //  INFOS("U[0]="<<U[0]);
  INFOS("U="<<U);
  INFOS("ICI"); 

  Vector2D S(U),R(U);
  
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

  typedef Virtual StorageMode;
  //typedef Actual StorageMode;

  // ********* Matrix construction : YMatrixInterface ***********
  
  typedef YMatrixDefinition                                        YMD;
  //  typedef InputMatrixOptions<StorageMode>                          YMO;
  typedef InputMatrixOptions<StorageMode,Legolas::SparseGaussSeidel> YMO;
  typedef RealElementInterface<double>                             YEI;

  typedef GenericMatrixInterface<YMD,YMO,YEI>    YMatrixInterface;

  // ************ YMatrixInterface Test **************************
  // Y MatrixInterface Test . XYii=diagonal element of XY matrix
  // Y MatrixInterface Test . XYij=off diagonal element of XY matrix
  {
    SizeType smallSize=4;
    
    Legolas::MatrixShape<1> ms(smallSize,smallSize);

    YMatrixDefinition::Data initDiag(ms,true);
    YMatrixDefinition::Data initNoDiag(ms,false);
    //    YMatrixDefinition::Data initNoDiag(false,false,smallSize);

    //    YMatrixDefinition::Data initDiag(true,false,smallSize);
    //    YMatrixDefinition::Data initNoDiag(false,false,smallSize);
    
    YMatrixInterface::Matrix XYij(initNoDiag);
    YMatrixInterface::Matrix XYii(initDiag);
    INFOS("YMatrixInterface::Matrix::structureName()="<<YMatrixInterface::Matrix::structureName());
    INFOS("XYij.getStrutureName()="<<XYij.getStructureName());

    YMatrixInterface::Matrix::structureName();

    INFOS("XYii.getRowShape()="<<XYii.getRowShape());
    INFOS("XYii.getColShape()="<<XYii.getColShape());

    cout << "*** XYii *** " << endl; 
    Legolas::displayLatex(XYii,"XYii.tex",0);
    //    display(XYii);
    cout << "*** XYij *** " << endl; 
    Legolas::displayLatex(XYij,"XYij.tex",0);
    //    display(XYij);
  }

  // ********* Matrix construction : XYMatrixInterface ***********

  typedef XYMatrixDefinition                                  XYMD;
  //  typedef InputMatrixOptions<StorageMode>                          XYMO;
  typedef InputMatrixOptions<StorageMode,Legolas::SparseGaussSeidel> XYMO;
  //  //  typedef InputMatrixOptions<StorageMode>                     XYMO;
  typedef GenericMatrixInterface<XYMD,XYMO,YMatrixInterface>  XYMatrixInterface;

  
  // ****** Test XYMatrixInterface

  {
    SizeType smallSize=4;
    XYMatrixInterface::Matrix LXY(smallSize);
    Legolas::displayLatex(LXY,"LXY.tex",0);
    //    Legolas::dumpFlatMatrix(LXY,"LXY.dat");
    //    Legolas::displayLatexFlatMatrix(LXY,"LXYF.tex",0);

    //    Legolas::displayLatex(XYij,"XY.tex",3);
    //    display2D(LXY);
  }


  XYMatrixInterface::Matrix LXY(size);

  INFOS("LXY.getRowShape()="<<LXY.getRowShape());
  INFOS("LXY.getColShape()="<<LXY.getColShape());




  //  Legolas::displayLatex(LXY,"LXY.tex",3);

  //  Legolas::dumpFlatMatrix(LXY,"LXY.dat");

  //  Legolas::displayLatexFlatMatrix(LXY,"LXYF.tex",3);

  //  INFOS("XYMD::getRowShape().size()="<<XYMD::getRowShape(size).size());
  //  INFOS("Legolas::getFlatSize(XYMD::getRowShape())="<<Legolas::getFlatSize(XYMD::getRowShape(size)));

  //  Legolas::MultiVector<unsigned long,1> flatRowIndexes;
  //  Legolas::Vector<unsigned long> flatRowIndexes;
  //  Legolas::computeFlatIndexes(LXY.getRowShape(),0,flatRowIndexes);
  //  Legolas::computeFlatIndexes(XYMD::getRowShape(size),0,flatRowIndexes);
  //  INFOS("flatRowIndexes="<<flatRowIndexes);
 

  INFOS("LXY.getStrutureName()="<<LXY.getStructureName());



  // ********* Linear solver test : AU=S ***********

  INFOS("U="<<U);
  
  X86Timer chronos;
  chronos.start();

  Legolas::solve(LXY,U,S);

		 //  typedef Legolas::SparseGaussSeidel::Engine< XYMatrixInterface::Matrix,Vector2D,Vector2D > Solver;
		 //  Solver solver(LXY,U,S);

		 //  solver.solve(LXY,U,S);

  //  U=S/LXY;

  //  U=S/LXY; // solves LXYU=S
  chronos.stop();
  chronos.storeResult("U=S/LXY");

  INFOS("U[0]="<<U[0]);

  R=LXY*U;

  INFOS("(LXY*U)[0]="<<R[0]);
  R-=S;

  INFOS("R[0]="<<R[0]);

  cout << "Residual="<< (Legolas::dot(R,R)) << endl ;
  
  {
    ofstream outfile ("result.dat",ios::out) ;
    
    for (SizeType i=1 ; i < mesh.size()-1 ; i++){
      for (SizeType j=1 ; j < mesh.size()-1 ; j++){
	
	double xi= mesh[i];
	double yj= mesh[j];
      
	double uij=U[i-1][j-1];

	double eij=myFunction.exact(xi,yj);

	outfile << xi << " " << yj << " " << uij << " " << eij << " " << uij-eij<< " " << endl;
      }
      outfile << endl;
    }
    outfile.close();
  }
  {
    
    ofstream outfile ("u.dat",ios::out) ;

    for (SizeType i=1 ; i < mesh.size()-1 ; i++){
      for (SizeType j=1 ; j < mesh.size()-1 ; j++){
	
	//	double xi= mesh[i];
	//	double yj= mesh[j];
	
	double uij=U[i-1][j-1];
	
	//	double eij=myFunction.exact(xi,yj);
	
	outfile << uij << " " ;
      }
      outfile << endl;
    }

    outfile.close();
  }

#ifndef UT
#endif

}
