#ifndef __LEGOLAS_DISPLAYLATEX_HXX__
#define __LEGOLAS_DISPLAYLATEX_HXX__

#include "LatexMatrixStream.hxx"

namespace Legolas{

  void inline displayLatex(const AnyMatrix & anyMatrix,
			   std::string filename, int flag=0)
  {
    INFOS("filename="<<filename);
    const int MAXDISP=200; 
    
    const int frs=anyMatrix.getFlatRowSizeMethod();
    const int fcs=anyMatrix.getFlatColSizeMethod();
    
    if ( (frs>MAXDISP) || (frs>MAXDISP) ){
      INFOS("Display is skipped because the matrix is too large ("<<frs<<"x"<<fcs<<")");
    }
    else{
      Legolas::LatexMatrixStream lms(filename,flag,anyMatrix.getLevel());
      anyMatrix.iterateOverBlocksMethod(lms);
    }
  }
}

#endif //__LEGOLAS_DISPLAYLATEX_HXX__


//     typedef typename MATRIX::StoreType MatrixType;
//     const MatrixType A=matrixOperand.getCMR();


//     //    const int fcs=A.computeFlatColSize() ;
//     //    const int frs=1;
//     //    const int fcs=1;

//     if ( (frs>MAXDISP) || (frs>MAXDISP) ){
//       INFOS("Display is skipped because the matrix is too large ("<<frs<<"x"<<fcs<<")");
//     }
//     else{
//       LatexFilter filter(filename,flag,A.level);
//       BlockRecursiveTraversal::template Engine<LatexFilter>::applyBlock(A,filter);
//     }
//   }

  
//   void displayLatex(const AnyMatrix & anyMatrix,
// 		    std::string filename, int flag=0)
//   {
//     anyMatrix.displayLatexMethod(filename,flag);
//   }



// //   template <class MATRIX>
// //   void inline dumpLatex(const BaseMatrix<MATRIX> & matrixOperand,
// // 			std::string filename, int flag=0)
// //   {
// //     const MatrixType A=matrixOperand.getCMR();
    

// //     INFOS("filename="<<filename);

// //     typedef typename MATRIX::StoreType MatrixType;


// //     const int MAXDISP=200; 
    
    

// //     const int frs=Legolas::getFlatSize(A.getRowShape());
// //     const int fcs=Legolas::getFlatSize(A.getColShape());
// //     //    const int fcs=A.computeFlatColSize() ;
// //     //    const int frs=1;
// //     //    const int fcs=1;

// //     if ( (frs>MAXDISP) || (frs>MAXDISP) ){
// //       INFOS("Display is skipped because the matrix is too large ("<<frs<<"x"<<fcs<<")");
// //     }
// //     else{
// //       LatexFilter filter(filename,flag,A.level);
// //       BlockRecursiveTraversal::template Engine<LatexFilter>::applyBlock(A,filter);
// //     }
// //   }

// }//end namespace legolas


  
























//   template <class REAL_TYPE>
//   void realDisplayLatex(const REAL_TYPE & value, std::ofstream & this->outfile(), int flag=0){
//     if (flag==0){
//       if (std::abs(value)>1.e-15){
// 	this->outfile() << "\\matbox{"<<value<<"}" ;
// 	//this->outfile() << "\\myframebox{"<<value<<"}" ;
//       }
//       else{
// 	this->outfile() << "\\matbox{ $\\circ$ }" ;
//       }
//     }
    
//     if (flag==1){
//       if (std::abs(value)>1.e-15){
// 	this->outfile() << "\\blackbox" ;
//       }
//       else{
// 	this->outfile() << "\\whitebox" ;
//       }
//     }

//     if (flag==2){
//       if (std::abs(value)>1.e-15){
// 	this->outfile() << "\\framedblackbox" ;
//       }
//       else{
// 	this->outfile() << "\\whitebox" ;
// 	//	this->outfile() << "\\matbox{ $\\bullet$ }" ;
//       }
//     }
    
//   }

  
//   inline void indexDisplayLatex(const int value, std::ofstream & this->outfile(), int flag=0){
//     if (flag==0){
//       this->outfile() << "\\matbox{"<<value<<"}" ;
//     }
//   }

