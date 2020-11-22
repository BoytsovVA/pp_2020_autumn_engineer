  // Copyright 2020 Zlobin George
#include <mpi.h>
#include <cstring>
#include "../../../modules/task_2/zlobin_g_gather/gather.h"


int Gather(void *sbuf, int scount, MPI_Datatype stype,
               void *rbuf, int rcount, MPI_Datatype rtype, int root, MPI_Comm comm) {
    MPI_Barrier(comm);

    int size, rank;
    int err_comm_size, err_comm_rank;
    err_comm_size = MPI_Comm_size(MPI_COMM_WORLD, &size);
    err_comm_rank = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (err_comm_size != MPI_SUCCESS)
        return err_comm_size;
    if (err_comm_rank != MPI_SUCCESS)
        return err_comm_rank;
    if (scount != rcount || scount < 0)
        return MPI_ERR_COUNT;
    if (stype != rtype)
        return MPI_ERR_TYPE;

    if (rank == root) {
        int size_type;
        MPI_Type_size(stype, &size_type);

        int size_buf = scount * size_type + MPI_BSEND_OVERHEAD;
        char* buf = new char[size_buf];

        MPI_Buffer_attach(buf, size_buf);
        MPI_Request ibsend_request;
        MPI_Status ibsend_status;

        MPI_Ibsend(sbuf, scount, stype, root, 0, comm, &ibsend_request);

        MPI_Request* all_request = new MPI_Request[size];
        MPI_Status* all_status = new MPI_Status[size];

        for (int i = 0; i < size; i++) {
            MPI_Irecv(static_cast<char*>(rbuf) + i * rcount * size_type, rcount, rtype, i, 0, comm, &all_request[i]);
        }

        MPI_Wait(&ibsend_request, &ibsend_status);
        MPI_Buffer_detach(&buf, &size_buf);
        MPI_Waitall(size, all_request, all_status);

        delete[] buf;
        delete[] all_request;
        delete[] all_status;
    } else {
        MPI_Send(sbuf, scount, stype, root, 0, comm);
    }
    return MPI_SUCCESS;
}
