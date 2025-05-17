#include <stdio.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_FOOD 7

// Food structure
typedef struct {
    char name[MAX_NAME];
    int calorie;
} Food;

// Queue for food tracking
typedef struct {
    Food foods[MAX_FOOD];
    int front, rear, size;
} FoodQueue;

// Initialize queue
void initQueue(FoodQueue *q) {
    q->front = 0;
    q->rear = -1;
    q->size = 0;
}

// Add food to queue
int enqueue(FoodQueue *q, const char *name, int calorie) {
    if (q->size >= MAX_FOOD) {
        printf("\n식단은 하루 최대 %d개까지 입력 가능합니다.\n", MAX_FOOD);
        return 0;
    }
    int totalCalories = 0;
    for (int i = 0; i < q->size; i++) {
        int idx = (q->front + i) % MAX_FOOD;
        totalCalories += q->foods[idx].calorie;
    }
    if (totalCalories + calorie > 1500) {
        printf("총 칼로리 제한(1500kcal)을 초과했습니다.\n");
        return 0;
    }
    q->rear = (q->rear + 1) % MAX_FOOD;
    strcpy(q->foods[q->rear].name, name);
    q->foods[q->rear].calorie = calorie;
    q->size++;
    return 1;
}

// Show current food records
void printFoodQueue(FoodQueue *q) {
    printf("\n[남은 식단 기록]\n");
    int total = 0;
    for (int i = 0; i < q->size; i++) {
        int index = (q->front + i) % MAX_FOOD;
        printf("%d. %s - %d kcal\n", i + 1, q->foods[index].name, q->foods[index].calorie);
        total += q->foods[index].calorie;
    }
    printf("총 섭취 칼로리: %d kcal\n", total);
}

// Set daily food (Problem 8)
void setMyDailyFood(FoodQueue *q) {
    int calories[] = {100, 150, 200, 250, 300};
    char foodName[MAX_NAME];
    int cal, success;
    printf("[식단 입력 - 하루 최대 7개, 1500kcal 이하]\n");
    while (q->size < MAX_FOOD) {
        printf("음식명 입력 (종료는 end): ");
        scanf("%s", foodName);
        if (strcmp(foodName, "end") == 0) break;

        printf("칼로리 선택 (100,150,200,250,300): ");
        scanf("%d", &cal);

        success = 0;
        for (int i = 0; i < 5; i++) {
            if (cal == calories[i]) {
                success = 1;
                break;
            }
        }
        if (!success) {
            printf("잘못된 칼로리입니다. 다시 입력해주세요.\n");
            continue;
        }

        if (!enqueue(q, foodName, cal)) break;
    }
    printFoodQueue(q);
}

// Manage calorie (Problem 9)
void mngMyDailyCalorie(FoodQueue *q) {
    int exerciseCalories[] = {0, 200, 250, 300, 150, 250, 300, 100};
    int exercise;
    printf("\n운동 종류 번호 선택 (1~7): ");
    scanf("%d", &exercise);

    if (exercise < 1 || exercise > 7) {
        printf("잘못된 입력입니다.\n");
        return;
    }

    int burn = exerciseCalories[exercise];

    if (q->size == 0) {
        printf("식단 기록이 없습니다.\n");
        return;
    }

    Food *frontFood = &q->foods[q->front];
    if (burn >= frontFood->calorie) {
        printf("%s (%dkcal) 제거됨. 소모된 칼로리: %d kcal\n", frontFood->name, frontFood->calorie, burn);
        q->front = (q->front + 1) % MAX_FOOD;
        q->size--;
    } else {
        printf("운동량 부족! %s (%dkcal)를 제거하려면 %d kcal가 더 필요합니다.\n",
               frontFood->name, frontFood->calorie, frontFood->calorie - burn);
    }

    printFoodQueue(q);
}

// Test main
int main() {
    FoodQueue q;
    initQueue(&q);

    int choice;
    while (1) {
        printf("\n[메뉴]\n1. 식단 입력\n2. 칼로리 관리\n0. 종료\n선택: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: setMyDailyFood(&q); break;
            case 2: mngMyDailyCalorie(&q); break;
            case 0: return 0;
            default: printf("잘못된 선택입니다.\n");
        }
    }

    return 0;
}
