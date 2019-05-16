# mrp-utils
Some MARPLE low-level data structures:

* Vector: Euclidian vector of arbitrary dimension.
* Tensor: Tensor of rank 2.
* QState: Tuple of named quantities.

# Build
```bash
mkdir build && cd build &&\
cmake .. &&\
make
```

# known issues
* no documentation
* QState is overcomplicated
* clang/msvc can't build tst\_qstate due to their bugs (see https://stackoverflow.com/a/53576384/8802124)
* ...
