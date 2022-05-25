#ifndef LEM_IPC_LEM_IPC_H
#define LEM_IPC_LEM_IPC_H

#include <stdlib.h>
#include <signal.h>
#include <curses.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>

#define HEIGHT      25
#define WIDTH       65
#define SIZE_BOARD  (HEIGHT * WIDTH)
#define PERM        0644
#define FTOK        ftok("./lem-ipc", 'U')

#define MSG_DIED    "DIED"
#define MSG_NEW     "NEW"

union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

typedef struct  s_msg
{
    int     team;
    char    str[256];
}               t_msg;

typedef struct  s_msgbuf
{
    long    mtype;
    t_msg   msg;
}               t_msgbuf;

typedef struct  s_resources
{
    int     shmid;
    int     msqid;
    int     semid;
    char    *board;
    bool    is_display;
    int     teams[8];
    char    teamid;
    int     x;
    int     y;
}               t_resources;

extern t_resources resources;

/**
 * lem-ipc.c
 */
bool    is_first_process(void);

/**
 * signal_handler.c
 */
bool    set_signal(void);
void    exit_handler(int sig);

/**
 * semaphores.c
 */
bool    sem_wait_systemv(void);
bool    sem_post_systemv(void);

/**
 * display.c
 */
void    init_ncurses(void);
void    draw_board(void);
char    winner_check(void);
void    msg_handler(t_msgbuf *msg);
void    display(void);

/**
 * player.c
 */
bool    check_space_player(void);
void    start_player(void);
bool    create_player(void);

/**
 * resources.c
 */
bool    create_resources(void);
bool    get_resources(void);
int     clean_resources(void);

/**
 * libft.c
 */
void    *ft_memset(void *s, int c, size_t n);
int     ft_atoi(const char *str);
int     ft_memcmp(const void *s1, const void *s2, size_t n);
size_t  ft_strlen(const char *s);

#endif
