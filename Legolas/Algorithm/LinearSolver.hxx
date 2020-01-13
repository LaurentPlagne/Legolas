#ifndef __LEGOLAS_LINEARSOLVER_HXX__
#define __LEGOLAS_LINEARSOLVER_HXX__

//#include "Legolas/Algorithm/BlockLinearSolverTraits.hxx"
#include "Legolas/Algorithm/AnySolver.hxx"
#include "Legolas/Vector/VectorPool.hxx"
#include "Legolas/Algorithm/TemporaryVectorReferenceProvider.hxx"


namespace Legolas{

  template <class TA, class TX, class TB>
  class LinearSolver: public AnySolver{
  private:
    
    typedef typename TA::MatrixElementInterface::Solver BlockSolver;

    typedef typename TA::ConstGetElement ETA;
    typedef typename TX::Element         ETX;
    typedef typename TB::Element         ETB;

    static const int level=TA::level;
    typedef typename TX::RealType RealType;

  public:
    typedef VectorPool<RealType,level> * VectorPoolPtr;

    typedef TemporaryVectorReferenceProvider<RealType,level>   VectorProvider;
    typedef TemporaryVectorReferenceProvider<RealType,level-1> ElementProvider;
    typedef typename VectorProvider::ReturnType  Vector;
    typedef typename ElementProvider::ReturnType Element;
    

  private:
    
    
    //     typedef typename BlockLinearSolverTraits<ETA>::Type 
    
    std::vector<BlockSolver> blockSolvers_;
    bool isRoot_;
    VectorPoolPtr vectorPoolPtr_;
    
  public:

    LinearSolver(void):blockSolvers_(),isRoot_(false),vectorPoolPtr_(0){}
    
    LinearSolver(const TA & A, TX & X, const TB & B):blockSolvers_(),isRoot_(true),vectorPoolPtr_(0){
      vectorPoolPtr_=new VectorPool<RealType,level>();
      for (int i=0 ; i<A.nrows() ; i++){
	blockSolvers_.push_back(BlockSolver(A.sparseGetElement(i,i),X[i],B[i],vectorPoolPtr_));
      }
    }

    LinearSolver(const TA & A, TX & X, const TB & B, const VectorPoolPtr & vectorPoolPtr):blockSolvers_(),isRoot_(false),vectorPoolPtr_(vectorPoolPtr){
      for (int i=0 ; i<A.nrows() ; i++){
 	blockSolvers_.push_back(BlockSolver(A.sparseGetElement(i,i),X[i],B[i],vectorPoolPtr_));
      }
    }

    
    ~LinearSolver( void ){
      if (isRoot_){
	assert(vectorPoolPtr_!=0);
	delete vectorPoolPtr_;
      }
    }


    VectorPoolPtr getVectorPoolPtr( void ){ return vectorPoolPtr_ ;}


    inline void solve(const Legolas::AnyMatrix & aA, Legolas::AnyVector & aX, const Legolas::AnyVector & aB){
      TX * XPtr=static_cast<TX*>(&aX);
      const TB * BPtr=static_cast<const TB*>(&aB);
      
      TX & X(*XPtr);
      const TB & B(*BPtr);
      
      const TA * Aptr=static_cast<const TA*>(&aA);
      const TA & A(*Aptr);
      
      this->solve(A,X,B);
    }
    
    template <int LEVEL>
    inline TemporaryVectorReferenceProvider<RealType,LEVEL> getTemporaryVectorProvider( void ){
      return TemporaryVectorReferenceProvider<RealType,LEVEL>(vectorPoolPtr_);
    }
    
    inline VectorProvider getVectorProvider( void ){
      return VectorProvider(vectorPoolPtr_);
    }

    inline ElementProvider getElementProvider( void ){
      return ElementProvider(vectorPoolPtr_);
    }
    

//     inline MultiVector<RealType,level> * aquireVectorPtr(const typename ShapeTraits<level>::Type & shape){
//       return vectorPoolPtr_->aquireVectorPtr(shape);
//     }

//     inline void releaseVectorPtr(MultiVector<RealType,level> * vptr){
//       return vectorPoolPtr_->releaseVectorPtr(vptr);
//     }

//     inline ETX * aquireElementPtr(const ETX & element){
//       return vectorPoolPtr_->aquireElementPtr(element);
//     }

//     inline void releaseElementPtr(ETX * vptr){
//       return vectorPoolPtr_->releaseElementPtr(vptr);
//     }



    inline void transposeSolve(const Legolas::AnyMatrix & aA, Legolas::AnyVector & aX, const Legolas::AnyVector & aB){
      TX * XPtr=static_cast<TX*>(&aX);
      const TB * BPtr=static_cast<const TB*>(&aB);
      
      TX & X(*XPtr);
      const TB & B(*BPtr);
      
      const TA * Aptr=static_cast<const TA*>(&aA);
      const TA & A(*Aptr);
      //      INFOS("ICI");
      //      INFOS("A.nrows()="<<A.nrows());

      this->transposeSolve(A,X,B);
    }
      
    
    inline virtual void solve(const TA & A, TX & X, const TB & B){
      INFOS("not yet implemented");
      throw std::runtime_error("Not yet implemented");
    }

    inline virtual void transposeSolve(const TA & A, TX & X, const TB & B){
      INFOS("not yet implemented");
      throw std::runtime_error("Not yet implemented");
    }

    //    template <class ETA,class ETX, class ETB>
    //    inline void blockSolve(ETA aii, ETX & xi, const ETB & bi){
    //      blockSolver_.solve(aii,xi,bi);
    //    }

    inline void blockSolve(ETA aii, ETX & xi, const ETB & bi, int i=0){
      blockSolvers_[i].solve(aii,xi,bi);
    }

    inline void blockTransposeSolve(ETA aii, ETX & xi, const ETB & bi, int i=0){
      blockSolvers_[i].transposeSolve(aii,xi,bi);
    }
  };
}


#endif
