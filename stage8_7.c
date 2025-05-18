#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // sleep 함수 (유닉스계열)

#define SET_COUNT 5
#define LINE_COUNT 2
#define MIN_MSG_LEN 24
#define MAX_MSG_LEN 42
#define DISPLAY_WIDTH 18

// 외부 변수로 선언
char adMessages[SET_COUNT][LINE_COUNT][MAX_MSG_LEN + 1]; // 널 종료 포함
int adMsgLengths[SET_COUNT][LINE_COUNT];

// 문자열 왼쪽으로 한 칸씩 이동해서 출력하는 함수
void printScrollingLine(const char* msg, int length, int startIdx) {
    for (int i = 0; i < DISPLAY_WIDTH; i++) {
        int idx = (startIdx + i) % length;
        putchar(msg[idx]);
    }
    putchar('\n');
}

// 2줄 메시지를 왼쪽으로 흘러가게 출력하는 함수
void displayAdSet(int setIdx) {
    int len1 = adMsgLengths[setIdx][0];
    int len2 = adMsgLengths[setIdx][1];

    int pos1 = 0; // 윗줄 시작 위치
    int pos2 = 0; // 아랫줄 시작 위치

    int totalTime = 24; // 총 24초간 출력
    int elapsed = 0;

    while (elapsed < totalTime) {
        // 클리어 화면 (윈도우: system("cls"); 유닉스: system("clear");)
        system("clear");

        // 윗줄 출력 (속도 1)
        printScrollingLine(adMessages[setIdx][0], len1, pos1);

        // 아랫줄 출력 (속도 2배)
        printScrollingLine(adMessages[setIdx][1], len2, pos2);

        // 속도 조절
        usleep(500000); // 0.5초 대기 (500,000 마이크로초)

        // 위치 업데이트
        pos1 = (pos1 + 1) % len1;

        // 아랫줄은 2배 빠르므로 1초에 2칸씩 이동
        pos2 = (pos2 + 2) % len2;

        elapsed += 0.5; // 0.5초씩 증가
    }
}

void showMyAdMsg() {
    printf("홍보 메시지를 5세트 입력하세요. 각 줄마다 24~42자여야 합니다.\n");

    for (int set = 0; set < SET_COUNT; set++) {
        for (int line = 0; line < LINE_COUNT; line++) {
            while (1) {
                printf("세트 %d, 줄 %d: ", set + 1, line + 1);
                if (fgets(adMessages[set][line], MAX_MSG_LEN + 1, stdin) == NULL) {
                    printf("입력 오류\n");
                    exit(1);
                }
                // fgets는 개행문자 포함 가능 -> 제거
                size_t len = strlen(adMessages[set][line]);
                if (len > 0 && adMessages[set][line][len - 1] == '\n') {
                    adMessages[set][line][len - 1] = '\0';
                    len--;
                }
                if (len >= MIN_MSG_LEN && len <= MAX_MSG_LEN) {
                    adMsgLengths[set][line] = (int)len;
                    break;
                }
                printf("문자 수가 조건에 맞지 않습니다 (24~42자). 다시 입력하세요.\n");
            }
        }
    }

    printf("광고판 출력 시작합니다...\n");
    for (int set = 0; set < SET_COUNT; set++) {
        displayAdSet(set);
    }
}
