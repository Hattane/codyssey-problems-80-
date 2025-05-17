#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SENTENCES 10
#define INITIAL_SENTENCES 5
#define MAX_MEMBERS 20
#define MAX_GROUP_NAME_LEN 30
#define MAX_MEMBER_NAME_LEN 30
#define MAX_NATIONALITY_LEN 20
#define MAX_SENTENCE_LEN 256
#define MAX_PRONUNCIATION_LEN 256

typedef struct {
    char koreanSentence[MAX_SENTENCE_LEN];
    char pronunciation[MAX_PRONUNCIATION_LEN];
    char inputter[MAX_MEMBER_NAME_LEN];  // 입력자 이름
} KoreanSentence;

typedef struct {
    int maxMatch;   // 최대 일치 단어 개수
    int minMatch;   // 최소 일치 단어 개수
    int totalScore; // 총점 (20점/문제 * 5문제)
} DictationResult;

typedef struct {
    char groupName[MAX_GROUP_NAME_LEN];
    char memberName[MAX_MEMBER_NAME_LEN];
    char nationality[MAX_NATIONALITY_LEN];
} Member;

typedef enum { SORT_ASC, SORT_DESC } SortOrder;
typedef enum { SORT_TOTAL_SCORE, SORT_MAX_MATCH, SORT_MIN_MATCH } SortColumn;

typedef struct {
    Member *player;
    DictationResult result;
} PlayerResult;

KoreanSentence koreanSentences[MAX_SENTENCES] = {
    {"안녕하세요", "annyeonghaseyo", ""},
    {"저는 학생입니다", "jeoneun haksaengimnida", ""},
    {"오늘 날씨가 좋네요", "oneul nalssiga jonneyo", ""},
    {"한국어 공부는 재미있어요", "hangugeo gongbuneun jaemiisseoyo", ""},
    {"함께 열심히 해요", "hamkke yeolsimhi haeyo", ""}
};

int currentSentenceCount = INITIAL_SENTENCES;

Member members[MAX_MEMBERS] = {
    {"Millieways", "Ariel", "Korean"},
    {"Millieways", "Arthur", "American"},
    {"Millieways", "Sally", "Chinese"},
    {"Millieways", "John", "British"},
    {"Millieways", "Mina", "Korean"}
};
int memberCount = 5;

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int findMemberIndex(const char *name) {
    int i;
    for (i = 0; i < memberCount; i++) {
        if (strcmp(members[i].memberName, name) == 0)
            return i;
    }
    return -1;
}

void inputKoreanSentences(void) {
    if (currentSentenceCount >= MAX_SENTENCES) {
        printf("이미 10개의 문장이 등록되어 있습니다. 추가 입력을 건너뜁니다.\n");
        return;
    }

    char inputterName[MAX_MEMBER_NAME_LEN];
    printf("입력자 이름을 입력하세요 (한국인 멤버만 가능): ");
    scanf("%s", inputterName);
    clearInputBuffer();

    int idx = findMemberIndex(inputterName);
    if (idx == -1) {
        printf("입력자 이름이 멤버 목록에 없습니다.\n");
        return;
    }
    if (strcmp(members[idx].nationality, "Korean") != 0) {
        printf("입력자는 한국인이 아닙니다. 입력 불가.\n");
        return;
    }

    printf("한국어 문장과 발음 영문자 표기를 입력하세요.\n");

    for (; currentSentenceCount < MAX_SENTENCES; currentSentenceCount++) {
        printf("문장 #%d 한국어 문장: ", currentSentenceCount + 1);
        fgets(koreanSentences[currentSentenceCount].koreanSentence, MAX_SENTENCE_LEN, stdin);
        koreanSentences[currentSentenceCount].koreanSentence[strcspn(koreanSentences[currentSentenceCount].koreanSentence, "\n")] = 0;

        printf("발음 영문자 표기: ");
        fgets(koreanSentences[currentSentenceCount].pronunciation, MAX_PRONUNCIATION_LEN, stdin);
        koreanSentences[currentSentenceCount].pronunciation[strcspn(koreanSentences[currentSentenceCount].pronunciation, "\n")] = 0;

        strncpy(koreanSentences[currentSentenceCount].inputter, inputterName, MAX_MEMBER_NAME_LEN);
    }
}

