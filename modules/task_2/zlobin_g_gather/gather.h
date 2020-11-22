// Copyright 2020 Zlobin George
#ifndef MODULES_TASK_2_ZLOBIN_G_GATHER_GATHER_H_
#define MODULES_TASK_2_ZLOBIN_G_GATHER_GATHER_H_

int Gather(void *sbuf, int scount, MPI_Datatype stype,
               void *rbuf, int rcount, MPI_Datatype rtype, int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_ZLOBIN_G_GATHER_GATHER_H_
