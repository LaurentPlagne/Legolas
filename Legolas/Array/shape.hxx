#include <cassert>

namespace Legolas {

typedef int size_t;

template <int LEVEL>
class RectangularShape {
public:
  typedef RectangularShape<LEVEL-1> SubShape;

  RectangularShape (const size_t size, const SubShape & subshape)
    : size_     (size),
      subshape_ (subshape)
  {}

  size_t flatsize () const {
    return size_ * subshape_.flatsize();
  }

  size_t size () const {
    return size_;
  }

  const SubShape & operator[] (size_t index) const {
    assert (index < size_);
    return subshape_;
  }

  size_t offset (size_t index) const {
    return index * subshape_.flatsize();
  }

private:
  size_t   size_;
  SubShape subshape_;
};

template <>
class RectangularShape<1> {
public:
  RectangularShape (const size_t size)
  : size_ (size)
  {}

  size_t flatsize () const {
    return size_;
  }

  size_t size () const {
    return size_;
  }

private:
  size_t size_;
};
}
