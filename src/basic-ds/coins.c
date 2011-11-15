#include <stdio.h>
#include <stdbool.h>

#define array_size(a)                           \
    (sizeof(a) / sizeof((a)[0]))                \

#define min(a, b)                               \
    ({  __typeof__(a) _a = (a);                 \
        __typeof__(b) _b = (b);                 \
        _a < _b ? _a : _b; })

void change_coins(unsigned int n, unsigned int k, unsigned int coins[k],
                  unsigned int solution[k][n + 1])
{
    unsigned int i, j;

    for (i = 0; i < k; i++)
        solution[i][0] = 0;
    for (j = 1; j <= n; j++)
        solution[0][j] = j;

    for (i = 1; i < k; i++)
        for (j = 1; j <= n; j++)
            if (j < coins[i])
                solution[i][j] = solution[i - 1][j];
            else
                solution[i][j] = min(solution[i - 1][j],
                                     1 + solution[i][j - coins[i]]);
}

void recover_coins(unsigned int n, unsigned int k, unsigned int coins[k],
                   unsigned int solution[k][n + 1], unsigned int counts[k])
{
    unsigned int i, j;

    for (i = 0; i < k; i++)
        counts[i] = 0;

    i = k - 1;
    j = n;
    while (true) {
        if (0 == j)
            break;

        if (0 == i) {
            counts[0] += j;
            break;
        }

        if (solution[i - 1][j] < (1 + solution[i][j - coins[i]]))
            i -= 1;
        else {
            counts[i] += 1;
            j -= coins[i];
        }
    }
}

int main(void)
{
    unsigned int i, j;
    unsigned int n = 8;
    unsigned int coins[] = { 1, 4, 6 };
    unsigned int k = array_size(coins);
    unsigned int solution[k][n + 1];
    unsigned int counts[k];

    change_coins(n, k, coins, solution);

    for (i = 0; i < k; i++) {
        for (j = 0; j <= n; j++)
            printf("%d ", solution[i][j]);
        printf("\n");
    }

    if (0 == solution[k - 1][n])
        printf("No solution\n");
    else {
        recover_coins(n, k, coins, solution, counts);

        for (i = 0; i < k; i++)
            printf ("%d kop.: %d\n", coins[i], counts[i]);
    }

    return 0;
}
