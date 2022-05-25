#include "../include/lem-ipc.h"

bool    sem_wait_systemv(void)
{
    struct sembuf sb;

    sb.sem_num = 0;
    sb.sem_flg = 0;
    sb.sem_op = -1;
    if (semop(resources.semid, &sb, 1) < 0)
        return FALSE;
    return TRUE;
}

bool    sem_post_systemv(void)
{
    struct sembuf sb;

    sb.sem_num = 0;
    sb.sem_flg = 0;
    sb.sem_op = 1;
    if (semop(resources.semid, &sb, 1) < 0)
        return FALSE;
    return TRUE;
}