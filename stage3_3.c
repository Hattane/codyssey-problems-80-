#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEMBER_COUNT 4
#define DIALECTS_PER_REGION 5
#define REGION_COUNT 4
#define MAX_NAME_LEN 16
#define MAX_WORD_LEN 32
#define MAX_GROUP_NAME_LEN 32

// --- 데이터 구조체 ---

// 사투리-표준어 구조체
typedef struct {
    char dialectWord[MAX_WORD_LEN];
    char region[MAX_NAME_LEN];
    char stdWord[MAX_WORD_LEN];
} Dialect;

// 성취도 A,B,C,F 열거형
typedef enum {
    GRADE_A,
    GRADE_B,
    GRADE_C,
    GRADE_F
} Grade;

// 성취도 구조체
typedef struct {
    char groupName[MAX_GROUP_NAME_LEN];
    char learnerName[MAX_NAME_LEN];
    int scoreStd;      // 표준어 점수
    int scoreDialect;  // 사투리 점수
    Grade gradeStd;
    Grade gradeDialect;
} Achievement;

// 아이돌 멤버 구조체
typedef struct {
    char groupName[MAX_GROUP_NAME_LEN];
    char memberName[MAX_NAME_LEN];
} IdolMember;

// --- 밀리웨이즈 멤버 배열 ---
IdolMember millieways[MEMBER_COUNT] = {
    {"Millieways", "Alice"},
    {"Millieways", "Bob"},
    {"Millieways", "Cindy"},
    {"Millieways", "Dave"}
};

// --- 사투리 데이터 초기화 ---
// 4개 지역 * 5개 단어 = 20개
Dialect dialects[REGION_COUNT * DIALECTS_PER_REGION] = {
    // 서울
    {"그랭", "서울", "그래"},
    {"댓바람", "서울", "즉시"},
    {"고마", "서울", "고마워"},
    {"잉", "서울", "응"},
    {"우째", "서울", "어떻게"},
    // 부산
    {"거시기", "부산", "저것"},
    {"머라카노", "부산", "뭐라고 하냐"},
    {"쪼매", "부산", "조금"},
    {"와따", "부산", "왔다"},
    {"믄디", "부산", "문제"},
    // 대구
    {"가래", "대구", "가래침"},
    {"시방", "대구", "지금"},
    {"보그라", "대구", "봐라"},
    {"어데", "대구", "어디"},
    {"알켔나", "대구", "알겠나"},
    // 광주
    {"얘", "광주", "이야"},
    {"어이", "광주", "저기"},
    {"하이소", "광주", "안녕하세요"},
    {"거시기하네", "광주", "그렇네"},
    {"심보", "광주", "성격"}
};

// --- 함수 선언 ---
void initializeAchievementLevel(Achievement *achievements, IdolMember *members, int count, const char *groupName);
void learnDialect(void);
void guessStdWord(Achievement *ach);
void guessDialectWord(Achievement *ach);
int randomRegionIndexExcept(int exceptIndexes[], int exceptCount);
Grade calcGrade(int score);
void printGradeMessage(const char *type, Grade grade);

// --- 전역변수 ---
Achievement achievements[MEMBER_COUNT];

// --- 공통 무작위 지역 선택 함수 ---
// exceptIndexes[]: 제외할 인덱스 배열
// exceptCount: 제외할 개수
// 반환: 선택된 지역 인덱스 (0~REGION_COUNT-1)
int randomRegionIndexExcept(int exceptIndexes[], int exceptCount) {
    int available[REGION_COUNT];
    int count = 0;
    int i, j, excluded;

    for(i = 0; i < REGION_COUNT; i++) {
        excluded = 0;
        for(j = 0; j < exceptCount; j++) {
            if(i == exceptIndexes[j]) {
                excluded = 1;
                break;
            }
        }
        if(!excluded) {
            available[count++] = i;
        }
    }
    if(count == 0) return -1; // 선택 불가
    int r = rand() % count;
    return available[r];
}

