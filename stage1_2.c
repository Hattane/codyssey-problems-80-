#include <stdio.h>
#include <string.h>

#define MAX_FOOD 7
#define MAX_NAME 50

typedef struct {
    char name[MAX_NAME];
    int calories;
} Food;

Food stack[MAX_FOOD];
int top = -1;

int isValidCalorie(int cal) {
    int allowed[] = {100, 150, 200, 250, 300};
    for (int i = 0; i < 5; i++)
        if (allowed[i] == cal) return 1;
    return 0;
}

int push(char name[], int cal, int currentTotal) {
    if (top >= MAX_FOOD - 1) {
        printf("식단 입력은 하루 최대 7개입니다.\n");
        return currentTotal;
    }
    if (currentTotal + cal > 1500) {
        printf("총 칼로리는 1500을 초과할 수 없습니다.\n");
        return currentTotal;
    }

    ++top;
    strcpy(stack[top].name, name);
    stack[top].calories = cal;
    return currentTotal + cal;
}

void viewDailyFood() {
    printf("\n[오늘 섭취한 음식 목록]\n");
    for (int i = 0; i <= top; i++)
        printf("%d. %s (%d kcal)\n", i + 1, stack[i].name, stack[i].calories);
}

void setMyDailyFood() {
    char foodName[MAX_NAME];
    int calorie, totalCalories = 0;

    while (top < MAX_FOOD - 1 && totalCalories < 1500) {
        printf("음식 이름 입력 (종료: END): ");
        fgets(foodName, MAX_NAME, stdin);
        foodName[strcspn(foodName, "\n")] = 0;
        if (strcmp(foodName, "END") == 0) break;

        printf("칼로리 입력 (100/150/200/250/300): ");
        scanf("%d", &calorie); getchar();

        if (!isValidCalorie(calorie)) {
            printf("허용되지 않는 칼로리입니다.\n");
            continue;
        }

        totalCalories = push(foodName, calorie, totalCalories);
        printf("현재 총 칼로리: %d kcal\n", totalCalories);
    }

    viewDailyFood();
    printf("최종 총 섭취 칼로리: %d kcal\n", totalCalories);
}
