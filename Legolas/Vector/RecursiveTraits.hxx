#ifndef __LEGOLAS_RECURSIVETRAITS_HXX__
#define __LEGOLAS_RECURSIVETRAITS_HXX__

#include <iostream>
#include <fstream>

namespace Legolas{
  
  /*! \brief RecursiveTraits is an helper class of RectangularMultiVector.
    This class is exclusively used by the RectangularMultiVector class.
  */
  template <class VECTOR, class SUB_VECTOR ,class RT, int LEVEL> 
  struct RecursiveTraits{

    typedef typename VECTOR::SizeType SizeType;
    
    typedef RT RealType;
    typedef VECTOR Vector;
    typedef SUB_VECTOR Element;
    typedef Element VirtualElement;

    typedef typename ShapeTraits<LEVEL>::Type Shape;
    typedef typename ShapeTraits<LEVEL-1>::Type SubShape;
    
    static void initialize(Shape shape, RealType * realDataPtr, size_t flatSize, Vector & vec){

      vec.shape()=shape;
      vec.flatSize()=flatSize;
      vec.dataPtr()=new Element[shape.size()];
      vec.realDataPtr()=realDataPtr;

      typedef typename Shape::List            List;
      typedef typename Shape::ConstIterator   ConstIterator;
      
      const List & l=shape.getList();
      ConstIterator iter=l.begin();
      ConstIterator iterEnd=l.end();

      int index=0;
      
      for ( ; iter != iterEnd ; iter++){
	int n=iter->first;
	SubShape elementShape=iter->second;
	size_t elementFlatSize=elementShape.flatSize();
	//	size_t elementFlatSize=Legolas::getFlatSize(elementShape);

	for (int i=0 ; i<n ; i++){
	  //	  Element::RCT::initialize(elementShape,realDataPtr,elementFlatSize,vec[index]);
	  vec[index].initialize(elementShape,realDataPtr,elementFlatSize);
	  index++;
	  realDataPtr+=elementFlatSize;
	}
      }
    }


    template <class DERIVED>
    static Shape buildShapeFromBaseVector(const BaseVector<DERIVED> & right){
      Shape result;
      const DERIVED & r=right.getCVR();
      for (uint i=0 ; i< r.size() ; i++){
	result.appendElement(Element::RCT::buildShapeFromBaseVector(r[i]));
      }
      return result;
    }

    static inline       Element & getElement(      Vector & vec, SizeType i){ return vec.dataPtr()[i];}
    static inline const Element & getElement(const Vector & vec, SizeType i){ return vec.dataPtr()[i];}

    static inline       VirtualElement & getVirtualElement(      Vector & vec, SizeType i){ return vec.dataPtr()[i];}
    static inline const VirtualElement & getVirtualElement(const Vector & vec, SizeType i){ return vec.dataPtr()[i];}


    static inline SizeType size(const Shape & shape){ return shape.size();}

    static inline void resize(SizeType size,Vector & vec){
      INFOS("No resize is allowed for multi vector (LEVEL="<<LEVEL<<")");
      throw std::runtime_error("No resize is allowed for multi vector");
    }

    static inline void display(const Vector & vec, std::ostream& os, const std::string & prefix){
      //      os << " -- realDataPtr_=";
      //      os << vec.realDataPtr() << std::endl;
      os << std::endl;
      //      INFOS("vec.size()="<<vec.size());

      for (SizeType i=0 ; i < vec.size() ; i++){
	std::string indexString(prefix);
	indexString+="[\t";
	indexString+=stringConvert(i);
	indexString+="\t]";
	vec[i].display(os,indexString);
      }
      
      return ;
    }

    static inline void dump(const Vector & vec, std::ofstream& os, const std::string & prefix){
      for (SizeType i=0 ; i < vec.size() ; i++){
	std::string indexString(prefix);
	indexString+=stringConvert(i);
	indexString+=" ";
	vec[i].dump(os,indexString);
      }
      
      return ;
    }


  };
    
  /////////////////////////////////////////////////////////////////////////////////////////////:

  /*! \brief RecursiveTraits is an helper class of RectangularMultiVector.
    
  This class is exclusively used by the RectangularMultiVector class.
  */
  template <class VECTOR, class SUB_VECTOR ,class RT> 
  struct RecursiveTraits<VECTOR,SUB_VECTOR,RT,1>{
    
    typedef typename VECTOR::SizeType SizeType;
    
    typedef RT RealType;
    typedef VECTOR Vector;
    typedef RT Element;
    typedef Vector VirtualElement;
    typedef typename ShapeTraits<1>::Type Shape;

    static void initialize(const Shape & shape, RealType * realDataPtr, int flatSize, Vector & vec){

      vec.shape()=shape;
      vec.flatSize()=flatSize;
      vec.realDataPtr()=realDataPtr;

    }


    template <class DERIVED>
    static Shape buildShapeFromBaseVector(const BaseVector<DERIVED> & right){
      const DERIVED & r=right.getCVR();
      return r.size();
    }

    //    static inline int size(const Shape & shape){ return shape;}
    static inline int size(const Shape & shape){ return shape.size();}
    
    static inline void resize(size_t size,Vector & vec){
      if (size!=vec.flatSize()){
	if (!vec.realDataPtr()){ // only possible with DefaultCtor
	  vec.reshape(size);
	}
	else{
	  if (vec.owner()){
	    delete [] vec.realDataPtr();
	    vec.realDataPtr()=0;	    
	    vec.reshape(size);
	  }
	  else{
	    INFOS("No resize is allowed for vector leafs");
	    throw std::runtime_error("No resize is allowed for vector leafs");
	  }
	}
      }
    }

    static inline       Element & getElement(      Vector & vec, int i){ return vec.realDataPtr()[i];}
    static inline const Element & getElement(const Vector & vec, int i){ return vec.realDataPtr()[i];}


    static inline       Vector & getVirtualElement(      Vector & vec, int i){ INFOS("ERROR") ; return vec;}
    static inline const Vector & getVirtualElement(const Vector & vec, int i){ INFOS("ERROR") ; return vec;}



    
    static inline void display(const Vector & vec, std::ostream& os, const std::string & prefix){
      os << prefix << "={ " ;
      
      for(SizeType k=0 ; k< vec.size() ; k++ ) { 
	os  << vec[k] << ' ';
      }
      os << "}" << std::endl;
      
      return ;
    }


    static inline void dump(const Vector & vec, std::ofstream& os, const std::string & prefix){
      for(SizeType k=0 ; k< vec.size() ; k++ ) { 
	os << prefix << k << " " << vec[k] << std::endl;
      }
      return ;
    }

  };
    
}

#endif

