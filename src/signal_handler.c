#include "../include/lem-ipc.h"

void    exit_handler(int sig)
{
    clean_resources();
    exit(0);
}

bool    set_signal(void)
{
    struct  sigaction sa = {0};

    sa.sa_handler = exit_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, 0) < 0)
        return FALSE;
    return TRUE;
}