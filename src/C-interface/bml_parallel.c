#include "bml_parallel.h"
#include "bml_introspection.h"
#include "bml_logger.h"
#include "dense/bml_parallel_dense.h"
#include "ellpack/bml_parallel_ellpack.h"
#include "ellblock/bml_parallel_ellblock.h"
#include "csr/bml_parallel_csr.h"
#ifdef BML_USE_MPI
#include "distributed2d/bml_allocate_distributed2d.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#ifdef BML_USE_MPI
#include <mpi.h>
#endif

static int myRank = 0;
static int nRanks = 1;
#ifdef BML_USE_MPI
MPI_Comm bml_mpi_comm = MPI_COMM_NULL;
#endif

/** Initialize.
 *
 * \ingroup init_group_C
 *
 * \param argc Number of args
 * \param argv Args
 */

/** Get number of MPI ranks.
 */
int
bml_getNRanks(
    void)
{
    return nRanks;
}

/** Get local MPI rank.
 */
int
bml_getMyRank(
    void)
{
    return myRank;
}

/* \details
 *
 * For now this is just a check for rank 0 but in principle it could be more
 * complex.  It is also possible to suppress practically all output by causing
 * this function to return 0 for all ranks.
 */
int
bml_printRank(
    void)
{
    if (myRank == 0)
        return 1;
    return 0;
}

#ifdef BML_USE_MPI
void
bml_initParallel(
    MPI_Comm comm)
{
    bml_mpi_comm = comm;
    MPI_Comm_rank(bml_mpi_comm, &myRank);
    MPI_Comm_size(bml_mpi_comm, &nRanks);

    if (bml_printRank())
        printf("MPI started in bml with %d ranks\n", nRanks);

    bml_setcomm_distributed2d(comm);
}
#endif

void
bml_initParallelF(
    int fcomm)
{
#ifdef BML_USE_MPI
    MPI_Comm comm = MPI_Comm_f2c(fcomm);
    bml_initParallel(comm);
#endif
}

void
bml_shutdownParallelF(
    void)
{
    bml_shutdownParallel();
}

void
bml_shutdownParallel(
    void)
{
#ifdef BML_USE_MPI
#endif
}

void
bml_barrierParallel(
    void)
{
#ifdef BML_USE_MPI
    MPI_Barrier(bml_mpi_comm);
#endif
}

void
bml_sumRealReduce(
    double *value)
{
#ifdef BML_USE_MPI
    double sLocal[1], sGlobal[1];

    sLocal[0] = *value;
    MPI_Allreduce(sLocal, sGlobal, 1, MPI_DOUBLE, MPI_SUM, bml_mpi_comm);
    *value = sGlobal[0];
#endif
}

void
bml_minRealReduce(
    double *value)
{
#ifdef BML_USE_MPI
    double sLocal[1], sGlobal[1];

    sLocal[0] = *value;
    MPI_Allreduce(sLocal, sGlobal, 1, MPI_DOUBLE, MPI_MIN, bml_mpi_comm);
    *value = sGlobal[0];
#endif
}

void
bml_maxRealReduce(
    double *value)
{
#ifdef BML_USE_MPI
    double sLocal[1], sGlobal[1];

    sLocal[0] = *value;
    MPI_Allreduce(sLocal, sGlobal, 1, MPI_DOUBLE, MPI_MAX, bml_mpi_comm);
    *value = sGlobal[0];
#endif
}

/** Exchange pieces of matrix across MPI ranks.
 *
 * \param A Matrix A
 */
void
bml_allGatherVParallel(
    bml_matrix_t * A)
{
    switch (bml_get_type(A))
    {
        case dense:
            bml_allGatherVParallel_dense(A);
            break;
        case ellpack:
            bml_allGatherVParallel_ellpack(A);
            break;
        case ellblock:
            LOG_ERROR
                ("bml_allGatherVParallel function not implemented for ellblock\n");
        default:
            LOG_ERROR("unknown matrix type\n");
            break;
    }
}

