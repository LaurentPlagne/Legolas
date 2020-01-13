#ifndef __LEGOLAS_LATEXMATRIXSTREAM_HXX__
#define __LEGOLAS_LATEXMATRIXSTREAM_HXX__

#include "Legolas/Matrix/MatrixStream.hxx"

#include <string>
#include <fstream>

namespace Legolas{
  
  class LatexMatrixStream : public Legolas::MatrixStream{
  public:

    virtual std::string extension( void ) const {
      return ".tex";
    }

    virtual void checkExtension(const std::string & fileName) const {
      this->checkThisExtension(fileName,this->extension());
    }
    
    LatexMatrixStream( void ):MatrixStream(),
			      outfilePtr_(0),
			      withNumber_(true){};

      
    LatexMatrixStream(const std::string & filename,
		      int flag, 
		      int level):MatrixStream(filename,flag,level,0,0),
				 outfilePtr_(new std::ofstream(filename.c_str(),std::ios::out)),
				 withNumber_(!(flag==2))
    {
      this->checkExtension(filename);
      const std::string boxLineWidth="4pt";

      //      std::string boxLineWidth=stringConvert(level_);
      //      boxLineWidth+="pt";


      //For value display mode
      const std::string bw="5em";
      const std::string hbw="-2.5em";

      //For black square display mode
      // white square surrounding..
      const std::string bbw="1.8em";
      const std::string hbbw="0.9em";      
      //..a black square
      const std::string bbwp="1.2em";
      const std::string hbbwp="0.6em";

      this->outfile() << "\\documentclass{article}" << std::endl;
      this->outfile() << "\\usepackage{epsfig,color}" << std::endl;
      this->outfile() << "\\usepackage{amsmath,amssymb}" << std::endl;
      this->outfile() << "\\usepackage{amsfonts}" << std::endl;
      this->outfile() << "\\usepackage{latexsym}" << std::endl;
      this->outfile() << "\\usepackage{fancybox}" << std::endl;
      this->outfile() << "\\usepackage[table]{xcolor}" << std::endl;
      
      this->outfile() << "\\setlength{\\paperwidth}{12000pt}" << std::endl;
      this->outfile() << "\\setlength{\\textwidth}{12000pt}" << std::endl;
      this->outfile() << "\\setlength{\\paperheight}{12000pt}" << std::endl;
      this->outfile() << "\\setlength{\\textheight}{12000pt}" << std::endl;
      
      this->outfile() << "\\setlength{\\fboxrule}{1pt}" << std::endl;
      this->outfile() << "\\setlength{\\fboxsep}{0pt}" << std::endl;
      
      this->outfile() << "\\newcommand{\\matbox}[1]{\\ensuremath{\\makebox["<<bw<<"]{\\rule["<<hbw<<"]{0em}{"<<bw<<"}#1}}}" << std::endl;
      this->outfile() << "\\newcommand{\\cmatbox}[1]{\\ensuremath{\\makebox["<<bw<<"]{\\rule["<<hbw<<"]{0em}{"<<bw<<"}#1}}}" << std::endl;
      //     this->outfile() << "\\newcommand{\\matbox}[1]{\\ensuremath{\\makebox[1em]{\\rule[-0."<<bw<<"]{0em}{1em}#1}}}" << std::endl;
      
      this->outfile() << "\\definecolor{light}{gray}{.75}" << std::endl;
      this->outfile() << "\\definecolor{white}{gray}{.95}" << std::endl;
      
      //
      //      this->outfile() << "\\newcommand{\\myframebox}[1]{\\setlength{\\fboxrule}{"<<boxLineWidth<<"}\\setlength{\\fboxsep}{-\\fboxrule}\\fbox{#1}}" << std::endl;
      this->outfile() << "\\newcommand{\\myframebox}[1]{\\setlength{\\fboxrule}{"<<boxLineWidth<<"}\\setlength{\\fboxsep}{-\\fboxrule}\\fcolorbox{blue}{white}{#1}}" << std::endl;
      this->outfile() << "\\newcommand{\\myAltframebox}[1]{\\setlength{\\fboxrule}{"<<boxLineWidth<<"}\\setlength{\\fboxsep}{-\\fboxrule}\\fcolorbox{red}{white}{#1}}" << std::endl;
      this->outfile() << "\\newcommand{\\mytabular}[1]{\\setlength{\\tabcolsep}{0cm}\\begin{tabular}{#1}\\end{tabular}}" << std::endl;
      
      this->outfile() << "\\newcommand{\\blackbox}[0]{\\ensuremath{ \\makebox["<<bbw<<"]{\\rule[-"<<hbbw<<"]{"<<bbw<<"}{"<<bbw<<"}}}}" << std::endl;
      //    this->outfile() << "\\newcommand{\\whitebox}[0]{\\ensuremath{ \\makebox["<<bbw<<"]{\\rule[-"<<hbbw<<"]{0em}{"<<bbw<<"}}}}" << std::endl;
      this->outfile() << "\\newcommand{\\whitebox}[0]{\\ensuremath{ \\makebox["<<bbw<<"]{\\rule[-"<<hbbw<<"]{0em}{"<<bbw<<"}}}}" << std::endl;
      this->outfile() << "\\newcommand{\\framedblackbox}[0]{\\ensuremath{ \\makebox["<<bbw<<"]{\\rule[-"<<hbbw<<"]{0em}{"<<bbw<<"} \\rule[-"<<hbbwp<<"]{"<<bbwp<<"}{"<<bbwp<<"}}}}" << std::endl;
      
      this->outfile() << "\\begin{document}" << std::endl;
      this->outfile() << "\\pagestyle{empty}"<< std::endl;
      
      this->outfile().precision(1);
      this->outfile() << std::scientific ;

    }

