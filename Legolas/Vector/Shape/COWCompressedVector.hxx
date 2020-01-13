#ifndef __LEGOLAS_COWCOMPRESSEDVECTOR_HXX__
#define __LEGOLAS_COWCOMPRESSEDVECTOR_HXX__

#include "UTILITES.hxx"
#include "Legolas/Vector/Shape/CompressedVector.hxx"

//#include <memory>
#include <memory>
#include "stringConvert.hxx"
#include "Legolas/Vector/Backtrace.hxx"
#include "Legolas/Vector/Shape/L1Shape.hxx"
#include <string>
#include <fstream>


namespace Legolas{

  // std::ifstream && myOpenFile(std::string filename){
  //   std::ifstream inputFile(filename.c_str(),std::ios::in | std::ios::binary );
  //   return inputFile;
  // }
  // __attribute__((no_sanitize_memory))
  // inline auto getCString(std::string filename){
  //   return filename.c_str();
  // }


  template <class ELEMENT>
  class COWCompressedVector : public VirtualVectorShape {
  private:

    typedef CompressedVector<ELEMENT> CVE;
    //    typedef std::shared_ptr<CVE>    CVEPtr;
    typedef std::shared_ptr<float>    Toto;
    typedef std::shared_ptr<CVE>    CVEPtr;

    CVEPtr ptr_;

  public:
    typedef ELEMENT                        Element;
  private:

    typedef SizeType                       Index;
    typedef std::pair<Index,Element>       Pair; 

  public:


    typedef std::list<Pair>                List;
    typedef typename List::const_iterator  ConstIterator;

    static const int Level=Element::Level+1;


    int level( void ) const{
      return Level;
    }

     COWCompressedVector( void ):ptr_(new CVE())
    {
      MESSAGE("Legolas::COWCompressedVector Default Ctor");
    }

    COWCompressedVector(const COWCompressedVector & source):ptr_(source.ptr_)
    {
      MESSAGE("Legolas::COWCompressedVector Copy Ctor");
    }
  
    COWCompressedVector(const std::list<Element> & flatList):ptr_(new CVE(flatList))
    {
      MESSAGE("Legolas::COWCompressedVector FlatList Ctor");
    }

    COWCompressedVector(const Index & n, 
			const Element & element):ptr_(new CVE(n,element))
    {
      MESSAGE("Legolas::COWCompressedVector n x Element Ctor");
    } 

    COWCompressedVector(const std::string & chaine):ptr_(new CVE(chaine))
    {
      MESSAGE("Legolas::COWCompressedVector string Ctor");
    }

    CVE & getMutableRef( void ){
      //Copy On Write
      if (!ptr_.unique()){ ptr_.reset(new CVE(*ptr_)); }
      return *ptr_;
    }

    void clear( void ){
      ptr_.reset(new CVE());
    }

    const CVE & getConstRef( void ) const {
      return *ptr_;
    }

    void appendNElement(const Index & n,const Element & element){
      this->getMutableRef().appendNElement(n,element);
    }

    void appendElement(const Element & element){
      this->getMutableRef().appendNElement(1,element);
    }

    void appendFlatList(const std::list<Element> & flatList){
      this->getMutableRef().appendFlatList(flatList);
    }      

    bool operator == (const COWCompressedVector & right) const {
      return this->getConstRef().operator == (right.getConstRef());
    }

    bool operator < (const COWCompressedVector & right) const {
      return this->getConstRef().operator < (right.getConstRef());
    }

    bool operator != (const COWCompressedVector & right) const {
      return this->getConstRef().operator != (right.getConstRef());
    }

    Index size ( void ) const { return this->getConstRef().size() ; }

    Element operator[]( Index i) const {
      return this->getConstRef().operator[] (i);
    }
  
    const List & getList( void ) const { 
      return  this->getConstRef().getList();
    }

    inline size_t flatSize( void ) const {
      size_t result=0;
    
      const List & l=this->getList();
      ConstIterator iter=l.begin();
      
      for ( ; iter!=l.end(); iter++){
	//	result+=iter->first*Legolas::getFlatSize(iter->second);
	result+=iter->first*(iter->second.flatSize());
      }
      return result;
    }

    inline size_t subFlatSize( int index ) const { return (*this)[index].flatSize() ; }      

    void dump(const std::string filename) const {
      std::ofstream outfile(filename.c_str(),std::ios::out | std::ios::binary );
      outfile <<Level << std::endl;
      this->dumpElements(outfile);
      outfile.close();
    }

    void save(std::ostream & outfile) const {
      outfile <<Level << std::endl;
      this->dumpElements(outfile);
    }

