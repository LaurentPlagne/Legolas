#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/BlockMatrix/BlockProfile.hxx"

namespace Legolas{
  
  Matrix::Matrix( void ):virtualMatrixShapePtr_(0),father_(0),solverPtr_(0),multOperatorPtr_(0){}
  Matrix::Matrix(const Matrix & s):virtualMatrixShapePtr_(0),father_(s.father_),solverPtr_(0),multOperatorPtr_(0){
    //    INFOS("Matric Copy CTor");
    if (s.virtualMatrixShapePtr_) virtualMatrixShapePtr_=s.virtualMatrixShapePtr_->clone();
    if (s.solverPtr_) solverPtr_=s.solverPtr_->clone();
    if (s.multOperatorPtr_) multOperatorPtr_=s.multOperatorPtr_->clone();
  }

  Matrix::Matrix(const VirtualMatrixShape & virtualMatrixShape):virtualMatrixShapePtr_(virtualMatrixShape.clone()),father_(0),solverPtr_(0),multOperatorPtr_(0){}

  Matrix::~Matrix( void ){ 
    if (virtualMatrixShapePtr_){ delete virtualMatrixShapePtr_ ; virtualMatrixShapePtr_=0;}
    if (solverPtr_){ delete solverPtr_ ; solverPtr_=0;}
    if (multOperatorPtr_){ delete multOperatorPtr_ ; multOperatorPtr_=0;}
  }
  
    //solve AX=B;
  void Matrix::solve(const VirtualVector & B, VirtualVector & X) const {
    //    Legolas::BlockProfile bp(*this,1);
    //    assert(solverPtr_);
    //    INFOS("ICI");
    //    INFOS(" solverPtr_="<< solverPtr_);
    solverPtr_->solve(*this,B,X);
  }    
    
  //Y<- Y + a*A*X
   void Matrix::addMult(const double & a,
			const VirtualVector & X, VirtualVector & Y) const {
    Legolas::BlockProfile bp(*this,2);
    assert(multOperatorPtr_);
    multOperatorPtr_->addMult(*this,a,X,Y);
  }

  //Y=A*X
   void Matrix::mult(const VirtualVector & X, VirtualVector & Y) const {
    Legolas::BlockProfile bp(*this,2);
    assert(multOperatorPtr_);
    multOperatorPtr_->mult(*this,X,Y);
  }

    //solve AtX=B;
  void Matrix::transposeSolve(const VirtualVector & B, VirtualVector & X) const {
    //    Legolas::BlockProfile bp(*this,1);
    //    assert(solverPtr_);
    //    INFOS("ICI");
    //    INFOS(" solverPtr_="<< solverPtr_);
    solverPtr_->transposeSolve(*this,B,X);
  }    
    
  //Y<- Y + a*At*X
  void Matrix::transposeAddMult(const double & a,
				const VirtualVector & X, VirtualVector & Y) const {
    Legolas::BlockProfile bp(*this,2);
    assert(multOperatorPtr_);
    multOperatorPtr_->transposeAddMult(*this,a,X,Y);
  }

  //Y=At*X
  void Matrix::transposeMult(const VirtualVector & X, VirtualVector & Y) const {
    Legolas::BlockProfile bp(*this,2);
    assert(multOperatorPtr_);
    multOperatorPtr_->transposeMult(*this,X,Y);
  }


 
  int Matrix::nrows( void ) const { return virtualMatrixShapePtr_->nrows();}
  int Matrix::ncols( void ) const { return virtualMatrixShapePtr_->ncols();}
    

  int Matrix::level( void ) const { return virtualMatrixShapePtr_->getLevel();}
    
  void Matrix::displayLatex(std::string filename, int flag) const
  {
    INFOS("filename="<<filename);
    const int MAXDISP=300; 
    
    const int frs=this->getVirtualMatrixShape().getRowShape().flatSize();
    const int fcs=this->getVirtualMatrixShape().getColShape().flatSize();
      
    if ( (frs>MAXDISP) || (frs>MAXDISP) ){
      INFOS("Display is skipped because the matrix is too large ("<<frs<<"x"<<fcs<<")");
    }
    else{
      Legolas::LatexMatrixStream lms(filename,flag,this->level());
      this->iterateOverElements(lms);
    }
  }
  

  void Matrix::displayPNG(std::string filename, int flag) const 
  {
    INFOS("filename="<<filename);
    const int MAXDISP=4000; 
    
    const int frs=this->getVirtualMatrixShape().getRowShape().flatSize();
    const int fcs=this->getVirtualMatrixShape().getColShape().flatSize();
    
    int borderSize=1;
    int unitCellSize=5;
    
    if ( (frs>MAXDISP) || (frs>MAXDISP) ){
      INFOS("Display is skipped because the matrix is too large ("<<frs<<"x"<<fcs<<")");
    }
      else{
	Legolas::PNGMatrixStream pms(filename,flag,this->level(),frs,fcs,borderSize,unitCellSize);
	this->iterateOverElements(pms);
      }
  }

