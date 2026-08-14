#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 16
#define MAX_NAME_LEN 20
#define SCORE_FILE "score.txt"
#define DIFF_COUNT 3

typedef struct {
    char desc[MAX_NAME_LEN];
    int max;
    int guess_times;
    int time_limit;
} difficulty;

typedef struct {
    char name[MAX_NAME_LEN];
    int score;
    int diff;
} record;

static void clear_line(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

static int is_number(const char *input)
{
    if (input == NULL || input[0] == '\0') {
        return 0;
    }
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isdigit((unsigned char)input[i])) {
            return 0;
        }
    }
    return 1;
}

static int read_token(char *buf, int size)
{
    char fmt[16];
    snprintf(fmt, sizeof fmt, "%%%ds", size - 1);
    return scanf(fmt, buf) == 1;
}

static int load_record(record *rec)
{
    FILE *fp = fopen(SCORE_FILE, "r");
    if (fp == NULL) {
        return 0;
    }
    int ok = fscanf(fp, "%19s %d %d", rec->name, &rec->score, &rec->diff) == 3;
    fclose(fp);
    if (!ok || rec->diff < 1 || rec->diff > DIFF_COUNT) {
        return 0;
    }
    return 1;
}

static int save_record(const record *rec)
{
    FILE *fp = fopen(SCORE_FILE, "w");
    if (fp == NULL) {
        printf("保存最高分失败\n");
        return 0;
    }
    fprintf(fp, "%s %d %d\n", rec->name, rec->score, rec->diff);
    fclose(fp);
    return 1;
}

static double elapsed_sec(clock_t start)
{
    time_t now = time(NULL);
    return (double)(now - start);
}

static int timed_out(clock_t start, int time_limit)
{
    return elapsed_sec(start) >= time_limit;
}

int main(void)
{
    difficulty diff[] = {
        {"简单", 50, 10, 60},
        {"中等", 100, 7, 45},
        {"困难", 500, 5, 30}
    };

    srand((unsigned)time(NULL));

    char player[MAX_NAME_LEN];
    printf("请输入你的名字：");
    if (!read_token(player, MAX_NAME_LEN)) {
        printf("读取名字失败\n");
        return 1;
    }

    record rec = {0};
    int has_record = load_record(&rec);
    if (has_record) {
        printf("最高分：%s %d %s\n", rec.name, rec.score, diff[rec.diff - 1].desc);
    } else {
        printf("暂无最高分记录\n");
    }

    while (1) {
        printf("===猜数字游戏===\n");
        printf("1. 简单(1~50)\n");
        printf("2. 中等(1~100)\n");
        printf("3. 困难(1~500)\n");
        printf("请选择难度(1~3)：");

        char input[MAX_INPUT_LEN];
        if (!read_token(input, MAX_INPUT_LEN) || !is_number(input)) {
            printf("无效的输入，请重新输入\n");
            continue;
        }
        int choice = atoi(input);
        if (choice < 1 || choice > DIFF_COUNT) {
            printf("无效的输入，请重新输入\n");
            continue;
        }

        difficulty d = diff[choice - 1];

        printf("1. 普通模式\n");
        printf("2. 计时模式（限时%d秒）\n", d.time_limit);
        printf("请选择模式(1~2)：");
        if (!read_token(input, MAX_INPUT_LEN) || !is_number(input)) {
            printf("无效的输入，请重新输入\n");
            continue;
        }
        int mode = atoi(input);
        if (mode != 1 && mode != 2) {
            printf("无效的输入，请重新输入\n");
            continue;
        }
        int timed = (mode == 2);

        int max_step = d.guess_times;
        int target = rand() % d.max + 1;
        printf("目标数字已生成（1~%d），你有%d次机会", d.max, max_step);
        if (timed) {
            printf("，限时%d秒", d.time_limit);
        }
        printf("\n");

        int step = 0;
        int min = 1;
        int max = d.max;
        int won = 0;
        int timeout = 0;
        clock_t start = time(NULL);

        while (step < max_step) {
            if (timed && timed_out(start, d.time_limit)) {
                timeout = 1;
                break;
            }
            if (timed) {
                printf("第%d次猜测（剩余%.1f秒）：",
                       step + 1, d.time_limit - elapsed_sec(start));
            } else {
                printf("第%d次猜测：", step + 1);
            }
            if (!read_token(input, MAX_INPUT_LEN) || !is_number(input)) {
                if (timed && timed_out(start, d.time_limit)) {
                    timeout = 1;
                    break;
                }
                printf("无效的输入，请重新输入\n");
                continue;
            }
            if (timed && timed_out(start, d.time_limit)) {
                timeout = 1;
                break;
            }
            int guess = atoi(input);
            if (guess < min || guess > max) {
                printf("请输入 %d~%d 之间的数字\n", min, max);
                continue;
            }

            step++;
            if (guess > target) {
                max = guess - 1;
                printf("→太大了，范围缩小到%d~%d\n", min, max);
            } else if (guess < target) {
                min = guess + 1;
                printf("太小了，范围缩小到%d~%d\n", min, max);
            } else {
                int score = (max_step - step + 1) * choice * 100;
                printf("🎉 恭喜你猜对了，答案是%d, 总共猜测次数：%d 得分：%d",
                       target, step, score);
                if (timed) {
                    printf("，耗时%.1f秒", elapsed_sec(start));
                }
                printf("\n");
                if (!has_record || score > rec.score) {
                    snprintf(rec.name, sizeof rec.name, "%s", player);
                    rec.score = score;
                    rec.diff = choice;
                    if (save_record(&rec)) {
                        has_record = 1;
                    }
                }
                won = 1;
                break;
            }
            printf("剩余次数：%d\n", max_step - step);
        }

        if (timeout) {
            printf("时间到！本局耗时%.1f秒，正确答案是%d\n",
                   elapsed_sec(start), target);
        } else if (!won) {
            printf("次数用尽，正确答案是%d\n", target);
        }

        char c;
        while (1) {
            printf("是否再来一局?(y/n)：");
            if (scanf(" %c", &c) != 1) {
                printf("读取输入失败\n");
                return 1;
            }
            clear_line();
            if (c == 'y' || c == 'Y' || c == 'n' || c == 'N') {
                break;
            }
            printf("无效的输入，请重新输入\n");
        }
        if (c == 'n' || c == 'N') {
            break;
        }
    }
    return 0;
}
