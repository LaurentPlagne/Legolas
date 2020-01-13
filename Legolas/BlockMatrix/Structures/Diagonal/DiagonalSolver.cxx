#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalBlockMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Diagonal/DiagonalSolver.hxx"

namespace Legolas{

  DiagonalSolver::DiagonalSolver( void ){}
  DiagonalSolver::DiagonalSolver(const DiagonalSolver & source){}
  DiagonalSolver::~DiagonalSolver( void ){}
  
  void DiagonalSolver::solve(const DiagonalVirtualBlockMatrix & A, const VirtualVector & B, VirtualVector & X){

    for (int i=A.beginRow() ; i < A.endRow() ; i++ ){
      A.diagonalGetElement(i).solve(B.getElement(i),X.getElement(i));
    }
      
  }
}
    
