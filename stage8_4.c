#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stage8.h"

// 전처리기 - gcc 9.x 버전 체크 (보너스)
#if !defined(__GNUC__) || __GNUC__ != 9
    #error "컴파일러 버전이 맞지 않아 더 이상 컴파일을 진행할 수 없습니다."
#endif

// 스택 (나쁜 습관) 구조
typedef struct {
    Node *top;
} Stack;

static void stackInit(Stack *s) {
    s->top = NULL;
}

static int stackIsEmpty(Stack *s) {
    return s->top == NULL;
}

static void stackPush(Stack *s, const char *habit) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        perror("메모리 할당 실패");
        exit(EXIT_FAILURE);
    }
    strcpy(node->habit, habit);
    node->next = s->top;
    s->top = node;
}

static void stackPop(Stack *s) {
    if (stackIsEmpty(s)) return;
    Node *temp = s->top;
    s->top = s->top->next;
    free(temp);
}

static void stackPrint(Stack *s) {
    Node *cur = s->top;
    printf("[나쁜 습관]\n");
    if (!cur) {
        printf("없음\n");
        return;
    }
    while (cur) {
        printf("- %s\n", cur->habit);
        cur = cur->next;
    }
}

// 큐 (좋은 습관) 구조
typedef struct {
    Node *front;
    Node *rear;
} Queue;

static void queueInit(Queue *q) {
    q->front = q->rear = NULL;
}

static int queueIsEmpty(Queue *q) {
    return q->front == NULL;
}

static void queueEnqueue(Queue *q, const char *habit) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        perror("메모리 할당 실패");
        exit(EXIT_FAILURE);
    }
    strcpy(node->habit, habit);
    node->next = NULL;
    if (queueIsEmpty(q)) {
        q->front = q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
}

static void queueDequeue(Queue *q) {
    if (queueIsEmpty(q)) return;
    Node *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
}

static void queuePrint(Queue *q) {
    Node *cur = q->front;
    printf("[좋은 습관]\n");
    if (!cur) {
        printf("없음\n");
        return;
    }
    while (cur) {
        printf("- %s\n", cur->habit);
        cur = cur->next;
    }
}

// 파일에서 습관 읽기 및 초기화
static void loadHabits(const char *filename, Stack *badStack, Queue *goodQueue) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("파일 열기 실패");
        exit(EXIT_FAILURE);
    }
    char line[256];
    enum { NONE, BAD, GOOD } section = NONE;

    while (fgets(line, sizeof(line), fp)) {
        // 줄 끝 개행 제거
        line[strcspn(line, "\r\n")] = 0;

        if (strcmp(line, "[나쁜 습관]") == 0) {
            section = BAD;
            continue;
        } else if (strcmp(line, "[좋은 습관]") == 0) {
            section = GOOD;
            continue;
        }

        if (section == BAD && line[0] != '\0') {
            stackPush(badStack, line);
        } else if (section == GOOD && line[0] != '\0') {
            queueEnqueue(goodQueue, line);
        }
    }
    fclose(fp);
}

void promoteGoodHabit(void) {
    Stack badStack;
    Queue goodQueue;
    stackInit(&badStack);
    queueInit(&goodQueue);

    loadHabits("habit.txt", &badStack, &goodQueue);

    int choice;
    while (1) {
        printf("\n1. 나쁜 습관 제거\n");
        printf("2. 좋은 습관 드러내기\n");
        printf("0. 종료\n");
        printf("선택: ");
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); // 입력 버퍼 정리
            printf("잘못된 입력입니다.\n");
            continue;
        }

        switch(choice) {
            case 1:
                if (stackIsEmpty(&badStack)) {
                    printf("제거할 나쁜 습관이 없습니다.\n");
                } else {
                    printf("제거된 나쁜 습관: %s\n", badStack.top->habit);
                    stackPop(&badStack);
                    stackPrint(&badStack);
                }
                break;
            case 2:
                if (queueIsEmpty(&goodQueue)) {
                    printf("드러낼 좋은 습관이 없습니다.\n");
                } else {
                    printf("드러낸 좋은 습관: %s\n", goodQueue.front->habit);
                    queueDequeue(&goodQueue);
                    queuePrint(&goodQueue);
                }
                break;
            case 0:
                // 메모리 해제
                while (!stackIsEmpty(&badStack)) stackPop(&badStack);
                while (!queueIsEmpty(&goodQueue)) queueDequeue(&goodQueue);
                return;
            default:
                printf("잘못된 선택입니다.\n");
                break;
        }
    }
}