// --- 성취도 초기화 함수 ---
void initializeAchievementLevel(Achievement *achievements, IdolMember *members, int count, const char *groupName) {
    int i;
    for(i=0; i<count; i++) {
        strncpy(achievements[i].groupName, groupName, MAX_GROUP_NAME_LEN);
        strncpy(achievements[i].learnerName, members[i].memberName, MAX_NAME_LEN);
        achievements[i].scoreStd = 0;
        achievements[i].scoreDialect = 0;
        achievements[i].gradeStd = GRADE_F;
        achievements[i].gradeDialect = GRADE_F;
    }
}

// --- 표준어 성취도 계산 함수 ---
Grade calcGrade(int score) {
    if(score >= 90) return GRADE_A;
    if(score >= 80) return GRADE_B;
    if(score >= 70) return GRADE_C;
    return GRADE_F;
}

// --- 성취도별 메시지 출력 ---
void printGradeMessage(const char *type, Grade grade) {
    switch(grade) {
        case GRADE_A:
            printf("당신은 %s 전문가 입니다.\n", type);
            break;
        case GRADE_B:
            printf("당신의 %s 실력은 훌륭합니다.\n", type);
            break;
        case GRADE_C:
            printf("%s에 익숙하네요.\n", type);
            break;
        case GRADE_F:
            printf("조금 더 공부해서 다시 도전 하세요.\n");
            break;
    }
}

// --- 1. 표준어 맞추기 ---
// 4개 지역 중 무작위 2개 지역 선택 후 해당 지역 사투리 10개 출제
void guessStdWord(Achievement *ach) {
    printf("\n[표준어 맞추기 시작]\n");
    int selectedRegions[2];
    selectedRegions[0] = rand() % REGION_COUNT;
    selectedRegions[1] = randomRegionIndexExcept(selectedRegions, 1);

    Dialect quiz[10];
    int idx = 0, i;

    // 두 지역 사투리 5개씩 뽑기
    for(i=0; i < REGION_COUNT * DIALECTS_PER_REGION && idx < 10; i++) {
        if((strcmp(dialects[i].region, dialects[selectedRegions[0]*DIALECTS_PER_REGION].region) == 0 && idx < 5)
            || (strcmp(dialects[i].region, dialects[selectedRegions[1]*DIALECTS_PER_REGION].region) == 0 && idx >= 5)) {
            quiz[idx++] = dialects[i];
        }
    }

    int correctCount = 0;
    char input[MAX_WORD_LEN];
    for(i=0; i<10; i++) {
        printf("%d번 문제 - 사투리 단어: %s\n표준어를 입력하세요: ", i+1, quiz[i].dialectWord);
        scanf("%s", input);
        if(strcmp(input, quiz[i].stdWord) == 0) {
            printf("정답!\n");
            correctCount++;
        } else {
            printf("오답! 정답은: %s\n", quiz[i].stdWord);
        }
    }

    ach->scoreStd = (correctCount * 100) / 10;
    ach->gradeStd = calcGrade(ach->scoreStd);
}

// --- 2. 지역 별 사투리 맞추기 ---
// 표준어 제시 -> 무작위 지역 표시 -> 사투리 입력
void guessDialectWord(Achievement *ach) {
    printf("\n[지역 별 사투리 맞추기 시작]\n");
    int i;
    int correctCount = 0;
    char input[MAX_WORD_LEN];
    for(i=0; i < 10; i++) {
        // 무작위 사투리 선택
        int randIdx = rand() % (REGION_COUNT * DIALECTS_PER_REGION);
        Dialect *d = &dialects[randIdx];

        // 무작위 지역 표시 (보너스 공통 함수)
        int regionIdx = -1;
        for(int r=0; r<REGION_COUNT; r++) {
            if(strcmp(d->region, dialects[r*DIALECTS_PER_REGION].region) == 0) {
                regionIdx = r;
                break;
            }
        }
        if(regionIdx == -1) regionIdx = 0;

        printf("%d번 문제 - 표준어 단어: %s\n", i+1, d->stdWord);
        printf("사투리 지역: %s\n사투리를 입력하세요: ", dialects[regionIdx*DIALECTS_PER_REGION].region);
        scanf("%s", input);
        if(strcmp(input, d->dialectWord) == 0) {
            printf("정답!\n");
            correctCount++;
        } else {
            printf("오답! 정답은: %s\n", d->dialectWord
