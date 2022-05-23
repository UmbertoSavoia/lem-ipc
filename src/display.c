#include "../include/lem-ipc.h"

void    init_ncurses(void)
{
    initscr();
    keypad(stdscr, TRUE);
    nonl();
    cbreak();
    noecho();

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

void    draw_board(void)
{
    for (int i = 0; i < WIDTH+1; ++i) {
        mvaddch(0, i, ' ' | COLOR_PAIR(7));
        mvaddch(HEIGHT+1, i, ' ' | COLOR_PAIR(7));
    }
    for (int i = 0; i < HEIGHT+2; ++i) {
        mvaddch(i, 0, ' ' | COLOR_PAIR(7));
        mvaddch(i, WIDTH+1, ' ' | COLOR_PAIR(7));
    }
    for (char i = 0; i < HEIGHT; ++i)
        for (char j = 0; j < WIDTH; ++j)
            mvaddch(i+1, j+1, ' ' | COLOR_PAIR(2));
}

void    display(void)
{
    init_ncurses();

    while (1) {
        int c = getch();
        draw_board();
    }
}