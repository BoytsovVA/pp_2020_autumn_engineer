// Copyright 2020 Tronin Dmitry
#ifndef MODULES_TASK_2_TRONIN_D_SLEEPING_HAIRDRESSER_SLEEPING_HAIRDRESSER_H_
#define MODULES_TASK_2_TRONIN_D_SLEEPING_HAIRDRESSER_SLEEPING_HAIRDRESSER_H_

#include <cstddef>

void WaitingRoom(size_t queue_capacity);
void Barber();
void Customer(int rank);
void SleepingBarber(size_t queue_capacity);
void CrossPlatformSleep(size_t sleepMs);

#endif  // MODULES_TASK_2_TRONIN_D_SLEEPING_HAIRDRESSER_SLEEPING_HAIRDRESSER_H_
