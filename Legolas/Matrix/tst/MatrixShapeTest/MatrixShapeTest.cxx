/**
 * project  DESCARTES
 *
 * @file     DenseMatrixTest.cxx
 *
 * @author Laurent PLAGNE
 * @date   september 2005
 *
 * @par Modifications
 * - author date object
 *   
 * (c) Copyright EDF R&D - CEA 2001-2005 
 */
# include <cstdlib>
# include <cmath>
# include <iostream>

# include "UTILITES.hxx" 
# include "Legolas/Vector/Vector.hxx"

# include "Legolas/Matrix/MatrixOptions/InputMatrixOptions.hxx"
# include "Legolas/Matrix/RealElement/RealElementInterface.hxx"
# include "Legolas/Matrix/MatrixInterface/GenericMatrixInterface.hxx"
# include "Legolas/Matrix/MatrixVectorOperations.hxx"

# include "AMatrixDefinition.hxx"
# include <vector>


using namespace std; using namespace Legolas;
using namespace Legolas;

int main( int argc,  char *argv[] )
{
  Legolas::MatrixShape<2>::Shape rowShape(3,2);
  Legolas::MatrixShape<2>::Shape colShape(2,3);
  
  Legolas::MatrixShape<2> matrixShape(rowShape,colShape);

  INFOS("matrixShape="<<matrixShape);

  
  Legolas::VirtualMatrixShape & vms=matrixShape;

  INFOS("vms.nrows()="<<vms.nrows());
  INFOS("vms.ncols()="<<vms.ncols());

  Legolas::VirtualMatrixShape * vms00Ptr=matrixShape.getSubMatrixShapePtr(0,0);

  Legolas::VirtualMatrixShape & vms00=*vms00Ptr;

  INFOS("vms00.nrows()="<<vms00.nrows());
  INFOS("vms00.ncols()="<<vms00.ncols());

  delete vms00Ptr ;


//   const Legolas::MatrixShape<1> & ms00=static_cast<const Legolas::MatrixShape<1> &>(vms00);

//   INFOS("ms00="<<ms00);
  
}
