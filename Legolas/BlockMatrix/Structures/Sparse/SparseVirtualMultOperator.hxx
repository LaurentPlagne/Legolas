#ifndef __LEGOLAS_SPARSEVIRTUALMULTOPERATOR_HXX__
#define __LEGOLAS_SPARSEVIRTUALMULTOPERATOR_HXX__

#include "Legolas/BlockMatrix/VirtualMultOperator.hxx"

namespace Legolas{

  class SparseVirtualBlockMatrix;
  
  class SparseVirtualMultOperator : public VirtualMultOperator{
  public:

    virtual void addMult(const Matrix & Av, const double & a, const VirtualVector & X, VirtualVector & Y);
    virtual void addMult(const SparseVirtualBlockMatrix & A, const double & a, const VirtualVector & X, VirtualVector & Y) = 0;

    virtual void mult(const Matrix & Av, const VirtualVector & X, VirtualVector & Y);
    virtual void mult(const SparseVirtualBlockMatrix & A, const VirtualVector & X, VirtualVector & Y) = 0;

    virtual void transposeAddMult(const Matrix & Av, const double & a, const VirtualVector & X, VirtualVector & Y);
    virtual void transposeAddMult(const SparseVirtualBlockMatrix & A, const double & a, const VirtualVector & X, VirtualVector & Y){
      INFOS("not yet implemented");
      throw std::runtime_error("Not yet implemented");
    }

    virtual void transposeMult(const Matrix & Av, const VirtualVector & X, VirtualVector & Y);
    virtual void transposeMult(const SparseVirtualBlockMatrix & A, const VirtualVector & X, VirtualVector & Y){
      INFOS("not yet implemented");
      throw std::runtime_error("Not yet implemented");
    }

    virtual ~SparseVirtualMultOperator( void ){};
  };
  
}

#endif