//   inline void recursiveDisplayLatex(const double & value, std::ofstream & outfile, int flag=0){ 
//     realDisplayLatex(value,outfile,flag);
//   }

//   inline void recursiveDisplayLatex(const float & value, std::ofstream & outfile, int flag=0){
//     realDisplayLatex(value,outfile,flag);
//   }

//   inline void sparseRecursiveDisplayLatex(const double & value, std::ofstream & outfile, int flag=0){ 
//     realDisplayLatex(value,outfile,flag);
//   }

//   inline void sparseRecursiveDisplayLatex(const float & value, std::ofstream & outfile, int flag=0){
//     realDisplayLatex(value,outfile,flag);
//   }


//   template <class MATRIX>
//   void recursiveDisplayLatex(const BaseMatrix<MATRIX> & matrixOperand,
// 			     std::ofstream & outfile, int flag=0)
//   {
//     typedef typename MATRIX::StoreType MatrixType;
//     MatrixType m=matrixOperand.getCMR();

//     outfile << "\\framebox{" ;

//     //    outfile << "\\begin{math}" << std::endl;    
//     //    outfile << "\\left (" << std::endl;

//     outfile << "\\begin{tabular}{" ;
//     for (int j=0 ; j < m.ncols_() ; j++){
//       outfile << "c" ;
//       //      if (j!=m.ncols_()-1) outfile << "@{\\hspace{0.1em}}" ;
//     }
//     outfile << "}" << std::endl;    
    
//     for (int i=0 ; i < m.nrows_() ; i++){
//       for (int j=0 ; j < m.ncols_() ; j++){
// 	recursiveDisplayLatex(m(i,j),outfile,flag);
// 	if (j!=m.ncols_()-1){
// 	  outfile << "&" ;  
// 	}
// 	else{
// 	  if (i!=m.nrows_()){
// 	    outfile << "\\\\" ;  
// 	  }
// 	  outfile << std::endl;
// 	  //	  outfile << "\\hline" << std::endl;
// 	}
//       }
      
//     }

//     outfile << "\\end{tabular}" << std::endl;

//     outfile << "}" << std::endl;

//     //    outfile << "\\vspace{0.1em}" << std::endl;



//     //    outfile << "}" << std::endl;
//   };




//   template <class MATRIX>
//   void sparseRecursiveDisplayLatex(const BaseMatrix<MATRIX> & matrixOperand,
// 				   std::ofstream & outfile, int flag=0)
//   {
//     typedef typename MATRIX::StoreType MatrixType;
//     MatrixType m=matrixOperand.getCMR();

//     m.sparseRecursiveDisplayLatexMethod(outfile,flag);

//   }

  
//   template <class MATRIX>
//   void sparseRecursiveDisplayHTML(const BaseMatrix<MATRIX> & matrixOperand,
// 				  const std::string & matrixName, 
// 				  std::ofstream & outfile,
// 				  const std::string * parentNamePtr=0,
// 				  int flag=0)
//   {
//     typedef typename MATRIX::StoreType MatrixType;
//     MatrixType m=matrixOperand.getCMR();
    
//     m.sparseRecursiveDisplayHTMLMethod(matrixName,outfile,parentNamePtr,flag);
    
//   }


//   inline void sparseRecursiveDisplayHTML(const double & value, const std::string & matrixName,
// 					 std::ofstream & outfile, const std::string * parentNamePtr=0,int flag=0){
//     outfile << value;
//   }

//   inline void sparseRecursiveDisplayHTML(const float & value, const std::string & matrixName,
// 					 std::ofstream & outfile, const std::string * parentNamePtr=0,int flag=0){
//     outfile << value;
//   }


//   template <class MATRIX>
//   void displayLatexImplementation(const MATRIX & aMatrix,  std::string filename, int flag=0){

//     std::ofstream outfile (filename.c_str(),std::ios::out) ;
    
//     outfile << "\\documentclass{article}" << std::endl;
//     outfile << "\\usepackage{epsfig,color}" << std::endl;
//     outfile << "\\usepackage{amsmath,amssymb}" << std::endl;
//     outfile << "\\usepackage{amsfonts}" << std::endl;
//     outfile << "\\usepackage{latexsym}" << std::endl;
//     outfile << "\\usepackage{fancybox}" << std::endl;
    
