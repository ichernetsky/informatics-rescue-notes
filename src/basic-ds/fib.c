#include <stdio.h>

unsigned long fib_slow(unsigned long n)
{
    if (n <= 1)
        return n;
    return fib_slow(n - 2) + fib_slow(n - 1);
}

unsigned long fib_fast(unsigned long n)
{
    unsigned long t[n + 1];

    if (n <= 1)
        return n;

    t[0] = 0;
    t[1] = 1;
    for (unsigned long i = 2; i <= n; i++)
        t[i] = t[i - 2] + t[i - 1];

    return t[n];
}

unsigned long fib_fastest(unsigned long n)
{
    unsigned long t[] = { 0UL, 1UL };
    unsigned long tmp;

    if (n <= 1)
        return n;

    for (unsigned long i = 2; i <= n; i++) {
        tmp = t[0];
        t[0] = t[1];
        t[1] = t[1] + tmp;
    }

    return t[1];
}

int main(void)
{
    printf("%lu\n", fib_fastest(100));
    return 0;
}
