#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_INPUT_LEN 5

typedef struct {
    int max;
    int guess_times;
} difficulty;

int is_number(char *input)
{
    int n = strlen(input);
    for(int i = 0; i < n; i++) {
        if (input[i] < '0' || input[i] > '9') { 
            return 0;
        }
    }
    return 1;
}

int main()
{
    srand(time(NULL));
    difficulty diff[] = {{50, 10}, {100, 7}, {500, 5}};

    int guess;
    int step;
    int choice;
    int target;
    int max_step;
    char input[MAX_INPUT_LEN];
    while(1)
    {
        printf("===猜数字游戏===\n");
        printf("1. 简单(1~50)\n");
        printf("2. 中等(1~100)\n");
        printf("3. 困难(1~500)\n");
        printf("请选择难度(1~3)：");
        scanf("%d", &choice);
        if (choice < 1 || choice > 3)
        {
            printf("无效的输入，请重新输入\n");
            continue;
        }
        difficulty d = diff[choice - 1];
        max_step = d.guess_times;
        target = rand() % d.max + 1;
        printf("目标数字已生成（1~%d），你有%d次机会\n", d.max, max_step);
        step = 0;
        int min = 1, max = d.max;
        while (step++ < max_step) {
            printf("第%d次猜测：", step);
            scanf("%s", input);
            if (!is_number(input)) {
                printf("无效的输入，请重新输入\n");
                continue;
            }
            guess = atoi(input);
            if (guess > target) 
            {
                max = guess;
                printf("→太大了，范围缩小到%d~%d\n", min, max);
            }
            else if (guess < target) 
            {
                min = guess;
                printf("太小了，范围缩小到%d~%d\n", min, max);
            }
            else
            {
                printf("🎉 恭喜你猜对了，答案是%d, 总共猜测次数：%d\n", target, step);
                break;
            }
            printf("剩余次数：%d\n", max_step - step);
        }
        char c;
        while (1) {
            printf("是否再来一局?(y/n)：");
            scanf(" %c", &c);
            if (c == 'y' || c == 'Y' 
                || c == 'n' || c == 'N')
            {
                break;
            } 
            else {
                printf("无效的输入，请重新输入\n");
            }
        }
        if (c == 'n' || c == 'N') {
            break;
        }
    }
    return 0;
}