#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_WORDS 100
#define GAME_WORDS 42
#define MAX_WORD_LEN 5
#define MAX_NICK_LEN 20
#define MAX_NAME_LEN 30
#define GAME_TIME 60

/* 밀리웨이즈 멤버 닉네임 */
const char *members[] = {"arthur", "susan", "mark", "elly"};
const int memberCount = 4;

/* 100개 단어 하드코딩 예시 (임의 한글 2~4자 단어) */
const char wordList[TOTAL_WORDS][MAX_WORD_LEN] = {
    "가방", "나라", "다리", "마음", "사과", "아이", "자전", "차량", "타이어", "파도",
    "하늘", "강아지", "거울", "고기", "기차", "나무", "다이아", "라디오", "마술", "바다",
    "사랑", "아이언", "자동", "차가", "타자", "파란", "학교", "학생", "하루", "강물",
    "걷기", "고양이", "기본", "나비", "단어", "라면", "마을", "바람", "사자", "아기",
    "자석", "차별", "타임", "파티", "하마", "강풍", "건물", "고장", "기억", "나라",
    "단추", "라이트", "마스크", "바지", "사전", "아래", "자동차", "차이", "타월", "파이",
    "하얀", "강좌", "걱정", "고함", "기둥", "나중", "단점", "라디오", "마차", "바닥",
    "사탕", "아이템", "자동문", "차도", "타자기", "파란색", "하얘", "강원", "건강", "고소",
    "기호", "나이", "단결", "라벨", "마법", "바늘", "사전책", "아파트", "자동기", "차선",
    "타월링", "파출", "하숙", "강의", "걷다", "고르다", "기름", "나누다", "단단", "라임"
};

/* 스코어 구조체 */
typedef struct {
    char challenger[MAX_NICK_LEN];
    int challengerScore;
    int challengerTime;

    char opponent[MAX_NICK_LEN];
    int opponentScore;
    int opponentTime;
} Score;

