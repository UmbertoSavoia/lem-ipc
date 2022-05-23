#include "../include/lem-ipc.h"

t_resources resources = {0};

bool    is_first_process(void)
{
    key_t   key = 0;
    int     shmid = 0;
    int     msqid = 0;

    key = FTOK;
    if ((shmid = shmget(key, SIZE_BOARD, PERM)) < 0 &&
            (msqid = msgget(key, PERM)) < 0) {
        resources.is_display = TRUE;
        return TRUE;
    }
    resources.is_display = FALSE;
    return FALSE;
}

int     main(int ac, char **av)
{
    if (!set_signal())
        return 1;
    if (is_first_process()) {
        if (!create_resources()) {
            clean_resources();
            return 2;
        }
        display();
    }
    exit_handler(0);
}