//     outfile << "\\setlength{\\paperwidth}{12000pt}" << std::endl;
//     outfile << "\\setlength{\\textwidth}{12000pt}" << std::endl;
//     outfile << "\\setlength{\\paperheight}{12000pt}" << std::endl;
//     outfile << "\\setlength{\\textheight}{12000pt}" << std::endl;
    
//     outfile << "\\setlength{\\fboxrule}{3pt}" << std::endl;
//     outfile << "\\setlength{\\fboxsep}{0pt}" << std::endl;
    
//     //    outfile << "\\newcommand{\\matbox}[1]{\\ensuremath{\\makebox["<<bw<<"]{\\rule["<<hbw<<"]{0em}{"<<bw<<"}#1}}}" << std::endl;
//     outfile << "\\newcommand{\\matbox}[1]{\\ensuremath{\\makebox[1em]{\\rule[-0."<<bw<<"]{0em}{1em}#1}}}" << std::endl;
    
//     outfile << "\\definecolor{light}{gray}{.75}" << std::endl;
//     outfile << "\\definecolor{white}{gray}{.95}" << std::endl;
    
//     outfile << "\\newcommand{\\myframebox}[1]{\\setlength{\\fboxrule}{2pt}\\setlength{\\fboxsep}{-\\fboxrule}\\fbox{#1}}" << std::endl;
//     outfile << "\\newcommand{\\mytabular}[1]{\\setlength{\\tabcolsep}{0cm}\\begin{tabular}{#1}\\end{tabular}}" << std::endl;
    
//     outfile << "\\newcommand{\\blackbox}[0]{\\ensuremath{ \\makebox[4em]{\\rule[-2em]{4em}{4em}}}}" << std::endl;
//     //    outfile << "\\newcommand{\\whitebox}[0]{\\ensuremath{ \\makebox[4em]{\\rule[-2em]{0em}{4em}}}}" << std::endl;
//     outfile << "\\newcommand{\\whitebox}[0]{\\ensuremath{ \\makebox["<<bw<<"]{\\rule["<<hbw<<"]{0em}{"<<bw<<"}}}}" << std::endl;
//     outfile << "\\newcommand{\\framedblackbox}[0]{\\ensuremath{ \\makebox[4em]{\\rule[-2em]{0em}{4em} \\rule[-1."<<bw<<"]{3em}{3em}}}}" << std::endl;
    
//     outfile << "\\begin{document}" << std::endl;
//     outfile << "\\pagestyle{empty}"<< std::endl;
    
//     outfile.precision(1);
//     outfile << std::scientific ;

//     INFOS("ICI");

//     aMatrix.sparseRecursiveDisplayLatexMethod(outfile,flag);
    
//     outfile << "\\end{document}"<< std::endl;
//     outfile.close();

//   }

//   template <class MATRIX>
//   void displayHTMLImplementation(const MATRIX & aMatrix,  const std::string & matrixName, int flag=0){
    
//     std::string fileName(matrixName);
//     fileName+=".html";
//     std::ofstream outfile (fileName.c_str(),std::ios::out) ;
    
//     aMatrix.sparseRecursiveDisplayHTMLMethod(matrixName,outfile,0,flag);
    
//     outfile.close();
//   }


//   // LP : Passe prioritairement dans cette fonction qui ne contient pas
//   // de fonction virtuelle. 
//   template <class MATRIX>
//   void inline displayLatex(const BaseMatrix<MATRIX> & matrixOperand,
// 			   std::string filename, int flag=0)
//   {
//     const int MAXDISP=200;

//     const int frs=matrixOperand.getCMR().computeFlatRowSize() ;
//     const int fcs=matrixOperand.getCMR().computeFlatColSize() ;

//     if ( (frs>MAXDISP) || (frs>MAXDISP) ){
//       INFOS("Display is skipped because the matrix is too large ("<<frs<<"x"<<fcs<<")");
//     }
//     else{
//       typedef typename MATRIX::StoreType MatrixType;
//       const MatrixType A=matrixOperand.getCMR();
//       displayLatexImplementation(A,filename,flag);
//     }
//   }

