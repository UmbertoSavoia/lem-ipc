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
    int team = 0;

    if (!set_signal())
        return 1;
    if (is_first_process()) {
        if (!create_resources()) {
            clean_resources();
            return 2;
        }
        display();
    } else {
        if (ac < 2 || !(team = ft_atoi(av[1])) ||
            (team < 1 || team > 7) || !get_resources())
            return 3;
        resources.is_display = 0;
        resources.teams[team] += 1;
        resources.teamid = team;
    }
    exit_handler(0);
}