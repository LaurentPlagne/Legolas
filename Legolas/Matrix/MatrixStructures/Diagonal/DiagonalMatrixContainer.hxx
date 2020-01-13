/**
 * project  DESCARTES
 *
 * @file     DiagonalMatrixContainer.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DIAGONALMATRIXCONTAINER_HXX__
#define __LEGOLAS_DIAGONALMATRIXCONTAINER_HXX__

#include "UTILITES.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicEngine.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicData.hxx"

namespace Legolas{

  class DiagonalMatrixContainer 
  {
  public:

    template < class MATRIX_OPTIONS, class ELEMENT_DATA_DRIVER>
    class Engine : public MatrixBasicEngine<ELEMENT_DATA_DRIVER>
    {
    public:

      typedef typename ELEMENT_DATA_DRIVER::Data                     ElementData;
      typedef typename ELEMENT_DATA_DRIVER::RealType                 RealType;
      typedef typename MATRIX_OPTIONS::VectorContainer               VectorContainer;
      typedef typename VectorContainer::template Engine<ElementData> VectorInterface;
      typedef typename VectorInterface::Vector                       Vector;
    
      class Data : public MatrixBasicData<ELEMENT_DATA_DRIVER>{
      
      public:
      
	Data( void ):diagonal_()
	{
	  MESSAGE("DiagonalMatrixContainer.. Ctor");
	}
      
	inline Vector & diagonal( void ) { return diagonal_ ;}
	inline const Vector & diagonal( void ) const { return diagonal_ ;}

	inline void push_back(const ElementData & inputElement){
	  diagonal_.push_back(inputElement);
	}
      
      private:
      
	int nrows_;
	Vector diagonal_;
      
      };
    
      template <int LEVEL>
      static inline void resize(const MatrixShape<LEVEL> & shape, Data & data){
	data.setColShape()=shape.getColShape();
	data.setRowShape()=shape.getRowShape();
	VectorInterface::resize(shape.nrows(),data.diagonal());
      }


      static inline ElementData & diagonalGetElement(int row, Data & data){
	return VectorInterface::getElement(row,data.diagonal());
      }
    
      static inline  const ElementData & diagonalGetElement(int row, const Data & data){
	return VectorInterface::getElement(row,data.diagonal());
      }
    
    };
  };



}

#endif
  

  
