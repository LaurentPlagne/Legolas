/**
 * project  DESCARTES
 *
 * @file     ActualFromVirtualMatrixInterface.hxx
 *
 * @author Laurent PLAGNE
 * @date   june 2004 - january 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
#ifndef __LEGOLAS_ACTUALFROMVIRTUALMATRIXINTERFACE_HXX__
#define __LEGOLAS_ACTUALFROMVIRTUALMATRIXINTERFACE_HXX__

#include "Legolas/Matrix/VirtualMatrix/VirtualMatrixInterface.hxx"
#include "Legolas/Matrix/ActualMatrix/ActualMatrixInterface.hxx"
#include "Legolas/Matrix/ActualMatrix/PureVirtualMatrixInterfaceTraits.hxx"
#include "Legolas/Matrix/MatrixOptions/DefaultMatrixOptions.hxx"



namespace Legolas{
  
  template <class MATRIX_DEFINITION, class MATRIX_OPTIONS, class MATRIX_ELEMENT_INTERFACE, bool IsTransposed=false>
  class ActualFromVirtualMatrixInterface{
  public:
    typedef MATRIX_DEFINITION              MatrixDefinition;
  private:
    typedef MATRIX_ELEMENT_INTERFACE       MatrixElementInterface;
    typedef MATRIX_OPTIONS                 MatrixOptions;

    //typedef VirtualMatrixInterface<MatrixDefinition,MatrixOptions,MatrixElementInterface,IsTransposed> VMI;
  
    typedef typename PureVirtualMatrixInterfaceTraits<MatrixElementInterface>::Type PureVirtualMatrixElementInterface;
    typedef DefaultMatrixOptions<typename MatrixDefinition::MatrixStructure,Legolas::InputMatrixOptions<Legolas::Virtual> > PureVirtualMatrixOptions; 
    typedef VirtualMatrixInterface<MatrixDefinition,PureVirtualMatrixOptions,PureVirtualMatrixElementInterface,IsTransposed> VMI;

    typedef ActualMatrixInterface<MatrixOptions,MatrixElementInterface,IsTransposed>    AMI;
  
    class ActualFromVirtualMatrix : public AMI::Matrix
    {
    private:
      typedef typename AMI::Matrix BaseClass;
      typedef typename BaseClass::Data ActualData;
      typedef typename VMI::Matrix VirtualMatrix;
      typedef typename VirtualMatrix::Data VirtualData;
    
    public:
      ActualFromVirtualMatrix( void ):BaseClass(ActualData()){
	MESSAGE("ActualFromVirtualMatrix Data Default Ctor");
      }

      ActualFromVirtualMatrix(const ActualData & init):BaseClass(init){
	MESSAGE("ActualFromVirtualMatrix Data Ctor");
      }

      ActualFromVirtualMatrix(const typename AMI::Matrix & source):BaseClass(source){
      	MESSAGE("ActualFromVirtualMatrix Copy Ctor");
      }

      ActualFromVirtualMatrix( const VirtualData & init):BaseClass(VirtualMatrix(init)){
	MESSAGE("ActualFromVirtualMatrix Ctor via VirtualMatrix");
      }
    };
  public:

    typedef typename AMI::View       View;
    typedef typename AMI::ConstView  ConstView;

//     typedef typename AMI::TransposedView       TransposedView;
//     typedef typename AMI::TransposedConstView  TransposedConstView;

    typedef typename AMI::DataDriver DataDriver;

    typedef ActualFromVirtualMatrix  Matrix;
    typedef Matrix                   Copy;

    typedef typename Matrix::Solver Solver;
    
    //    static bool isAMatrix( void ) { return true ; }
    static const bool isAMatrix_=true;
  };
  
} //end of namespace Legolas

#endif


  
