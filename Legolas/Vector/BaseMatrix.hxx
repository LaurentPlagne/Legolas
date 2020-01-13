#ifndef __LEGOLAS_BASEMATRIX_HXX__
#define __LEGOLAS_BASEMATRIX_HXX__

#include "UTILITES.hxx"
#include "BaseVector.hxx"
#include <iostream>
#include "Legolas/Matrix/MatrixStream.hxx"
#include "Legolas/Algorithm/LinearSolverWorkSpace.hxx"
#include "Legolas/Algorithm/AnySolver.hxx"
//#include "Legolas/Vector/VectorPool.hxx"


namespace Legolas{

  //  struct AnyVectorPool;

  struct AnyMatrix{
    

    inline virtual std::string getStructureName( void ) const {
      INFOS("getStructureName not yet implemented");
      throw std::runtime_error("getStructureName not yet implemented");
    }

    inline virtual void sparseRecursiveDisplayLatexMethod(std::ofstream & outfile, int flag=0) const {
      INFOS("sparseRecursiveDisplayLatexMethod not yet implemented");
      throw std::runtime_error("sparseRecursiveDisplayLatexMethod not yet implemented");
    }
    inline virtual void sparseRecursiveDisplayHTMLMethod(const std::string & filename, std::ofstream & outfile, const std::string * parentNamePtr=0, int flag=0) const{
      INFOS("sparseRecursiveDisplayHTMLMethod not yet implemented");
      throw std::runtime_error("sparseRecursiveDisplayHTMLMethod not yet implemented");
      
    }

    inline virtual void matrixVectorProduct(const Legolas::AnyVector & X, Legolas::AnyVector & Y, int addFlag) const {
      INFOS("matrixVectorProduct not yet implemented");
      throw std::runtime_error("matrixVectorProduct not yet implemented");
    };

    inline virtual void matrixVectorInverse(const Legolas::AnyVector & X, Legolas::AnyVector & Y) const {
      INFOS("matrixVectorInverse not yet implemented");
      throw std::runtime_error("matrixVectorInverse not yet implemented");
    };


    inline virtual void matrixVectorInverse(const Legolas::AnyVector & X, Legolas::AnyVector & Y, Legolas::AnyWorkSpace & ws) const {
      INFOS("matrixVectorInverse not yet implemented");
      throw std::runtime_error("matrixVectorInverse not yet implemented");
    };

    inline virtual void transposeMatrixVectorProduct(const Legolas::AnyVector & X, Legolas::AnyVector & Y, int addFlag) const {
      INFOS("transposeMatrixVectorProduct not yet implemented");
      throw std::runtime_error("transposeMatrixVectorProduct not yet implemented");
    };


    inline virtual Legolas::AnySolver * getSolverPtr(AnyVector & aX, const AnyVector & aB) const {
      INFOS("getLevel()="<<this->getLevel());
      INFOS("getStructureName()="<<this->getStructureName());
      INFOS("getSolverPtr(AnyVector & aX, const AnyVector & aB)");
      throw std::runtime_error("");
    };
    
    inline virtual Legolas::AnySolver * getSolverPtr(AnyVector & aX, const AnyVector & aB, void * avp) const {
      INFOS("getLevel()="<<this->getLevel());
      INFOS("getStructureName()="<<this->getStructureName());
      INFOS("getSolverPtr(AnyVector & aX, const AnyVector & aB)");
      throw std::runtime_error("");
    };
      

 //    virtual void transposeMatrixVectorInverse(const Legolas::AnyVector & X, Legolas::AnyVector & Y) const {
//       INFOS("transposeMatrixVectorInverse not yet implemented");
//       throw std::runtime_error("transposeMatrixVectorInverse not yet implemented");
//     };
    
 //    virtual void displayLatexMethod(std::string filename, int flag=0) const {
//       INFOS("displayLatex not yet implemented");
//       throw std::runtime_error("displayLatex not yet implemented");
//     }

    virtual void iterateOverBlocksMethod(Legolas::MatrixStream & aStream) const {
      INFOS("iterateOverBlocksMethod not yet implemented");
       throw std::runtime_error("iterateOverBlocksMethod not yet implemented");
    }


    virtual int getFlatRowSizeMethod( void ) const {
      INFOS("getFlatRowSizeMethod not yet implemented");
      return 0;
    }

    virtual int getFlatColSizeMethod( void ) const {
      INFOS("getFlatcolSizeMethod not yet implemented");
      return 0;
    }

    virtual int getLevel( void ) const {
      INFOS("level not yet implemented");
      return 0;
    }

//     virtual int & setMaxIteration( void ){
//       INFOS("setMaxIteration not yet implemented");
//       int i=0;
//       return i;
//     }

    virtual ~AnyMatrix( void ){} ;

    virtual AnyMatrix * clone( void ) const {
      INFOS("clone not yet implemented");
      return 0;
    }



//      virtual std::string getStructureName( void ) const = 0 ;

//     virtual void sparseRecursiveDisplayLatexMethod(std::ofstream & outfile, int flag=0) const =0 ;
//     virtual void sparseRecursiveDisplayHTMLMethod(const std::string & filename, std::ofstream & outfile, const std::string * parentNamePtr=0, int flag=0) const =0 ;
    

//     virtual void matrixVectorProduct(const AnyVector & X, AnyVector & Y) const = 0;
    
  };

 //  void displayLatex(const AnyMatrix & anyMatrix,
// 		    std::string filename, int flag=0)
//   {
//     anyMatrix.displayLatexMethod(filename,flag);
//   }
    


  template <class MATRIX>
  struct BaseMatrix : public AnyMatrix{

    typedef const MATRIX & CMR;
    
    CMR getCMR( void ) const {
      return static_cast<CMR>(*this);
    }

    BaseMatrix( void ){
      MESSAGE("BaseMatrix default Ctor" );    
    }
    virtual ~BaseMatrix( ) {
      MESSAGE("BaseMatrix default Dtor" );    
    }
  };

  template <class MATRIX>
  class TransposedBaseMatrix{
  public:


    TransposedBaseMatrix( const BaseMatrix<MATRIX> & matrix):data_(matrix){}
    
    const BaseMatrix<MATRIX> & getBaseMatrix( void ) const { return data_ ;}

  private:
    const BaseMatrix<MATRIX> & data_;
  };

}

#endif
