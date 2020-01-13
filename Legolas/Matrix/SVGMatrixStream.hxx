#ifndef __LEGOLAS_SVGMATRIXSTREAM_HXX__
#define __LEGOLAS_SVGMATRIXSTREAM_HXX__

#include "Legolas/Matrix/MatrixStream.hxx"

#include <string>
#include <fstream>

//<?xml version="1.0" standalone="no"?>
//<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd"> 


namespace Legolas{
  
  class SVGMatrixStream : public Legolas::MatrixStream{
  public:

    virtual std::string extension( void ) const { return ".svg";}

    virtual void checkExtension(const std::string & fileName) const {
      this->checkThisExtension(fileName,this->extension());
    }
    
    SVGMatrixStream( void ):MatrixStream(),
			    outfilePtr_(0),
			    borderSize_(0),
			    unitCellSize_(0){}


      
    SVGMatrixStream(const std::string & filename,
		    int flag, 
		    int level,
		    int flatRowSize,
		    int flatColSize,
		    int borderSize,
		    int unitCellSize):MatrixStream(filename,flag,level,flatRowSize,flatColSize),
				      outfilePtr_(new std::ofstream(filename.c_str(),std::ios::out)),
				      borderSize_(borderSize),
				      unitCellSize_(unitCellSize)
    {

      const int maxLength=std::max(flatRowSize,flatColSize);
      const int minImageSize=800;
      int minUnitSize=4;
      
      //Draw text
      if (this->flag()!=0) minUnitSize=34;
      

      unitCellSize_=std::max(minImageSize/maxLength,minUnitSize);
      
      this->outfile().precision(1);
      this->outfile() << std::scientific ;

      this->maxLevel_=this->level();
      this->checkExtension(filename);
      this->outfile() << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;

      
      this->outfile() << "<!DOCTYPE svg PUBLIC " ;
      this->outfile() << "\"-//W3C" ;
      this->outfile() << "//DTD SVG 1.1//EN\"";
      this->outfile() << " \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl;
      //      this->outfile() << "//DTD SVG 1.1//EN" << std::endl;
      
      

      this->outfile() << " <svg viewBox = \"0 0 "<<unitCellSize_*flatColSize<<" "<<unitCellSize_*flatRowSize<<"\" version = \"1.1\">" << std::endl;
    }


    SVGMatrixStream(SVGMatrixStream & parent,
		    int cornerFlatRowIndex,
		    int cornerFlatColIndex,
		    int blockFlatRowSize,
		    int blockFlatColSize):MatrixStream(parent,cornerFlatRowIndex,cornerFlatColIndex,blockFlatRowSize,blockFlatColSize),
					  outfilePtr_(parent.outfilePtr_),
					  borderSize_(parent.borderSize_),
					  unitCellSize_(parent.unitCellSize_),
					  maxLevel_(parent.maxLevel_)
    {}
    

    
    SVGMatrixStream * child(int i, int j, 
			    int cornerFlatRowIndex,
			    int cornerFlatColIndex,
			    int blockFlatRowSize,
			    int blockFlatColSize){
      SVGMatrixStream * result= new SVGMatrixStream(*this,cornerFlatRowIndex,cornerFlatColIndex,blockFlatRowSize,blockFlatColSize);
      return result;
    }
    
    
    //    std::ofstream & svgwriter( void ) { return *svgwriterPtr_;}

    ~SVGMatrixStream( void ){
      if (this->isRoot()){
	this->outfile() << "</svg>"<< std::endl;
	this->outfile().close(); ;
	delete outfilePtr_;
	outfilePtr_=0;
      }
    }

    void beginSquare(int xFrom, int yFrom, int xTo, int yTo,int linewidth, int red=0, int green=0, int blue=0){
      this->outfile() << "<rect ";
      this->outfile() << " x= \""<<xFrom<<"\"" ;
      this->outfile() << " y= \""<<yFrom<<"\"" ;
      this->outfile() << " width= \""<<xTo-xFrom<<"\"" ;
      this->outfile() << " height= \""<<yTo-yFrom<<"\"" ;
      this->stroke(red,green,blue);
      this->outfile() << " stroke-width= \""<<linewidth<<"\"" ;
    }
    
    void noFill( void ){
      this->outfile() << " fill= \"none\"" ;
    }

    void stroke(int red, int green, int blue ){
     this->outfile() << " stroke= \"rgb(" ;
      this->outfile() << red<<",";
      this->outfile() << green<<",";
      this->outfile() << blue<<") \"";
    };
    
    void fill(int red, int green, int blue ){
     this->outfile() << " fill= \"rgb(" ;
      this->outfile() << red<<",";
      this->outfile() << green<<",";
      this->outfile() << blue<<") \"";
    };

    
    void endSquare( void ){
      this->outfile() << "/>"<< std::endl;      
    }
    
    
    void square(int xFrom, int yFrom, int xTo, int yTo,int linewidth,int red=255, int green=255, int blue=255){
      this->beginSquare(xFrom,yFrom,xTo,yTo,linewidth,red,green,blue);
      this->noFill();
      this->endSquare();
    }
    
    
    void filledSquare(int xFrom, int yFrom, int xTo, int yTo,int linewidth, int red, int green, int blue){
      this->beginSquare(xFrom,yFrom,xTo,yTo,linewidth);
      //      this->noFill();
      this->fill(red,green,blue);
      this->endSquare();
    }

