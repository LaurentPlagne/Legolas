#ifndef __LEGOLAS_FLATRECURSIVETRAITS_HXX__
#define __LEGOLAS_FLATRECURSIVETRAITS_HXX__

#include <iostream>
#include <fstream>


namespace Legolas{
  
  template <class VECTOR, class SUB_VECTOR ,class RT, int LEVEL> 
  struct FlatRecursiveTraits{
    
    typedef RT RealType;
    typedef VECTOR Vector;
    typedef SUB_VECTOR Element;
    typedef typename ShapeTraits<LEVEL>::Type Shape;
    typedef typename ShapeTraits<LEVEL-1>::Type SubShape;


    static inline Element * recursiveInitialize(const std::vector< std::vector<int> > & levelSizes){
      
      typename Element::Element * flatElementPtr=Element::recursiveInitialize(levelSizes);

      const int thisLevelSize=levelSizes[LEVEL-1].size();

      Element * result= 0;
      
      if (thisLevelSize!=0) result= new Element[thisLevelSize];

      //      INFOS("LEVEL="<<LEVEL<<" Element Array Size :"<<thisLevelSize);
      
      for (int i=0 ; i<thisLevelSize ; i++){
	result[i].elementPtr()=flatElementPtr;
	result[i].size()=levelSizes[LEVEL-1][i];
	flatElementPtr+=levelSizes[LEVEL-1][i];
	if (i==0) result[i].owner()=true;
      }
      
      return result;
    }
    
    static inline RealType * getRealDataPtr(const Vector & vec){
      return Element::getRealDataPtr(vec[0]);
    }


    static void setRealDataPtr(Shape shape, RealType * realDataPtr, int flatSize, Vector & vec){

      //      INFOS("flatSize="<<flatSize);

      vec.shape()=shape;
      vec.flatSize()=flatSize;
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
	const size_t elementFlatSize=elementShape.flatSize();
	//	int elementFlatSize=Legolas::getFlatSize(elementShape);

	for (int i=0 ; i<n ; i++){
	  vec[index].setRealDataPtr(elementShape,realDataPtr,elementFlatSize);
	  index++;
	  realDataPtr+=elementFlatSize;
	}
      }
    }

    
    static void initialize(Shape shape, RealType * realDataPtr, int flatSize, Vector & vec){

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
	const size_t elementFlatSize=elementShape.flatSize();
	//	int elementFlatSize=Legolas::getFlatSize(elementShape);

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
      for (int i=0 ; i< r.size() ; i++){
	result.appendElement(Element::FRT::buildShapeFromBaseVector(r[i]));
      }
      return result;
    }

    static inline       Element & getElement(      Vector & vec, int i){ return vec.dataPtr()[i];}
    static inline const Element & getElement(const Vector & vec, int i){ return vec.dataPtr()[i];}

    static inline int size(const Shape & shape){    return shape.size();}

    static inline void resize(int size,Vector & vec){
      INFOS("No resize is allowed for multi vector (LEVEL="<<LEVEL<<")");
      throw std::runtime_error("No resize is allowed for multi vector");
    }

    static inline void display(const Vector & vec, std::ostream& os, const std::string & prefix){
      //      os << " -- realDataPtr()=";
      //      os << vec.realDataPtr() << std::endl;
      os << std::endl;
      //      INFOS("vec.size()="<<vec.size());

      for (int i=0 ; i < vec.size() ; i++){
	std::string indexString(prefix);
	indexString+="[\t";
	indexString+=stringConvert(i);
	indexString+="\t]";
	vec[i].display(os,indexString);
      }
      
      return ;
    }

    static inline void dump(const Vector & vec, std::ofstream& os, const std::string & prefix){
      for (int i=0 ; i < vec.size() ; i++){
	std::string indexString(prefix);
	indexString+=stringConvert(i);
	indexString+=" ";
	vec[i].dump(os,indexString);
      }
      
      return ;
    }


  };
    
  /////////////////////////////////////////////////////////////////////////////////////////////:

  /*! \brief FlatRecursiveTraits is an helper class of RectangularMultiVector.
    
  This class is exclusively used by the RectangularMultiVector class.
  */
  template <class VECTOR, class SUB_VECTOR ,class RT> 
  struct FlatRecursiveTraits<VECTOR,SUB_VECTOR,RT,1>{
    typedef RT RealType;
    typedef VECTOR Vector;
    typedef RT Element;
    typedef typename ShapeTraits<1>::Type Shape;


    static inline Element * recursiveInitialize(const std::vector< std::vector<int> > & levelSizes){

      int size=0;
      for (uint i=0 ; i< levelSizes[1].size(); i++) size+=levelSizes[1][i];
      //      INFOS("RealType array size :"<<size);

      Element * result=0;

      if (size!=0) result= new Element[size];
      //      INFOS("result="<<result);
      
      return result;
    }


    static inline RealType * getRealDataPtr(const Vector & vec){
      return vec.elementPtr();
    }

    static void setRealDataPtr(Shape shape, RealType * realDataPtr, int flatSize, Vector & vec){

      //      INFOS("flatSize="<<flatSize);
      vec.shape()=shape;
      vec.flatSize()=flatSize;
      vec.realDataPtr()=realDataPtr;
    }
    

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

    static inline int size(const Shape & shape){    return shape.size();}

    static inline void resize(int size,Vector & vec){
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
    static inline const Element & getElement(const Vector & vec, int i){ 
      return vec.realDataPtr()[i];
    }

    
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

