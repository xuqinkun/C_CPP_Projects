#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    int max = 100;
    int target = rand() % max + 1;
    int guess;
    int step = 0;
    while(1)
    {
        printf("请输入猜测的数字(1~%d)：", max);
        scanf("%d", &guess);
        step++;
        if (guess > target) 
        {
            printf("太大了\n");
        }
        else if (guess < target) 
        {
            printf("太小了\n");
        }
        else
        {
            break;
        }
    }
    printf("恭喜你猜对了，总共猜测次数：%d\n", step);
    return 0;
}