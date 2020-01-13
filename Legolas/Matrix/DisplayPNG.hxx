#ifndef __LEGOLAS_DISPLAYPNG_HXX__
#define __LEGOLAS_DISPLAYPNG_HXX__

#include "PNGMatrixStream.hxx"

namespace Legolas{

  void inline displayPNG(const AnyMatrix & anyMatrix,
			   std::string filename, int flag=0)
  {
    INFOS("filename="<<filename);
    const int MAXDISP=4000; 
    
    const int frs=anyMatrix.getFlatRowSizeMethod();
    const int fcs=anyMatrix.getFlatColSizeMethod();

    int borderSize=1;
    int unitCellSize=5;
    
    if ( (frs>MAXDISP) || (frs>MAXDISP) ){
      INFOS("Display is skipped because the matrix is too large ("<<frs<<"x"<<fcs<<")");
    }
    else{
      Legolas::PNGMatrixStream pms(filename,flag,anyMatrix.getLevel(),frs,fcs,borderSize,unitCellSize);
      anyMatrix.iterateOverBlocksMethod(pms);
    }
  }
}

#endif
