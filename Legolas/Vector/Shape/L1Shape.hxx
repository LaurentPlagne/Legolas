#ifndef __L1SHAPE_HXX__
#define __L1SHAPE_HXX__

#include <stdexcept>
#include "Legolas/Common/BasicTypes.hxx"
#include "Legolas/Vector/Shape/VirtualVectorShape.hxx"

#include <string>
#include <fstream>
#include <cassert>

#ifdef LEGOLAS_USE_EIGEN
#define MIN_VEC_SIZE 4
#else
#define MIN_VEC_SIZE 1
#endif

namespace Legolas{
  
  class L1Shape : public VirtualVectorShape{
    SizeType size_;
    SizeType internalSize_;


    static inline SizeType computeInternalSize(SizeType size){
      SizeType result=size;
      SizeType rem=size-MIN_VEC_SIZE*(size/MIN_VEC_SIZE);
      if (rem!=0){
	result=((size/MIN_VEC_SIZE)+1)*MIN_VEC_SIZE;
      }      
      return result;
    }

  public:

    static const int Level=1;

    inline L1Shape( void ):size_(0),internalSize_(0){}


    inline L1Shape( SizeType size ):size_(size){
      internalSize_=computeInternalSize(size_);
    }

    inline bool operator == (const L1Shape & right) const {return (size_== right.size_);}
    inline bool operator <  (const L1Shape & right) const {return (size_<  right.size_);}
    inline bool operator != (const L1Shape & right) const {return (size_!= right.size_);}
    
    inline int level( void ) const { return Level ;}
    inline SizeType size( void ) const { return size_ ;}
    inline size_t flatSize( void ) const { return internalSize_ ;}
    inline size_t subFlatSize( int i ) const { return 1 ;}

    void dump(const std::string filename) const {
      std::ofstream outfile(filename.c_str(),std::ios::out | std::ios::binary);
      outfile <<Level << std::endl;
      this->dumpElements(outfile);
      outfile.close();
    }

    void dumpElements(std::ostream & outfile) const {
      outfile << size_ << std::endl;
      outfile << internalSize_ << std::endl;
    }


    void read(const std::string filename){
      std::ifstream inputFile(filename.c_str(),std::ios::in | std::ios::binary);
      int l=0;
      if (inputFile >> l){
	assert(l==1);
	this->readElements(inputFile);
      }
      else{
	INFOS(" error while reading "<<filename);
	throw std::runtime_error("Error while reading");
      }
      inputFile.close();
    }


    void readElements(std::istream & inputFile){
      SizeType is;
      if (inputFile >> size_){
	internalSize_=computeInternalSize(size_);
	if (inputFile >> is){
	  if (is!=internalSize_){
	    INFOS("Error: is!=internalSize: padding problem...");
	    throw std::runtime_error("Error: is!=internalSize: padding problem...");
	  }
	}
	else{
	  INFOS(" error while reading file");
	  throw std::runtime_error("Error while reading file");
	}
      }
      else{
	INFOS(" error while reading file");
    throw std::runtime_error("Error while reading file");
      }
    }


  };

  inline std::ostream& operator<< ( std::ostream& os, 
				    const L1Shape & source ){
    return os << source.size() ;
  }

}

#endif

      
    