    LatexMatrixStream(LatexMatrixStream & parent, bool child):MatrixStream(parent,0,0,0,0),
							      outfilePtr_(parent.outfilePtr_),
							      withNumber_(parent.withNumber_){
    }

    LatexMatrixStream * child(int i, int j, int d1, int d2, int d3, int d4){ 
      LatexMatrixStream * result= new LatexMatrixStream(*this,true);
      return result;
    }
    
    std::ofstream & outfile( void ) { return *outfilePtr_;}

    ~LatexMatrixStream( void ){
      if (this->isRoot()){
	this->outfile() << "\\end{document}"<< std::endl;
	this->outfile().close(); ;
	delete outfilePtr_;
      }
    }


    inline void createEmptyBlock(int i, int j ){
      this->storeElement(0.0);
    }

    inline void createNonEmptyBlock(int i, int j){
      //      this->storeElement(0.0);
    }

    void dumpElement(int i, int j, const float & element){ this->storeElement(element);}
    void dumpElement(int i, int j, const double & element){ this->storeElement(element);}


    template <class REAL_TYPE>
    inline void storeElement(const REAL_TYPE & value){
      if (this->flag()==0){
	if (std::abs(value)>1.e-15){
	  this->outfile() << "\\matbox{"<<value<<"}" ;
	  //this->outfile() << "\\myframebox{"<<value<<"}" ;
	}
	else{
	  this->outfile() << "\\matbox{ $\\circ$ }" ;
	}
      }
      
      if (this->flag()==1){
	if (std::abs(value)>1.e-15){
	  this->outfile() << "\\blackbox" ;
	}
	else{
	  this->outfile() << "\\whitebox" ;
	}
      }
      
      if (this->flag()==2){
	if (std::abs(value)>1.e-15){
	  this->outfile() << "\\framedblackbox" ;
	}
	else{
	  this->outfile() << "\\whitebox" ;
	}
      }
    }
    
    
    inline void blockInitialize(int nrows, int ncols){

      this->nrows()=nrows;
      this->ncols()=ncols;

      //      const bool isPair=( (this->level_%2)==0);
      const bool isPair=( ((nrows+ncols+this->level())%2)==0);

      if (isPair){
	this->outfile() << "\\myframebox{" ;
      }
      else{
	this->outfile() << "\\myAltframebox{" ;
      }
	
      this->outfile() << "\\setlength{\\tabcolsep}{0cm}" ;
      this->outfile() << "\\begin{tabular}{" ;
      
      //      INFOS("ICI");

      for (int j=0 ; j < this->ncols() ; j++){
	this->outfile() << "c" ;
      }

      if (withNumber_)  this->outfile() << "c" ;

      this->outfile() << "}"; 

      if (withNumber_){
	if (this->ncols()>0){ 
	  this->outfile() << " &" ;
	  for (int j=0 ; j < this->ncols()-1 ; j++){
	    this->outfile() << "\\cmatbox{"<<j<<"}" ;
	    this->outfile() << "&" ;
	  }
	  this->outfile() <<  "\\cmatbox{" << this->ncols()-1 << "}" << " \\\\" ;
	  this->outfile() << std::endl;
	}
      }
    }

    inline void blockFinalize( void ){
      this->outfile() << "\\end{tabular}}" << std::endl;
    }

    inline void rowInitialize(int rowIndex){
      if (withNumber_){
	this->outfile() << "\\matbox{"<<rowIndex<<"}" ;
	if (this->ncols()>0) this->outfile() << "&" ;
      }
    }

    inline void rowFinalize(int rowIndex ){
      if (rowIndex!=this->nrows()-1) this->outfile() << "\\\\" ; // not last row
      this->outfile() << std::endl ;        
    }

    inline void colInRowInitialize(int rowIndex, int colIndex){}

    inline void colInRowFinalize(int rowIndex, int colIndex){
      if (colIndex!=this->ncols()-1)  this->outfile() << "&" ; // not last col
    }
    
    int cornerFlatRowIndex( void ){ return 0;}
    int cornerFlatColIndex( void ){ return 0;}

private:
    
    std::ofstream * outfilePtr_;
    bool withNumber_;

  };
  

}

#endif
