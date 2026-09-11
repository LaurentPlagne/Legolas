# API Reference: `Legolas::Array`

Header: `#include "Legolas/Array/Array.hxx"`

The `Legolas::Array` class template represents an $N$-dimensional tensor with optional compile-time data interleaving along a designated dimension.

---

## Template Definition

```cpp
namespace Legolas {

template <
    class SCALAR_TYPE, 
    int LEVEL, 
    int PACK_SIZE = 1, 
    int PACK_LEVEL = 1
>
class Array;

}
```

### Template Parameters
- `SCALAR_TYPE`: The underlying arithmetic type (e.g. `float`, `double`, `int`).
- `LEVEL`: The dimensionality/rank of the array ($1$ for 1D vectors, $2$ for 2D matrices, etc.).
- `PACK_SIZE`: Number of problem instances interleaved contiguously in memory ($P=1$ for scalar, $P=4$ for NEON/SSE, $P=8$ for AVX2, $P=16$ for AVX-512). Default is `1`.
- `PACK_LEVEL`: The dimension index along which interleaving occurs ($1$-based index, e.g. `2` for outer dimension of a 2D array). Default is `1`.

---

## Constructors

```cpp
// 1. Construct 1D vector of length size
Array<float, 1>(int size);

// 2. Construct 2D array of shape (s2, s1)
Array<float, 2, 4, 2>(int s2, int s1);

// 3. Construct with explicit Shape
Array(const Shape& shape);
```

---

## Member Functions

### Element Access

```cpp
// Returns element or sub-array at index i (inlined with LEGOLAS_ALWAYS_INLINE)
Element operator[](int i);
const Element operator[](int i) const;
```

### Shape & Size

```cpp
// Returns the shape of the array
const Shape& shape() const;

// Returns the number of elements in the current dimension
int size() const;

// Returns total number of scalar elements in the entire buffer
size_t flatSize() const;
```

### Memory Initialization & Assignment

```cpp
// Sets all elements to the specified scalar value
void fill(RealType value);

// Deep copy assignment
Array& operator=(const Array& other);

// Element-wise compound assignments
Array& operator+=(const Array& other);
Array& operator-=(const Array& other);
Array& operator*=(RealType scalar);
```

### SIMD Vector Views

```cpp
// Returns a packed view where elements are mapped to Eigen::Array<SCALAR_TYPE, PACK_SIZE, 1>
PackedArrayView getPackedView();
const PackedArrayView getPackedView() const;

// Returns a flattened 1D view of packed vector registers
Array<PackedRealType, 1> getFlatPackedView();
```
