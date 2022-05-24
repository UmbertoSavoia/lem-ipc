#include "../include/lem-ipc.h"

void    *ft_memset(void *s, int c, size_t n)
{
    char *temp;

    temp = s;
    while (n--)
        *temp++ = (unsigned char)c;
    return (s);
}

int     ft_atoi(const char *str)
{
    int ret = 0;
    int neg = 1;

    while (*str == '\n' || *str == '\t' || *str == ' '
           || *str == '\v' || *str == '\r' || *str == '\f')
        str++;
    if (*str == '+' || *str == '-') {
        if (*str == '-')
            neg *= -1;
        str++;
    }
    while (*str >= '0' && *str <= '9') {
        ret = ret * 10 + (*str - '0');
        str++;
    }
    return (ret * neg);
}

int ft_memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *su1 = s1;
    const unsigned char *su2 = s2;
    int                 ret = 0;

    while (0 < n) {
        if ((ret = *su1 - *su2) != 0)
            break ;
        su1++;
        su2++;
        n--;
    }
    return (ret);
}

size_t  ft_strlen(const char *s)
{
    size_t ret = 0;

    while (s && *s++)
        ret++;
    return (ret);
}