#include "../include/lem-ipc.h"

bool create_resources(void)
{
    key_t key = FTOK;
    struct sembuf sb;

    if ((resources.shmid = shmget(key, SIZE_BOARD, PERM | IPC_CREAT)) < 0)
        return FALSE;
    if ((resources.board = shmat(resources.shmid, 0, 0)) == (char *)-1)
        return FALSE;
    ft_memset(resources.board, 0, SIZE_BOARD);
    if ((resources.msqid = msgget(key, PERM | IPC_CREAT)) < 0)
        return FALSE;
    if ((resources.semid = semget(key, 1, PERM | IPC_CREAT)) < 0)
        return FALSE;
    sb.sem_num = sb.sem_flg = 0;
    sb.sem_op = 1;
    if (semop(resources.semid, &sb, 1) < 0)
        return FALSE;

    return TRUE;
}

bool    wait_init_sem(void)
{
    int ready = 0;
    struct semid_ds buf;
    union semun arg;

    arg.buf = &buf;
    for (int i = 0; i < 10 && !ready; ++i) {
        semctl(resources.semid, 0, IPC_STAT, arg);
        if (arg.buf->sem_otime != 0)
            ready = 1;
        else
            sleep(1);
    }
    if (!ready)
        return FALSE;
    return TRUE;
}

bool    get_resources(void)
{
    key_t key = FTOK;

    if ((resources.semid = semget(key, 1, PERM)) < 0)
        return FALSE;
    if (!wait_init_sem())
        return FALSE;
    if ((resources.shmid = shmget(key, SIZE_BOARD, PERM)) < 0)
        return FALSE;
    if ((resources.board = shmat(resources.shmid, 0, 0)) == (char *)-1)
        return FALSE;
    if ((resources.msqid = msgget(key, PERM)) < 0)
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