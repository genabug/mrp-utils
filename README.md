# mrp-utils
Some MARPLE low-level data structures:

* Vector: Euclidian vector of arbitrary dimension and type.
* Tensor: Tensor of rank 2 parametrized by dimension and type.
* QState: Tuple of named quantities.
* ObjectsFactory: Factory template by generating function.

# Build
```bash
mkdir build && cd build &&\
cmake .. &&\
make
ctest
```

# known issues
* QState seems to be overcomplicated a bit.. dunno.
* ...
