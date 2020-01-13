#include "shape.hxx"
#include <vector>

namespace Legolas {


  /*************************** StoredVector ****************************/

template <typename SHAPE,
          typename LEAF>
class StoredVector {
public:
  typedef SHAPE  Shape;
  typedef LEAF   Leaf;

  StoredVector (const Shape & shape, Leaf *const data)
    : shape_ (shape),
      data_  (data)
  {}

  size_t flatsize() const {
    return shape_.flatsize();
  }

  Leaf& operator() (size_t index) {
    return data_[index];
  }

  const Leaf& operator() (size_t index) const {
    return data_[index];
  }

  size_t size () const {
    return shape_.size();
  }

protected:
  Shape const shape_;
  Leaf *const data_;
};

  /*************************** Vector ****************************/

template <typename SHAPE,
          typename LEAF>
class Vector
  : public StoredVector<SHAPE, LEAF>
{
public:
  typedef SHAPE                                   Shape;
  typedef StoredVector<SHAPE, LEAF>               Base;
  typedef Vector<typename Shape::SubShape, LEAF>  SubVector;

  Vector (const SHAPE & shape, LEAF *const data)
    : Base (shape, data)
  {}

  SubVector operator[] (size_t index) {
    return SubVector (this->shape_[index],
                      this->data_ + this->shape_.offset(index));
  }

  const SubVector operator[] (size_t index) const {
    return SubVector (this->shape_[index],
                      this->data_ + this->shape_.offset(index));
  }
};

template <typename LEAF>
class Vector<RectangularShape<1>, LEAF>
  : public StoredVector<RectangularShape<1>, LEAF>
{
public:
  typedef RectangularShape<1>                      Shape;
  typedef LEAF                                     Leaf;
  typedef StoredVector<RectangularShape<1>, LEAF>  Base;
  typedef Vector<RectangularShape<1>, LEAF>        Self;

  Vector (const Shape & shape, LEAF *const data)
    : Base (shape, data)
  {}

  LEAF & operator[] (size_t index) {
    return this->data_[index];
  }

  const LEAF & operator[] (size_t index) const {
    return this->data_[index];
  }

  Self&& self () {
    return std::move(*this);
  }
};

  /*************************** VectorData ****************************/

template <typename SHAPE,
          typename LEAF>
class VectorData {
public:
  VectorData (const SHAPE & shape)
  : shape_ (shape),
    data_  (shape.flatsize())
  {}

  Vector<SHAPE, LEAF> vector () {
    return Vector<SHAPE, LEAF> (shape_, &data_[0]);
  }

private:
  SHAPE             shape_;
  std::vector<LEAF> data_;
};
}//end of namespace Legolas
