/**
 * project  DESCARTES
 *
 * @file     BandedCCompactMatrixContainer.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_BANDEDCCOMPACTMATRIXCONTAINER_HXX__
#define __LEGOLAS_BANDEDCCOMPACTMATRIXCONTAINER_HXX__

#include "UTILITES.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicEngine.hxx"
#include "Legolas/Matrix/MatrixStructures/MatrixBasicContainer/MatrixBasicData.hxx"

namespace Legolas{

  class BandedCCompactMatrixContainer
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
      typedef typename VectorContainer::template Engine<Vector>      MatrixInterface;
      typedef typename MatrixInterface::Vector                       Matrix;

      class Data : public MatrixBasicData<ELEMENT_DATA_DRIVER>{
    
      public:
    
	Data( void ):linf_(0),lsup_(0),matrix_()
	{
	  MESSAGE("Compact::Data Default Ctor");
	}
    
	inline int & linf( void ) { return linf_ ;}
	inline int linf( void ) const { return linf_ ;}
	inline int & lsup( void ) { return lsup_ ;}
	inline int lsup( void ) const { return lsup_ ;}
	inline Matrix & matrix( void ) { return matrix_ ;}
	inline const Matrix & matrix( void ) const { return matrix_ ;}
      
      private:
      
	int linf_;
	int lsup_;
	Matrix matrix_;
      };

      template <int LEVEL>
      static inline void resize(const MatrixShape<LEVEL> & shape, int linf, int lsup, Data & data){
	//      static inline void resize(int nrows, int linf , int lsup, Data & data){

	data.setColShape()=shape.getColShape();
	data.setRowShape()=shape.getRowShape();
	const int nrows=shape.nrows();
      
	data.linf()=linf;
	data.lsup()=lsup;
	int width=linf+lsup+1;
      
	MatrixInterface::resize(nrows,data.matrix());
    
	for (int i=0 ; i < nrows ; i++){
	  VectorInterface::resize(width,(data.matrix())[i]);
	}
      }
    
      static inline ElementData & bandedGetElement(int row, int col, Data & data ){
	int I=row;
	int J=col-row+data.linf();
	return VectorInterface::getElement(J,MatrixInterface::getElement(I,data.matrix()));
      }

      static inline const ElementData & bandedGetElement(int row, int col, const Data & data ) {
	int I=row;
	int J=col-row+data.linf();
	return VectorInterface::getElement(J,MatrixInterface::getElement(I,data.matrix()));
      }
    
      static inline int lsup( const Data & data ) {
	return data.lsup();
      }
    
      static inline int & lsup( Data & data ) {
	return data.lsup();
      }
    
      static inline int linf( const Data & data ) {
	return data.linf();
      }
    
      static inline int & linf( Data & data ) {
	return data.linf();
      }
    
    };
  };



}

#endif
  

  
