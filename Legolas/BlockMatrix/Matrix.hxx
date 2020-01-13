#ifndef __LEGOLAS_MATRIX_HXX__
#define __LEGOLAS_MATRIX_HXX__

#include "UTILITES.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/Matrix/MatrixStream.hxx"
#include "Legolas/Matrix/LatexMatrixStream.hxx"
#include "Legolas/Matrix/PNGMatrixStream.hxx"
#include "Legolas/Matrix/SVGMatrixStream.hxx"
#include "Legolas/BlockMatrix/VirtualSolver.hxx"
#include "Legolas/BlockMatrix/VirtualMultOperator.hxx"
#include "Legolas/BlockMatrix/MatrixSolver.hxx"
#include "Legolas/BlockMatrix/MatrixMultOperator.hxx"
#include "Legolas/BlockMatrix/Profile.hxx"


namespace Legolas{
  
  class Matrix{
  public:
    
    Matrix( void );
    Matrix(const Matrix & m);
    Matrix(const VirtualMatrixShape & virtualMatrixShape);
    virtual ~Matrix( void );
    
    //solve AX=B;
    void solve(const VirtualVector & B, VirtualVector & X) const;
    //solve AtX=B;
    void transposeSolve(const VirtualVector & B, VirtualVector & X) const;
    //Y<- Y + a*A*X
    void addMult(const double & a,
		 const VirtualVector & X, VirtualVector & Y) const;
    //Y<- Y + a*At*X
    void transposeAddMult(const double & a,
			  const VirtualVector & X, VirtualVector & Y) const;
    
    //Y=A*X
    void mult(const VirtualVector & X, VirtualVector & Y) const;
    //Y=At*X
    void transposeMult(const VirtualVector & X, VirtualVector & Y) const;
    
    int nrows( void ) const ;
    int ncols( void ) const ;
    
    int level( void ) const ;
    
    void displayLatex(std::string filename, int flag=0) const ;
    void displayPNG(std::string filename, int flag=0) const ;
    void displaySVG(std::string filename, int flag=0) const ;

    virtual std::string name( void ) const { return "anonymous";}
    virtual void iterateOverElements(Legolas::MatrixStream & matrixStream) const { return ;}
    virtual void collectStatistics(Profile & profile) const {return;}

    virtual int getPrecisionInBytePerRealElement( void ) const {return -1;}
    
    void displayProfile( void ) const ;
    
    int iterationNumber( void ) const ;
    double relativeDifference( void ) const ;
    
    //Accessor to members

    void setVirtualMatrixShapePtr(VirtualMatrixShape * virtualMatrixShapePtr);
    const VirtualMatrixShape * getVirtualMatrixShapePtr( void ) const ;
    const VirtualMatrixShape & getVirtualMatrixShape( void ) const ;

    const int & maxIteration( void ) const ;
    int & maxIteration( void ) ;
    
    const double & epsilon( void ) const ; 
    double & epsilon( void ) ;

    void modifyEpsilon( double epsilon ) const ;
    
    const bool & fixedIterationNumber( void ) const ;
    bool & fixedIterationNumber( void ) ;

    void setVirtualSolverPtr(VirtualSolver * solverPtr) ;
    void setVirtualMultOperatorPtr(VirtualMultOperator * multOperatorPtr) ;
    
    VirtualSolver * virtualSolver( void ) const ; 
    VirtualMultOperator * virtualMultOperator( void ) const ; 

    void setSolverPtr(MatrixSolver * solverPtr) ;
    void setMultOperatorPtr(MatrixMultOperator * multOperatorPtr) ;

  private:
    
    VirtualMatrixShape * virtualMatrixShapePtr_;
    Matrix * father_;
    VirtualSolver * solverPtr_;
    VirtualMultOperator * multOperatorPtr_;
    
  };
  
}

#endif
    
    
    
    
    

  
