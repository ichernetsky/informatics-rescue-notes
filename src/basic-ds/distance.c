#include <stdio.h>
#include <string.h>

#define min(a, b)                               \
    ({  __typeof__(a) _a = (a);                 \
        __typeof__(b) _b = (b);                 \
        _a < _b ? _a : _b; })

void distance(char *word1, char *word2, unsigned int *distances)
{
    unsigned int m = strlen(word1) + 1;
    unsigned int n = strlen(word2) + 1;
    unsigned int i, j;
    unsigned int add, del, change;

    for (i = 0; i < m; i++)
        distances[i * n] = i;
    for (j = 1; j < n; j++)
        distances[j] = j;

    for (i = 1; i < m; i++)
        for (j = 1; j < n; j++) {
            add = distances[i * n + (j - 1)] + 1;
            del = distances[(i - 1) * n + j] + 1;
            change = (word1[i - 1] == word2[j - 1]) ? 0 : 1;
            change += distances[(i - 1) * n + (j - 1)];

            distances[i * n + j] = min(min(add, del), change);
        }
}

int main(void)
{
    char *word1 = "cat";
    char *word2 = "kate";
    unsigned int m = strlen(word1) + 1;
    unsigned int n = strlen(word2) + 1;
    unsigned int distances[m][n];
    unsigned int i, j;

    distance(word1, word2, (unsigned int *)distances);

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++)
            printf("%u ", distances[i][j]);
        printf("\n");
    }

    return 0;
}
