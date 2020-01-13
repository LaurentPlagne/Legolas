/**
 * project  DESCARTES
 *
 * @file     TriDiagonalMatrixContainer.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_TRIDIAGONALMATRIXCONTAINER_HXX__
#define __LEGOLAS_TRIDIAGONALMATRIXCONTAINER_HXX__

#include "UTILITES.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicEngine.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicData.hxx"

namespace Legolas{

  class TriDiagonalMatrixContainer 
  {
  public:

    template < class MATRIX_OPTIONS, class ELEMENT_DATA_DRIVER>
    class Engine : public MatrixBasicEngine<ELEMENT_DATA_DRIVER>
    {
    public:

      typedef typename ELEMENT_DATA_DRIVER::Data                      ElementData;
      typedef typename ELEMENT_DATA_DRIVER::RealType                  RealType;
      typedef typename MATRIX_OPTIONS::VectorContainer                VectorContainer;
      typedef typename VectorContainer::template Engine<ElementData>  VectorInterface;
      typedef typename VectorInterface::Vector                        Vector;
    
      class Data : public MatrixBasicData<ELEMENT_DATA_DRIVER>{
      
      public:
      
	Data( void ):lower_(),diagonal_(),upper_()
	{
	  MESSAGE("Compact::Data Default Ctor");
	}
      
	inline Vector & lower( void ) { return lower_ ;}
	inline const Vector & lower( void ) const { return lower_ ;}
	inline Vector & diagonal( void ) { return diagonal_ ;}
	inline const Vector & diagonal( void ) const { return diagonal_ ;}
	inline Vector & upper( void ) { return upper_ ;}
	inline const Vector & upper( void ) const { return upper_ ;}
    
      private:
      
	Vector lower_;
	Vector diagonal_;
	Vector upper_;
      
      };
    
      static inline void resize(int nrows, Data & data){
      
	data.nrows()=nrows;
	data.ncols()=nrows;
      
	VectorInterface::resize(nrows,data.lower());
	VectorInterface::resize(nrows,data.diagonal());
	VectorInterface::resize(nrows,data.upper());
      
      }

      static inline ElementData & diagonalGetElement(int row, Data & data){
	return VectorInterface::getElement(row,data.diagonal());
      }
    
      static inline  const ElementData & diagonalGetElement(int row, const Data & data){
	return VectorInterface::getElement(row,data.diagonal());
      }
    
      static inline ElementData & upperDiagonalGetElement(int row, Data & data){
	return VectorInterface::getElement(row,data.upper());
      }
    
      static inline  const ElementData & upperDiagonalGetElement(int row, const Data & data){
	return VectorInterface::getElement(row,data.upper());
      }
    
      static inline ElementData & lowerDiagonalGetElement(int row, Data & data){
	return VectorInterface::getElement(row,data.lower());
      }
    
      static inline  const ElementData & lowerDiagonalGetElement(int row, const Data & data){
	return VectorInterface::getElement(row,data.lower());
      }
    
    };
  };



}

#endif
  

  
