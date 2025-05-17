#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEMBER_COUNT 4
#define WORDS_PER_LEVEL 10
#define LEVEL_COUNT 3
#define HIGH_LEVEL_WORDS 5
#define STEP_WORDS 3

// Millieways members (replace or extend as needed)
const char *members[MEMBER_COUNT] = {"Alice", "Bob", "Cindy", "Dave"};

// WordCard struct for dictionary entries
typedef struct {
    char word[16];
    char explanation[64];
    int level; // 1=Beginner, 2=Intermediate, 3=Advanced
} WordCard;

// Challenge struct to track progress of each member
typedef struct {
    char name[16];      
    int level;          // current challenge level (1~3), 0 if not started
    int score;          // number of correct answers in last challenge
    int passStatus;     // 1,2,3 = passed that level, 4 = final pass
    int passDate;       // YYYYMMDD of passing final stage
} MyChallenge;

// Sample dictionary: 10 words per level
WordCard dictionary[LEVEL_COUNT * WORDS_PER_LEVEL] = {
    // Level 1 (Beginner)
    {"사과", "과일의 일종", 1}, {"바다", "물로 가득 찬 큰 곳",1}, {"책", "읽는 물건",1},
    {"학교", "배우는 곳",1}, {"강아지", "작은 개",1}, {"꽃", "예쁜 식물",1}, {"비", "하늘에서 내리는 물",1},
    {"나무", "크고 높은 식물",1}, {"달", "밤에 빛나는 천체",1}, {"바람", "공기의 움직임",1},

    // Level 2 (Intermediate)
    {"연필", "글씨 쓰는 도구",2}, {"자동차", "사람이 타는 탈것",2}, {"기차", "선로 위를 달리는 탈것",2},
    {"컴퓨터", "계산하는 기계",2}, {"전화", "멀리 있는 사람과 이야기",2}, {"음악", "소리의 예술",2},
    {"영화", "보는 이야기",2}, {"운동장", "운동하는 장소",2}, {"도서관", "책이 많은 곳",2}, {"시장", "물건 파는 곳",2},

    // Level 3 (Advanced)
    {"철학", "인생과 존재를 연구",3}, {"경제학", "재화의 생산과 소비",3}, {"정치학", "국가 운영의 학문",3},
    {"문학", "글쓰기 예술",3}, {"역사", "과거의 기록",3}, {"생태계", "자연의 상호 작용",3},
    {"기술", "과학의 응용",3}, {"문화", "사회의 생활 양식",3}, {"법률", "사회 규칙",3}, {"과학", "자연 현상의 탐구",3}
};

// Challenges array to track each member
MyChallenge challenges[MEMBER_COUNT];

// Flags to mark selected members in current round
int selectedFlags[MEMBER_COUNT];

// Get current date in YYYYMMDD format
int getCurrentDate() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    return (tm->tm_year + 1900)*10000 + (tm->tm_mon + 1)*100 + tm->tm_mday;
}

// Initialize challenge records
void initChallenges() {
    for(int i=0; i<MEMBER_COUNT; i++) {
        strcpy(challenges[i].name, members[i]);
        challenges[i].level = 0;
        challenges[i].score = 0;
        challenges[i].passStatus = 0;
        challenges[i].passDate = 0;
    }
}

// Check if all members passed final challenge
int allPassed() {
    for(int i=0; i<MEMBER_COUNT; i++) {
        if(challenges[i].passStatus != 4) return 0;
    }
    return 1;
}

// Select a member randomly who hasn't been selected this round and is not passed final
int selectMember() {
    int available[MEMBER_COUNT];
    int count = 0;
    for(int i=0; i<MEMBER_COUNT; i++) {
        if(!selectedFlags[i] && challenges[i].passStatus != 4) {
            available[count++] = i;
        }
    }
    if(count == 0) return -1;
    int r = rand() % count;
    selectedFlags[available[r]] = 1;
    return available[r];
}

