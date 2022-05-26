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

unsigned long   mix(unsigned long a, unsigned long b, unsigned long c)
{
    a=a-b;  a=a-c;  a=a^(c >> 13);
    b=b-c;  b=b-a;  b=b^(a << 8);
    c=c-a;  c=c-b;  c=c^(b >> 13);
    a=a-b;  a=a-c;  a=a^(c >> 12);
    b=b-c;  b=b-a;  b=b^(a << 16);
    c=c-a;  c=c-b;  c=c^(b >> 5);
    a=a-b;  a=a-c;  a=a^(c >> 3);
    b=b-c;  b=b-a;  b=b^(a << 10);
    c=c-a;  c=c-b;  c=c^(b >> 15);
    return c;
}

int     main(int ac, char **av)
{
    int team = 0;
    unsigned long seed = 0;

    seed = mix(clock(), time(0), getpid());
    srand(seed);
    if (!set_signal())
        return 1;
    if (is_first_process()) {
        if (!create_resources() && clean_resources())
            return 2;
        display();
    } else {
        if (ac < 2 || !(team = ft_atoi(av[1])) ||
            (team < 1 || team > 7) || !get_resources())
            return 3;
        resources.is_display = 0;
        resources.teams[team] += 1;
        resources.teamid = team;
        if (create_player())
            start_player();
        else
            puts("Il giocatore non può essere aggiunto!");
    }
    exit_handler(0);
}