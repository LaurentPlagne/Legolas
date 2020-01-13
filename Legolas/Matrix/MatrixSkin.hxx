/**
 * project  DESCARTES
 *
 * @file     MatrixSkin.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_MATRIXSKIN_HXX__
#define __LEGOLAS_MATRIXSKIN_HXX__

#include "Legolas/Vector/BaseMatrix.hxx"
#include "Legolas/Vector/Vector.hxx"

#include "Legolas/Matrix/StorageModeTraits/GetElementTraits.hxx"
#include "Legolas/Matrix/Transpose/TransposeTraits.hxx"
#include "Legolas/Matrix/StorageModeTraits/ConstRefTraits.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/Matrix/IterateOverBlocks.hxx"
#include "Legolas/Vector/VectorPool.hxx"

#include "stringConvert.hxx"

namespace Legolas{


  template <class MATRIX_KERNEL, template <class> class OWNERSHIP_POLICY,bool IsTransposed=false>
  class MatrixSkin:public OWNERSHIP_POLICY<typename MATRIX_KERNEL::Data>, 
		   public BaseMatrix<MatrixSkin<MATRIX_KERNEL,OWNERSHIP_POLICY,IsTransposed> >
  {
  public:

    // Typedefs...
  
    typedef typename MATRIX_KERNEL::MatrixDataDriver       MatrixDataDriver;
    typedef typename MATRIX_KERNEL::MatrixElementInterface MatrixElementInterface;
    typedef typename MATRIX_KERNEL::MatrixVectorProduct    MatrixVectorProduct;
    typedef typename MATRIX_KERNEL::MatrixVectorInverse    MatrixVectorInverse;
    typedef typename MATRIX_KERNEL::Data                   Data;
    typedef typename MatrixDataDriver::StorageMode         StorageMode;

    typedef typename MatrixDataDriver::RealType            RealType;
    typedef typename Data::Shape               Shape;

  private:
    typedef OWNERSHIP_POLICY<Data>                         OwnerShipPolicy;
    typedef typename OwnerShipPolicy::CtorType             CtorType;
  
    //*************** GetElementTraits resolution
  public:

    typedef typename GetElementTraits<MatrixElementInterface,OWNERSHIP_POLICY,StorageMode>::GetElement      GetElement;
    typedef typename GetElementTraits<MatrixElementInterface,OWNERSHIP_POLICY,StorageMode>::ConstGetElement ConstGetElement;

  public:

    typedef typename ConstRefTraits<MatrixSkin,StorageMode>::Type StoreType;

    //static bool isAMatrix( void ) { return true ; }
    //    bool isAMatrixOfMatrix( void ) const { return MatrixElementInterface::isAMatrix() ; }
    
    //    static const bool matrixOfMatrices=MatrixElementInterface::isAMatrix_;

    //    static bool elementIsAMatrix( void ) { return MatrixElementInterface::isAMatrix() ; }
    //    bool isAMatrixOfMatrices( void ) const { return MatrixElementInterface::isAMatrix() ; }

    static std::string structureName( void ) { return MatrixDataDriver::name() ; }

    std::string getStructureName( void ) const { return MatrixDataDriver::name() ; }

    //    static const int level=LevelTraits<Shape>::Level;
    static const int level=MatrixDataDriver::level;

    typedef Legolas::MultiVector<RealType,level> VL;

    //    void mvp(const VL & X, VL & Y) const {
      
	     

    //#define UNDEFINED
    //#ifdef UNDEFINED

    inline void matrixVectorProduct(const AnyVector & aX, AnyVector & aY, int addFlag) const {
      //      INFOS("!!!!!!!!!!!!!!!Entering  matrixVectorProduct");
      typedef Legolas::MultiVector<RealType,level> V;
      const V * XPtr=static_cast<const V*>(&aX);
      V * YPtr=static_cast<V*>(&aY);
      const Legolas::MultiVector<RealType,level> & X(*XPtr);
      Legolas::MultiVector<RealType,level> & Y(*YPtr);

      if (addFlag==0){
	typedef typename MatrixVectorProduct::template Engine< EqualAssign > Operator;
	Operator::apply(*this,X,Y);
      }
      if (addFlag==1){
	typedef typename MatrixVectorProduct::template Engine< PlusAssign > Operator;
	Operator::apply(*this,X,Y);
      }
      if (addFlag==-1){
	typedef typename MatrixVectorProduct::template Engine< MinusAssign > Operator;
	//	INFOS("Y="<<Y);
	Operator::apply(*this,X,Y);
	//	INFOS("Y="<<Y);
      }
    }

  //   inline void matrixVectorInverse(const AnyVector & aX, AnyVector & aY, AnyWorkSpace & aws) const {
//       //      INFOS("!!!!!!!!!!!!!!!Entering  matrixVectorInverse");
//       typedef Legolas::MultiVector<RealType,level> V;
//       const V * XPtr=static_cast<const V*>(&aX);
//       V * YPtr=static_cast<V*>(&aY);
//       const Legolas::MultiVector<RealType,level> & X(*XPtr);
//       Legolas::MultiVector<RealType,level> & Y(*YPtr);

//       typedef typename MatrixVectorInverse::template Engine< EqualAssign > Operator;

//       //      INFOS("ICI");

//       typedef LinearSolverWorkSpace<V> WS;

//       WS * wsPtr=static_cast< WS* >(&aws);

//       //      INFOS("ICI");
//       WS & ws(*wsPtr);

//       //      INFOS("ICI");
      
//       //Operator::apply(*this,X,Y);
//       Operator::apply(*this,X,Y,ws);
      
//       //      INFOS("ICI");
//       //      Operator::apply(*this,X,Y);
//       //      INFOS("!!!!!!!!!!!!!!!Out of   matrixVectorInverse");
//     }
    

    typedef Legolas::MultiVector<RealType,level> VectorOperand;
    typedef typename MatrixVectorInverse::template Engine< MatrixSkin,VectorOperand,VectorOperand > Solver;


    inline virtual Legolas::AnySolver * getSolverPtr(AnyVector & aX, const AnyVector & aB, void * avp) const {
      
      typedef Legolas::MultiVector<RealType,level> V;
      V * XPtr=static_cast<V*>(&aX);
      const V * BPtr=static_cast<const V*>(&aB);
      
      Legolas::MultiVector<RealType,level> & X(*XPtr);
      const Legolas::MultiVector<RealType,level> & B(*BPtr);

      Legolas::VectorPool<RealType,level> * vp=static_cast<Legolas::VectorPool<RealType,level> *>(avp);
      
      typedef typename MatrixVectorInverse::template Engine< MatrixSkin,V,V > Operator;

      //      return new Operator(*this,X,B);
      return new Operator(*this,X,B,vp);
      
    }

    
    inline void matrixVectorInverse(const AnyVector & aX, AnyVector & aY) const {
      //      INFOS("!!!!!!!!!!!!!!!Entering  matrixVectorInverse");
      typedef Legolas::MultiVector<RealType,level> V;
      const V * XPtr=static_cast<const V*>(&aX);
      V * YPtr=static_cast<V*>(&aY);
      const Legolas::MultiVector<RealType,level> & X(*XPtr);
      Legolas::MultiVector<RealType,level> & Y(*YPtr);

      //      typedef typename MatrixVectorInverse::template Engine< EqualAssign > Operator;
      //      Operator::apply(*this,X,Y);

      typedef typename MatrixVectorInverse::template Engine< MatrixSkin,V,V > Operator;
      Operator solver(*this,Y,X);
      //      INFOS("ICI");
//       throw std::runtime_error("ICI");
      solver.solve(*this,Y,X);
    }


    inline void transposeMatrixVectorProduct(const AnyVector & aX, AnyVector & aY, int addFlag) const {
      //      INFOS("!!!!!!!!!!!!!!!Entering  transposematrixVectorProduct");
      typedef Legolas::MultiVector<RealType,level> V;
      const V * XPtr=static_cast<const V*>(&aX);
      V * YPtr=static_cast<V*>(&aY);
      const Legolas::MultiVector<RealType,level> & X(*XPtr);
      Legolas::MultiVector<RealType,level> & Y(*YPtr);

      if (addFlag==0){
	typedef typename MatrixVectorProduct::Transpose::template Engine< EqualAssign > Operator;
	Operator::apply(*this,X,Y);
      }
      if (addFlag==1){
	typedef typename MatrixVectorProduct::Transpose::template Engine< PlusAssign > Operator;
	Operator::apply(*this,X,Y);
      }
      if (addFlag==-1){
	typedef typename MatrixVectorProduct::Transpose::template Engine< MinusAssign > Operator;
	//	INFOS("Y="<<Y);
	Operator::apply(*this,X,Y);
	//	INFOS("Y="<<Y);
      }
    }

 //    void transposeMatrixVectorInverse(const AnyVector & aX, AnyVector & aY) const {
//       INFOS("!!!!!!!!!!!!!!!Entering  matrixVectorInverse");
//       typedef Legolas::MultiVector<RealType,level> V;
//       const V * XPtr=static_cast<const V*>(&aX);
//       V * YPtr=static_cast<V*>(&aY);
//       const Legolas::MultiVector<RealType,level> & X(*XPtr);
//       Legolas::MultiVector<RealType,level> & Y(*YPtr);

//       typedef typename MatrixVectorInverse::Transpose::template Engine< EqualAssign > Operator;
//       Operator::apply(*this,X,Y);
//       INFOS("!!!!!!!!!!!!!!!Out of   matrixVectorInverse");
//     }

    inline void displayLatexMethod(std::string filename, int flag=0) const
    {
      Legolas::displayLatex((*this),filename,flag);
    }

    typedef typename MatrixDataDriver::FilledRows DriverFilledRows;
    typedef typename MatrixDataDriver::FilledCols DriverFilledCols;


    struct FilledRows : public DriverFilledRows{
      FilledRows( const MatrixSkin & matrix):DriverFilledRows(matrix.getRef()){}
    };
 
    struct FilledCols : public DriverFilledCols{
      FilledCols( const MatrixSkin & matrix):DriverFilledCols(matrix.getRef()){}
    };




    void iterateOverBlocksMethod(Legolas::MatrixStream & matrixStream) const {
      Legolas::IterateOverBlocks::apply((*this),matrixStream);
    }


    //    int getFlatRowSizeMethod( void ) const { return Legolas::getFlatSize(this->getRowShape());}
    //    int getFlatColSizeMethod( void ) const { return Legolas::getFlatSize(this->getColShape());}

    int getFlatRowSizeMethod( void ) const { return this->getRowShape().flatSize() ; }
    int getFlatColSizeMethod( void ) const { return this->getColShape().flatSize() ; }

    int getLevel( void ) const { return level;}

    
    //#endif

    
    AnyMatrix * clone( void ) const { 
      return new MatrixSkin(*this) ;
    }
    

    // Copy Type

    // Member functions

    MatrixSkin( CtorType init):OwnerShipPolicy(init){
      MESSAGE("MatrixSkin Ctor");
    }

    virtual ~MatrixSkin( void ){
      MESSAGE("MatrixSkin Dtor");
    }

    // Templated Copy Ctor...
  
    template <template <class> class ANOTHER_OWNERSHIP_POLICY, bool OtherTransposed>
    MatrixSkin( const MatrixSkin<MATRIX_KERNEL,ANOTHER_OWNERSHIP_POLICY,OtherTransposed> & source):OwnerShipPolicy(source.getRef()){
      MESSAGE("MatrixSkin: Template Copy Ctor");
    }
  
    template <template <class> class ANOTHER_OWNERSHIP_POLICY>
    MatrixSkin &  operator = (const MatrixSkin<MATRIX_KERNEL,ANOTHER_OWNERSHIP_POLICY,IsTransposed> & source){
      this->getRef()=source.getRef();
      return *this;
    }
    
    typedef TransposeTraits<IsTransposed> TT;


    //******************************* Dense API

    inline ConstGetElement operator () ( int i , int j ) const
    {
      return MatrixDataDriver::getElement(TT::getI(i,j),TT::getJ(i,j),this->getRef());
    }

    inline GetElement operator () ( int i , int j ) 
    {
      return MatrixDataDriver::getElement(TT::getI(i,j),TT::getJ(i,j),this->getRef());
    }

    inline int nrows( void ) const
    {
      return this->getRef().nrows();
      //      return this->getRef().nrows();
      //      return MatrixDataDriver::nrows(this->getRef());
    }


    inline int ncols( void ) const
    {
      //      return this->getRef().getColShape().ncols();
      return this->getRef().ncols();
      //      return MatrixDataDriver::ncols(this->getRef());
    }

    //******************************* Sparse API
    
    inline ConstGetElement sparseGetElement ( int i , int j ) const
    {
      
      //      ConstGetElement b=2.0;
      //      int a=MatrixDataDriver::sparseGetElement(i,j,this->getRef());

      //      return a;
      //
      return MatrixDataDriver::sparseGetElement(i,j,this->getRef());
    }


    inline GetElement sparseGetElement ( int i , int j ) 
    {
      return MatrixDataDriver::sparseGetElement(i,j,this->getRef());
    }


    //******************************* Banded API

      //    inline ConstGetElement zero ( void ) const
      //    {
      //      return MatrixDataDriver::zero( this->getRef() );
      //    }
  
    inline ConstGetElement bandedGetElement ( int i , int j ) const
    {
      return MatrixDataDriver::bandedGetElement(i,j,this->getRef());
    }

    inline GetElement bandedGetElement ( int i , int j ) 
    {
      return MatrixDataDriver::bandedGetElement(i,j,this->getRef());
    }

    inline int linf( void ) const
    {
      return MatrixDataDriver::linf(this->getRef());
    }

    inline int lsup( void ) const
    {
      return MatrixDataDriver::lsup(this->getRef());
    }



    //******************************* TinyBandedSymmetric API

    inline ConstGetElement lowerBandedGetElement ( int i , int j ) const
    {
      return MatrixDataDriver::lowerBandedGetElement(i,j,this->getRef());
    }
    
    inline GetElement lowerBandedGetElement ( int i , int j ) 
    {
      return MatrixDataDriver::lowerBandedGetElement(i,j,this->getRef());
    }

    inline int getHalfBandwidth( void ) const
    {
      return MatrixDataDriver::getHalfBandwidth(this->getRef());
    }

    //******************************* Diagonal API



    inline ConstGetElement diagonalGetElement ( int i ) const
    {
      //      MatrixDataDriver::diagonalGetElement(i,this->getRef());
      //      ConstGetElement a= MatrixDataDriver::diagonalGetElement(i,this->getRef());
      //      ConstGetElement b= a;
            return MatrixDataDriver::diagonalGetElement(i,this->getRef());
      //      return b;
    }

    inline GetElement diagonalGetElement ( int i )
    {
      return MatrixDataDriver::diagonalGetElement(i,this->getRef());
    }

    //******************************* TriDiagonal API

    inline ConstGetElement upperDiagonalGetElement ( int i ) const
    {
      return MatrixDataDriver::upperDiagonalGetElement(i,this->getRef());
    }

    inline GetElement upperDiagonalGetElement ( int i ) 
    {
      return MatrixDataDriver::upperDiagonalGetElement(i,this->getRef());
    }
  
    inline ConstGetElement lowerDiagonalGetElement ( int i ) const
    {
      return MatrixDataDriver::lowerDiagonalGetElement(i,this->getRef());
    }

    inline GetElement lowerDiagonalGetElement ( int i ) 
    {
      return MatrixDataDriver::lowerDiagonalGetElement(i,this->getRef());
    }
  
    inline void display( std::string matrixName="Matrix" ) const{
    
      std::cout << "******************** "<< matrixName <<" *********************"<<std::endl; 
      for (int i=0 ; i < nrows() ; i++){
	for (int j=0 ; j < ncols() ; j++){
	  
	  std::cout << matrixName<<"("<<i<<","<<j<<")="<<(*this)(i,j) << std::endl ;
	
	}
      }
    }

    Legolas::MatrixShape<level> getMatrixShape( void ) const {
      return Legolas::MatrixShape<level>(this->getRowShape(),this->getColShape());
    }
      

    inline void reShape( const Legolas::MatrixShape<level> & shape){
      MatrixDataDriver::resize(shape,this->getRef());
    }
    
    
    //    inline const Shape & getRowShape( void ) const {
    inline Shape getRowShape( void ) const {
      //     return MatrixDataDriver::getRowShape(this->getRef());
      return this->getRef().getRowShape();
    }


    //    inline const Shape & getColShape( void ) const {
    inline Shape getColShape( void ) const {
      //      return MatrixDataDriver::getColShape(this->getRef());
      return this->getRef().getColShape();
    }

    inline Shape & setRowShape( void ) {
      //     return MatrixDataDriver::getRowShape(this->getRef());
      return this->getRef().setRowShape();
    }


    inline Shape & setColShape( void ) {
      //      return MatrixDataDriver::getColShape(this->getRef());
      return this->getRef().setColShape();
    }


    inline unsigned int computeFlatRowSize( void ) const{
      return getFlatSize(this->getRowShape());
      //      return getFlatSize(MatrixDataDriver::getRowShape(this->getRef()));
    }

    inline unsigned int computeFlatColSize( void ) const{
      return getFlatSize(this->getColShape());
      //      return getFlatSize(MatrixDataDriver::getColShape(this->getRef()));
    }


    inline int getMaxIteration( void ) const {
      return MatrixDataDriver::getMaxIteration(this->getRef());
    }

    inline int & setMaxIteration( void ) {
      return MatrixDataDriver::setMaxIteration(this->getRef());
    }

    inline RealType getEpsilon( void ) const {
      return MatrixDataDriver::getEpsilon(this->getRef());
    }

    inline RealType & setEpsilon( void ) {
      return MatrixDataDriver::setEpsilon(this->getRef());
    }


    inline const Data & getDataReference( void ) const { return this->getRef(); }
    inline Data & getDataReference( void ) { return this->getRef(); }


//     template <class NON_ZERO_INDEX>
//     bool nonZeroElement(int index, const NON_ZERO_INDEX & nzi) const {
//       bool result=false;
//       int i=nzi.begin();
//       while(i<nzi.end() && (result==false)){
// 	//	INFOS("i="<<i);
// 	if (index==i) result=true;
// 	nzi.next(i);
//       }
//       return result;
//     }


    template <class NON_ZERO_INDEX>
    bool nonZeroElement(int index, const NON_ZERO_INDEX & nzi) const {
      bool result=false;
      int i=nzi.begin();
      while(i<=index) nzi.next(i);

      if (i==index) result=true;
      
      return result;
    }


    void displayDelimiter(int i, int j, std::ofstream & outfile) const {
      if (j < this->ncols()-1){
	outfile << "&" ;  
      }
      else{
	if (i < this->nrows()-1){
	  outfile << "\\\\" ;  
	}
	outfile << std::endl ;        
      }
    }
    

//     void sparseRecursiveDisplayHTMLMethod(const std::string & matrixName, std::ofstream & outfile, const std::string * parentNamePtr=0, int flag=0) const
//     {
      

//       outfile << "<html>" << std::endl;
//       outfile << "<head>" << std::endl;
//       outfile << "<title>"<< matrixName << "</title>" << std::endl;
//       outfile << "<META HTTP-EQUIV=\"CACHE-CONTROL\" CONTENT=\"NO-CACHE\">" << std::endl;
//       outfile << "</head>" << std::endl;
//       outfile << "<body>" << std::endl;

//       outfile << "<TABLE BORDER CELLPADDING=2>" << std::endl;
//       outfile << "<TR>" << std::endl;
//       outfile << "<TH ALIGN=CENTER, COLSPAN=" << this->ncols()+1 << ">" ;
//       if (parentNamePtr){
// 	const std::string & parentName=*parentNamePtr;
// 	std::string fileName(parentName);
// 	fileName+=".html";
// 	std::string indexName(matrixName.begin()+parentName.size(),matrixName.end());
// 	//	indexName.erase(0,parentName.size());
// 	outfile << "<a href="<<fileName<<" target=blank>"<< parentName << "  </a> " << indexName ;
//       }
//       else{
// 	outfile << matrixName << std::endl;
//       }

//       outfile << "</TH>" << std::endl;
//       outfile << "</TR>" << std::endl;
      
//       outfile << "<TR>" << std::endl;
//       outfile << "<TH ALIGN=CENTER>   </TH>"  << std::endl;
//       for (int j=0 ; j < this->ncols() ; j++)
// 	outfile << "<TH ALIGN=CENTER> "<< j << "</TH>"  << std::endl;


//       FilledRows rows(*this);
      
//       for (int i=0 ; i < this->nrows() ; i++){    

// 	outfile << "<TR>" << std::endl;
// 	outfile << "<TH ALIGN=CENTER> "<< i << "</TH>"  << std::endl;


// 	if (this->nonZeroElement(i,rows)){
// 	  typename FilledRows::FilledCols cols=rows.filledColsInRow(i);
	  
// 	  for (int j=0 ; j < this->ncols() ; j++){
// 	    outfile << "<TH ALIGN=CENTER> " ;
// 	    if (this->nonZeroElement(j,cols)){
// 	      if (Level==1){
// 		sparseRecursiveDisplayHTML(this->sparseGetElement(i,j),matrixName,outfile,0,flag);
// 	      }
// 	      else{
// 		std::string subMatrixName(matrixName);
// 		subMatrixName+="(";
// 		subMatrixName+=stringConvert(i);
// 		subMatrixName+=",";
// 		subMatrixName+=stringConvert(j);
// 		subMatrixName+=")";

// 		std::string newFileName(subMatrixName);
// 		newFileName+=".html";
// 		outfile << "<a href="<<newFileName<<" target=blank>"<< subMatrixName << "  </a> ";		
		
// 		std::ofstream subOutfile (newFileName.c_str(),std::ios::out) ;
// 		sparseRecursiveDisplayHTML(this->sparseGetElement(i,j),subMatrixName,subOutfile,&matrixName,flag);
// 		subOutfile.close();
		
// 	      }
// 	    }
// 	    outfile << "</TH> " ;
// 	  }
// 	}
//       }
	
//       outfile << "</body>"  << std::endl;
//       outfile << "</html>"  << std::endl;

//     }
    
    template <class TRAVERSAL_POLICY>
    struct ElementaryTraversal{
      template <class BLOCK_FILTER,class BLOCK>
      static inline void apply(const BLOCK & block,int i, int j, BLOCK_FILTER & filter){
	typedef typename TRAVERSAL_POLICY:: template Engine<BLOCK_FILTER> ET;
	ConstGetElement mij=block.sparseGetElement(i,j);
	ET::applyBlock(mij,filter);
      }
    };
	
//     static inline void plusEqual(const double & source, double & target){ target+=source;}
//     static inline void plusEqual(const float & source, float & target){ target+=source;}
    

//     static inline void plusEqual(const MatrixSkin & source, MatrixSkin & target){
	
//       FilledRows sourceRows(source);
//       FilledRows targetRows(target);

//       int iS=sourceRows.begin();
//       int iT=targetRows.begin();
      
//       while ( (iS<sourceRows.end()) ){
// 	if (iS==iT){//both rows contain nonzero elements
// 	  int i=iS;
// 	  typename FilledRows::FilledCols sourceFilledColsInRow=sourceRows.filledColsInRow(i);
// 	  typename FilledRows::FilledCols targetFilledColsInRow=targetRows.filledColsInRow(i);
	  
// 	  int jS=sourceFilledColsInRow.begin();
// 	  int jT=targetFilledColsInRow.begin();
	  
// 	  while ( jS<sourceFilledColsInRow.end() ){
	    
// 	    if (jS==jT){
// 	      const int j=jS;
// 	      MatrixElementInterface::Matrix::plusEqual(source.sparseGetElement(i,j),target.sparseGetElement(i,j));
// 	      sourceFilledColsInRow.next(jS);
// 	      targetFilledColsInRow.next(jT);
	      
// 	    }
// 	    else{
// 	      if(jS<jT){
// 		target.sparseGetElement(i,jS)=source.sparseGetElement(i,jS);
// 		sourceFilledColsInRow.next(jS);
// 	      }
// 	      else{//jS<jT
// 		targetFilledColsInRow.next(jT);
// 	      }
// 	    }
// 	  }
// 	  sourceRows.next(iS);
// 	  targetRows.next(iT);
// 	}
// 	else{
// 	  if (iT>iS){//Source got a not empty row to be added to the target
// 	    typename FilledRows::FilledCols sourceFilledColsInRow=sourceRows.filledColsInRow(iS);
// 	    for (int jS=sourceFilledColsInRow.begin(); jS<sourceFilledColsInRow.begin(); sourceFilledColsInRow.next(jS)){
// 	      target.sparseGetElement(iS,jS)=source.sparseGetElement(iS,jS);
// 	    }
// 	    sourceRows.next(iS);
// 	  }
// 	  else{//iT<iS
// 	    targetRows.next(iT);
// 	  }
// 	}
//       }//end of while ( (iS<sourceRows.end()) ||  (iT<targetRows.end()) )
//     }
  };
}

#endif
  
  
  
  

  
  