#ifdef BML_USE_MPI
void
bml_mpi_send(
    bml_matrix_t * A,
    const int dst,
    MPI_Comm comm)
{
    switch (bml_get_type(A))
    {
        case dense:
            bml_mpi_send_dense(A, dst, comm);
            break;
        case ellpack:
            bml_mpi_send_ellpack(A, dst, comm);
            break;
        case ellblock:
            bml_mpi_send_ellblock(A, dst, comm);
            break;
        case csr:
            bml_mpi_send_csr(A, dst, comm);
            break;
        default:
            LOG_ERROR("unknown matrix type\n");
            break;
    }
}

void
bml_mpi_recv(
    bml_matrix_t * A,
    const int src,
    MPI_Comm comm)
{
    switch (bml_get_type(A))
    {
        case dense:
            bml_mpi_recv_dense(A, src, comm);
            break;
        case ellpack:
            bml_mpi_recv_ellpack(A, src, comm);
            break;
        case ellblock:
            bml_mpi_recv_ellblock(A, src, comm);
            break;
        case csr:
            bml_mpi_recv_csr(A, src, comm);
            break;
        default:
            LOG_ERROR("bml_mpi_recv - unknown matrix type\n");
            break;
    }
}

void
bml_mpi_irecv(
    bml_matrix_t * A,
    const int src,
    MPI_Comm comm)
{
    switch (bml_get_type(A))
    {
        case dense:
            bml_mpi_irecv_dense(A, src, comm);
            break;
        case ellpack:
            bml_mpi_irecv_ellpack(A, src, comm);
            break;
        case ellblock:
            bml_mpi_irecv_ellblock(A, src, comm);
            break;
        case csr:
            bml_mpi_irecv_csr(A, src, comm);
            break;
        default:
            LOG_ERROR("bml_mpi_irecv - unknown matrix type\n");
            break;
    }
}

void
bml_mpi_irecv_complete(
    bml_matrix_t * A)
{
    switch (bml_get_type(A))
    {
        case dense:
            bml_mpi_irecv_complete_dense(A);
            break;
        case ellpack:
            bml_mpi_irecv_complete_ellpack(A);
            break;
        case ellblock:
            bml_mpi_irecv_complete_ellblock(A);
            break;
        case csr:
            bml_mpi_irecv_complete_csr(A);
            break;
        default:
            LOG_ERROR("unknown matrix type\n");
            break;
    }
}

bml_matrix_t *
bml_mpi_recv_matrix(
    bml_matrix_type_t matrix_type,
    bml_matrix_precision_t matrix_precision,
    int N,
    int M,
    const int src,
    MPI_Comm comm)
{
    switch (matrix_type)
    {
        case dense:
            return bml_mpi_recv_matrix_dense(matrix_precision, N, M, src,
                                             comm);
            break;
        case ellpack:
            return bml_mpi_recv_matrix_ellpack(matrix_precision, N, M, src,
                                               comm);
            break;
        case ellblock:
            return bml_mpi_recv_matrix_ellblock(matrix_precision, N, M, src,
                                                comm);
            break;
        case csr:
            return bml_mpi_recv_matrix_csr(matrix_precision, N, M, src, comm);
            break;
        default:
            LOG_ERROR("unknown matrix type\n");
            return NULL;
            break;
    }
}

void
bml_mpi_bcast_matrix(
    bml_matrix_t * A,
    const int root,
    MPI_Comm comm)
{
    switch (bml_get_type(A))
    {
        case ellpack:
            return bml_mpi_bcast_matrix_ellpack(A, root, comm);
            break;
        case dense:
            return bml_mpi_bcast_matrix_dense(A, root, comm);
            break;
        case csr:
            return bml_mpi_bcast_matrix_csr(A, root, comm);
            break;
        case ellblock:
            return bml_mpi_bcast_matrix_ellblock(A, root, comm);
            break;
        default:
            LOG_ERROR("bml_mpi_bcast: matrix format not implemented\n");
            break;
    }
}

#endif