void shuffleIndices(int *arr, int size) {
    int i;
    for (i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

int countMatchingWords(const char *answer, const char *correct) {
    char ansCopy[MAX_SENTENCE_LEN];
    char corCopy[MAX_SENTENCE_LEN];
    char *ansToken;

    strncpy(ansCopy, answer, MAX_SENTENCE_LEN);
    strncpy(corCopy, correct, MAX_SENTENCE_LEN);

    int matchCount = 0;

    ansToken = strtok(ansCopy, " ");
    while (ansToken != NULL) {
        if (strstr(corCopy, ansToken) != NULL) {
            matchCount++;
        }
        ansToken = strtok(NULL, " ");
    }
    return matchCount;
}

void evalAnswer(char userAnswers[][MAX_SENTENCE_LEN], int indices[], DictationResult *result) {
    int i;
    int maxMatch = -1;
    int minMatch = 1000;
    int totalScore = 0;

    for (i = 0; i < INITIAL_SENTENCES; i++) {
        int idx = indices[i];
        int matchWords = countMatchingWords(userAnswers[i], koreanSentences[idx].koreanSentence);

        if (matchWords > maxMatch) maxMatch = matchWords;
        if (matchWords < minMatch) minMatch = matchWords;

        int score = matchWords * 4;
        if (score > 20) score = 20;

        totalScore += score;
    }

    result->maxMatch = maxMatch;
    result->minMatch = minMatch;
    result->totalScore = totalScore;
}

void playGame(Member *player, PlayerResult *result) {
    int indices[INITIAL_SENTENCES] = {0, 1, 2, 3, 4};
    shuffleIndices(indices, INITIAL_SENTENCES);

    char userAnswers[INITIAL_SENTENCES][MAX_SENTENCE_LEN];
    int i;

    printf("발음 영문자 표기를 보고 한글 문장을 입력하세요. (총 %d문제)\n", INITIAL_SENTENCES);

    for (i = 0; i < INITIAL_SENTENCES; i++) {
        int idx = indices[i];
        printf("문제 #%d: %s\n", i + 1, koreanSentences[idx].pronunciation);
        printf("입력: ");
        fgets(userAnswers[i], MAX_SENTENCE_LEN, stdin);
        userAnswers[i][strcspn(userAnswers[i], "\n")] = 0;
    }

    DictationResult res;
    evalAnswer(userAnswers, indices, &res);

    printf("\n[%s] 받아쓰기 결과:\n", player->memberName);
    printf("총점 = %d / 100\n", res.totalScore);
    printf("최대 일치 단어 개수 = %d\n", res.maxMatch);
    printf("최소 일치 단어 개수 = %d\n\n", res.minMatch);

    result->player = player;
    result->result = res;
}

int comparePlayerResults(const void *a, const void *b, void *arg) {
    SortColumn col = *((SortColumn *)arg);
    SortOrder order = *(((SortOrder *)arg) + 1);

    PlayerResult *pa = (PlayerResult *)a;
    PlayerResult *pb = (PlayerResult *)b;
    int diff = 0;

    switch (col) {
        case SORT_TOTAL_SCORE:
            diff = pa->result.totalScore - pb->result.totalScore;
            break;
        case SORT_MAX_MATCH:
            diff = pa->result.maxMatch - pb->result.maxMatch;
            break;
        case SORT_MIN_MATCH:
            diff = pa->result.minMatch - pb->result.minMatch;
            break;
    }

    if (order == SORT_DESC)
        diff = -diff;

    return diff;
}

void sortResults(PlayerResult results[], int count, SortOrder order, SortColumn col) {
  
}

SortOrder g_sortOrder;
SortColumn g_sortColumn;

int globalCompare(const void *a, const void *b) {
    PlayerResult *pa = (PlayerResult *)a;
    PlayerResult *pb = (PlayerResult *)b;
    int diff = 0;

    switch (g_sortColumn) {
        case SORT_TOTAL_SCORE:
            diff = pa->result.totalScore - pb->result.totalScore;
            break;
        case SORT_MAX_MATCH:
            diff = pa->result.maxMatch - pb->result.maxMatch;
            break;
        case SORT_MIN_MATCH:
            diff = pa->result.minMatch - pb->result.minMatch;
            break;
    }

    if (g_sortOrder == SORT_DESC)
        diff = -diff;

    return diff;
}

void printAllResults(PlayerResult results[], int count) {
    int i;
    printf("\n=== 받아쓰기 테스트 결과 ===\n");
