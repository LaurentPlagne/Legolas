#pragma once

#include "Legolas/Vector/Shape/VirtualVectorShape.hxx"

namespace Legolas{

  template <class MULTI_ARRAY_SHAPE, class ARRAY_SHAPE, int MULTI_ARRAY_LEVEL>
  struct MultiArrayShapeTraits{
    typedef MULTI_ARRAY_SHAPE SubShape;
  };
  
  template <class MULTI_ARRAY_SHAPE,class ARRAY_SHAPE>
  struct MultiArrayShapeTraits<MULTI_ARRAY_SHAPE,ARRAY_SHAPE,1>{
    typedef ARRAY_SHAPE SubShape;
  };

  

  template <class ARRAY_SHAPE, int MULTI_ARRAY_LEVEL>
  struct MultiArrayShape:public VirtualVectorShape{


    typedef MultiArrayShape<ARRAY_SHAPE,MULTI_ARRAY_LEVEL-1> SubMultiArrayShape;
    typedef MultiArrayShapeTraits<SubMultiArrayShape,ARRAY_SHAPE,MULTI_ARRAY_LEVEL> MAST;
    typedef typename MAST::SubShape SubShape;
    
    std::vector<SubShape> data_;
    size_t flatSize_;

    MultiArrayShape( void ):data_(),flatSize_(0){
    }

    MultiArrayShape(const MultiArrayShape & other):data_(other.data_),
						   flatSize_(other.flatSize_){
      // INFOS("MULTI_ARRAY_LEVEL="<<MULTI_ARRAY_LEVEL);
      // INFOS("ICI <other.data_.size()="<<other.data_.size());
      // for (size_t i=0 ; i<other.data_.size() ; i++){
      // 	INFOS("MULTI_ARRAY_LEVEL="<<MULTI_ARRAY_LEVEL<<" i="<<i);
      // 	data_.push_back(other.data_[i]);
      // }
      // INFOS("data_.size()="<<data_.size());
    }

    MultiArrayShape & operator=(const MultiArrayShape & other){
      //      INFOS("MULTI_ARRAY_LEVEL="<<MULTI_ARRAY_LEVEL);
      flatSize_=other.flatSize_;
      //      INFOS("other.data_.size()="<<other.data_.size());
      data_=other.data_;
      // INFOS("other.data_.size()="<<other.data_.size());
      // data_.resize(other.data_.size());
      // for (size_t i=0 ; i<other.data_.size() ; i++){
      // 	INFOS("MULTI_ARRAY_LEVEL="<<MULTI_ARRAY_LEVEL<<" i="<<i);
      // 	data_.push_back(other.data_[i]);
      // }
      //      INFOS("data_.size()="<<data_.size());
      return *this;
    }

    
						   

    static const int multiArrayLevel_=MULTI_ARRAY_LEVEL;
    static const int level_=MULTI_ARRAY_LEVEL+ARRAY_SHAPE::level_;

    const SubShape & operator[](size_t i) const { return data_[i] ;}

    bool operator ==(const MultiArrayShape & other) const {
      bool result=(flatSize_==other.flatSize());
      result=result&&(data_.size()==other.size());
      if (result){
	for (size_t i=0 ; i<data_.size() ; i++){
	  result=result&&(data_[i]==other[i]);
	}
      }
      return result;
    }
    bool operator !=(const MultiArrayShape & other) const {
      bool equal=(*this)==other;
      return (!equal);
    }
	
    
    void push_back(const SubShape & ss){
      data_.push_back(ss);
      flatSize_+=ss.flatSize();
    }

    
    size_t size( void ) const {
      //      INFOS("data_.size()="<<data_.size());
      return data_.size();
    }
    size_t flatSize( void ) const { return flatSize_;}
    int level( void ) const { return level_;}
    size_t subFlatSize( int i) const { return data_[i].flatSize();}

    void save(std::ostream & os){
      os << data_.size() << std::endl;
      for (uint i=0 ; i<data_.size() ; i++){
	data_[i].save(os);
      }
      os << flatSize_ << std::endl;
    }
    
    void load(std::istream & is){
      //      INFOS("Load MultiArrayShape<ARRAY_SHAPE,"<<MULTI_ARRAY_LEVEL<<">");
      uint dataSize=0;
      is >> dataSize ;
      //      INFOS("dataSize="<<dataSize);
      data_.resize(dataSize);
      for (uint i=0 ; i<data_.size() ; i++){
	data_[i].load(is);
      }
      is >> flatSize_ ;
      //      INFOS("flatSize_="<<flatSize_);
    }

    
  };

  template <class ARRAY_SHAPE, int MULTI_ARRAY_LEVEL>
  void dump(const MultiArrayShape<ARRAY_SHAPE, MULTI_ARRAY_LEVEL> & mas){
    std::cout << "MultiArrayShape MultiArrayLevel="<<MULTI_ARRAY_LEVEL<<" size="<<mas.size() << std::endl;
    for (size_t i=0 ; i<mas.size() ; i++){
      dump(mas[i]);
    }
  }

}
    
  