//   // LP : Passe prioritairement dans cette fonction qui ne contient pas
//   // de fonction virtuelle. 
//   template <class MATRIX>
//   void inline displayHTML(const BaseMatrix<MATRIX> & matrixOperand,
// 			  std::string filename, int flag=0)
//   {
//     typedef typename MATRIX::StoreType MatrixType;
//     const MatrixType A=matrixOperand.getCMR();

//     const long long  MAXDISP=1000000;
//     const long long frs=matrixOperand.getCMR().computeFlatRowSize() ;
//     const long long fcs=matrixOperand.getCMR().computeFlatColSize() ;

//     INFOS ("frs="<<frs);
//     INFOS ("fcs="<<fcs);
//     INFOS ("frs*fcs="<<frs*fcs);

//     if ( (frs*fcs>MAXDISP)){
//       INFOS("HTML Display is skipped because the matrix is too large ("<<frs<<"x"<<fcs<<")");
//     }
//     else{
//       displayHTMLImplementation(A,filename,flag);
//     }
//   }


//   // LP : Passe par cette fonction qui contient des fonctions virtuelles
//   // si la fonction précédente ne correspond pas. C'est seulement le cas
//   // si on travaille sur des pointeurs de AnyMatrix.
//   inline void displayLatex(const AnyMatrix & anyMatrix,  std::string filename, int flag=0){
//     //    INFOS("LA (displayLatex(...))");
//     displayLatexImplementation(anyMatrix,filename,flag);
//   }

//   // LP : Passe par cette fonction qui contient des fonctions virtuelles
//   // si la fonction précédente ne correspond pas. C'est seulement le cas
//   // si on travaille sur des pointeurs de AnyMatrix.
//   inline void displayHTML(const AnyMatrix & anyMatrix,  std::string filename, int flag=0){
//     displayHTMLImplementation(anyMatrix,filename,flag);
//   }
  

//   class FlatElement{
//   public:
//     FlatElement(const int I, const int J, const double & value):I_(I),J_(J),value_(value){}
//     const int I_;
//     const int J_;
//     const double value_;
//   };
  

//   template <class STREAM>
//   static inline void storeElement(const FlatElement & e, STREAM & outStream){
//     outStream << e.I_ << " " << e.J_ << " " <<e.value_ << std::endl;
//   }
  
//   class FileStreamElementFilter{
//   public:

//     FileStreamElementFilter(std::string filename):outfile(filename.c_str(),std::ios::out){}
    
//     void store(const FlatElement & e){
//       storeElement(e,outfile);
//     }
    
//     std::ofstream outfile;
//   };

//   class ScilabStreamElementFilter : public FileStreamElementFilter{
//   public:

//     template <class MATRIX>
//     ScilabStreamElementFilter(const BaseMatrix<MATRIX> & matrixOperand,
// 			      const std::string & matrixName):FileStreamElementFilter(matrixName+".sce"),
// 							      matrixName_(matrixName)
      
//     {
//       outfile << matrixName_<<"=0*eye(" ;
//       //      outfile <<Legolas::computeFlatRowSize(matrixOperand) ;
//       outfile <<matrixOperand.getCMR().computeFlatRowSize() ;
//       outfile <<",";
//       //      outfile <<Legolas::computeFlatColSize(matrixOperand) ;
//       outfile <<matrixOperand.getCMR().computeFlatColSize() ;
//       outfile << ");" << std::endl;
//     }
    
//     void store(const FlatElement & e){
//       outfile << matrixName_ << "(" << e.I_+1 << "," << e.J_+1 << ")=" <<e.value_ << ";"<< std::endl;
//     }
    
//   private:
//     std::string matrixName_;
//   };
    
  
//   template <bool IS_MATRIX_OF_MATRICES>
//   struct MatrixOfMatricesTraits{
//     template <class MATRIX, class FILTER>
//     static void recursiveGetFlatMatrixElements(const BaseMatrix<MATRIX> & matrixOperand, 
// 					       FILTER & filter, 
// 					       int I, int J){

//       typedef typename MATRIX::StoreType MatrixType;
//       const MatrixType A=matrixOperand.getCMR();
      
//       typename MATRIX::FilledRows rows(A);
      
//       typedef typename MATRIX::ConstGetElement CGE;
//       typedef MatrixOfMatricesTraits<CGE::matrixOfMatrices> ElementMMT;

