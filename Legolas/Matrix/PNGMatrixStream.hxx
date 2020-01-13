#ifndef __LEGOLAS_PNGMATRIXSTREAM_HXX__
#define __LEGOLAS_PNGMATRIXSTREAM_HXX__

#include "Legolas/Matrix/MatrixStream.hxx"

#include <string>
#include <fstream>

#ifdef __USEPNGWRITER__
#include <pngwriter.h>
#else
struct pngwriter{
  pngwriter(int sizeX, int sizeY, double value, const char * name){
    INFOS("pngwriter is not used (recompile with -D__USEPNGWRITER__");
//     INFOS("sizeX="<<sizeX);
//     INFOS("sizeY="<<sizeY);
//     INFOS("value="<<value);
//     INFOS("name="<<name);
  }

  void plot(int i, int j, double red, double green, double blue){
 //    std::cout<<"i="<<i<<" j="<<j<<" red ="<<red<<" green="<<green<<" blue="<<blue<<std::endl;
  }

  void square(int xb,int yb,int xe, int ye, double red, double green, double blue){
  //   std::cout<<"square xb="<<xb<<" yb="<<yb<<" xe="<<xe<<" ye="<<ye<< " red ="<<red<<" green="<<green<<" blue="<<blue<<std::endl;
  }
  void filledsquare_blend(int xb,int yb,int xe, int ye, double opacity, double red, double green, double blue){
 //    std::cout<<"filledsquare_blend xb="<<xb<<" yb="<<yb<<" xe="<<xe<<" ye="<<ye<< " red ="<<red<<" green="<<green<<" blue="<<blue<<std::endl;
  }

  void filledsquare(int xb,int yb,int xe, int ye, double red, double green, double blue){
  //   std::cout<<"filledsquare xb="<<xb<<" yb="<<yb<<" xe="<<xe<<" ye="<<ye<< " red ="<<red<<" green="<<green<<" blue="<<blue<<std::endl;
  }
  
  void close( void ){ return;}
};

#endif


namespace Legolas{
  
  class PNGMatrixStream : public Legolas::MatrixStream{
  public:

    virtual std::string extension( void ) const { return ".png";}

    virtual void checkExtension(const std::string & fileName) const {
      this->checkThisExtension(fileName,this->extension());
    }
    
    PNGMatrixStream( void ):MatrixStream(),
			    pngwriterPtr_(0),
			    borderSize_(0),
			    unitCellSize_(0){}


      
    PNGMatrixStream(const std::string & filename,
		    int flag, 
		    int level,
		    int flatRowSize,
		    int flatColSize,
		    int borderSize,
		    int unitCellSize):MatrixStream(filename,flag,level,flatRowSize,flatColSize),
				      pngwriterPtr_(new ::pngwriter(unitCellSize*flatColSize,unitCellSize*flatRowSize,1.0,filename.c_str())),
				      borderSize_(borderSize),
				      unitCellSize_(unitCellSize)
    {
      this->checkExtension(filename);
    }


    PNGMatrixStream(PNGMatrixStream & parent,
		    int cornerFlatRowIndex,
		    int cornerFlatColIndex,
		    int blockFlatRowSize,
		    int blockFlatColSize):MatrixStream(parent,cornerFlatRowIndex,cornerFlatColIndex,blockFlatRowSize,blockFlatColSize),
					  pngwriterPtr_(parent.pngwriterPtr_),
					  borderSize_(parent.borderSize_),
					  unitCellSize_(parent.unitCellSize_){}
    

    
    PNGMatrixStream * child(int i, int j, 
			    int cornerFlatRowIndex,
			    int cornerFlatColIndex,
			    int blockFlatRowSize,
			    int blockFlatColSize){
      PNGMatrixStream * result= new PNGMatrixStream(*this,cornerFlatRowIndex,cornerFlatColIndex,blockFlatRowSize,blockFlatColSize);
      return result;
    }
    
    
    //    std::ofstream & pngwriter( void ) { return *pngwriterPtr_;}

    ~PNGMatrixStream( void ){
      if (this->isRoot()){
	if (pngwriterPtr_){
	  (*pngwriterPtr_).close();
	  delete pngwriterPtr_;
	  pngwriterPtr_=0;
	}
      }
    }


    inline void blockInitialize(int i, int j){
      int xFrom=unitCellSize_*this->cornerFlatColIndex();
      int xTo=unitCellSize_*(this->cornerFlatColIndex()+this->blockFlatColSize());
      
      int swapYFrom=this->cornerFlatRowIndex();
      int swapYTo=this->cornerFlatRowIndex()+this->blockFlatRowSize();
      
      int yFrom=unitCellSize_*(this->flatRowSize()-swapYFrom);
      int yTo=unitCellSize_*(this->flatRowSize()-swapYTo);
      
      if (this->flag()==1){
	(*pngwriterPtr_).square(xFrom,yFrom,xTo,yTo,0.0,0.0,1.0);

      }

      if (this->flag()==2){
	//double opacity=0.3*std::max(0.0,1.0-double(this->level()-1.0)/8.0);
	//	double opacity=0.05;
	double opacity=0.15;
	(*pngwriterPtr_).square(xFrom,yFrom,xTo,yTo,0.0,0.0,0.0);
	(*pngwriterPtr_).filledsquare_blend(xFrom,yFrom,xTo,yTo,opacity,0.1,0.1,0.1);


      }
      
    }      
    
    template <class REAL_TYPE>
    inline void storeElement(const REAL_TYPE & value){

      int xFrom=unitCellSize_*this->cornerFlatColIndex();
      int xTo=unitCellSize_*(this->cornerFlatColIndex()+this->blockFlatColSize());
      
      int swapYFrom=this->cornerFlatRowIndex();
      int swapYTo=this->cornerFlatRowIndex()+this->blockFlatRowSize();
      
      int yFrom=unitCellSize_*(this->flatRowSize()-swapYFrom);
      int yTo=unitCellSize_*(this->flatRowSize()-swapYTo);
      
      (*pngwriterPtr_).square(xFrom,yFrom,xTo,yTo,0.0,0.0,0.0);
      
    
      //Add Border

       xFrom+=1;
       yFrom-=1;
       xTo-=1;
       yTo+=1;


       (*pngwriterPtr_).filledsquare(xFrom,yFrom,xTo,yTo,1.0,0.0,0.0);


    }

    void dumpElement(int i, int j,const float & element){ this->storeElement(element);}
    void dumpElement(int i, int j,const double & element){ this->storeElement(element);}



private:
    
    ::pngwriter * pngwriterPtr_;
    int borderSize_;
    int unitCellSize_;
    
  };
  

}

#endif
