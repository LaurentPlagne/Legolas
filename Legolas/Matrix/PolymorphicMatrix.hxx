#ifndef __POLYMORPHICMATRIX__
#define __POLYMORPHICMATRIX__

#include <memory>
//#include "Legolas/Algorithm/PolymorphicSolver.hxx"

namespace Legolas{

  class PolymorphicMatrix : public Legolas::AnyMatrix {
  public:

    PolymorphicMatrix(void):matrixPtr_(){}

    PolymorphicMatrix(std::shared_ptr<Legolas::AnyMatrix> matrixPtr):matrixPtr_(matrixPtr){}
    
    PolymorphicMatrix(const AnyMatrix & source):matrixPtr_(source.clone()){}

    inline void matrixVectorProduct(const Legolas::AnyVector & X, Legolas::AnyVector & Y, int addFlag) const {
      matrixPtr_->matrixVectorProduct(X,Y,addFlag);
    };

    inline void transposeMatrixVectorProduct(const Legolas::AnyVector & X, Legolas::AnyVector & Y, int addFlag) const {
      matrixPtr_->transposeMatrixVectorProduct(X,Y,addFlag);
    };

    inline void matrixVectorInverse(const Legolas::AnyVector & X, Legolas::AnyVector & Y, Legolas::AnyWorkSpace & ws) const {
      matrixPtr_->matrixVectorInverse(X,Y,ws);
    };


    inline void matrixVectorInverse(const Legolas::AnyVector & X, Legolas::AnyVector & Y) const {
      matrixPtr_->matrixVectorInverse(X,Y);
    };

    inline int getFlatRowSizeMethod( void ) const { return matrixPtr_->getFlatRowSizeMethod();}
    inline int getFlatColSizeMethod( void ) const { return matrixPtr_->getFlatColSizeMethod();}

    inline void iterateOverBlocksMethod(Legolas::MatrixStream & aStream) const { matrixPtr_->iterateOverBlocksMethod(aStream);}

    inline int getLevel( void ) const { return matrixPtr_->getLevel();}

    //    inline int & setMaxIteration( void ){ return matrixPtr_->setMaxIteration() ; }

    
    inline virtual std::string getStructureName( void ) const {
      std::string result="Polymorphic(";
      result+=matrixPtr_->getStructureName();
      result+=")";
      return result;
    }

 //    inline Legolas::AnySolver * getSolverPtr(AnyVector & aX, const AnyVector & aB) const {
//       return matrixPtr_->getSolverPtr(aX,aB);
//     }

    inline Legolas::AnySolver * getSolverPtr(AnyVector & aX, const AnyVector & aB, void * avp) const {
      return matrixPtr_->getSolverPtr(aX,aB,avp);
    }

    inline std::shared_ptr<Legolas::AnyMatrix> getMatrixPtr( void ) const { return matrixPtr_;}

  private:
    
    std::shared_ptr<Legolas::AnyMatrix>  matrixPtr_;

  };

}

#endif