//       Vector<int> rowFlatIndexes;
//       Legolas::computeFlatIndexes(A.getRowShape(),I,rowFlatIndexes);
//       Vector<int> colFlatIndexes(A.ncols_());
//       Legolas::computeFlatIndexes(A.getColShape(),I,colFlatIndexes);
      
//       for (int i=rows.begin(); i< rows.end() ; rows.next(i)){
// 	typename MATRIX::FilledRows::FilledCols cols=rows.filledColsInRow(i);
	
// 	for (int j=cols.begin(); j< cols.end() ; cols.next(j)){

// 	  const int rowFlatIndex=rowFlatIndexes[i];
// 	  const int colFlatIndex=colFlatIndexes[j];
	  
// 	  ElementMMT::recursiveGetFlatMatrixElements(A.sparseGetElement(i,j),filter,rowFlatIndex,colFlatIndex);
// 	}
//       }
//     }
//   };
  

//   template <>
//   struct MatrixOfMatricesTraits<false>{
    
//     template <class MATRIX, class FILTER>
//     static void recursiveGetFlatMatrixElements(const BaseMatrix<MATRIX> & matrixOperand, 
// 					       FILTER & filter, 
// 					       int I, int J){

//       typedef typename MATRIX::StoreType MatrixType;
//       const MatrixType A=matrixOperand.getCMR();
      
//       typename MATRIX::FilledRows rows(A);
      
//       for (int i=rows.begin(); i< rows.end() ; rows.next(i)){
	
// 	typename MATRIX::FilledRows::FilledCols cols=rows.filledColsInRow(i);
	
// 	for (int j=cols.begin(); j< cols.end() ; cols.next(j)){
// 	  FlatElement fe(i+I,j+J,A.sparseGetElement(i,j));
// 	  filter.store(fe);
// 	}
//       }
//     }

//   };


//   template <class MATRIX, class FILTER>
//   void getFlatMatrixElements(const BaseMatrix<MATRIX> & matrixOperand, FILTER & filter){
//     int I=0;
//     int J=0;
    
//     typedef MatrixOfMatricesTraits<MATRIX::matrixOfMatrices> MMT;
//     return MMT::recursiveGetFlatMatrixElements(matrixOperand,filter,I,J);    
//   }


//   template <class MATRIX>
//   void dumpFlatMatrix(const BaseMatrix<MATRIX> & matrixOperand,const std::string filename){
//     FileStreamElementFilter filter(filename);
//     return getFlatMatrixElements(matrixOperand,filter);    
//   }

//   class StdOutElementFilter{
//   public:
//     void store(const FlatElement & e){
//       storeElement(e,std::cout);
//     }
//   };

//   template <class MATRIX>
//   void displayFlatMatrix(const BaseMatrix<MATRIX> & matrixOperand){

//     StdOutElementFilter outFilter;
//     return getFlatMatrixElements(matrixOperand,outFilter);    
//   }

//   template <class MATRIX>
//   void dumpScilabFlatMatrix(const BaseMatrix<MATRIX> & matrixOperand,const std::string matrixName){
//     ScilabStreamElementFilter filter(matrixOperand,matrixName);
//     return getFlatMatrixElements(matrixOperand,filter);    
//   }


//   template <class ACTUAL_MATRIX>
//   struct ActualMatrixFilter{
//     ActualMatrixFilter(ACTUAL_MATRIX & am):am_(am){}
    
//     void store(const FlatElement & e){
//       am_.sparseGetElement(e.I_,e.J_)=e.value_;
//       //      am_(e.I_,e.J_)=e.value_;
//     }
//     ACTUAL_MATRIX & am_ ;
//   };

//   template <class MATRIX>
//   void displayLatexFlatMatrix(const BaseMatrix<MATRIX> & matrixOperand,const std::string filename, int flag=0){

//     typedef typename MATRIX::StoreType MatrixType;
//     MatrixType m=matrixOperand.getCMR();

//     RealSparseMatrix<double> S(m.computeFlatRowSize(),m.computeFlatColSize());
//     ActualMatrixFilter< RealSparseMatrix<double> > amf(S);
//     getFlatMatrixElements(matrixOperand,amf);
//     displayLatex(S,filename,flag);
//   }

