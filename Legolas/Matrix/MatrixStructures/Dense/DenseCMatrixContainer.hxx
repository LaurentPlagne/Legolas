/**
 * project  DESCARTES
 *
 * @file     DenseCMatrixContainer.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DENSECMATRIXCONTAINER_HXX__
#define __LEGOLAS_DENSECMATRIXCONTAINER_HXX__

// INSTANCE of the MATRIX_CONTAINER_CONCEPT

#include "UTILITES.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicEngine.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicData.hxx"

namespace Legolas{

  class DenseCMatrixContainer
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
      typedef typename VectorContainer::template Engine<Vector>       MatrixInterface;
      typedef typename MatrixInterface::Vector                        Matrix;
    
      class Data : public MatrixBasicData<ELEMENT_DATA_DRIVER>{
      public:
	Data( void ):matrix_()
	{
	  MESSAGE("C::Data Default Ctor");
	}
	inline Matrix & matrix( void ) { return matrix_ ;}
	inline const Matrix & matrix( void ) const { return matrix_ ;}
      
      private:
	Matrix matrix_ ;
      };
    
      static inline 
      ElementData & getElement(int i, int j, Data & A) {
	return A.matrix()[i][j];
      } 

      static inline 
      const ElementData & getElement(int i, int j,const Data & A) {
	return A.matrix()[i][j];
      } 
    
      template <int LEVEL>
      static inline void resize(const MatrixShape<LEVEL> & shape, Data & A){
	A.setColShape()=shape.getColShape();
	A.setRowShape()=shape.getRowShape();
	const int nrows=shape.nrows();
	const int ncols=shape.ncols();

	MatrixInterface::resize(nrows,A.matrix());
	for (int i=0 ; i < nrows ; i++){
	  VectorInterface::resize(ncols,A.matrix()[i]);
	}
	
      }

    };
    
  };


}

#endif
  

  
