#include <stdio.h>
#include <time.h>
#include <ctype.h>

#define MAX_MEMBERS 7
#define MAX_MEMORIES 100
#define MIN_CONTENT_LEN 30
#define MAX_CONTENT_LEN 70
#define DATE_STR_LEN 9   // YYYYMMDD + \0
#define DATE_PRINT_LEN 15 // YYYY년 MM월 DD일\0

typedef struct {
    char name[20];
    char nickname[20];
} Member;

typedef struct {
    char memberName[20];
    char date[DATE_STR_LEN]; // YYYYMMDD
    char content[MAX_CONTENT_LEN + 1];
    int wowEffect; // 0~10
} MemoryRecord;

const Member members[MAX_MEMBERS] = {
    {"아서", "Arthur"},
    {"영진", "Youngjin"},
    {"지훈", "Jihoon"},
    {"민서", "Minseo"},
    {"수진", "Sujin"},
    {"하늘", "Haneul"},
    {"재민", "Jaemin"}
};

MemoryRecord memories[MAX_MEMORIES];
int memoryCount = 0;

// Globals for current usable hour (18~22)
int currentHour = -1;

// Helper: manual strlen without string.h
int my_strlen(const char *s) {
    int len = 0;
    while (s[len] != '\0') len++;
    return len;
}

// Helper: manual strcmp (case sensitive)
int my_strcmp(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}

// Helper: manual strcpy
void my_strcpy(char *dest, const char *src) {
    int i = 0;
    while ((dest[i] = src[i]) != '\0') i++;
}

// Helper: manual tolower
char my_tolower(char c) {
    if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
    return c;
}

// Helper: manual strncmp (case insensitive for 'y','n' input)
int my_stricmp(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        char ca = a[i], cb = b[i];
        if (ca >= 'A' && ca <= 'Z') ca += 'a' - 'A';
        if (cb >= 'A' && cb <= 'Z') cb += 'a' - 'A';
        if (ca != cb) return ca - cb;
        i++;
    }
    return a[i] - b[i];
}

// Helper: clear input buffer until newline or EOF
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Get current hour or from cheat key input
void updateCurrentHour() {
    printf("치트키(T 또는 t)를 입력하려면 입력하고, 그냥 엔터하려면 엔터를 누르세요: ");
    int c = getchar();
    if (c == 'T' || c == 't') {
        clearInputBuffer();
        char input[13] = {0}; // YYYYMMDDHH + \0
        int valid = 0;
        for (int tries = 0; tries < 3 && !valid; tries++) {
            printf("사용할 시간(YYYYMMDDHH) 입력: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("입력 오류\n");
                continue;
            }
            // Remove newline
            for (int i = 0; input[i] != '\0'; i++) {
                if (input[i] == '\n') {
                    input[i] = '\0';
                    break;
                }
            }
            // Validate length 10 digits numeric only
            int len = my_strlen(input);
            if (len != 10) {
                printf("잘못된 형식입니다. 10자리 숫자를 입력하세요.\n");
                continue;
            }
            valid = 1;
            for (int i = 0; i < 10; i++) {
                if (input[i] < '0' || input[i] > '9') {
                    printf("숫자만 입력해야 합니다.\n");
                    valid = 0;
                    break;
                }
            }
            if (!valid) continue;
            // Extract hour HH
            int hour = (input[8]-'0')*10 + (input[9]-'0');
            if (hour < 0 || hour > 23) {
                printf("시간은 0~23 사이여야 합니다.\n");
                valid = 0;
                continue;
            }
            currentHour = hour;
        }
        if (!valid) {
            printf("치트키 시간 설정 실패, 현재 시간으로 자동 설정합니다.\n");
            currentHour = -1; // fallback to real time
        }
    } else {
        clearInputBuffer();
        currentHour = -1; // real time
    }
}

// Get system current hour
int getSystemHour() {
    time_t now = time(NULL);
    struct tm *lt = localtime(&now);
    return lt->tm_hour;
}

// Validate usable time (18~22)
int isUsableTime() {
    int hour = currentHour;
    if (hour == -1) hour = getSystemHour();
    return (hour >= 18 && hour <= 22);
}

// Map wow effect int to string category
void wowEffectToCategory(int wow, char *out) {
    if (wow >= 0 && wow <= 3) {
        my_strcpy(out, "Calm");
    } else if (wow >= 4 && wow <= 6) {
        my_strcpy(out, "Exciting");
    } else if (wow >= 7 && wow <= 10) {
        my_strcpy(out, "Meaningful");
    } else {
        my_strcpy(out, "Unknown");
    }
}

// Print date YYYYMMDD as "YYYY년 MM월 DD일"
void printFormattedDate(const char *date) {
    // date format guaranteed YYYYMMDD
    printf("%.4s년 %.2s월 %.2s일", date, date+4, date+6);
}

// Input integer from stdin, returns -1 on invalid
int inputInt() {
    int val;
    int ret = scanf("%d", &val);
    clearInputBuffer();
    if (ret != 1) return -1;
    return val;
}

// Input string with manual fgets and removes newline
void inputString(char *buffer, int maxLen) {
    if (fgets(buffer, maxLen, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    // Remove newline
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
            break;
        }
    }
}

// Input journey content with length check
int inputContent(char *buffer) {
    for (int tries = 0; tries < 3; tries++) {
        printf("여정 내용 입력 (30자 이상 70자 이하): ");
        inputString(buffer, MAX_CONTENT_LEN + 1);
        int len = my_strlen(buffer);
        if (len >= MIN_CONTENT_LEN && len <= MAX_CONTENT_LEN) return 1;
        printf("입력한 내용 길이가 조건에 맞지 않습니다. 다시 시도하세요.\n");
    }
    return 0;
}

// Input wow effect rating 0~10
int inputWowEffect() {
    for (int tries = 0; tries < 3; tries++) {
        printf("wow 효과 점수 입력 (0~10): ");
        int val = inputInt();
        if (val >=0 && val <= 10) return val;
        printf("0에서 10 사이의 숫자를 입력하세요.\n");
    }
    return -1;
}

// Find member index by nickname, -1 if not found
int findMemberByNickname(const char *nick) {
    for (int i = 0; i < MAX_MEMBERS; i++) {
        if (my_strcmp(members[i].nickname, nick) == 0) {
            return i;
        }
    }
    return -1;
}

// Print all memories of a member
void displayMemory() {
    char nick[20];
    printf("멤버 닉네임 입력: ");
    inputString(nick, sizeof(nick));
    int idx = findMemberByNickname(nick);
    if (idx == -1) {
        printf("존재하지 않는 닉네임입니다.\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < memoryCount; i++) {
        if (my_strcmp(memories[i].memberName, members[idx].name) == 0) {
            printf("\n멤버 이름: %s\n", members[idx].name);
            printf("여정 내용: %s\n", memories[i].content);
            printf("인상 깊은 순간: ");
            printFormattedDate(memories[i].date);
            char cat[20];
            wowEffectToCategory(memories[i].wowEffect, cat);
            printf(", %d점 (%s)\n", memories[i].wowEffect, cat);
            found = 1;
        }
    }
    if (!found) printf("기록된 여정이 없습니다.\n");
}

// Find and print memories by wow effect category
void findImpressiveMoment()