    void dumpElements(std::ostream & outfile) const {
      const List & l=this->getList();
      ConstIterator iter=l.begin();
      
      outfile << l.size() << std::endl;

      for ( ; iter!=l.end(); iter++){
	outfile << (*iter).first << std::endl;
	(*iter).second.dumpElements(outfile);
      }
    }

    void load(std::istream & inputFile){
      this->clear();
      int l=0;
      if (inputFile >> l){
	assert(l==Level);
	this->readElements(inputFile);
      }
      else{
	INFOS(" error while reading ");
	throw std::runtime_error("Error while reading");
      }
    }
    
    void read(const std::string filename){
      this->clear();
      
      std::ifstream inputFile(filename.c_str(),std::ios::in | std::ios::binary );
      //      INFOS("ICI");
      //      auto cstr=getCString(filename);
      //      INFOS("ICI");
      //      std::ifstream inputFile=myOpenFile(filename);
      //      std::ifstream inputFile(cstr,std::ios::in | std::ios::binary );
      //      std::ifstream inputFile(cstr,std::ios::in | std::ios::binary );
      //      INFOS("ICI");
      int l=0;
      if (inputFile >> l){
	assert(l==Level);
	this->readElements(inputFile);
      }
      else{
	INFOS(" error while reading "<<filename);
	throw std::runtime_error("Error while reading");
      }
      inputFile.close();
    }


    void readElements(std::istream & inputFile){
      this->clear();
      uint listSize;
      if (inputFile >> listSize){
	for (uint i=0 ; i<listSize;i++){
	  SizeType first;
	  if (inputFile >> first){
	    Element second;
	    second.readElements(inputFile);
	    this->appendNElement(first,second);
	  }
	  else{
	    INFOS(" error in readElements");
	    throw std::runtime_error("Error is readElements");
	  }
	}
      }
      else{
	INFOS(" error in readElements");
	throw std::runtime_error("Error is readElements");
      }

    }


      
      
      
    
  };//End of the COWCompressedVector<> class
  
  /********************************************************************************/
  /********************************************************************************/
  /** Shape functions and operations 
   **/
  /********************************************************************************/
  /********************************************************************************/

  // template <class SIZE_TYPE> 
  // inline const SizeType & getSize(const SIZE_TYPE & s){
  //   return s;
  // }

  // template <class ELEMENT> 
  // inline SizeType getSize(const COWCompressedVector<ELEMENT> &  shape){
  //   return shape.size();
  // }


  // inline size_t getFlatSize(const L1Shape & s){
  //   return s.size();
  // }


  // template <class ELEMENT> 
  // inline size_t getFlatSize(const COWCompressedVector<ELEMENT> &  shape){

  //   size_t result=0;
    
  //   const typename COWCompressedVector<ELEMENT>::List & l=shape.getList();
  //   typename COWCompressedVector<ELEMENT>::ConstIterator iter=l.begin();

  //   for ( ; iter!=l.end(); iter++)
  //     result+=iter->first*Legolas::getFlatSize(iter->second);
    
  //   return result;
  // }



  // template <class SIZE_TYPE> 
  // inline size_t getSubFlatSize(const SIZE_TYPE & s, int index){
  //   return 1;
  // }

  // template <class ELEMENT> 
  // inline size_t getSubFlatSize(const COWCompressedVector<ELEMENT> &  shape, int index){
  //   //    INFOS("shape[index]="<<shape[index]);
  //   //    INFOS("getFlatSize(shape[index]="<<getFlatSize(shape[index]));
  //   return shape[index].flatSize();
  //   //    return Legolas::getFlatSize(shape[index]);
  // }


  


 //  template <class SIZE_TYPE> 
//   inline size_t getFlatDisplaySize(const SIZE_TYPE & s, int borderSize, int unitCellSize){
//     return 2*borderSize+unitCellSize;
//   }

//   template <class ELEMENT> 
//   inline size_t getFlatDisplaySize(const COWCompressedVector<ELEMENT> &  shape, int borderSize, int unitCellSize){

//     size_t result=0;
    
//     const typename COWCompressedVector<ELEMENT>::List & l=shape.getList();
//     typename COWCompressedVector<ELEMENT>::ConstIterator iter=l.begin();

//     for ( ; iter!=l.end(); iter++)
//       result+=iter->first*(2*borderSize+getFlatDisplaySize(iter->second,borderSize,unicetCellSize));
//     return result;
//   }


  template <class ELEMENT, class SIZE_TYPE, class VECTOR>
  inline void computeFlatIndexes(const COWCompressedVector<ELEMENT> &  shape, 
				 const SIZE_TYPE & I,
				 VECTOR & flatIndexes){
    

    
    const typename COWCompressedVector<ELEMENT>::List & l=shape.getList();
    typename COWCompressedVector<ELEMENT>::ConstIterator iter=l.begin();
	
    flatIndexes.resize(shape.size());

    SIZE_TYPE index=0;
    for ( ; iter!=l.end(); iter++){

      //      const SIZE_TYPE elementFlatSize=getFlatSize(iter->second);
      const SIZE_TYPE elementFlatSize=iter->second.flatSize();
      
      for (SizeType j=0 ; j< iter->first ; j++){
	if (index==0){
	  flatIndexes[index]=I;
	}
	else{
	  flatIndexes[index]=flatIndexes[index-1]+elementFlatSize;
	}
	index++;
      }
    }
  }


