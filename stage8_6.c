#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "stage8.h"

// 문제6 관련 전역변수
int staffCount = 4;

static seatNode *head = NULL;

static seatNode* createNode(int seatNumber, int fanID) {
    seatNode *node = (seatNode*)malloc(sizeof(seatNode));
    assert(node);
    node->seatNumber = seatNumber;
    node->fanID = fanID;
    node->next = NULL;
    return node;
}

static void insertNode(seatNode **head, seatNode *newNode) {
    if (*head == NULL || (*head)->seatNumber > newNode->seatNumber) {
        newNode->next = *head;
        *head = newNode;
        return;
    }
    seatNode *cur = *head;
    while (cur->next && cur->next->seatNumber < newNode->seatNumber)
        cur = cur->next;
    newNode->next = cur->next;
    cur->next = newNode;
}

static void drawFans(int *selectedFans, int count) {
    assert(count == 38);
    int selectedCount = 0;
    int attempts = 0;

    while (selectedCount < count) {
        int candidate = 5 + rand() % (MAX_FANS - 4);
        bool duplicate = false;
        for (int i = 0; i < selectedCount; i++) {
            if (selectedFans[i] == candidate) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            selectedFans[selectedCount++] = candidate;
        }
        if (++attempts > 10000) {
            fprintf(stderr, "팬 추첨 무한 루프 위험\n");
            exit(EXIT_FAILURE);
        }
    }
}

static void assignFanSeats(int *fanSeats) {
    int assignedCount = 0;
    int attempts = 0;

    while (assignedCount < 38) {
        int seatNum = 5 + rand() % (MAX_SEATS - 4);
        bool duplicate = false;
        for (int i = 0; i < assignedCount; i++) {
            if (fanSeats[i] == seatNum) {
                duplicate = true;
                break;
            }
        }
        if (!duplicate) {
            fanSeats[assignedCount++] = seatNum;
        }
        if (++attempts > 10000) {
            fprintf(stderr, "팬 좌석 배정 무한 루프 위험\n");
            exit(EXIT_FAILURE);
        }
    }
}

static void assignSeats(int *fanIDs, int *fanSeats) {
    while (head) {
        seatNode *tmp = head;
        head = head->next;
        free(tmp);
    }

    for (int i = 1; i <= staffCount; i++) {
        seatNode *node = createNode(i, i);
        insertNode(&head, node);
    }

    for (int i = 0; i < 38; i++) {
        seatNode *node = createNode(fanSeats[i], fanIDs[i]);
        insertNode(&head, node);
    }
}

static void printSeats(void) {
    seatNode *cur = head;
    int seatMap[MAX_SEATS + 1] = {0};

    while (cur) {
        assert(cur->seatNumber >= 1 && cur->seatNumber <= MAX_SEATS);
        seatMap[cur->seatNumber] = cur->fanID;
        cur = cur->next;
    }

    printf("\n[좌석 배치]\n");
    for (int r = 0; r < MAX_ROWS; r++) {
        for (int c = 0; c < MAX_COLS; c++) {
            int seatNum = r * MAX_COLS + c + 1;
            if (seatNum > MAX_SEATS) break;
            if (seatMap[seatNum] != 0)
                printf("%02d:%04d  ", seatNum, seatMap[seatNum]);
            else
                printf("%02d:----  ", seatNum);
        }
        printf("\n");
    }
}

void openDemoConcert(void) {
    printf("운영진 수를 입력하세요 (1~10): ");
    int count;
    if (scanf("%d", &count) != 1 || count < 1 || count > MAX_STAFF) {
        fprintf(stderr, "운영진 수 입력 오류\n");
        while(getchar() != '\n');
        return;
    }
    staffCount = count;

    srand((unsigned int)time(NULL));

    int fanIDs[38];
    int fanSeats[38];

    drawFans(fanIDs, 38);
    assignFanSeats(fanSeats);
    assignSeats(fanIDs, fanSeats);
    printSeats();
}

//////////////////////////
// 문제8-G 원형 큐 + 연결 리스트 구현

// 원형 큐 초기화
static void initQueue(CircularQueue *q) {
    q->front = 0;
    q->rear = 0;
    q->count = 0;
}

// 큐 포화 상태 확인
static bool isFull(CircularQueue *q) {
    return q->count == MSG_BUFFER_SIZE;
}

