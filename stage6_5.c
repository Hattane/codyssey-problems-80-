#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TYPES 10
#define MAX_DESC 100
#define MAX_QUEUE 20

typedef struct {
    int id;
    int arrival_time;
    char gender;
    char makeup_type[50];
    int duration;
    int wait_time;
} Trainee;

typedef struct {
    char gender;
    char type[50];
    char description[MAX_DESC];
} MakeupType;

typedef struct {
    Trainee queue[MAX_QUEUE];
    int front;
    int rear;
    int size;
} CircularQueue;

MakeupType makeupList[MAX_TYPES];
int maleCount = 0, femaleCount = 0;

// 원형 큐 함수
void initQueue(CircularQueue *q) {
    q->front = 0;
    q->rear = 0;
    q->size = 0;
}

int isFull(CircularQueue *q) {
    return q->size == MAX_QUEUE;
}

int isEmpty(CircularQueue *q) {
    return q->size == 0;
}

void enqueue(CircularQueue *q, Trainee t) {
    if (isFull(q)) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }
    q->queue[q->rear] = t;
    q->rear = (q->rear + 1) % MAX_QUEUE;
    q->size++;
}

Trainee dequeue(CircularQueue *q) {
    Trainee t = {0};
    if (isEmpty(q)) {
        printf("Queue is empty. Cannot dequeue.\n");
        return t;
    }
    t = q->queue[q->front];
    q->front = (q->front + 1) % MAX_QUEUE;
    q->size--;
    return t;
}

// 파일에서 메이크업 목록 읽기
void loadMakeupTypes() {
    FILE *fp = fopen("makeup.txt", "r");
    if (!fp) {
        perror("makeup.txt 파일을 열 수 없습니다.");
        exit(1);
    }

    int count = 0;
    while (fscanf(fp, " %c %[^:]: %[^\n]", &makeupList[count].gender, makeupList[count].type, makeupList[count].description) == 3) {
        if (makeupList[count].gender == 'M') maleCount++;
        else if (makeupList[count].gender == 'F') femaleCount++;
        count++;
    }

    fclose(fp);
}

void testMakeUp() {
    loadMakeupTypes();
    CircularQueue makeupRoom;
    initQueue(&makeupRoom);

    int currentTime = 0;
    int nextAvailableTime = 0;
    int traineeID = 1;
    int totalWait = 0;
    int maleWait = 0, femaleWait = 0;

    srand((unsigned int)time(NULL));

    printf("\n[메이크업 테스트 시작]\n");

    while (currentTime < 60) {
        int arrivalChance = rand() % 11;
        if (arrivalChance < 3) {
            // 새로운 연습생 생성
            Trainee t;
            t.id = traineeID++;
            t.arrival_time = currentTime;
            t.gender = (t.id % 2 == 1) ? 'M' : 'F';

            // 해당 성별의 메이크업 중 무작위 선택
            int count = (t.gender == 'M') ? maleCount : femaleCount;
            int offset = (t.gender == 'M') ? 0 : maleCount;
            int choice = rand() % count;

            strcpy(t.makeup_type, makeupList[offset + choice].type);
            t.duration = 3 + rand() % 4;
            t.wait_time = (currentTime < nextAvailableTime) ? (nextAvailableTime - currentTime) : 0;

            enqueue(&makeupRoom, t);

            if (t.gender == 'M') maleWait += t.wait_time;
            else femaleWait += t.wait_time;
            totalWait += t.wait_time;
        }

        // 현재 시간에 테스트 가능
        if (!isEmpty(&makeupRoom) && currentTime >= nextAvailableTime) {
            Trainee t = dequeue(&makeupRoom);
            printf("시간 %2d분: 연습생 %d (%c) - %s [%d분 소요, 대기 %d분]\n",
                   currentTime, t.id, t.gender, t.makeup_type, t.duration, t.wait_time);
            nextAvailableTime = currentTime + t.duration;
        }

        currentTime++;
    }

    printf("\n--- 테스트 종료 ---\n");
    printf("총 대기 시간: %d분\n", totalWait);
    printf("남자 대기 시간: %d분\n", maleWait);
    printf("여자 대기 시간: %d분\n", femaleWait);
}

