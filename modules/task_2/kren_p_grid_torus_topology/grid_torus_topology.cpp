// Copyright 2020 Kren Polina
#include <mpi.h>
#include "../../../modules/task_2/kren_p_grid_torus_topology/grid_torus_topology.h"

MPI_Comm getCommTorus(const MPI_Comm comm_base, int height, int width) {
    int base_size;
    MPI_Comm_size(comm_base, &base_size);

    int dims[] = { height, width };
    int mul = 1;
    if (height > 0)
        mul *= height;
    if (width > 0)
        mul *= width;
    if (base_size % mul != 0)
        return MPI_COMM_NULL;

    MPI_Dims_create(base_size, 2, dims);

    MPI_Comm comm_torus;
    int period[] = { 1, 1 };

    MPI_Cart_create(comm_base, 2, dims, period, 1, &comm_torus);

    return comm_torus;
}

bool testGridTorus(const MPI_Comm comm_torus) {
    int gridtorus;
    MPI_Topo_test(comm_torus, &gridtorus);
    if (gridtorus != MPI_CART)
        return false;

    int dims_count;
    MPI_Cartdim_get(comm_torus, &dims_count);
    if (dims_count != 2)
        return false;

    int dims[2];
    int period[2];
    int coords[2];

    MPI_Cart_get(comm_torus, 2, dims, period, coords);
    if ((period[0] != 1) || (period[1] != 1))
        return false;
    return true;
}

bool testRelation(const MPI_Comm comm_torus) {
    int rank;
    MPI_Comm_rank(comm_torus, &rank);

    int coords[2];
    MPI_Cart_coords(comm_torus, rank, 2, coords);

    int main_flag = 0;
    int front, back;

    MPI_Cart_shift(comm_torus, 0, 1, &back, &front);
    if ((back == MPI_PROC_NULL) || (front == MPI_PROC_NULL))
        main_flag = 1;

    MPI_Cart_shift(comm_torus, 1, 1, &back, &front);
    if ((back == MPI_PROC_NULL) || (front == MPI_PROC_NULL))
        main_flag = 1;

    if (rank == 0) {
        int result = 0;
        MPI_Reduce(&main_flag, &result, 1, MPI_INT, MPI_LOR, 0, comm_torus);
        if (result == 0)
            return true;
    }
    if (rank != 0)
        MPI_Reduce(&main_flag, NULL, 1, MPI_INT, MPI_LOR, 0, comm_torus);
    return false;
}
