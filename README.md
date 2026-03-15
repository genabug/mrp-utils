# numkit

A C++ header-only library providing low-level mathematical data structures for numerical computations.

## Core Components

### Vector (`math/Vector.h`)

A templated Euclidean vector class for arbitrary dimensions and types:

- Parameterized by dimension `N`, component type `T`, and a boolean `is_euclidian`
- Supports standard arithmetic operations (`+`, `-`, `*`, `/`)
- Provides dot product, cross product (`%`), magnitude (`fabs`), and angle functions (`cos`, `sin`)
- Includes IO support with customizable formatting (brackets vs bare components)
- Type aliases: `Vector2D`, `Vector3D`, and `Array<N,T>` for simple componentwise arithmetic

### Tensor (`math/Tensor.h`)

A rank-2 tensor (matrix) class for arbitrary dimensions:

- Full matrix operations: transpose, inverse, determinant, trace
- Matrix multiplication, addition, subtraction
- Vector-tensor operations for linear algebra
- Type aliases: `Tensor2D`, `Tensor3D`

### State (`quantities/State.h`)

A heterogeneous tuple of named quantities for scientific state vectors:

- Uses `Traits` for type metadata with compile-time string names
- Supports arithmetic and comparison operations on states
- Allows access by index, type-name, or variable
- Useful for representing physical states (e.g., density, temperature, velocity)

### ObjectsFactory (`factory/ObjectsFactory.h`)

A generic factory pattern implementation:

- Creates objects by string identifiers
- Uses static registration via constructors
- Supports arbitrary creation functions with typed arguments

## Project Structure

```text
numkit/
├── CMakeLists.txt       # Main build config
├── math/                # Math library
│   └── math/            # Vector, Tensor
├── quantities/          # Quantities library
│   └── quantities/      # State, Traits
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
- Materials with deducing this 8) (c++23)
- GHA to run the tests on PRs
- IO::Mode for State
- thread safety
- units of quantities
- generate doxygen docs
- ...
