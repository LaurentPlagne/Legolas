#pragma once
#include <iostream>

//#include "Legolas/Vector/Shape/VirtualVectorShape.hxx"

namespace Legolas{

  template <int PACK_SIZE, int PACK_LEVEL, int LEVEL>
  struct ComputeNPack{
    static inline int apply(int size){ 
      //      std::cout <<"ICI"<<std::endl;
      return size; 
    }
  };
  
  template <int PACK_SIZE, int PACK_LEVEL>
  struct ComputeNPack<PACK_SIZE,PACK_LEVEL,PACK_LEVEL>{
    static inline int apply(int size){
      int npack=size/PACK_SIZE;
      if (size!=npack*PACK_SIZE) npack++;
      //      std::cout << "PACK_SIZE="<<PACK_SIZE<<" PACK_LEVEL="<<PACK_LEVEL<<" npack="<<npack <<std::endl;
      return npack;
    }
  };


  template <int PACK_SIZE, int PACK_LEVEL, int LEVEL>
  struct ComputeShift{
    static inline int apply(int index, int elementSize){
      return index*elementSize;
    }
  };
    
  template <int PACK_SIZE, int PACK_LEVEL>
  struct ComputeShift<PACK_SIZE,PACK_LEVEL,PACK_LEVEL>{
    static inline int apply(int index, int elementSize){
      const int packIndex=index/PACK_SIZE;
      const int componentIndex=index-packIndex*PACK_SIZE;
      return packIndex*elementSize+componentIndex;
    }
  };

  //LP: rajout
  template <int PACK_SIZE, int PACK_LEVEL>
  struct ComputeShift<PACK_SIZE,PACK_LEVEL,1>{
    static inline int apply(int index, int elementSize){
      return index*PACK_SIZE;
    }
  };


  template <int PACK_SIZE>
  struct ComputeShift<PACK_SIZE,1,1>{
    static inline int apply(int index, int elementSize){
      return index;
    }
  };



  template <int PACK_SIZE, int PACK_LEVEL, int LEVEL>
  struct Stride{
    static const size_t stride_=1;
  };

  // template <int PACK_SIZE,int PACK_LEVEL>
  // struct Stride<PACK_SIZE,PACK_LEVEL,PACK_LEVEL>{
  //   static const size_t stride_=PACK_SIZE;
  // };

  template <int PACK_SIZE, int PACK_LEVEL>
  struct Stride<PACK_SIZE,PACK_LEVEL,1>{
    static const size_t stride_=PACK_SIZE;
  };

  
  template <int PACK_SIZE>
  struct Stride<PACK_SIZE,1,1>{
    static const size_t stride_=1;
  };




  





  //  template <int PACK_SIZE,int PACK_LEVEL,int LEVEL>
  //  struct ArrayShape: public VirtualVectorShape{
  template <int PACK_SIZE,int PACK_LEVEL,int LEVEL>
  struct ArrayShape{//: public VirtualVectorShape{

    static const int level_=LEVEL;
    
    int size_;
    typedef ArrayShape<PACK_SIZE,PACK_LEVEL,LEVEL-1> SubShape;
    SubShape subShape_;
    long dataSize_;


    
    ArrayShape( void ):size_(0),
     		       subShape_(),
     		       dataSize_(0){
    }


    ArrayShape(const ArrayShape & other):size_(other.size_),
					 subShape_(other.subShape_),
					 dataSize_(other.dataSize_){
    }

    ArrayShape & operator=(const ArrayShape & other){
      size_=other.size_;
      subShape_=other.subShape_;
      dataSize_=other.dataSize_;
      return *this;
    }

    
    template <typename... Args>
    ArrayShape(int first,Args... rest):size_(first),
				       subShape_(rest...),
				       dataSize_(ComputeNPack<PACK_SIZE,PACK_LEVEL,LEVEL>::apply(size_)*subShape_.dataSize_)
    {
    } 

    ArrayShape(int size, const SubShape & subShape):size_(size),
						    subShape_(subShape),
						    dataSize_(ComputeNPack<PACK_SIZE,PACK_LEVEL,LEVEL>::apply(size_)*subShape_.dataSize_)
    {
    }


    const SubShape & operator[](int i) const {return subShape_;}
  
    inline int computeShift(int index) const{
      return ComputeShift<PACK_SIZE,PACK_LEVEL,LEVEL>::apply(index,subShape_.dataSize_);
    }

    bool operator==(const ArrayShape & other) const{
      return ( (subShape_==other.subShape_) && (size_==other.size_) );
    }

    bool operator!=(const ArrayShape & other) const{
      return !((*this)==other);
    }


    ArrayShape<1,1,LEVEL> getPackedShape( void ) const {
      ArrayShape<1,1,LEVEL> result(ComputeNPack<PACK_SIZE,PACK_LEVEL,LEVEL>::apply(this->size_),
				   this->subShape_.getPackedShape());
      return result;
      //      result.size_=ComputeNPack<PACK_SIZE,PACK_LEVEL,LEVEL>::apply(this->size_);
      //      result.subShape_=this->subShape_.getPackedShape();
      //      result.dataSize_=result.size_*result.subShape_.dataSize_;
      //      return result;
    }

    // ArrayShape<1,1,LEVEL> getPackedShape( void ){
    //   ArrayShape result;
    //   result.size()

