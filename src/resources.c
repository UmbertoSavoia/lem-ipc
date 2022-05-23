#include "../include/lem-ipc.h"

bool create_resources(void)
{
    key_t key = FTOK;

    if ((resources.shmid = shmget(key, SIZE_BOARD, PERM | IPC_CREAT)) < 0)
        return FALSE;
    if ((resources.board = shmat(resources.shmid, 0, 0)) == (char *)-1)
        return FALSE;
    ft_memset(resources.board, 0, SIZE_BOARD);
    if ((resources.msqid = msgget(key, PERM | IPC_CREAT)) < 0)
        return FALSE;
    if ((resources.semid = semget(key, 1, PERM | IPC_CREAT)) < 0)
        return FALSE;
    if (semctl(resources.semid, 0, SETVAL, 1) < 0)
        return FALSE;

    return TRUE;
}

void    clean_resources(void)
{
    shmdt(resources.board);
    if (resources.is_display) {
        msgctl(resources.msqid, IPC_RMID, 0);
        shmctl(resources.shmid, IPC_RMID, 0);
        semctl(resources.semid, IPC_RMID, 0);
    }
    endwin();
}