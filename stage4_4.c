#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DAYS_IN_WEEK 7
#define WEEKS_IN_MONTH 4
#define MAX_ENTRIES 31
#define MAX_CHECKLIST 10

const char *activities[MAX_CHECKLIST] = {
    "아침 복부 호흡 연습",
    "저녁 경구 호흡 연습",
    "가볍고 부드러운 발성 연습",
    "음의 높낮이와 음조 연습",
    "발음과 억양 연습",
    "연습 후 목소리에 부담되는 활동 피하기",
    "충분히 물 마시기",
    "적절한 습도 유지하기",
    "목소리 사용 후 적당한 휴식",
    "전문 보컬 튜터와 레슨 또는 상담"
};

const char *conditions[MAX_CHECKLIST] = {
    "5 ~ 10분",
    "5 ~ 10분",
    "10 ~ 15분",
    "10분",
    "10분",
    "긴 시간 대화/통화, 고음/저음, 소음 속 대화, 장시간 연습/스트레스",
    "8잔",
    "40% ~ 60%",
    "30분 이상",
    "1회"
};

const char *frequencies[MAX_CHECKLIST] = {
    "매일", "매일", "매일", "매일", "매일", "매일", "매일", "매일", "매일", "매주"
};

// 보컬 체크리스트 상태 기록 구조체
typedef struct {
    int checklist[MAX_ENTRIES][MAX_CHECKLIST];
    int entryCount;
} VocalHealthLog;

void printChecklistMenu() {
    printf("===== 보컬 건강 체크리스트 =====\n");
    for (int i = 0; i < MAX_CHECKLIST; i++) {
        printf("%d. %s | 조건: %s | 주기: %s\n", i + 1, activities[i], conditions[i], frequencies[i]);
    }
    printf("\n");
}

void inputChecklist(VocalHealthLog *log) {
    if (log->entryCount >= MAX_ENTRIES) {
        printf("더 이상 데이터를 입력할 수 없습니다.\n");
        return;
    }

    int *today = log->checklist[log->entryCount];

    printf("오늘의 체크리스트를 입력하세요 (1=완료, 0=미완료)\n");

    for (int i = 0; i < MAX_CHECKLIST; i++) {
        if (i == 5) { // 6번 항목 특별 처리
            printf("%d. %s\n조건 중 하나라도 해당되면 1, 아니면 0: ", i + 1, activities[i]);
        } else {
            printf("%d. %s: ", i + 1, activities[i]);
        }
        scanf("%d", &today[i]);
    }

    log->entryCount++;
    printf("오늘의 체크리스트가 저장되었습니다.\n\n");
}

void showWeeklyStats(VocalHealthLog *log) {
    if (log->entryCount < DAYS_IN_WEEK) {
        printf("주간 통계를 확인하려면 최소 7일의 데이터가 필요합니다.\n");
        return;
    }

    printf("==== 주간 통계 ====");
    int start = log->entryCount - DAYS_IN_WEEK;
    int checklistSums[MAX_CHECKLIST] = {0};
    int total = 0;

    for (int i = start; i < log->entryCount; i++) {
        for (int j = 0; j < MAX_CHECKLIST; j++) {
            checklistSums[j] += log->checklist[i][j];
            total += log->checklist[i][j];
        }
    }

    for (int j = 0; j < MAX_CHECKLIST; j++) {
        printf("\n%d. %s 완료율: %.2f%%", j + 1, activities[j], (checklistSums[j] / (float)DAYS_IN_WEEK) * 100);
    }
    printf("\n전체 평균 완료 수: %.2f개\n\n", total / (float)DAYS_IN_WEEK);
}

void showMonthlyStats(VocalHealthLog *log) {
    if (log->entryCount < WEEKS_IN_MONTH * DAYS_IN_WEEK) {
        printf("월간 통계를 확인하려면 최소 4주의 데이터가 필요합니다.\n");
        return;
    }

    printf("==== 월간 통계 ====");
    int start = log->entryCount - (WEEKS_IN_MONTH * DAYS_IN_WEEK);
    int checklistSums[MAX_CHECKLIST] = {0};
    int total = 0;

    for (int i = start; i < log->entryCount; i++) {
        for (int j = 0; j < MAX_CHECKLIST; j++) {
            checklistSums[j] += log->checklist[i][j];
            total += log->checklist[i][j];
        }
    }

    for (int j = 0; j < MAX_CHECKLIST; j++) {
        printf("\n%d. %s 완료율: %.2f%%", j + 1, activities[j], (checklistSums[j] / (float)(WEEKS_IN_MONTH * DAYS_IN_WEEK)) * 100);
    }
    printf("\n전체 평균 완료 수: %.2f개\n\n", total / (float)(WEEKS_IN_MONTH * DAYS_IN_WEEK));
}

void checkList4VocalHealth() {
    VocalHealthLog log = {0};

    int choice;
    while (1) {
        printf("\n==== 보컬 건강 관리 시스템 ====");
        printf("\n1. 체크리스트 입력\n2. 주간 통계\n3. 월간 통계\n0. 종료\n> ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                inputChecklist(&log);
                break;
            case 2:
                showWeeklyStats(&log);
                break;
            case 3:
                showMonthlyStats(&log);
                break;
            case 0:
                return;
            default:
                printf("잘못된 선택입니다.\n");
        }
    }
}