  inline std::ostream& display( std::ostream& os, 
				const L1Shape & source,std::string & shapeString){
    
    std::string subShapeString(shapeString);
    subShapeString+="[L";
    subShapeString+=stringConvert(1);
    subShapeString+="::";
    subShapeString+=stringConvert(0);
    //    if ((source-1)!=0){
      subShapeString+="->";
      subShapeString+=stringConvert(source.size()-1);
      //    }
    subShapeString+="]";
    
    os<<subShapeString<<std::endl;
    
    return os;
    
  }  


  // inline std::ostream& display( std::ostream& os, 
  // 				const int & source,std::string & shapeString){
    
  //   std::string subShapeString(shapeString);
  //   subShapeString+="[L";
  //   subShapeString+=stringConvert(1);
  //   subShapeString+="::";
  //   subShapeString+=stringConvert(0);
  //   //    if ((source-1)!=0){
  //     subShapeString+="->";
  //     subShapeString+=stringConvert(source-1);
  //     //    }
  //   subShapeString+="]";
    
  //   os<<subShapeString<<std::endl;
    
  //   return os;
    
  // }  




  template <class ELEMENT> 
  std::ostream& display ( std::ostream& os, 
			  const COWCompressedVector<ELEMENT> & source, std::string & shapeString){
    
    typedef typename COWCompressedVector<ELEMENT>::List          List;
    typedef typename COWCompressedVector<ELEMENT>::ConstIterator ConstIterator;
    

    const List & l=source.getList();
    ConstIterator iter=l.begin();

    //    const int Level=LevelTraits< COWCompressedVector<ELEMENT> >::Level;
    const int Level=COWCompressedVector<ELEMENT>::Level;

    if (iter==l.end()){
      std::string subShapeString;
      subShapeString+="[L";
      subShapeString+=stringConvert(Level);
      subShapeString+="::Empty Shape]";
      os<<subShapeString<<std::endl;
      //      display(os,iter->second,subShapeString);
    }
    else{

      int beginIndex=0;
    
      for ( ; iter !=l.end() ; iter++){
	std::string subShapeString;
	if (beginIndex==0){
	  subShapeString=shapeString;
	}
	else{
	  for (unsigned int i=0 ; i< shapeString.size() ; i++) subShapeString+=" ";
	}
	subShapeString+="[L";
	subShapeString+=stringConvert(Level);
	subShapeString+="::";
	subShapeString+=stringConvert(beginIndex);
	const int lastIndex=beginIndex+iter->first-1;
	//      if (lastIndex!=beginIndex){
	subShapeString+="->";
	subShapeString+=stringConvert(lastIndex);
	//      }
	subShapeString+="]";
	display(os,iter->second,subShapeString);
	beginIndex=lastIndex+1;
      }
    }
    
    return os;
    
  }  


  //////////////////////////////////////////////////////////////////

  template <class ELEMENT> 
  std::ostream& operator<< ( std::ostream& os, 
			     const COWCompressedVector<ELEMENT> & source )
  {
    os<<std::endl;
    std::string firstString("VShape: ");
    //    firstString+=stringConvert(Legolas::getFlatSize(source));
    firstString+=stringConvert(source.flatSize());
    firstString+=" ";
    display(os,source,firstString);
    
    return os;
  }

  template <class ELEMENT> 
  static inline void assertSameShapes(const COWCompressedVector<ELEMENT> & shapeLeft,const COWCompressedVector<ELEMENT> & shapeRight){
#if DK_HAS_DEBUG != 0    
    if (shapeLeft!=shapeRight){
      INFOS("shapeLeft="<<shapeLeft);
      INFOS("shapeRight="<<shapeRight);
      BACKTRACE
	throw std::runtime_error("");
    }
    assert(shapeLeft==shapeRight);
#endif
  }

  template <class INDEX_TYPE> 
  static void assertSameShapes(const INDEX_TYPE & shapeLeft,const INDEX_TYPE & shapeRight){
#if DK_HAS_DEBUG != 0    
    if (shapeLeft!=shapeRight){
      INFOS("shapeLeft="<<shapeLeft);
      INFOS("shapeRight="<<shapeRight);
      BACKTRACE
	throw std::runtime_error("");
    }
    assert(shapeLeft==shapeRight);
#endif
  }




  
}




#endif
