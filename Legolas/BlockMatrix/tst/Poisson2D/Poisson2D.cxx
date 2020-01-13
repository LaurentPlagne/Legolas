# include <cstdlib>
# include <cmath>
# include <iostream>

# include "UTILITES.hxx" 
# include "Legolas/Vector/Vector.hxx"
 
# include "XYLightBandedBlockMatrix.hxx"
# include "XYBandedBlockMatrix.hxx"
# include "XYBandedPolyBlockMatrix.hxx"
# include "XYDiagonalBlockMatrix.hxx"


#include "X86Timer.hxx"

#include "SourceFunctor.hxx"

#include "Legolas/BlockMatrix/tst/LegolasTestSolver.hxx"
#include "Legolas/BlockMatrix/ConjugateGradientSolver.hxx"
#include "Legolas/BlockMatrix/PreconditionedConjugateGradientSolver.hxx"
#include "Legolas/BlockMatrix/BiCGStabSolver.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseParallelMultOperator.hxx"
#include "Legolas/BlockMatrix/SteepestDescentSolver.hxx"
#include "Legolas/BlockMatrix/SteepestDescentMinResSolver.hxx"

using namespace std; using namespace Legolas;

int main( SizeType argc,  char *argv[] )
{
  //  typedef Legolas::Vector<double> Vector1D;
  typedef Legolas::MultiVector<double,1> Vector1D;

  const int maxIteration=10000;
  
  // ************* Physical data ***********************

  // Mesh : the 1D mesh is a regular partition of [0,1];
  const SizeType meshSize=128;
  //  const SizeType meshSize=1024;
  //  const SizeType meshSize=512;
  
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
  
  
  U=0.0;

  Vector2D S(U);
  
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

  //  Legolas::MultiVector<double,2>::Shape  shape(size,size);
  Legolas::MatrixShape<2> ms2(shape,shape);
  
  if (1==0)
  {
    XYLightBandedBlockMatrix  LXY(ms2);
    LXY.maxIteration()=maxIteration;
    LXY.displayLatex("LXY.tex");
    LXY.solve(S,U);
    LXY.displayProfile();
    INFOS("U[0]="<<U[0]);
    Vector2D R(S);
    LXY.addMult(-1.0,U,R);
    INFOS("R[0]="<<R[0]);
    cout << "Residual="<< (Legolas::dot(R,R)) << endl ;
  }

  if (1==0)
  {
    U=0.0;
    XYBandedBlockMatrix  HXY(ms2);
    HXY.maxIteration()=maxIteration;
    HXY.solve(S,U);
    HXY.displayProfile();
    INFOS("U[0]="<<U[0]);
    Vector2D R(S);
    HXY.addMult(-1.0,U,R);
    INFOS("R[0]="<<R[0]);
    cout << "Residual="<< (Legolas::dot(R,R)) << endl ;
  }

  if (1==1)
  {
    U=0.0;
    XYBandedPolyBlockMatrix  PXY(ms2);
    PXY.maxIteration()=maxIteration;
    PXY.epsilon()=1.e-5;
    PXY.solve(S,U);

    INFOS("PXY.iterationNumber()="<<PXY.iterationNumber());
    INFOS("PXY.relativeDifference()="<<PXY.relativeDifference());
    
    PXY.displayProfile();
    INFOS("U[0]="<<U[0]);
    Vector2D R(S);
    PXY.addMult(-1.0,U,R);
    INFOS("R[0]="<<R[0]);
    cout << "Residual="<< (Legolas::dot(R,R)) << endl ;
  }

  {
    U=0.0;
    XYBandedPolyBlockMatrix  PXY(ms2);

    PXY.setSolverPtr(new Legolas::ConjugateGradientSolver() );
    PXY.setMultOperatorPtr(new Legolas::SparseParallelMultOperator() );

    
    PXY.maxIteration()=maxIteration;
    PXY.fixedIterationNumber()=false;
    PXY.epsilon()=1.e-5;

    INFOS("PXY.maxIteration()="<<PXY.maxIteration());


    PXY.solve(S,U);

    INFOS("PXY.iterationNumber()="<<PXY.iterationNumber());
    INFOS("PXY.relativeDifference()="<<PXY.relativeDifference());
    
    PXY.displayProfile();
    INFOS("U[0]="<<U[0]);
    Vector2D R(S);
    PXY.addMult(-1.0,U,R);
    INFOS("R[0]="<<R[0]);
    cout << "Residual="<< (Legolas::dot(R,R)) << endl ;
  }

  {
    U=0.0;
    XYBandedPolyBlockMatrix  PXY(ms2);

    PXY.setSolverPtr(new Legolas::BiCGStabSolver() );
    PXY.setMultOperatorPtr(new Legolas::SparseParallelMultOperator() );


    PXY.maxIteration()=maxIteration;
    PXY.fixedIterationNumber()=false;
    PXY.epsilon()=1.e-5;

    INFOS("PXY.maxIteration()="<<PXY.maxIteration());


    PXY.solve(S,U);

    INFOS("PXY.iterationNumber()="<<PXY.iterationNumber());
    INFOS("PXY.relativeDifference()="<<PXY.relativeDifference());
    
    PXY.displayProfile();
    INFOS("U[0]="<<U[0]);
    Vector2D R(S);
    PXY.addMult(-1.0,U,R);
    INFOS("R[0]="<<R[0]);
    cout << "Residual="<< (Legolas::dot(R,R)) << endl ;
  }

  {
    U=0.0;
    XYBandedPolyBlockMatrix  PXY(ms2);

    //    XYDiagonalBlockMatrix * Preconditioner = new XYDiagonalBlockMatrix(ms2);
    Legolas::Matrix * Preconditioner = new XYDiagonalBlockMatrix(ms2);
    INFOS("Preconditioner="<<Preconditioner);

    auto pcg=new Legolas::PreconditionedConjugateGradientSolver(Preconditioner);

    INFOS("pcg->name()="<<pcg->name());

    PXY.setSolverPtr(pcg );
    PXY.setMultOperatorPtr(new Legolas::SparseParallelMultOperator() );


    PXY.maxIteration()=maxIteration;
    PXY.fixedIterationNumber()=false;
    PXY.epsilon()=1.e-5;

    INFOS("PXY.maxIteration()="<<PXY.maxIteration());


    PXY.solve(S,U);

    INFOS("PXY.iterationNumber()="<<PXY.iterationNumber());
    INFOS("PXY.relativeDifference()="<<PXY.relativeDifference());
    
    PXY.displayProfile();
    INFOS("U[0]="<<U[0]);
    Vector2D R(S);
    PXY.addMult(-1.0,U,R);
    INFOS("R[0]="<<R[0]);
    cout << "Residual="<< (Legolas::dot(R,R)) << endl ;
  }

 
  {
    U=0.0;
    XYBandedPolyBlockMatrix  PXY(ms2);

    auto solver=new Legolas::SteepestDescentSolver();
    INFOS("solver->name()="<<solver->name());

    PXY.setSolverPtr(solver );
    PXY.setMultOperatorPtr(new Legolas::SparseParallelMultOperator() );

    
    PXY.maxIteration()=maxIteration;
    PXY.fixedIterationNumber()=false;
    PXY.epsilon()=1.e-5;

    INFOS("PXY.maxIteration()="<<PXY.maxIteration());


    PXY.solve(S,U);

    INFOS("PXY.iterationNumber()="<<PXY.iterationNumber());
    INFOS("PXY.relativeDifference()="<<PXY.relativeDifference());
    
    PXY.displayProfile();
    INFOS("U[0]="<<U[0]);
    Vector2D R(S);
    PXY.addMult(-1.0,U,R);
    INFOS("R[0]="<<R[0]);
    cout << "Residual="<< (Legolas::dot(R,R)) << endl ;
  }

  {
    U=0.0;
    XYBandedPolyBlockMatrix  PXY(ms2);

    auto solver=new Legolas::SteepestDescentMinResSolver();
    INFOS("solver->name()="<<solver->name());

    PXY.setSolverPtr(solver);
    PXY.setMultOperatorPtr(new Legolas::SparseParallelMultOperator() );

    
    PXY.maxIteration()=maxIteration;
    PXY.fixedIterationNumber()=false;
    PXY.epsilon()=1.e-5;

    INFOS("PXY.maxIteration()="<<PXY.maxIteration());


    PXY.solve(S,U);

    INFOS("PXY.iterationNumber()="<<PXY.iterationNumber());
    INFOS("PXY.relativeDifference()="<<PXY.relativeDifference());
    
    PXY.displayProfile();
    INFOS("U[0]="<<U[0]);
    Vector2D R(S);
    PXY.addMult(-1.0,U,R);
    INFOS("R[0]="<<R[0]);
    cout << "Residual="<< (Legolas::dot(R,R)) << endl ;
  }

  if (1==0)
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

  if (1==0)
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

#ifndef UU 
#endif
  
}