    int size( void ) const { return size_;}
    size_t flatSize( void ) const { return dataSize_;}
    int level( void ) const { return LEVEL;}
    size_t subFlatSize( int i) const { return subShape_.dataSize_ ;}

    void dump( void ) const {
      std::cout<<"ArrayShape<"<<PACK_SIZE<<","<<PACK_LEVEL<<","<<LEVEL<<">:[size_="<<size_<<" dataSize_"<<dataSize_<<"]"<<std::endl;
      subShape_.dump();
    }

    void save(std::ostream & os){
      os << this->size_ ;
      os << std::endl;
      os << this-> dataSize_;
      os << std::endl;
      subShape_.save(os);
    }

    void load(std::istream & is){
      //      std::cout<<"Load ArrayShape<"<<PACK_SIZE<<","<<PACK_LEVEL<<","<<LEVEL<<">"<<std::endl;
      is >> this->size_ ;
      //      INFOS("this->size_="<<this->size_);
      is >> this-> dataSize_;
      subShape_.load(is);
      //      INFOS("this->dataSize_="<<this->dataSize_);
    }


  };


  //  template <int PACK_SIZE,int PACK_LEVEL>
  //  struct ArrayShape<PACK_SIZE,PACK_LEVEL,1>: public VirtualVectorShape{

  template <int PACK_SIZE,int PACK_LEVEL>
  struct ArrayShape<PACK_SIZE,PACK_LEVEL,1>{//: public VirtualVectorShape{
    static const int level_=1;
    int size_;
    typedef int SubShape;
    long dataSize_;

    static const int stride_=Stride<PACK_SIZE,PACK_LEVEL,1>::stride_;

  
    static inline long computeDataSize(int size){
      return ComputeNPack<PACK_SIZE,PACK_LEVEL,1>::apply(size)*PACK_SIZE;
    }

    
    
    ArrayShape( void ):size_(0),
		       //		       subShape_(0),
		       dataSize_(0){
    }

    
    ArrayShape(const ArrayShape & other):size_(other.size_),
					 //					 subShape_(other.subShape_),
					 dataSize_(other.dataSize_){
    }
    


    ArrayShape & operator=(const ArrayShape & other){
      size_=other.size_;
      dataSize_=other.dataSize_;
      return *this;
    }

    
  
    template <typename... Args>
    ArrayShape(int size):size_(size),
			 dataSize_(computeDataSize(size))
    {
    } 
  
    bool operator==(const ArrayShape & other) const {
      return ( (size_==other.size_) && (dataSize_==other.dataSize_) );
    }

    inline int computeShift(int index) const {
      return ComputeShift<PACK_SIZE,PACK_LEVEL,1>::apply(index,PACK_SIZE);
      //      return index*PACK_SIZE;
    }

    ArrayShape<1,1,1> getPackedShape( void ) const {
      ArrayShape<1,1,1> result;
      //      INFOS("this->size_="<<this->size_);
      //      result.size_=this->size_;
      result.size_=ComputeNPack<PACK_SIZE,PACK_LEVEL,1>::apply(this->size_);
      result.dataSize_=result.size_;
      return result;
    }

    
    int size( void ) const { return size_;}
    size_t flatSize( void ) const { return dataSize_;}
    int level( void ) const { return 1;}
    size_t subFlatSize( int i) const { return 1 ;}

    void dump( void ) const {
      std::cout<<"ArrayShape<"<<PACK_SIZE<<","<<PACK_LEVEL<<","<<1<<">:[size_="<<size_<<" dataSize_"<<dataSize_<<"]"<<std::endl;
    }

    void save(std::ostream & os){
      os << this->size_ ;
      os << std::endl;
      os << this-> dataSize_;
      os << std::endl;
    }

    void load(std::istream & is){
      //      INFOS("Load ArrayShape<"<<PACK_SIZE<<","<<PACK_LEVEL<<","<<this->level()<<">");
      is >> this->size_ ;
      //      INFOS("this->size_="<<this->size_);
      is >> this-> dataSize_;
      //      INFOS("this->dataSize_="<<this->dataSize_);
    }


  };


  // template <int PACK_SIZE>
  // struct ArrayShape<PACK_SIZE,1,1>{
  //   int size_;
  //   long dataSize_;
  //   int subShape_;
    
  
  //   static inline long computeDataSize(int size){
  //     return ComputeNPack<PACK_SIZE,1,1>::apply(size)*PACK_SIZE;
  //   }

  //   ArrayShape( void ):size_(0),
  // 		       subShape_(0),
  // 		       dataSize_(0){
  //   }


  
  //   template <typename... Args>
  //   ArrayShape(int size):size_(size),
  // 			 dataSize_(computeDataSize(size))
  //   {
  //   } 
  
  //   inline int computeShift(int index) const {
  //     return index;
  //   }

  //   ArrayShape<1,1,1> getPackedShape( void ) const {
  //     ArrayShape<1,1,1> result;
  //     result.size_=ComputeNPack<PACK_SIZE,1,1>::apply(this->size_);
  //     result.dataSize_=result.size_;
  //     return result;
  //   }

  // };


  template <int PACK_SIZE,int PACK_LEVEL,int LEVEL>
  void dump(const ArrayShape<PACK_SIZE,PACK_LEVEL,LEVEL> & as){
    as.dump();
  }





}//namespace Legolas