    void dumpValue(int xFrom, int yFrom, int xTo, int yTo, double value){
      this->outfile() << "<text " ;
      this->outfile() << " x= \""<<xFrom<<"\"" ;
      this->outfile() << " y= \""<<(yFrom+yTo)/2<<"\"" ;
      this->fill(0,0,0);
      this->outfile() << " font-size= \"8\" >" << std::endl;
      this->outfile() << value << std::endl;
      this->outfile() << "</text>" << std::endl;
    }      

    
    inline void blockInitialize(int i, int j){
      int xFrom=unitCellSize_*this->cornerFlatColIndex();
      int xTo=unitCellSize_*(this->cornerFlatColIndex()+this->blockFlatColSize());

      int yFrom=unitCellSize_*this->cornerFlatRowIndex();
      int yTo=unitCellSize_*(this->cornerFlatRowIndex()+this->blockFlatRowSize());
      
      // int swapYFrom=this->cornerFlatRowIndex();
      // int swapYTo=this->cornerFlatRowIndex()+this->blockFlatRowSize();
      
      // int yFrom=unitCellSize_*(this->flatRowSize()-swapYFrom);
      // int yTo=unitCellSize_*(this->flatRowSize()-swapYTo);


      //      INFOS("this->level()="<<this->level());
      //      double opacity=std::min(1.0,double(this->level())/10.0);

      double minTransparency=0.5;
      double maxTransparency=0.9;
      
      double transparency=maxTransparency;
      
      if (maxLevel_>1){
	
	double a=(maxTransparency-minTransparency)/double(maxLevel_-1);
	double b=minTransparency-a;

	transparency=a*double(this->level())+b;
      }

      int iop=int(255.0*transparency);
      //      INFOS("iop="<<iop);

      
      this->filledSquare(xFrom,yFrom,xTo,yTo,1,iop,iop,iop);
      
   

      // }
      
    }     


    void createNonEmptyBlock(int i, int j){

    }
      

    
    template <class REAL_TYPE>
    inline void storeElement(int i,int j,const REAL_TYPE & value){
      int xFrom=unitCellSize_*(this->cornerFlatColIndex()+j);
      int xTo=unitCellSize_*(this->cornerFlatColIndex()+j+1);

      //      int yFrom=unitCellSize_*this->cornerFlatRowIndex();
      //      int yTo=unitCellSize_*(this->cornerFlatRowIndex()+this->blockFlatRowSize());

      int yFrom=unitCellSize_*(this->cornerFlatRowIndex()+i);
      int yTo=unitCellSize_*(this->cornerFlatRowIndex()+i+1);
      
      // int swapYFrom=this->cornerFlatRowIndex()+i;
      // int swapYTo=this->cornerFlatRowIndex()+i+1;
      
      // int yFrom=unitCellSize_*(this->flatRowSize()-swapYFrom);
      // int yTo=unitCellSize_*(this->flatRowSize()-swapYTo);

      if (this->flag()==0){
      
	if (unitCellSize_>4){
	  
	  this->filledSquare(xFrom,yFrom,xTo,yTo,1,255,255,255);
	  xFrom+=1;
	  yFrom+=1;
	xTo-=1;
	yTo-=1;
	
	this->filledSquare(xFrom,yFrom,xTo,yTo,1,255,0,0);
	}
	else{
	  this->filledSquare(xFrom,yFrom,xTo,yTo,1,255,0,0);
	}
      }
      else{//text
	this->filledSquare(xFrom,yFrom,xTo,yTo,1,255,255,255);
	xFrom+=1;
	yFrom+=1;
	xTo-=1;
	yTo-=1;
	this->filledSquare(xFrom,yFrom,xTo,yTo,1,255,0,0);
	xFrom+=1;
	this->dumpValue(xFrom,yFrom,xTo,yTo,value);
      }

	
      
      
      // int xFrom=unitCellSize_*this->cornerFlatColIndex();
      // int xTo=unitCellSize_*(this->cornerFlatColIndex()+this->blockFlatColSize());

      // int yFrom=unitCellSize_*this->cornerFlatRowIndex();
      // int yTo=unitCellSize_*(this->cornerFlatRowIndex()+this->blockFlatRowSize());
      
   
      //Add Border

      // xFrom+=1;
      // yFrom-=1;
      // xTo-=1;
      // yTo+=1;

      // this->filledSquare(xFrom,yFrom,xTo,yTo,1,255,0,0);
       

      //       (*svgwriterPtr_).filledsquare(xFrom,yFrom,xTo,yTo,1.0,0.0,0.0);


    }

    void dumpElement(int i, int j, const float & element){ this->storeElement(i,j,element);}
    void dumpElement(int i, int j, const double & element){ this->storeElement(i,j,element);}

    std::ofstream & outfile( void ) { return *outfilePtr_;}

    

  private:

    std::ofstream * outfilePtr_;
    
    int borderSize_;
    int unitCellSize_;
    int maxLevel_;
    
  };
  
}

#endif
