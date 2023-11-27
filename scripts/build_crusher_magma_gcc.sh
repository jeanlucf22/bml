#!/bin/bash

# Make sure all the paths are correct

rm -r build
rm -r install

MY_PATH=$(pwd)

export CC=${CC:=gcc}
export FC=${FC:=gfortran}
export CXX=${CXX:=g++}
export BLAS_VENDOR=${BLAS_VENDOR:=OpenBLAS}
export BML_OPENMP=${BML_OPENMP:=yes}
export BML_MAGMA=${BML_MAGMA:=yes}
export MAGMA_ROOT=${OLCF_MAGMA_ROOT}
export BML_ROCSOLVER=${BML_ROCSOLVER:=no}
export BML_SCALAPACK=${BML_SCALAPACK:=yes}
export BML_ELPA=${BML_ELPA:=yes}

export ELPA_DIR=${ELPA_DIR:=/ccs/proj/csc304/elpa/hipcc}
export SCALAPACK_LIBRARIES=${SCALAPACK_LIBRARIES:="$CRAY_LIBSCI_PREFIX_DIR/lib/libsci_gnu_mpi_mp.so"}

export BML_TESTING=${BML_TESTING:=yes}
export CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE:=Release}
export EXTRA_FFLAGS=${EXTRA_FFLAGS:="-ffree-form -ffree-line-length-200"}
export EXTRA_CFLAGS=${EXTRA_CFLAGS:="-I${HIP_PATH}/include"}
export INSTALL_DIR="/autofs/nccs-svm1_proj/csc304/bml/crusher/gcc"

if [[ -z "$CMAKE_PREFIX_PATH" ]]; then
    export CMAKE_PREFIX_PATH=${ROCM_PATH}
else
    export CMAKE_PREFIX_PATH="${ROCM_PATH};${CMAKE_PREFIX_PATH}"
fi

./build.sh configure

pushd build
make -j8
make install
popd
