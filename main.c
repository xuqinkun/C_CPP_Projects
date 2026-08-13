#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    int t = rand();
    printf("%d\n", t);
    return 0;
}