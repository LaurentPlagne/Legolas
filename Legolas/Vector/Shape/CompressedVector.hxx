#ifndef __LEGOLAS_COMPRESSEDVECTOR_HXX__
#define __LEGOLAS_COMPRESSEDVECTOR_HXX__

#include <vector>
#include <list>
#include <string>
#include <iostream>

#include "UTILITES.hxx"
#include "Legolas/Vector/Shape/MicroParserHelper.hxx"
#include "Legolas/Common/BasicTypes.hxx"
#include <string>

namespace Legolas{

  /** \brief CompressedVector: Classe principale d'implémentation des Shape Legolas++

  Cette classe correspond à la classe principale d'implémentation du module Shape.
  
  \ingroup VectorShapeGroup
  */
  template <class ELEMENT>
  class CompressedVector {
  public:
    typedef ELEMENT                        Element;
    typedef SizeType                       Index;
    typedef std::pair<Index,Element>       Pair; 
    typedef std::list<Pair>                List;
    typedef typename List::const_iterator  ConstIterator;
    typedef typename List::iterator  Iterator;

    typedef Legolas::MicroParserHelper<std::string> MPH;

  private:
    typedef std::list<Element>                 FlatList;
    typedef typename FlatList::const_iterator  FlatListConstIterator;    
  public:
    
    CompressedVector( void ):size_(0),
			     list_(){
      MESSAGE("Legolas::CompressedVector Default Ctor");
    }
    
    CompressedVector(const CompressedVector & source):size_(source.size_),
						      list_(source.list_)
    {
     INFOS("Legolas::CompressedVector Copy Ctor");
    }

    CompressedVector(const std::list<Element> & flatList):size_(0),
							  list_()
    {
      MESSAGE("Legolas::CompressedVector FlatList Ctor");
      this->appendFlatList(flatList);
    }
    
    CompressedVector(const Index & n, 
		     const Element & element):size_(0),
					      list_()
    {
      MESSAGE("Legolas::CompressedVector n x Element Ctor");
      this->appendNElement(n,element);
    } 

    bool isAList(const std::string & chaine) const {
      return (MPH::doesBeginWith(chaine,"(") && MPH::doesEndWith(chaine,")") ); 
    }


    CompressedVector(const std::string & chaine):size_(0),
						 list_(){

      typedef typename MPH::StringToElementCtor<Element> SE;

      ASSERT(isAList(chaine));
      
      std::string s(chaine);

      MPH::eraseBeginning(s,"(");
      MPH::eraseEnding(s,")");

      while (s.size()>0){

	Index n=MPH::stringToUI(MPH::extractFirstUnsignedInteger(s));
	MPH::eraseBeginning(s,"x");

	if ( MPH::doesBeginWith(s,"(") ){

	  const std::string subString=MPH::extractFirstList(s);
	  Element e(SE::apply(subString));

	  this->appendNElement(n,e);
	}
	else{
	  const std::string subString=MPH::extractFirstUnsignedInteger(s);

	  Element e(SE::apply(subString));
	  this->appendNElement(n,e);
	}

	if (s.size()>0) MPH::eraseBeginning(s,",");

      }
    }
	    

    void appendNNewElement(const Index n, 
			   const Element & element){
      list_.push_back(Pair(n,element));
      size_+=n;
    }

    void appendNewElement(const Element & element){
      this->appendNNewElement(1,element);
    }

    void appendNElement(const Index & n,
			const Element & element){
      if (size_==0){
	this->appendNNewElement(n,element);
      }
      else{
	typename List::reverse_iterator iter=list_.rbegin();
	const Element & oldValue=iter->second;
	if (oldValue==element){
	  iter->first+=n;
	  size_+=n;
	}
	else{
	  this->appendNNewElement(n,element);
	}
      }
      return;
    }

    void appendElement(const Element & element){
      this->appendNElement(1,element);
    }
	
    void appendFlatList(const std::list<Element> & flatList){
      FlatListConstIterator iter=flatList.begin();
      for ( ; iter!=flatList.end() ; iter++){
	this->appendElement(*iter);
      }
    }      


    bool operator == (const CompressedVector & right) const {
      
      bool isEqual=false;

      if (this->size_==right.size_){

	const List & lL=this->list_;
	const List & lR=right.list_;
	
	if (lL.size()==lR.size()){

	  isEqual=true;

	  ConstIterator iterL=lL.begin();
	  ConstIterator iterR=lR.begin();

	  const ConstIterator iterLEnd=lL.end();

	  while ( isEqual && ( iterL!=iterLEnd) ){
	    const bool firstEqual=(*iterL).first==(*iterR).first;
	    const bool secondEqual=(*iterL).second==(*iterR).second;
	    
	    isEqual=firstEqual && secondEqual ;
	    
	    iterL++;
	    iterR++;
	  }
	}
      }
      
      return isEqual;
    }


    bool operator < (const CompressedVector & right) const {
      
      bool isEqual=true;
      bool isSmaller=false;

      //lexicographic order
      
      if ( (this->size_==0) && (right.size_!=0) ) isSmaller=true;

      if (!isSmaller){

	const List & lL=this->list_;
	const List & lR=right.list_;

	ConstIterator iterL=lL.begin();
	ConstIterator iterR=lR.begin();

	const ConstIterator iterLEnd=lL.end();
	const ConstIterator iterREnd=lR.end();
	
	while ( isEqual && (iterL!=iterLEnd) && (iterR!=iterREnd) ){
	  
	  const bool firstEqual=(*iterL).first==(*iterR).first;
	  const bool secondEqual=(*iterL).second==(*iterR).second;
	  
	  isEqual=firstEqual && secondEqual ;
	  
	  iterL++;
	  iterR++;
	}
	
	if (!isEqual){
	  isSmaller=((*iterL).first < (*iterR).first);
	  if (!isSmaller){
	    isSmaller=((*iterL).second < (*iterR).second);
	  }
	}
      }
      
      return isSmaller;
    }



    bool operator != (const CompressedVector & right) const {
      return (!(this->operator == (right)));
    }



    Index size ( void ) const { return size_ ; }

    const Element & operator[]( Index i) const {
      
      ASSERT(i<size_);

      ConstIterator iter=list_.begin();
      Index j=iter->first;
      while (j<=i){
	iter++;
	j+=iter->first;
      }
      //i<j
      return iter->second;
    }
    
    const List & getList( void ) const { return list_ ;}

    
  private:
    Index size_;
    List list_;
  };


  //////////////////////////////////////////////////////////////////

  template <class ELEMENT> 
  std::ostream& operator<< ( std::ostream& os, 
			     const CompressedVector<ELEMENT> & source )
  {
    typedef typename CompressedVector<ELEMENT>::List          List;
    typedef typename CompressedVector<ELEMENT>::ConstIterator ConstIterator;
    
    const List & l=source.getList();
    ConstIterator iter=l.begin();
    // Opening
    os << "(" ; 
    // First Item
    if (iter !=l.end()){
      os << iter->first << "x" << iter->second ;
      iter++;
    }
    // Next Items
    for ( ; iter !=l.end() ; iter++){
      os << "," ;
      os << iter->first << "x" << iter->second ;
    }
    // Closing
    os << ")" ; 
    
    return os;
  }
  
}




#endif
