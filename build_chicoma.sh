#!/bin/bash

# Make sure all the paths are correct

module load cmake cray-libsci

rm -r build
rm -r install

MY_PATH=$(pwd)

export CC=${CC:=cc}
export FC=${FC:=ftn}
export CXX=${CXX:=CC}
export BLAS_VENDOR=${BLAS_VENDOR:=Auto}
export BML_OPENMP=${BML_OPENMP:=yes}
export INSTALL_DIR=${INSTALL_DIR:="${MY_PATH}/install"}
export BML_TESTING=${BML_TESTING:=yes}
export CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE:=Release}
export EXTRA_CFLAGS=${EXTRA_CFLAGS:="-g"}
export EXTRA_LINK_FLAGS=${EXTRA_LINK_FLAGS:="-g -L/apps/openblas/0.3.12/lib -lopenblas"}
export CMAKE_Fortran_FLAGS=${CMAKE_Fortran_FLAGS:="-ef -L/apps/openblas/0.3.12/lib -lopenblas"}

./build.sh configure

                                                                                                                                                                                              
                                    
