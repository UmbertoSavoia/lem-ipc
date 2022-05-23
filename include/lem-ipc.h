#ifndef LEM_IPC_LEM_IPC_H
#define LEM_IPC_LEM_IPC_H

#include <stdlib.h>
#include <signal.h>
#include <curses.h>
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

typedef struct  s_resources
{
    int     shmid;
    int     msqid;
    int     semid;
    char    *board;
    bool    is_display;
    char    teams[8];
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
 * display.c
 */
void    init_ncurses(void);
void    draw_board(void);
void    display(void);

/**
 * resources.c
 */
bool    create_resources(void);
void    clean_resources(void);

/**
 * libft.c
 */
void    *ft_memset(void *s, int c, size_t n);

#endif
