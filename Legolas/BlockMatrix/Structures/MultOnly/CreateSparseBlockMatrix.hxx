#pragma once

#include "Legolas/BlockMatrix/Matrix.hxx"
#include "Legolas/Matrix/MatrixShape.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseScalarMatrix.hxx"
#include "Legolas/BlockMatrix/Structures/Sparse/SparseBlockMatrix.hxx"

#include "stringConvert.hxx"

namespace Legolas{

  template <class SCALAR, int LEVEL>
  struct CreateSparseBlockMatrix{
    
    static Matrix * apply(int ibase, int jbase,
			  const  Legolas::SparseScalarMatrix<SCALAR> & scalarSparseMatrix,
			  Legolas::MatrixShape<LEVEL> matrixShape){
      
      //      INFOS("ICI LEVEL="<<LEVEL);
      // INFOS("ibase="<<ibase<<" jbase="<<jbase);
      // INFOS("matrixShape="<<matrixShape);

      //      INFOS("*********************************** ICI***************************");
      
      Legolas::SparseBlockMatrix * result= new Legolas::SparseBlockMatrix(matrixShape);

      int isE=ibase;
      int jsE=jbase;


      // INFOS("result->nrows()="<<result->nrows());
      // INFOS("result->ncols()="<<result->ncols());

      for (int i=0 ; i<result->nrows() ; i++){
	const int isB=isE;
	isE=isB+matrixShape.getRowShape()[i].flatSize();
	jsE=jbase;
	  
	for (int j=0 ; j<result->ncols() ; j++){

	  // INFOS("i"<<stringConvert(LEVEL)<<"="<<i);
	  // INFOS("j"<<stringConvert(LEVEL)<<"="<<j);

	  //	  std::string istring("i="); istring+=stringConvert(LEVEL);
	  //	  std::string jstring("j="); jstring+=stringConvert(LEVEL);

	  

	  const int jsB=jsE;
	  jsE=jsB+matrixShape.getColShape()[j].flatSize();
	  
	  //	  INFOS("isB="<<isB<<" isE="<<isE);

	  bool isNull=true;

	  //	  INFOS("jsB="<<jsB<<" jsE="<<jsE);

	  for (int is=isB ; is<isE ; is++){


	    int js=scalarSparseMatrix.beginColInRow(is);

	    //	    INFOS("is="<<is<<" js="<<js);

	    while ( (js<scalarSparseMatrix.endColInRow(is)) && isNull ){
	      if ((js>=jsB)&&(js<jsE)){
		isNull=false;
	      }
	      else{
		js=scalarSparseMatrix.nextColInRow(is,js);
	      }
	    }

	  }//is
	    
	  if (!isNull){

	    // INFOS("is="<<is<<" js="<<js);
	    
	    // INFOS("FFFFFFFFFFOUND");

	    // INFOS("i"<<stringConvert(LEVEL)<<"="<<i);
	    // INFOS("j"<<stringConvert(LEVEL)<<"="<<j);
	    
	    
	      
	    Matrix * block=Legolas::CreateSparseBlockMatrix<SCALAR,LEVEL-1>::apply(isB,jsB,scalarSparseMatrix,matrixShape.getSubMatrixShape(i,j));
	    
	    //	    INFOS("block="<<block);
	    //	    INFOS("LEVEL="<<LEVEL<<"i="<<i<<" j="<<j);
	    //	      exit(-1);
	    
	    result->setSparseElementPtr(i,j,block);
	  }

	  // INFOS("OUT BLOCK IJ:"<<i<<" "<<j);
	  
	}//j
      }//i

      return result;

    }//apply

  };

  template <class SCALAR>
  struct CreateSparseBlockMatrix<SCALAR,1>{
    
    static Matrix * apply(int ibase, int jbase,
			  const  Legolas::SparseScalarMatrix<SCALAR> & scalarSparseMatrix,
			  Legolas::MatrixShape<1> matrixShape){

      // INFOS("ICI");
      
      Legolas::SparseScalarMatrix<SCALAR> * result= new Legolas::SparseScalarMatrix<SCALAR>(matrixShape);
      
      const int isB=ibase;
      const int isE=ibase+matrixShape.getRowShape().flatSize();
      
      const int jsB=jbase;
      const int jsE=jbase+matrixShape.getColShape().flatSize();

      // INFOS("isB="<<isB<<" isE="<<isE);
      // INFOS("jsB="<<isB<<" jsE="<<isE);

      // bool found=false;

      
      for (int is=isB ; is<isE ; is++){

        int js=scalarSparseMatrix.beginColInRow(is);

	// INFOS("is="<<is<<" js="<<js);

	while (js<scalarSparseMatrix.endColInRow(is)){
	  if ((js>=jsB)&&(js<jsE)){
	    // INFOS("FOUND");
	    // found=true;
	    // INFOS("scalarSparseMatrix.sparseGetElement(is,js)="<<scalarSparseMatrix.sparseGetElement(is,js));
	    // INFOS("is-isB="<<is-isB);
	    // INFOS("js-jsB="<<js-jsB);
	    result->setSparseElement(is-isB,js-jsB,scalarSparseMatrix.sparseGetElement(is,js));
	  }
	  js=scalarSparseMatrix.nextColInRow(is,js);
	}
	
      }

      // if (!found){
      // 	INFOS("ERREUR");
      // 	exit(1);
      // }

      // INFOS("OUT SCALAR");
	    
      return result;
      
    }//apply

  };
  
}
  
