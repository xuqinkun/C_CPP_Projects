#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_INPUT_LEN 5
#define MAX_NAME_LEN 20

typedef struct {
    char desc[MAX_NAME_LEN];
    int max;
    int guess_times;
} difficulty;

typedef struct {
    char name[MAX_NAME_LEN];
    int score;
    int diff;
} record;

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
    difficulty diff[] = {{"简单", 50, 10}, {"中等", 100, 7}, {"困难", 500, 5}};

    int guess;
    int step;
    int choice;
    int target;
    int max_step;
    char input[MAX_INPUT_LEN];
    char player[MAX_NAME_LEN];

    record rec;
    printf("请输入你的名字：");
    scanf("%19s", player);
    FILE *fp = fopen("score.txt", "r");
    if (fp == NULL) {
        printf("文件打开失败\n");
        return 1;
    }
    size_t items_read = fread(&rec, sizeof(record), 1, fp);
    if (items_read) {
        printf("最高分：%s %d %s\n", rec.name, rec.score, diff[rec.diff - 1].desc);
    }
    fclose(fp);
    while(1)
    {
        printf("===猜数字游戏===\n");
        printf("1. 简单(1~50)\n");
        printf("2. 中等(1~100)\n");
        printf("3. 困难(1~500)\n");
        printf("请选择难度(1~3)：");
        scanf("%4d", &choice);
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
        while (step < max_step) {
            printf("第%d次猜测：", step);
            scanf("%s", input);
            if (!is_number(input)) {
                printf("无效的输入，请重新输入\n");
                continue;
            }
            step++;
            guess = atoi(input);
            if (guess > target) 
            {
                max = guess - 1;
                printf("→太大了，范围缩小到%d~%d\n", min, max);
            }
            else if (guess < target) 
            {
                min = guess + 1;
                printf("太小了，范围缩小到%d~%d\n", min, max);
            }
            else
            {
                int score = (max_step - step) * choice * 100;
                printf("🎉 恭喜你猜对了，答案是%d, 总共猜测次数：%d 得分：%d\n", target, step, score);
                if ((items_read && score > rec.score) || items_read == 0) {
                    strcpy(rec.name, player);
                    rec.score = score;
                    rec.diff = choice; 
                    fp = fopen("score.txt", "w");
                    fwrite(&rec, sizeof(record), 1, fp);
                }
                fclose(fp);
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