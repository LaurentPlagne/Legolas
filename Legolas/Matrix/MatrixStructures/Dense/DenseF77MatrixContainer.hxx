/**
 * project  DESCARTES
 *
 * @file     DenseF77MatrixContainer.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_DENSEF77MATRIXCONTAINER_HXX__
#define __LEGOLAS_DENSEF77MATRIXCONTAINER_HXX__

// INSTANCE of the MATRIX_STORAGE_CONCEPT

#include "UTILITES.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicEngine.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicData.hxx"

namespace Legolas{

  class DenseF77MatrixContainer
  {
  public:

    template < class MATRIX_OPTIONS, class ELEMENT_DATA_DRIVER>
    class Engine : public MatrixBasicEngine<ELEMENT_DATA_DRIVER>
    {
    public:

      typedef typename ELEMENT_DATA_DRIVER::Data                       ElementData;
      typedef typename ELEMENT_DATA_DRIVER::RealType                   RealType;
      typedef typename MATRIX_OPTIONS::VectorContainer                 VectorContainer;
      typedef typename VectorContainer::template Engine<ElementData>   VectorInterface;
      typedef typename VectorInterface::Vector                         Vector;

      class Data : public MatrixBasicData<ELEMENT_DATA_DRIVER>{
      public:
	Data( void ):matrix_()
	{
	  MESSAGE("F77::Data Default Ctor");
	}
// 	Data(int nrows, int ncols, const Vector & initMatrix):MatrixBasicData<ELEMENT_DATA_DRIVER>(nrows,ncols,100,1.e-10),matrix_(initMatrix)
// 	{
// 	  MESSAGE("F77::Data Standart Ctor");
// 	}
	inline Vector & matrix( void ) { return matrix_ ;}
	inline const Vector & matrix( void ) const { return matrix_ ;}

      private:
	Vector matrix_ ;
      };
    
      static inline 
      ElementData & getElement(int i, int j, Data & A) {
	int I=A.ncols()*i+j;
	return VectorInterface::getElement(I,A.matrix());
      } 
    
      static inline 
      const ElementData & getElement(int i, int j, const Data & A) {
	int I=A.ncols()*i+j;
	return VectorInterface::getElement(I,A.matrix());
      } 
    
      template <int LEVEL>
      static inline void resize(const MatrixShape<LEVEL> & shape, Data & A){
	A.setColShape()=shape.getColShape();
	A.setRowShape()=shape.getRowShape();
	VectorInterface::resize(shape.nrows()*shape.ncols(),A.matrix());
      }
    
    };

  };


}

#endif
  

  
