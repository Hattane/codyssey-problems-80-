#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_MEMBERS 7
#define MAX_REFLECTIONS 100
#define MAX_ANSWER_LEN 51
#define MIN_ANSWER_LEN 20

typedef struct {
    char name[20];
    char nickname[20];
} Member;

typedef struct {
    char memberName[20];
    char date[11]; // YYYY-MM-DD
    char achievement[MAX_ANSWER_LEN];
    char difficulty[MAX_ANSWER_LEN];
    char plan[MAX_ANSWER_LEN];
} DailyReflection;

// Immutable list of members
const Member members[MAX_MEMBERS] = {
    {"아서", "Arthur"},
    {"영진", "Youngjin"},
    {"지훈", "Jihoon"},
    {"민서", "Minseo"},
    {"수진", "Sujin"},
    {"하늘", "Haneul"},
    {"재민", "Jaemin"}
};

// 3 reflection questions stored as 2D array (bonus)
const char questions[3][50] = {
    "오늘 어떤 성취를 이뤘나?",
    "어떤 어려움을 극복했나?",
    "내일은 어떤 계획을 세울까?"
};

DailyReflection reflections[MAX_REFLECTIONS];
int reflectionCount = 0;

// Helper function to get current date as YYYY-MM-DD
void getCurrentDate(char* dateStr, int size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(dateStr, size, "%04d-%02d-%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
}

// Helper function: remove spaces and return length of string without spaces
int lengthWithoutSpaces(const char* str) {
    int len = 0;
    while (*str) {
        if (!isspace((unsigned char)*str)) len++;
        str++;
    }
    return len;
}

// Input with length validation
void inputWithValidation(const char* prompt, char* buffer, int minLen, int maxLen) {
    char temp[256];
    while (1) {
        printf("%s (최소 %d자, 최대 %d자): ", prompt, minLen, maxLen - 1);
        if (fgets(temp, sizeof(temp), stdin) == NULL) {
            printf("입력 오류\n");
            continue;
        }
        // Remove trailing newline
        temp[strcspn(temp, "\n")] = 0;
        int len = strlen(temp);
        if (len < minLen) {
            printf("입력한 내용이 너무 짧습니다. 다시 시도하세요.\n");
            continue;
        }
        if (len >= maxLen) {
            printf("입력한 내용이 너무 깁니다. 다시 시도하세요.\n");
            continue;
        }
        strcpy(buffer, temp);
        break;
    }
}

// Input reflection answers and save to structure
void inputReflection(DailyReflection* ref, const char* memberName) {
    strcpy(ref->memberName, memberName);
    getCurrentDate(ref->date, sizeof(ref->date));

    inputWithValidation(questions[0], ref->achievement, MIN_ANSWER_LEN, MAX_ANSWER_LEN);
    inputWithValidation(questions[1], ref->difficulty, MIN_ANSWER_LEN, MAX_ANSWER_LEN);
    inputWithValidation(questions[2], ref->plan, MIN_ANSWER_LEN, MAX_ANSWER_LEN);
}

// Display reflection answers sorted by longest answer length (excluding spaces)
void displayReflection(const DailyReflection* ref) {
    // Array of pointers to questions and answers
    const char* ansPtrs[3] = {ref->achievement, ref->difficulty, ref->plan};
    int lengths[3];
    int indices[3] = {0, 1, 2};

    // Compute lengths ignoring spaces
    for (int i = 0; i < 3; i++) {
        lengths[i] = lengthWithoutSpaces(ansPtrs[i]);
    }

    // Simple bubble sort on indices based on lengths descending
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2 - i; j++) {
            if (lengths[indices[j]] < lengths[indices[j+1]]) {
                int temp = indices[j];
                indices[j] = indices[j+1];
                indices[j+1] = temp;
            }
        }
    }

    // Find nickname from members list
    const char* nickname = NULL;
    for (int i = 0; i < MAX_MEMBERS; i++) {
        if (strcmp(members[i].name, ref->memberName) == 0) {
            nickname = members[i].nickname;
            break;
        }
    }

    printf("닉네임: %s, 날짜: %s\n", nickname ? nickname : "Unknown", ref->date);
    for (int i = 0; i < 3; i++) {
        int idx = indices[i];
        printf("%s\n%s\n", questions[idx], ansPtrs[idx]);
    }
    printf("\n");
}

// Menu function to select member, input and display reflection
void doSelfReflection() {
    if (reflectionCount >= MAX_REFLECTIONS) {
        printf("더 이상 성찰을 저장할 수 없습니다.\n");
        return;
    }

    printf("멤버 목록:\n");
    for (int i = 0; i < MAX_MEMBERS; i++) {
        printf("%d. %s (%s)\n", i + 1, members[i].name, members[i].nickname);
    }

    int choice = 0;
    printf("성찰할 멤버 번호를 선택하세요: ");
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > MAX_MEMBERS) {
        printf("잘못된 선택입니다.\n");
        while (getchar() != '\n'); // Clear stdin buffer
        return;
    }
    while (getchar() != '\n'); // Clear newline from buffer

    DailyReflection newRef;
    inputReflection(&newRef, members[choice - 1].name);

    // Save reflection
    reflections[reflectionCount++] = newRef;

    printf("\n=== 입력된 성찰 ===\n");
    displayReflection(&newRef);
}