// Shuffle array of WordCard
void shuffleWordCards(WordCard *arr, int n) {
    for(int i=n-1; i>0; i--) {
        int j = rand() % (i+1);
        WordCard tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

// Select a subset of words by level randomly
void selectWordsByLevel(int level, WordCard *outArr, int count) {
    WordCard pool[WORDS_PER_LEVEL];
    int idx = 0;
    for(int i=0; i<LEVEL_COUNT*WORDS_PER_LEVEL; i++) {
        if(dictionary[i].level == level) {
            pool[idx++] = dictionary[i];
        }
    }
    shuffleWordCards(pool, WORDS_PER_LEVEL);
    for(int i=0; i<count; i++) {
        outArr[i] = pool[i];
    }
}

// Highest difficulty challenge (5 questions)
int challengeHighLevel(MyChallenge *challenge) {
    WordCard testSet[HIGH_LEVEL_WORDS];
    selectWordsByLevel(3, testSet, HIGH_LEVEL_WORDS);

    printf("\n[최고 난이도 시험 도전 시작! 설명을 듣고 단어를 입력하세요.]\n");
    int correct=0;
    char answer[32];
    for(int i=0; i<HIGH_LEVEL_WORDS; i++) {
        printf("문제 %d: %s\n답: ", i+1, testSet[i].explanation);
        scanf("%31s", answer);
        if(strcmp(answer, testSet[i].word) == 0) {
            correct++;
        }
    }
    if(correct == HIGH_LEVEL_WORDS) {
        printf("모든 문제 정답! 최종 합격!\n");
        challenge->passStatus = 4; // Final pass
        challenge->passDate = getCurrentDate();
        challenge->level = 3;
        return 1;
    } else {
        printf("시험 실패, 다시 모든 멤버가 도전해야 합니다.\n");
        return 0;
    }
}

// Step-by-step challenge (3 questions per level)
int challengeStepByStep(MyChallenge *challenge) {
    int level = challenge->level == 0 ? 1 : challenge->level;
    WordCard testSet[STEP_WORDS];
    selectWordsByLevel(level, testSet, STEP_WORDS);

    printf("\n[난이도 %d 단계 도전 시작! 설명을 듣고 단어를 입력하세요.]\n", level);
    int correct=0;
    char answer[32];
    for(int i=0; i<STEP_WORDS; i++) {
        printf("문제 %d: %s\n답: ", i+1, testSet[i].explanation);
        scanf("%31s", answer);
        if(strcmp(answer, testSet[i].word) == 0) {
            correct++;
        }
    }

    if(correct >= 2) {
        printf("난이도 %d 합격!\n", level);
        challenge->level = level;
        challenge->score = correct;
        if(level == 3) {
            challenge->passStatus = 4; // Final pass
            challenge->passDate = getCurrentDate();
            printf("축하합니다! 최종 합격입니다.\n");
            return 1;
        } else {
            challenge->passStatus = level;
            challenge->level++;
            return 1;
        }
    } else {
        printf("난이도 %d 실패, 모든 멤버가 다시 도전해야 합니다.\n", level);
        return 0;
    }
}

// Display final results
void displayResult() {
    printf("\n[게임 결과]\n");
    for(int i=0; i<MEMBER_COUNT; i++) {
        printf("멤버: %s, 상태: ", challenges[i].name);
        if(challenges[i].passStatus == 4) {
            printf("최종 합격 (합격일: %d)\n", challenges[i].passDate);
        } else {
            printf("미합격 또는 도전 중\n");
        }
    }
}

// Main dictionary game flow
void doDictionaryGame() {
    if(allPassed()) {
        printf("이 과정은 이미 통과했습니다. 다른 메뉴를 선택하세요.\n");
        return;
    }

    memset(selectedFlags, 0, sizeof(selectedFlags));

    int choice;
    int memberIndex;

    while((memberIndex = selectMember()) != -1) {
        MyChallenge *c = &challenges[memberIndex];
        printf("\n선택된 게이머: %s\n", c->name);
        printf("1. 최고 난이도 시험 도전\n2. 난이도 단계별 도전\n선택: ");
        scanf("%d", &choice);

        int result = 0;
        if(choice == 1) {
            result = challengeHighLevel(c);
        } else if(choice == 2) {
            result = challengeStepByStep(c);
        } else {
            printf("잘못된 선택입니다. 다시 선택하세요.\n");
            selectedFlags[memberIndex] = 0; // allow re-select
            continue;
        }

        if(!result) {
            // On fail, reset all who didn't pass final
            for(int i=0; i<MEMBER_COUNT; i++) {
                if(challenges[i].passStatus != 4) {
                    challenges[i].level = 0;
                    challenges[i].score = 0;
                    challenges[i].passStatus = 0;
                    challenges[i].passDate = 0;
                }
            }
            printf("게임이 초기화 되었습니다. 다시 시작하세요.\n");
            return;
        }
    }

    if(allPassed()) {
        printf("\n축하합니다! 밀리웨이즈 그룹은 딕셔너리 게임에 최종 합격했습니다.\n");
    }
    displayResult();
}

// Main program entry point (test loop)
int main() {
    srand((unsigned)time(NULL));
    initChallenges();

    int menu;
    while(1) {
        printf("\n[II. 훈련 > 3. 언어 및 발음 훈련 > E. 딕셔너리 게임]\n");
        printf("1. 딕셔너리 게임 시작\n0. 종료\n선택: ");
        scanf("%d", &menu);

        if(menu == 1) {
            doDictionaryGame();
        } else if(menu == 0) {
            printf("프로그램 종료.\n");
            break;
        } else {
            printf("잘못된 입력입니다.\n");
        }
    }

    return 0;
}
