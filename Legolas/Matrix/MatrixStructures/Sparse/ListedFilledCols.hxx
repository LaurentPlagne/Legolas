#ifndef __LEGOLAS_LISTEDFILLEDCOLS__
#define __LEGOLAS_LISTEDFILLEDCOLS__

#include "Legolas/Matrix/MatrixStructures/Sparse/ListedFilledRows.hxx"

//FIXME L.P. Adapted Copy/Paste of ListedFilledRows. Should share the same code

namespace Legolas{

   class ListedFilledCols{
  private:
    typedef std::list< std::pair<int,int> > List;
    typedef std::map<int, std::set<int> >   Data;
  public:
    template <class MATRIX>
    ListedFilledCols( const MATRIX & matrix){
      
      const List & myList=matrix.getFilledElementList();
      List::const_iterator iter=myList.begin();
      List::const_iterator theEnd=myList.end();
      for ( ; iter!=theEnd ; iter++){
	
	int i=(*iter).first;
	int j=(*iter).second;
	
	assert(i>=0); assert(i<matrix.nrows());
	assert(j>=0); assert(j<matrix.ncols());
	
	filledElements_[j].insert(i);
      }
    }

    class FilledRows{
    public:
      typedef std::set<int> Data;
      
      FilledRows(const Data & data): data_(data){}
	
      inline int begin( void ) const { 
	//	INFOS("begin"<<*(data_.begin()));
	return *(data_.begin());
      }
      inline int end( void ) const {
	Data::const_iterator endPtr=data_.end();
	//	int postlast=(*endPtr);
	//	INFOS("postlast="<<postlast);
	endPtr--;
	int last=(*endPtr);
	return last+1;
      }
      
      inline void next( int & i ) const {
	Data::const_iterator iPtr=data_.find(i);
	//	INFOS("i="<<i);
	      
	assert(iPtr!=data_.end());  //cette colonne doit exister
      
	iPtr++;
	if (iPtr!=data_.end()){ //la colonne suivante existe
	  i=*iPtr;
	}
	else{// C'etait la dernière
	  i++;
	}

	return;
      }
      
    private:
      const Data & data_;
    };
    
    inline FilledRows filledRowsInCol( int colIndex )
    {
      Data::const_iterator resultPtr=filledElements_.find(colIndex);
      assert(resultPtr!=filledElements_.end());
      const std::set<int> & cols=(*resultPtr).second;
      return FilledRows(cols);
    }
    
    
    inline int begin( void ) const { return (*filledElements_.begin()).first; }
    inline int end( void ) const { return (*filledElements_.rbegin()).first+1; }
    inline void next( int & i ) const {
      //      INFOS("i="<<i);
      Data::const_iterator iPtr=filledElements_.find(i);

      assert(iPtr!=filledElements_.end());  //cette ligne doit exister
      
      iPtr++;
      if (iPtr!=filledElements_.end()){ //la ligne suivante existe
	i=(*iPtr).first;
      }
      else{
	i++;
      }
      return;
    }


  public:
    Data filledElements_;
  };
  

}

#endif
      
