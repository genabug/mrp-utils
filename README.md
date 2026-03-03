# mrp-utils

A C++ header-only library providing low-level mathematical data structures for scientific/numerical computations.

## Core Components

### Vector (`utils/utils/Vector.h`)

A templated Euclidean vector class for arbitrary dimensions and types:

- Parameterized by dimension `N`, component type `T`, and a boolean `is_euclidian`
- Supports standard arithmetic operations (`+`, `-`, `*`, `/`)
- Provides dot product, cross product (`%`), magnitude (`fabs`), and angle functions (`cos`, `sin`)
- Includes IO support with customizable formatting (brackets vs bare components)
- Type aliases: `Vector2D`, `Vector3D`, and `Array<N,T>` for simple componentwise arithmetic

### Tensor (`utils/utils/Tensor.h`)

A rank-2 tensor (matrix) class for arbitrary dimensions:

- Full matrix operations: transpose, inverse, determinant, trace
- Matrix multiplication, addition, subtraction
- Vector-tensor operations for linear algebra
- Type aliases: `Tensor2D`, `Tensor3D`

### QState (`utils/utils/QState.h`)

A heterogeneous tuple of named quantities for scientific state vectors:

- Uses `QTraits` for type metadata with compile-time string names
- Supports arithmetic and comparison operations on states
- Allows access by index, type-name, or variable
- Useful for representing physical states (e.g., density, temperature, velocity)

### ObjectsFactory (`factory/factory/ObjectsFactory.h`)

A generic factory pattern implementation:

- Creates objects by string identifiers
- Uses static registration via constructors
- Supports arbitrary creation functions with typed arguments

## Project Structure

```text
mrp-utils/
├── CMakeLists.txt       # Main build config
├── utils/               # Core utility headers
│   └── utils/           # Vector, Tensor, QState, QTraits
├── factory/             # Factory pattern implementation
│   └── factory/         # ObjectsFactory
├── tests/               # Unit tests
│   ├── tst_vector.cpp
│   ├── tst_tensor.cpp
│   ├── tst_state.cpp
│   ├── tst_factory.cpp
│   └── ...
└── build/               # Build output
```

## Build

Requires CMake 3.23+ and a C++20 compatible compiler, as well as google test library.

```bash
mkdir build && cd build &&\
cmake .. &&\
make
```

## Run Tests

```bash
cd build && ctest
```

## Known Issues / TODOs

- No examples :(
- Remove Q prefix, do not pretend Qt ;)
- Get rid of mrp prefix
- Materials with deducing this 8) (c++23)
- review Vector and Tensor tests, it's a mess
- ...
