#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "stage8.h"

int staffCount = 4;  // 기본 운영진 수

static seatNode *head = NULL;

// 노드 생성
static seatNode* createNode(int seatNumber, int fanID) {
    seatNode *node = (seatNode *)malloc(sizeof(seatNode));
    assert(node != NULL);
    node->seatNumber = seatNumber;
    node->fanID = fanID;
    node->next = NULL;
    return node;
}

// 좌석번호 순서대로 삽입
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

// 팬 38명 랜덤 추첨 (운영진 제외)
static void drawFans(int *selectedFans, int count) {
    assert(count == 38);
    int selectedCount = 0;
    int attempts = 0;

    while (selectedCount < count) {
        int candidate = 5 + rand() % (MAX_FANS - 4); // 5~1000
        bool duplicate = false;
        for (int i=0; i<selectedCount; i++) {
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

// 팬 좌석 번호 랜덤 배정 (5~42 중 중복 없이 38개)
static void assignFanSeats(int *fanSeats) {
    int assignedCount = 0;
    int attempts = 0;

    while (assignedCount < 38) {
        int seatNum = 5 + rand() % (MAX_SEATS - 4); // 5~42
        bool duplicate = false;
        for (int i=0; i<assignedCount; i++) {
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

// 좌석 연결 리스트 생성
static void assignSeats(int *fanIDs, int *fanSeats) {
    // 기존 리스트 해제
    while (head) {
        seatNode *tmp = head;
        head = head->next;
        free(tmp);
    }

    // 운영진 좌석 1~staffCount
    for (int i = 1; i <= staffCount; i++) {
        seatNode *node = createNode(i, i); // 운영진 ID == 좌석번호
        insertNode(&head, node);
    }

    // 팬 좌석 38명
    for (int i = 0; i < 38; i++) {
        seatNode *node = createNode(fanSeats[i], fanIDs[i]);
        insertNode(&head, node);
    }
}

// 좌석-팬ID 출력 (6행 7열)
static void printSeats(void) {
    seatNode *cur = head;
    int seatMap[MAX_SEATS + 1] = {0}; // 1~42 좌석 팬ID 저장

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
        while(getchar() != '\n'); // 입력 버퍼 비우기
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
