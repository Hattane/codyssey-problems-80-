#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MEMBERS 20
#define MAX_TEAM_NAME_LEN 30

typedef struct {
    char memberName[30];
} Member;

typedef struct {
    int avgScoreLimit;
    int minScoreLimit;
    int minTimePerMember;
    int maxTimePerMember;
} TeamBaseline;

typedef struct {
    char teamName[MAX_TEAM_NAME_LEN];
    float avgScore;
    int minScore;
    int totalTime;
    int passed;
} TeamPerformance;

// 멤버 목록 예시
Member members[MAX_MEMBERS] = {
    {"Ariel"}, {"Arthur"}, {"Sally"}, {"John"}, {"Mina"}
};
int memberCount = 5;

// 팀 기준값 입력
void inputBaseLine(TeamBaseline *baseline, int maxAvgScore, int maxMinScore, int minTimePerMember, int maxTimePerMember) {
    int avgScore, minScore, timePerMember;
    do {
        printf("팀 평균 점수 상한 (최대 %d): ", maxAvgScore);
        scanf("%d", &avgScore);
    } while (avgScore < 0 || avgScore > maxAvgScore);
    do {
        printf("팀원 최소 점수 상한 (최대 %d): ", maxMinScore);
        scanf("%d", &minScore);
    } while (minScore < 0 || minScore > maxMinScore);
    do {
        printf("팀원 1인당 완료 시간 (분) [%d~%d]: ", minTimePerMember, maxTimePerMember);
        scanf("%d", &timePerMember);
    } while (timePerMember < minTimePerMember || timePerMember > maxTimePerMember);

    baseline->avgScoreLimit = avgScore;
    baseline->minScoreLimit = minScore;
    baseline->minTimePerMember = minTimePerMember;
    baseline->maxTimePerMember = maxTimePerMember;

    printf("기준값 입력 완료.\n");
}

// 팀 성적 평가
void evalTeamPerformance(TeamPerformance *performance, TeamBaseline *baseline, int *memberScores, int memberCount, int *memberTimes) {
    int i;
    int sumScores = 0;
    int minScore = 101; // 충분히 큰 값
    int totalTime = 0;
    for (i = 0; i < memberCount; i++) {
        if (memberScores[i] < minScore) minScore = memberScores[i];
        sumScores += memberScores[i];
        totalTime += memberTimes[i];
    }
    performance->avgScore = (float)sumScores / memberCount;
    performance->minScore = minScore;
    performance->totalTime = totalTime;

    if (performance->avgScore >= baseline->avgScoreLimit &&
        performance->minScore >= baseline->minScoreLimit &&
        totalTime <= baseline->maxTimePerMember * memberCount &&
        totalTime >= baseline->minTimePerMember * memberCount) {
        performance->passed = 1;
    } else {
        performance->passed = 0;
    }
}

// 한글 자모 학습 예시 함수 (문제2)
void learnHangul(void) {
    char teamName[MAX_TEAM_NAME_LEN];
    printf("팀 이름을 입력하세요: ");
    scanf("%s", teamName);

    TeamBaseline baseline;
    inputBaseLine(&baseline, 300, 300, 10, 30); // 평균 점수 최대 300, 최소 점수 최대 300, 시간 10~30분

    int memberScores[MAX_MEMBERS] = {280, 290, 270, 300, 275}; // 예시 점수
    int memberTimes[MAX_MEMBERS] = {25, 28, 20, 30, 22}; // 예시 소요 시간

    TeamPerformance performance;
    strcpy(performance.teamName, teamName);

    evalTeamPerformance(&performance, &baseline, memberScores, memberCount, memberTimes);

    printf("팀 %s 성적:\n", performance.teamName);
    printf("평균 점수: %.2f\n", performance.avgScore);
    printf("최소 점수: %d\n", performance.minScore);
    printf("총 완료 시간: %d 분\n", performance.totalTime);
    printf("최종 결과: %s\n", performance.passed ? "Pass" : "Fail");
}

// 한국어 문법 학습 예시 함수 (문제3)
void learnKoreanGrammar(void) {
    char teamName[MAX_TEAM_NAME_LEN];
    printf("팀 이름을 입력하세요: ");
    scanf("%s", teamName);

    TeamBaseline baseline;
    inputBaseLine(&baseline, 100, 100, 10, 20); // 평균 점수 최대 100, 최소 점수 최대 100, 시간 10~20분

    int memberScores[MAX_MEMBERS] = {90, 85, 80, 95, 88};
    int memberTimes[MAX_MEMBERS] = {15, 16, 14, 18, 17};

    TeamPerformance performance;
    strcpy(performance.teamName, teamName);

    evalTeamPerformance(&performance, &baseline, memberScores, memberCount, memberTimes);

    printf("팀 %s 성적:\n", performance.teamName);
    printf("평균 점수: %.2f\n", performance.avgScore);
    printf("최소 점수: %d\n", performance.minScore);
    printf("총 완료 시간: %d 분\n", performance.totalTime);
    printf("최종 결과: %s\n", performance.passed ? "Pass" : "Fail");
}

int main(void) {
    int menu;
    while (1) {
        printf("1. 한글 자모 학습\n");
        printf("2. 한국어 문법 학습\n");
        printf("0. 종료\n");
        printf("선택: ");
        scanf("%d", &menu);
        switch (menu) {
            case 1: learnHangul(); break;
            case 2: learnKoreanGrammar(); break;
            case 0: return 0;
            default: printf("잘못된 선택입니다.\n");
        }
    }
}
