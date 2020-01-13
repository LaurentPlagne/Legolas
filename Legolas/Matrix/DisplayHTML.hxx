/**
 * project  DESCARTES
 *
 * @file     DisplayHTML.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DISPLAYHTML_HXX__
#define __LEGOLAS_DISPLAYHTML_HXX__

#include "Legolas/Vector/BaseVector.hxx"
#include "Legolas/Vector/BaseMatrix.hxx"
#include "Legolas/Vector/Vector.hxx"
#include "Legolas/Vector/Expression/MatrixVectorExpression.hxx"

#include "Legolas/Matrix/BlockRecursiveTraversal.hxx"

#include <string>
#include <fstream>

//#include <pngwriter.h>

namespace Legolas{
  
  class HTMLFilter{
  public:    

    static std::string getMatrixName(const std::string & fileName){
      const int found=fileName.find(".html");
      const int extensionSize=5;// sizeof ".html"
      const int expectedValue=fileName.size()-extensionSize;
      if ((found!=expectedValue) || (found<0)){ //not found..
	INFOS("error!! fileName="<<fileName);
	INFOS("error!! fileName must have the .html extension !!!");
	throw std::runtime_error("error!! fileName must have the .html extension !!!");
      }
      return std::string(fileName.begin(),fileName.end()-extensionSize);
    }
    
    static std::string getParentName(const std::string & matrixName){
      return std::string(matrixName.begin(),matrixName.begin()+matrixName.rfind("("));
    }


    
    HTMLFilter(const std::string & fileName,int flag, int level, bool isRoot=true):fileName_(fileName),
										   outfilePtr_(new std::ofstream(fileName.c_str(),std::ios::out)),
										   flag_(flag),
										   level_(level),
										   nrows_(0),
										   ncols_(0),
										   isRoot_(true),
										   matrixName_(getMatrixName(fileName))
    {
      INFOS("matrixName="<<matrixName_);

      this->outfile() << "<html>" << std::endl;
      this->outfile() << "<head>" << std::endl;
      this->outfile() << "<title>"<< matrixName_ << "</title>" << std::endl;
      this->outfile() << "<META HTTP-EQUIV=\"CACHE-CONTROL\" CONTENT=\"NO-CACHE\">" << std::endl;
      this->outfile() << "</head>" << std::endl;
      this->outfile() << "<body>" << std::endl;
    }

    std::ofstream & outfile( void ) { return *outfilePtr_;}

    std::string makeChildName(const std::string & parentName, int i, int j){
      std::string result(parentName);
      result+="(";
      result+=stringConvert(i);
      result+=",";
      result+=stringConvert(j);
      result+=")";
      return result;
    }
    

  private:
    HTMLFilter(HTMLFilter & parent, int i, int j):fileName_(),
						  outfilePtr_(0),
						  flag_(parent.flag_),
						  level_(parent.level_-1),
						  nrows_(0),
						  ncols_(0),
						  isRoot_(false){
      if (level_!=0){
	matrixName_=makeChildName(parent.matrixName_,i,j);
	
	fileName_=matrixName_;
	fileName_+=".html";
	outfilePtr_=new std::ofstream(fileName_.c_str(),std::ios::out);
      }
      else{
	outfilePtr_=parent.outfilePtr_;
      }
    }
  public:
    
    HTMLFilter child(int i, int j, int d1, int d2, int d3, int d4){ 
      HTMLFilter result(*this,i,j);
      return result;
    }
    

    ~HTMLFilter( void ){
      if (level_!=0){
	this->outfile() << "</body>"  << std::endl;
	this->outfile() << "</html>"  << std::endl;
	this->outfile().close(); ;
	delete outfilePtr_;
      }
    }

    inline void blockInitialize(int nrows, int ncols){
      nrows_=nrows;
      ncols_=ncols;

      //On the first line of the array : a single column 
      this->outfile() << "<TABLE BORDER CELLPADDING=2>" << std::endl;
      this->outfile() << "<TR>" << std::endl;
      this->outfile() << "<TH ALIGN=CENTER, COLSPAN=" << ncols_+1 << ">" ;
      //Let us assume that the MAT(i,j)(k,l) is beeing displayed : 
      //One displays the name of the parent matrix (e.g. MAT(i,j)) which is a link to the corresponding
      // html file (e.g. MAT(i,j).html) followed by the current indexSet (e.g. (k,l)) :
      if (!isRoot_){
	const std::string parentName=getParentName(matrixName_);
	std::string indexName(matrixName_.begin()+parentName.size(),matrixName_.end());
	this->outfile() << "<a href="<<parentName<<".html target=blank>"<< parentName << "  </a> " << indexName ;
      }
      else{ // Root matrix
	this->outfile() << matrixName_ << std::endl;
      }
      this->outfile() << "</TH>" << std::endl;
      this->outfile() << "</TR>" << std::endl;
      
      this->outfile() << "<TR>" << std::endl;
      this->outfile() << "<TH ALIGN=CENTER>   </TH>"  << std::endl;
      // Display the column indexes
      for (int j=0 ; j < ncols_ ; j++)
	this->outfile() << "<TH ALIGN=CENTER> "<< j << "</TH>"  << std::endl;
    }


    inline void blockFinalize(){
      this->outfile() << " </TABLE>" << std::endl;
    }      

    inline void createEmptyBlock(int i, int j){
    }

    inline void createNonEmptyBlock(int i, int j){
      if (level_!=1){
	std::string childName=makeChildName(matrixName_,i,j);
	this->outfile()<< "<a href="<<childName<<".html target=blank>"<< childName << "  </a> ";		
      }      
    }
    
    inline void rowInitialize(int rowIndex){
      this->outfile()<< "<TR>" << std::endl;
      this->outfile()<< "<TH ALIGN=CENTER> "<< rowIndex << "</TH>"  ;
    }

    inline void rowFinalize(int rowIndex){
      this->outfile()<< "</TR>" << std::endl;
    }

    inline void colInRowInitialize(int i, int j){
      this->outfile()<< "<TH ALIGN=CENTER> " ;
    }
    
    inline void colInRowFinalize(int i, int j){
      this->outfile()<< "</TH> " ;
    }
    

    template <class REAL_TYPE>
    inline void storeElement(const REAL_TYPE & value){
      this->outfile() << value;
    }

    int cornerFlatRowIndex( void ){ return 0;}
    int cornerFlatColIndex( void ){ return 0;}
    
  private:
    std::string fileName_;
    std::ofstream * outfilePtr_;
    const int flag_;
    const int level_;
    int nrows_;
    int ncols_;
    const bool isRoot_;
    std::string matrixName_;
  };



  template <class MATRIX>
  void inline displayHTML(const BaseMatrix<MATRIX> & matrixOperand,
			   std::string fileName, int flag=0)
  {

    INFOS("fileName="<<fileName);

    typedef typename MATRIX::StoreType MatrixType;
    const MatrixType A=matrixOperand.getCMR();

    const int MAXDISP=200; 

    //    const int frs=A.computeFlatRowSize() ;
    //    const int fcs=A.computeFlatColSize() ;
    const int frs=1;
    const int fcs=1;

    if ( (frs>MAXDISP) || (frs>MAXDISP) ){
      INFOS("Display is skipped because the matrix is too large ("<<frs<<"x"<<fcs<<")");
    }
    else{
      HTMLFilter filter(fileName,flag,A.level);
      BlockRecursiveTraversal::template Engine<HTMLFilter>::applyBlock(A,filter);
      //      BlockRecursiveTraversal<HTMLFilter>::apply(A,filter,A.Level);
    }
  }

}//end namespace legolas

#endif //__LEGOLAS_DISPLAYHTML_HXX__
  

