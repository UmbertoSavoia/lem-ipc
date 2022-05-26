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

/**
 * check_surrounded()
 * @return numero nemici vicini
 */
char    check_surrounded(void)
{
    int pos = resources.y + WIDTH + resources.x;
    char enemy = 0;

    if (resources.x < WIDTH && resources.board[resources.y * WIDTH + (resources.x + 1)] != 0 &&
        resources.board[resources.y * WIDTH + (resources.x + 1)] != resources.teamid)
        enemy++;
    if (resources.x > 0 && resources.board[resources.y * WIDTH + (resources.x - 1)] != 0 &&
        resources.board[resources.y * WIDTH + (resources.x - 1)] != resources.teamid)
        enemy++;
    if (resources.y < HEIGHT && resources.board[(resources.y + 1) * WIDTH + resources.x] != 0 &&
        resources.board[(resources.y + 1) * WIDTH + resources.x] != resources.teamid)
        enemy++;
    if (resources.y > 0 && resources.board[(resources.y - 1) * WIDTH + resources.x] != 0 &&
        resources.board[(resources.y - 1) * WIDTH + resources.x] != resources.teamid)
        enemy++;
    return enemy;
}

void    send_refresh(void)
{
    t_msgbuf msg = {0};

    msg.mtype = 2;
    msg.msg.team = resources.teamid;
    snprintf(msg.msg.str, 256, "%s", MSG_REFRESH);
    msgsnd(resources.msqid, &msg, sizeof(t_msg), 0);
}

void    find_enemy(int *_x, int *_y)
{
    int post = 0;
    int pre = 0;
    int index = 0;
    int pos = resources.y * WIDTH + resources.x;

    for (int i = resources.y * WIDTH + resources.x; i < SIZE_BOARD; ++i)
        if (resources.board[i] != 0 && resources.board[i] != resources.teamid)
            post = i;
    for (int i = resources.y * WIDTH + resources.x; i >= 0; --i)
        if (resources.board[i] != 0 && resources.board[i] != resources.teamid)
            pre = i;
    index = (pos - pre > post - pos) ? pre : post;
    *_x = index % WIDTH;
    *_y = index / WIDTH;
}

void    new_pos_player(int x, int y)
{
    sem_wait_systemv();
    resources.board[resources.y * WIDTH + resources.x] = 0;
    resources.board[y * WIDTH + x] = resources.teamid;
    sem_post_systemv();
    send_refresh();
    resources.x = x;
    resources.y = y;
}

void    play(void)
{
    int x = -1;
    int y = -1;

    find_enemy(&x, &y);
    if (x != -1 && y != -1) {
        if (x < resources.x && resources.x-1 != x) {
            new_pos_player(resources.x-1, resources.y);
        } else if (x > resources.x && resources.x+1 != x) {
            new_pos_player(resources.x+1, resources.y);
        } else if (y < resources.y && resources.y-1 != y) {
            new_pos_player(resources.x, resources.y-1);
        } else if (y > resources.y && resources.y+1 != y) {
            new_pos_player(resources.x, resources.y+1);
        }
    }
}

void    player_dead(void)
{
    t_msgbuf msg = {0};

    msg.mtype = 2;
    msg.msg.team = resources.teamid;
    snprintf(msg.msg.str, 256, "%s", MSG_DIED);
    sem_wait_systemv();
    resources.board[resources.y * WIDTH + resources.x] = 0;
    sem_post_systemv();
    msgsnd(resources.msqid, &msg, sizeof(t_msg), 0);
}

void    start_player(void)
{
    while (1) {
        switch (check_surrounded()) {
            case 0:
                play();
                usleep(100000);
                break;
            case 1:
                sleep(1);
                break;
            default:
                resources.player_dead = TRUE;
                player_dead();
                return;
        }
    }
}