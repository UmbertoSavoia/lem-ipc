#include "../include/lem-ipc.h"

void    init_ncurses(void)
{
    initscr();
    keypad(stdscr, TRUE);
    nonl();
    cbreak();
    noecho();
    curs_set(0);

    if (has_colors())
    {
        start_color();

        init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
        init_pair(COLOR_RED, COLOR_RED, COLOR_RED);
        init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_GREEN);
        init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLUE);
        init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
        init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_CYAN);
        init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    }
}

void    draw_borders(void)
{
    for (int i = 0; i < WIDTH+1; ++i) {
        mvaddch(0, i, ' ' | COLOR_PAIR(7));
        mvaddch(HEIGHT+1, i, ' ' | COLOR_PAIR(7));
    }
    for (int i = 0; i < HEIGHT+2; ++i) {
        mvaddch(i, 0, ' ' | COLOR_PAIR(7));
        mvaddch(i, WIDTH+1, ' ' | COLOR_PAIR(7));
    }
}

void    draw_board(void)
{
    for (char y = 0; y < HEIGHT; ++y)
        for (char x = 0; x < WIDTH; ++x)
            mvaddch(y+1, x+1, ' ' | COLOR_PAIR(resources.board[y * WIDTH + x]));
    for (char i = 1; i < 8; ++i) {
        if (resources.teams[i] > 0) {
            mvaddch(5+i, WIDTH+10, ' ' | COLOR_PAIR(i));
            mvprintw(5+i, WIDTH+11, " Team %d - Giocatori: %d", i, resources.teams[i]);
        } else {
            mvprintw(5+i, WIDTH+10, "                       ");
        };
    }
}

char    winner_check(void)
{
    char ret = 0;
    char team = 0;

    for (char i = 1; i < 8; ++i) {
        if (resources.teams[i] > 0) {
            ret++;
            team = i;
        }
    }
    return (ret != 1) ? 0 : team;
}

void    remove_pid(int pid)
{
    for (int i = 0; i < SIZE_BOARD; ++i) {
        if (resources.pids[i] == pid) {
            resources.pids[i] = 0;
            break;
        }
    }
}

void    add_pid(int pid)
{
    for (int i = 0; i < SIZE_BOARD; ++i) {
        if (resources.pids[i] == 0) {
            resources.pids[i] = pid;
            break;
        }
    }
}

void    send_finish(void)
{
    for (int i = 0; i < SIZE_BOARD; ++i) {
        if (resources.pids[i] != 0)
            kill(resources.pids[i], SIGINT);
    }
}

void    msg_handler(t_msgbuf *msg)
{
    char        winner = 0;

    if (!ft_memcmp(msg->msg.str, MSG_DIED, ft_strlen(MSG_DIED))) {
        resources.teams[msg->msg.team] -= 1;
        remove_pid(msg->msg.pid);
        if ((winner = winner_check()) > 0) {
            mvprintw(HEIGHT / 2, WIDTH / 2, "Il team %d ha VINTO!!!", winner);
            refresh();
            send_finish();
            sleep(4);
            exit_handler(0);
        }
    } else if (!ft_memcmp(msg->msg.str, MSG_NEW, ft_strlen(MSG_NEW))) {
        resources.teams[msg->msg.team] += 1;
        add_pid(msg->msg.pid);
    }
}

void    display(void)
{
    t_msgbuf    msg = {0};

    init_ncurses();
    draw_borders();

    while (1) {
        //clear();
        draw_board();
        refresh();
        if (msgrcv(resources.msqid, &msg, sizeof msg.msg, 0, 0) == -1)
            break;
        msg_handler(&msg);
    }
}