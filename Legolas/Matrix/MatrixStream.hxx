#ifndef __LEGOLAS_MATRIXSTREAM_HXX__
#define __LEGOLAS_MATRIXSTREAM_HXX__

namespace Legolas{

  class MatrixStream{
  public:    
    
    virtual std::string extension( void ) const = 0;

    void checkThisExtension(const std::string & fileName, const std::string ext) const {
      const int found=fileName.find(ext);
      const int expectedValue=fileName.size()-ext.size();
      if ((found!=expectedValue) || (found<0)){ //not found..
	INFOS("error!! fileName="<<fileName);
	INFOS("error!! fileName must have the "<<ext<<" extension !!!");
	throw std::runtime_error("Bad extension");
      }
    }

    virtual void checkExtension(const std::string & fileName) const = 0;

    //      INFOS("error!!");
    //      throw std::runtime_error("error!!");
    //    }
    
//     virtual void checkExtension(const std::string & fileName) const {
//       INFOS("ICI checkext!!");
//       std::string ext=this->extension();

//       const int found=fileName.find(ext);
//       const int expectedValue=fileName.size()-ext.size();
//       if ((found!=expectedValue) || (found<0)){ //not found..
// 	INFOS("error!! fileName="<<fileName);
// 	INFOS("error!! fileName must have the "<<ext<<" extension !!!");
// 	throw std::runtime_error("Bad extension");
//       }
//     }



    
    MatrixStream( void ):flag_(-1),
			 level_(-1),
			 nrows_(0),
			 ncols_(0),
			 isRoot_(false),
			 cornerFlatRowIndex_(0),
			 cornerFlatColIndex_(0),
			 flatRowSize_(0),
			 flatColSize_(0),
			 blockFlatRowSize_(0),
			 blockFlatColSize_(0)
    {}
    
    MatrixStream(const MatrixStream & source ):flag_(source.flag_),
					       level_(source.level_),
					       nrows_(source.nrows_),
					       ncols_(source.ncols_),
					       isRoot_(false),
					       cornerFlatRowIndex_(source.cornerFlatRowIndex_),
					       cornerFlatColIndex_(source.cornerFlatColIndex_),
					       flatRowSize_(source.flatRowSize_),
					       flatColSize_(source.flatColSize_),
					       blockFlatRowSize_(source.blockFlatRowSize_),
					       blockFlatColSize_(source.blockFlatColSize_)
    {
      INFOS("MatrixStream Copy Ctor");
    }

    
    MatrixStream(const std::string & fileName,int flag, int level, 
		 int flatRowSize, 
		 int flatColSize):flag_(flag),
				  level_(level),
				  nrows_(0),
				  ncols_(0),
				  isRoot_(true),
				  cornerFlatRowIndex_(0),
				  cornerFlatColIndex_(0),
				  flatRowSize_(flatRowSize),
				  flatColSize_(flatColSize),
				  blockFlatRowSize_(flatRowSize),
				  blockFlatColSize_(flatColSize)
      
    {
    }

    MatrixStream(MatrixStream & parent, 
		 int cornerFlatRowIndex,
		 int cornerFlatColIndex,
		 int blockFlatRowSize,
		 int blockFlatColSize):flag_(parent.flag_),
				       level_(parent.level_-1),
				       nrows_(0),
				       ncols_(0),
				       isRoot_(false),
				       cornerFlatRowIndex_(cornerFlatRowIndex),
				       cornerFlatColIndex_(cornerFlatColIndex),
				       flatRowSize_(parent.flatRowSize_),
				       flatColSize_(parent.flatColSize_),
				       blockFlatRowSize_(blockFlatRowSize),
				       blockFlatColSize_(blockFlatColSize)
    {										
    }
    
    virtual MatrixStream * child(int i, int j, 
				 int cornerFlatRowIndex,
				 int cornerFlatColIndex,
				 int blockFlatRowSize,
				 int blockFlatColSize) = 0;
 //      INFOS("MatrixStream::child call");
//       throw std::runtime_error("MatrixStream::child call");
//     }

    
   
    virtual ~MatrixStream( void ){
      MESSAGE("MatrixStream Dtor");
    };
    

    virtual void blockInitialize(int i, int j){}      
    virtual void blockFinalize(){}      
    virtual void createEmptyBlock(int i, int j){}
    virtual void createNonEmptyBlock(int i, int j){}
    virtual void rowInitialize(int rowIndex){}
    virtual void rowFinalize(int rowIndex){}
    virtual void colInRowInitialize(int i, int j){}
    virtual void colInRowFinalize(int i, int j){}
    virtual void dumpElement(int i, int j, const double & value){}
    virtual void dumpElement(int i, int j, const float & value){}

    int cornerFlatRowIndex( void ) const { return cornerFlatRowIndex_;}
    int cornerFlatColIndex( void ) const { return cornerFlatColIndex_;}

    int blockFlatRowSize( void ) const { return blockFlatRowSize_;}
    int blockFlatColSize( void ) const { return blockFlatColSize_;}

    int flatRowSize( void ) const { return flatRowSize_;}
    int flatColSize( void ) const { return flatColSize_;}


    bool isRoot( void ) const { return isRoot_ ; }

    const int & nrows( void ) const { return nrows_;}
    const int & ncols( void ) const { return ncols_;}

    int & nrows( void ) { return nrows_;}
    int & ncols( void ) { return ncols_;}

    const int & level( void ) const { return level_;}

    int flag( void ) const {return flag_ ;}

  private:
    const int flag_;
    const int level_;
    int nrows_;
    int ncols_;
    const bool isRoot_;
    const int cornerFlatRowIndex_;
    const int cornerFlatColIndex_;
    const int flatRowSize_;
    const int flatColSize_;
    const int blockFlatRowSize_;
    const int blockFlatColSize_;
  };

}

#endif