/* 게임 단어 셔플 */
void shuffleWords(const char words[][MAX_WORD_LEN], char shuffled[][MAX_WORD_LEN], int count) {
    int indices[TOTAL_WORDS];
    int i;
    for(i=0; i<TOTAL_WORDS; i++) indices[i] = i;

    srand((unsigned int)time(NULL));
    // Fisher-Yates shuffle
    for(i=TOTAL_WORDS-1; i>0; i--) {
        int j = rand() % (i+1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
    // 첫 count개 복사
    for(i=0; i<count; i++) {
        strcpy(shuffled[i], wordList[indices[i]]);
    }
}

/* 닉네임 검사 */
int isValidMember(const char *nickname) {
    int i;
    for(i=0; i<memberCount; i++) {
        if(strcmp(members[i], nickname) == 0) return 1;
    }
    return 0;
}

/* 끝말잇기 연결 검사 */
int isValidNextWord(const char *prev, const char *next) {
    int prevLen = (int)strlen(prev);
    int nextLen = (int)strlen(next);
    if(prevLen == 0) return 1; // 첫 단어

    // 끝말잇기: prev 마지막 문자 == next 첫 문자
    if(prev[prevLen - 1] == next[0]) return 1;
    return 0;
}

/* 단어 배열에서 단어가 있는지 검사 */
int containsWord(const char wordArray[][MAX_WORD_LEN], int size, const char *word) {
    int i;
    for(i=0; i<size; i++) {
        if(strcmp(wordArray[i], word) == 0) return 1;
    }
    return 0;
}

/* 게임 진행 */
void playGame(const char *challenger, const char *opponent, Score *score) {
    char gameWords[GAME_WORDS][MAX_WORD_LEN];
    shuffleWords(wordList, gameWords, GAME_WORDS);

    printf("\n%s님과 %s님의 끝말잇기 게임을 시작합니다!\n", challenger, opponent);
    printf("엔터를 누르면 60초 타이머가 시작됩니다...\n");
    getchar();

    time_t start, now;
    start = time(NULL);

    int challengerCount = 0;
    int opponentCount = 0;

    char input[MAX_WORD_LEN];
    char prevWord[MAX_WORD_LEN] = "";

    // 도전자 차례
    printf("\n[%s] 도전자의 차례\n", challenger);
    while(1) {
        now = time(NULL);
        if((int)difftime(now, start) >= GAME_TIME) {
            printf("시간 종료!\n");
            break;
        }
        printf("단어 입력 (종료하려면 엔터만 누르세요): ");
        fgets(input, MAX_WORD_LEN, stdin);

        // 개행 제거
        int len = (int)strlen(input);
        if(len > 0 && input[len-1] == '\n') input[len-1] = '\0';

        if(strlen(input) == 0) break; // 종료

        if(!containsWord(gameWords, GAME_WORDS, input)) {
            printf("제시된 단어에 없습니다. 다시 시도하세요.\n");
            continue;
        }
        if(!isValidNextWord(prevWord, input)) {
            printf("끝말잇기 규칙에 맞지 않습니다. 다시 시도하세요.\n");
            continue;
        }

        strcpy(prevWord, input);
        challengerCount++;
    }
    score->challengerScore = challengerCount;
    score->challengerTime = (int)difftime(time(NULL), start);

    // 대상자 차례
    printf("\n[%s] 대상자의 차례입니다. 엔터를 누르면 시작됩니다...\n", opponent);
    getchar();

    start = time(NULL);
    opponentCount = 0;
    prevWord[0] = '\0';

    while(1) {
        now = time(NULL);
        if((int)difftime(now, start) >= GAME_TIME) {
            printf("시간 종료!\n");
            break;
        }
        printf("단어 입력 (종료하려면 엔터만 누르세요): ");
        fgets(input, MAX_WORD_LEN, stdin);
        len = (int)strlen(input);
        if(len > 0 && input[len-1] == '\n') input[len-1] = '\0';

        if(strlen(input) == 0) break;

        if(!containsWord(gameWords, GAME_WORDS, input)) {
            printf("제시된 단어에 없습니다. 다시 시도하세요.\n");
            continue;
        }
        if(!isValidNextWord(prevWord, input)) {
            printf("끝말잇기 규칙에 맞지 않습니다. 다시 시도하세요.\n");
            continue;
        }

        strcpy(prevWord, input);
        opponentCount++;
    }
    score->opponentScore = opponentCount;
    score->opponentTime = (int)difftime(time(NULL), start);

    strcpy(score->challenger, challenger);
    strcpy(score->opponent, opponent);
}

/* 게임 결과 출력 */
void displayWinner(const Score *score) {
    printf("\n[게임 결과]\n");
    printf("%s: 성공 단어 %d개, 걸린 시간 %d초\n", score->challenger, score->challengerScore, score->challengerTime);
    printf("%s: 성공 단어 %d개, 걸린 시간 %d초\n", score->opponent, score->opponentScore, score->opponentTime);

    if(score->challengerScore > score->opponentScore) {
        printf("우승자: %s\n", score->challenger);
    } else if(score->challengerScore < score->opponentScore) {
        printf("우승자: %s\n", score->opponent);
    } else { // 단어 개수 같으면 시간 비교
        if(score->challengerTime < score->opponentTime) {
            printf("우승자: %s\n", score->challenger);
        } else if(score->challengerTime > score->opponentTime) {
            printf("우승자: %s\n", score->opponent);
        } else {
            printf("무승부입니다.\n");
        }
    }
}

/* 이전 우승자 관리용 (보너스) */
static char lastWinner[MAX_NICK_LEN] = "";

/* 끝말잇기 메뉴 */
void doWordRelay() {
    Score gameScore = {0};
    char challenger[MAX_NICK_LEN];
    char opponent[MAX_NICK_LEN];

    printf("도전자 닉네임 입력: ");
    scanf("%s", challenger);
    getchar(); // 버퍼 제거

    if(!isValidMember(challenger)) {
        printf("멤버가 아닙니다. 종료합니다.\n");
        return;
    }

    // 보너스: 이전 우승자가 있으면 자동 대상자 지정
    if(strlen(lastWinner) > 0 && strcmp(lastWinner, challenger) != 0) {
        printf("자동으로 이전 우승자 %s와 게임을 진행합니다.\n", lastWinner);
        strcpy(opponent, lastWinner);
    } else {
        while(1) {
            printf("대상자 닉네임 입력: ");
            scanf("%s", opponent);
            getchar();

            if(!isValidMember(opponent)) {
                printf("멤버가 아닙니다. 다시 입력하세요.\n");
                continue;
            }
            if(strcmp(opponent, challenger) == 0) {
                printf("도전자와 대상자는 다르게 입력해야 합니다.\n");
                continue;
            }
            break;
        }
    }

    playGame(challenger, opponent, &gameScore);
    displayWinner(&gameScore);

    // 보너스: 우승자 저장
    if(gameScore.challengerScore > gameScore.opponentScore)
        strcpy(lastWinner, challenger);
    else if(gameScore.challengerScore < gameScore.opponentScore)
        strcpy(lastWinner, opponent);
    else // 동점시 시간으로 결정
        strcpy(lastWinner, gameScore.challengerTime <= gameScore.opponentTime ? challenger : opponent);

    // 보너스: 모범 끝말잇기 결과(여기서는 42개 단어를 가나다순 출력)
    printf("\n[모범 끝말잇기 결과: 제시 단어 42개]\n");
    char sortedWords[GAME_WORDS][MAX_WORD_LEN];
    shuffleWords(wordList, sortedWords, GAME_WORDS);
    // 간단한 버블 정렬
    int i, j;
    char tmp[MAX_WORD_LEN];
    for(i=0; i<GAME_WORDS-1; i++) {
        for(j=0; j<GAME_WORDS-1-i; j++) {
            if(strcmp(sortedWords[j], sortedWords[j+1]) > 0) {
                strcpy(tmp, sortedWords[j]);
                strcpy(sortedWords[j], sortedWords
