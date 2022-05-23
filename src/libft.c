#include "../include/lem-ipc.h"

void    *ft_memset(void *s, int c, size_t n)
{
    char *temp;

    temp = s;
    while (n--)
        *temp++ = (unsigned char)c;
    return (s);
}