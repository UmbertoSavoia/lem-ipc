#include "../include/lem-ipc.h"

bool    check_space_player(void)
{
    for (int i = 0; i < SIZE_BOARD; ++i)
        if (!resources.board[i])
            return TRUE;
    return FALSE;
}

bool    create_player(void)
{
    t_msgbuf msg = {0};

    if (!check_space_player())
        return FALSE;
    if (!sem_wait_systemv())
        return FALSE;
    while (1) {
        resources.x = rand() % WIDTH;
        resources.y = rand() % HEIGHT;
        if (!resources.board[resources.y * WIDTH + resources.x]) {
            resources.board[resources.y * WIDTH + resources.x] = resources.teamid;
            break;
        }
    }
    if (!sem_post_systemv())
        return FALSE;
    snprintf(msg.msg.str, 256, "%s", MSG_NEW);
    msg.msg.team = resources.teamid;
    msg.mtype = 2;
    if (msgsnd(resources.msqid, &msg, sizeof(t_msg), 0) < 0)
        return FALSE;
    return TRUE;
}

void    start_player(void)
{
    sleep(2);
    t_msgbuf msg = {0};
    msg.mtype = 2;
    msg.msg.team = resources.teamid;
    snprintf(msg.msg.str, 256, "%s", MSG_DIED);
    resources.board[resources.y * WIDTH + resources.x] = 0;
    msgsnd(resources.msqid, &msg, sizeof(t_msg), 0);
}