// 큐 공백 상태 확인
static bool isEmpty(CircularQueue *q) {
    return q->count == 0;
}

// 큐 삽입
static bool enqueue(CircularQueue *q, int val) {
    if (isFull(q))
        return false;
    q->buffer[q->rear] = val;
    q->rear = (q->rear + 1) % MSG_BUFFER_SIZE;
    q->count++;
    return true;
}

// 큐 삭제
static bool dequeue(CircularQueue *q, int *val) {
    if (isEmpty(q))
        return false;
    *val = q->buffer[q->front];
    q->front = (q->front + 1) % MSG_BUFFER_SIZE;
    q->count--;
    return true;
}

// 연결 리스트 생성/삽입 함수 (보너스)
static void appendMsgNode(MsgNode **head, int val) {
    MsgNode *newNode = (MsgNode*)malloc(sizeof(MsgNode));
    assert(newNode);
    newNode->val = val;
    newNode->next = NULL;
    if (*head == NULL) {
        *head = newNode;
    } else {
        MsgNode *cur = *head;
        while (cur->next) cur = cur->next;
        cur->next = newNode;
    }
}

// 연결 리스트 메모리 해제
static void freeMsgList(MsgNode *head) {
    while (head) {
        MsgNode *tmp = head;
        head = head->next;
        free(tmp);
    }
}

// 연결 리스트 출력
static void printMsgList(MsgNode *head) {
    printf("전달된 메시지 리스트: ");
    MsgNode *cur = head;
    while (cur) {
        printf("%d ", cur->val);
        cur = cur->next;
    }
    printf("\n");
}

// 보너스: 인접 노드 합이 24~42 사이인 경우의 수 계산
static int countAdjacentSumInRange(MsgNode *head) {
    int count = 0;
    MsgNode *cur = head;
    while (cur && cur->next) {
        int sum = cur->val + cur->next->val;
        if (sum >= 24 && sum <= 42) count++;
        cur = cur->next;
    }
    return count;
}

// 원형 큐 상태 출력
static void printQueueState(CircularQueue *q) {
    printf("원형큐 상태 (count=%d): [", q->count);
    int idx = q->front;
    for (int i = 0; i < q->count; i++) {
        printf("%d", q->buffer[idx]);
        if (i != q->count - 1) printf(", ");
        idx = (idx + 1) % MSG_BUFFER_SIZE;
    }
    printf("]\n");
}

// 문제8-G 수행 함수
void bufferMsgSpeed(void) {
    CircularQueue q;
    initQueue(&q);

    printf("처리할 메시지 개수를 입력하세요: ");
    int N;
    if (scanf("%d", &N) != 1 || N <= 0) {
        fprintf(stderr, "잘못된 메시지 개수 입력\n");
        while(getchar()!='\n');
        return;
    }

    srand((unsigned int)time(NULL));

    MsgNode *deliveredHead = NULL; // 전달 메시지 연결 리스트 헤드

    for (int i = 0; i < N; i++) {
        int randNum = rand() % 1000 + 1; // 1~1000 난수 생성

        // 5의 배수 -> 큐에 삽입 (좋은 메시지)
        if (randNum % 5 == 0) {
            bool success = enqueue(&q, randNum);
            if (success) {
                printf("[수신] 메시지 %d가 큐에 삽입되었습니다.\n", randNum);
                printQueueState(&q);
            } else {
                printf("[수신] 큐가 포화 상태입니다! 메시지 %d를 삽입할 수 없습니다.\n", randNum);
            }
        }

        // 10의 배수 -> 큐에서 꺼내어 전달 메시지 리스트에 추가
        if (randNum % 10 == 0) {
            int val;
            bool success = dequeue(&q, &val);
            if (success) {
                appendMsgNode(&deliveredHead, val);
                printf("[전달] 메시지 %d가 큐에서 꺼내져 전달 리스트에 추가되었습니다.\n", val);
                printQueueState(&q);
            } else {
                printf("[전달] 큐가 비어있어 메시지를 꺼낼 수 없습니다.\n");
            }
        }
    }

    // 보너스 출력
    if (deliveredHead != NULL) {
        printMsgList(deliveredHead);
        int cnt = countAdjacentSumInRange(deliveredHead);
        printf("연결 리스트에서 인접한 두 값의 합이 24 ~ 42 범위에 들어가는 경우의 수: %d\n", cnt);
    } else {
        printf("전달된
