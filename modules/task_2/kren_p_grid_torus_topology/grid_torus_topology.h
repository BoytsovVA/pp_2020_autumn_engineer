// Copyright 2020 Kren Polina
#ifndef MODULES_TASK_2_KREN_P_GRID_TORUS_TOPOLOGY_GRID_TORUS_TOPOLOGY_H_
#define MODULES_TASK_2_KREN_P_GRID_TORUS_TOPOLOGY_GRID_TORUS_TOPOLOGY_H_

#include <mpi.h>

MPI_Comm getCommTorus(const MPI_Comm comm_base, int height = 0, int width = 0);

bool testGridTorus(const MPI_Comm comm_torus);

bool testRelation(const MPI_Comm comm_torus);

#endif  // MODULES_TASK_2_KREN_P_GRID_TORUS_TOPOLOGY_GRID_TORUS_TOPOLOGY_H_
