#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PSYCHOLOGY 6
#define QUEUE_CAPACITY 3

typedef struct PsychologyNode {
    char topic[64];
    char description[256];
    int priority;
    struct PsychologyNode *next;
} PsychologyNode;

typedef struct {
    PsychologyNode *front;
    PsychologyNode *rear;
    int size;
    int capacity;
} PsychologyQueue;

void initQueue(PsychologyQueue *q, int capacity) {
    q->front = q->rear = NULL;
    q->size = 0;
    q->capacity = capacity;
}

PsychologyNode* createNode(const char *topic, const char *desc, int priority) {
    PsychologyNode *node = (PsychologyNode*)malloc(sizeof(PsychologyNode));
    if (!node) return NULL;
    strncpy(node->topic, topic, sizeof(node->topic)-1);
    node->topic[sizeof(node->topic)-1] = '\0';
    strncpy(node->description, desc, sizeof(node->description)-1);
    node->description[sizeof(node->description)-1] = '\0';
    node->priority = priority;
    node->next = NULL;
    return node;
}

int enqueue(PsychologyQueue *q, PsychologyNode *node) {
    if (q->size >= q->capacity) return 0;
    if (!q->rear) {
        q->front = q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
    q->size++;
    return 1;
}

PsychologyNode* dequeue(PsychologyQueue *q) {
    if (!q->front) return NULL;
    PsychologyNode *node = q->front;
    q->front = node->next;
    if (!q->front) q->rear = NULL;
    q->size--;
    node->next = NULL;
    return node;
}

void freeQueue(PsychologyQueue *q) {
    while (q->front) {
        PsychologyNode *tmp = dequeue(q);
        free(tmp);
    }
}

int loadPsychologyList(const char *filename, PsychologyNode *arr, int max) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("psychology.txt 열기 실패");
        return 0;
    }

    int count = 0;
    char line[256];
    while (count < max) {
        if (!fgets(line, sizeof(line), fp)) break;
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        strncpy(arr[count].topic, line, sizeof(arr[count].topic)-1);
        arr[count].topic[sizeof(arr[count].topic)-1] = '\0';

        if (!fgets(line, sizeof(line), fp)) break;
        line[strcspn(line, "\r\n")] = 0;
        strncpy(arr[count].description, line, sizeof(arr[count].description)-1);
        arr[count].description[sizeof(arr[count].description)-1] = '\0';

        if (!fgets(line, sizeof(line), fp)) break;
        arr[count].priority = atoi(line);

        arr[count].next = NULL;
        count++;
    }

    fclose(fp);
    return count;
}

void printPsychologyList(PsychologyNode *arr, int count) {
    printf("\n팬 심리 목록:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s (우선순위: %d)\n   %s\n", i+1, arr[i].topic, arr[i].priority, arr[i].description);
    }
}

int writeReport(const char *filename, PsychologyQueue *q) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("보고서 파일 열기 실패");
        return 0;
    }

    PsychologyNode *cur = q->front;
    while (cur) {
        fprintf(fp, "%s\n%s\nPriority: %d\n\n", cur->topic, cur->description, cur->priority);
        cur = cur->next;
    }

    fclose(fp);
    return 1;
}

void printReport(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("보고서 파일 열기 실패");
        return;
    }

    char line[256];
    printf("\n팬 심리 보고서 내용:\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    fclose(fp);
}

void analyzFanPsychology(void) {
    PsychologyNode psychologyList[MAX_PSYCHOLOGY];
    int count = loadPsychologyList("psychology.txt", psychologyList, MAX_PSYCHOLOGY);
    if (count == 0) {
        printf("팬 심리 목록 로드 실패\n");
        return;
    }

    PsychologyQueue queue;
    initQueue(&queue, QUEUE_CAPACITY);
    const char *reportFile = "fan_report.txt";

    int choice;
    while (1) {
        printf("\n[팬 심리 분석 메뉴]\n");
        printf("1. 팬 심리 목록 출력\n");
        printf("2. 팬 심리 선택하여 큐에 추가\n");
        printf("3. 팬 심리 보고서 파일 출력\n");
        printf("4. 종료\n");
        printf("선택: ");

        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); // 버퍼 비우기
            printf("잘못된 입력입니다.\n");
            continue;
        }

        if (choice == 4) {
            freeQueue(&queue);
            break;
        }

        switch (choice) {
            case 1:
                printPsychologyList(psychologyList, count);
                break;

            case 2: {
                printPsychologyList(psychologyList, count);
                printf("추가할 팬 심리 번호 선택: ");
                int selected;
                if (scanf("%d", &selected) != 1 || selected < 1 || selected > count) {
                    while(getchar() != '\n');
                    printf("잘못된 입력입니다.\n");
                    break;
                }

                PsychologyNode *node = createNode(
                    psychologyList[selected - 1].topic,
                    psychologyList[selected - 1].description,
                    psychologyList[selected - 1].priority);

                if (!node) {
                    printf("메모리 할당 실패\n");
                    break;
                }

                if (!enqueue(&queue, node)) {
                    printf("큐가 꽉 찼습니다. 보고서 파일에 저장합니다.\n");
                    if (writeReport(reportFile, &queue)) {
                        printf("보고서 파일 저장 성공\n");
                    }
                    freeQueue(&queue);
                    initQueue(&queue, QUEUE_CAPACITY);

                    enqueue(&queue, node);
                }
                break;
            }

            case 3:
                printReport(reportFile);
                break;

            default:
                printf("잘못된 선택입니다.\n");
        }
    }
}

int main(void) {
    analyzFanPsychology();
    return 0;
}
