/**
 * project  DESCARTES
 *
 * @file     AMatrixDefinition.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_AMATRIXDEFINITION_HXX__
#define __LEGOLAS_AMATRIXDEFINITION_HXX__

#include "Legolas/Matrix/MatrixStructures/MatrixStructureTags.hxx"
#include <vector>
#include "Legolas/Matrix/Helper/DefaultPrecision.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"

namespace Legolas{

  template <class REAL_TYPE>
  class AMatrixDefinition : public DefaultPrecision<REAL_TYPE>{
  public:
    //  Types that must be defined to model the MATRIX_DEFINITION concept 

    typedef Legolas::Dense                           MatrixStructure;
    typedef REAL_TYPE                                RealType;

    typedef std::vector< std::vector< RealType > >   V2D;

    class Data :  public Legolas::MatrixShape<1>
    {
    public:
      Data(const V2D & source):Legolas::MatrixShape<1>(source.size(),0),
			       data_(source){
	if (source.size()!=0) this->setColShape()=source[0].size();
      }
      
      REAL_TYPE operator()(int i, int j) const { return data_[i][j] ;}
      
    private:
      const V2D & data_;
    };
      


    typedef RealType                                 GetElement;

    //  function to be defined to model the MATRIX_DEFINITION concept 

    static inline GetElement getElement( int i , int j, const Data & data) {
      return data(i,j);
    }
  
  };

}


#endif
    

									     

