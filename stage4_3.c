#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX 10
#define QUIZ_COUNT 5

struct MaskedSinger {
    int id;
    char mask[20];
    char song[50];
};

void prepareMaskedStage();

void trimWhitespace(char *str);
int compareStringsIgnoreWhitespace(const char *a, const char *b);
void showSplashScreen();

struct MaskedSinger database[MAX] = {
    {1, "곰발바닥", "이 밤의 끝을 잡고"},
    {2, "주윤발", "단발머리"},
    {3, "달마시안", "나의 옛날 이야기"},
    {4, "천둥 호랑이", "Love Affair"},
    {5, "개구리 왕자", "지친 하루"},
    {6, "거북이", "그녀를 사랑해줘요"},
    {7, "코끼리", "첫 인상"},
    {8, "뱀파이어", "지금은 알 수 없어"},
    {9, "햄스터", "내 얘길 들어봐"},
    {10, "돌고래", "못다 핀 꽃 한 송이"}
};

void showSplashScreen() {
    printf("\n=== 복면 무대 연습 게임 ===\n");
    printf("가면의 16진수 코드에 맞는 노래 제목을 맞혀보세요!\n");
    printf("총 5문제가 출제됩니다.\n");
    printf("- 정답 시: 10점\n- 30초 내 정답 시: 보너스 5점\n- 1분 초과 정답 시: 5점만 부여\n\n");
}

void prepareMaskedStage() {
    showSplashScreen();
    
    int used[MAX] = {0};
    int selected[QUIZ_COUNT];
    int score = 0;
    srand((unsigned int)time(NULL));

    for (int i = 0; i < QUIZ_COUNT; i++) {
        int r;
        do {
            r = rand() % MAX;
        } while (used[r]);
        used[r] = 1;
        selected[i] = r;
    }

    for (int i = 0; i < QUIZ_COUNT; i++) {
        struct MaskedSinger *s = &database[selected[i]];

        time_t start, end;
        char answer[100];
        printf("[%d] 복면 코드: 0x%X\n", i + 1, s->id);

        time(&start);
        printf("노래 제목 입력: ");
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = 0;
        time(&end);

        int elapsed = (int)difftime(end, start);
        trimWhitespace(answer);

        if (compareStringsIgnoreWhitespace(answer, s->song)) {
            if (elapsed <= 30) {
                printf("정답! (+15점, 보너스 포함)\n");
                score += 15;
            } else if (elapsed <= 60) {
                printf("정답! (+10점)\n");
                score += 10;
            } else {
                printf("정답! (+5점, 시간 초과)\n");
                score += 5;
            }
        } else {
            printf("오답입니다! 정답: %s\n", s->song);
        }
        printf("(걸린 시간: %d초)\n\n", elapsed);
    }
    printf("총점: %d점\n", score);
}

void trimWhitespace(char *str) {
    char *dst = str, *src = str;
    while (*src) {
        if (!isspace((unsigned char)*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

int compareStringsIgnoreWhitespace(const char *a, const char *b) {
    char a_clean[100], b_clean[100];
    strncpy(a_clean, a, sizeof(a_clean));
    strncpy(b_clean, b, sizeof(b_clean));
    trimWhitespace(a_clean);
    trimWhitespace(b_clean);
    return strcmp(a_clean, b_clean) == 0;
}

// Usage in stage4.c:
// Add menu entry: prepareMaskedStage();