  void Matrix::displaySVG(std::string filename, int flag) const 
  {
    INFOS("filename="<<filename);
    const int MAXDISP=4000; 
    
    const int frs=this->getVirtualMatrixShape().getRowShape().flatSize();
    const int fcs=this->getVirtualMatrixShape().getColShape().flatSize();
    
    int borderSize=1;
    int unitCellSize=5;
    
    if ( (frs>MAXDISP) || (frs>MAXDISP) ){
      INFOS("Display is skipped because the matrix is too large ("<<frs<<"x"<<fcs<<")");
    }
      else{
	Legolas::SVGMatrixStream svgms(filename,flag,this->level(),frs,fcs,borderSize,unitCellSize);
	this->iterateOverElements(svgms);
      }
  }
  
  //  std::string Matrix::name( void ) const { return "Anonymous Matrix";}
  //  void Matrix::iterateOverElements(Legolas::MatrixStream & matrixStream) const { INFOS("not yet implemented"); throw std::runtime_error("Not yet implemented") ; }
  //  void Matrix::collectStatistics(Profile & profile) const { INFOS("not yet implemented"); throw std::runtime_error("Not yet implemented"); ; }

  //  int Matrix::getPrecisionInBytePerRealElement( void ) const { INFOS("not et implemented"); throw std::runtime_error("Not yet implemented"); ; }
  
  void Matrix::displayProfile( void ) const {
    
    Profile profile;
    this->collectStatistics(profile);
    
    INFOS("this->name()="<<this->name());
    
    dispProfile(profile);
  }

  int Matrix::iterationNumber( void ) const { assert(solverPtr_); return solverPtr_->iterationNumber() ;}
  double Matrix::relativeDifference( void ) const { assert(solverPtr_); return solverPtr_->relativeDifference() ; }

  
  void Matrix::setVirtualMatrixShapePtr(VirtualMatrixShape * virtualMatrixShapePtr){
    virtualMatrixShapePtr_=virtualMatrixShapePtr;
  }
    
  const VirtualMatrixShape * Matrix::getVirtualMatrixShapePtr( void ) const {
    assert(virtualMatrixShapePtr_);
    return virtualMatrixShapePtr_;
  }

  const VirtualMatrixShape & Matrix::getVirtualMatrixShape( void ) const {return *this->getVirtualMatrixShapePtr();}

  void Matrix::setVirtualSolverPtr(VirtualSolver * solverPtr){ 
    if (solverPtr_) delete solverPtr_;
    solverPtr_=solverPtr;
  }
  void Matrix::setVirtualMultOperatorPtr(VirtualMultOperator * multOperatorPtr){
    if (multOperatorPtr_) delete multOperatorPtr_;
    multOperatorPtr_=multOperatorPtr;
  }

  VirtualSolver * Matrix::virtualSolver( void ) const { return solverPtr_ ;}
  VirtualMultOperator * Matrix::virtualMultOperator( void ) const {return multOperatorPtr_;}

  void Matrix::setSolverPtr(MatrixSolver * solverPtr){ 
    //    INFOS("ICI");
    if (solverPtr_) delete solverPtr_;
    solverPtr_=solverPtr;
    //    INFOS("solverPtr_="<<solverPtr_);
  }
  void Matrix::setMultOperatorPtr(MatrixMultOperator * multOperatorPtr){
    if (multOperatorPtr_) delete multOperatorPtr_;
    multOperatorPtr_=multOperatorPtr;
  }
 

  const int & Matrix::maxIteration( void ) const { assert(solverPtr_); return solverPtr_->maxIteration();}
  int & Matrix::maxIteration( void ) { assert(solverPtr_); return solverPtr_->maxIteration();}
  const double & Matrix::epsilon( void ) const { assert(solverPtr_); return solverPtr_->epsilon();}
  void Matrix::modifyEpsilon( double epsilon ) const { assert(solverPtr_); return solverPtr_->modifyEpsilon(epsilon);}
  double & Matrix::epsilon( void ) { assert(solverPtr_); return solverPtr_->epsilon();}
  const bool & Matrix::fixedIterationNumber( void ) const { assert(solverPtr_); return solverPtr_->fixedIterationNumber();}
  bool & Matrix::fixedIterationNumber( void ) { assert(solverPtr_); return solverPtr_->fixedIterationNumber();}
  
}

    
    
    
    

  
