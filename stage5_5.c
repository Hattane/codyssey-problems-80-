#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINES 10
#define MAX_LEN 256

typedef struct {
    char original[MAX_LEN];
    char mirrored[MAX_LEN];
} MirroredText;

typedef struct {
    char userInput[MAX_LEN];
} AnswerText;

// 거울 문자 매핑 함수 (대소문자, 숫자 포함 일부만 대응)
char mirrorChar(char ch) {
    switch (ch) {
        case 'A': return 'A';
        case 'E': return '3';
        case '3': return 'E';
        case 'H': return 'H';
        case 'I': return 'I';
        case 'J': return 'L';
        case 'L': return 'J';
        case 'M': return 'M';
        case 'O': return 'O';
        case 'S': return '2';
        case '2': return 'S';
        case 'T': return 'T';
        case 'U': return 'U';
        case 'V': return 'V';
        case 'W': return 'W';
        case 'X': return 'X';
        case 'Y': return 'Y';
        case 'Z': return '5';
        case '5': return 'Z';
        case 'b': return 'd';
        case 'd': return 'b';
        case 'p': return 'q';
        case 'q': return 'p';
        case ' ': return ' ';
        case ',': return ',';
        case '.': return '.';
        case '!': return '!';
        default: return ch; // 변환 불가 문자 원본 유지
    }
}

// 원래 텍스트 파일에서 한 줄씩 불러오기
int LoadOriginalFile(const char *filename, MirroredText *texts, int maxLines) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "파일을 열 수 없습니다: %s\n", filename);
        return 0;
    }

    int count = 0;
    while (count < maxLines && fgets(texts[count].original, MAX_LEN, fp)) {
        // 줄 끝 개행 문자 제거
        texts[count].original[strcspn(texts[count].original, "\r\n")] = 0;
        count++;
    }

    fclose(fp);
    return count;
}

// 원래 텍스트를 거울 모드 텍스트로 변환
void TransMirrorText(MirroredText *texts, int count) {
    for (int i = 0; i < count; i++) {
        int len = (int)strlen(texts[i].original);
        int j, k = 0;
        // 거울 모드는 문자 순서가 거꾸로면서 각 문자도 매핑됨
        for (j = len - 1; j >= 0; j--) {
            char mirrored = mirrorChar(texts[i].original[j]);
            texts[i].mirrored[k++] = mirrored;
        }
        texts[i].mirrored[k] = '\0';
    }
}

// 문자열 비교 시 대소문자 무시, 공백 무시 간단판 (필요시 더 정교화 가능)
int equalsIgnoreCase(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == 0 && *b == 0;
}

// 입력 받는 함수 (사용자 입력)
void getInput(const char *prompt, char *buffer, int maxLen) {
    printf("%s", prompt);
    if (fgets(buffer, maxLen, stdin)) {
        buffer[strcspn(buffer, "\r\n")] = 0; // 개행 제거
    }
}

// 훈련 메인 함수
void PracticeMirrorMode() {
    MirroredText texts[MAX_LINES];
    AnswerText answers[MAX_LINES];

    printf("원래 텍스트 파일 경로를 입력하세요: ");
    char filename[128];
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("입력 오류\n");
        return;
    }
    filename[strcspn(filename, "\r\n")] = 0;

    int count = LoadOriginalFile(filename, texts, MAX_LINES);
    if (count == 0) {
        printf("원래 텍스트를 불러오지 못했습니다.\n");
        return;
    }

    TransMirrorText(texts, count);

    srand((unsigned int)time(NULL));

    int correctCount = 0;
    int failedIndices[MAX_LINES];
    int failCount = 0;

    for (int round = 0; round < 3; round++) {
        printf("\n거울 모드 연습 #%d\n", round + 1);

        // 무작위 인덱스 뽑기
        int idx = rand() % count;

        printf("옵션을 선택하세요:\n");
        printf("1. 거울 모드 텍스트 제시 후 원래 텍스트 입력\n");
        printf("2. 원래 텍스트 제시 후 거울 모드 텍스트 입력\n");
        printf("선택(1 또는 2): ");

        char option[8];
        if (!fgets(option, sizeof(option), stdin)) {
            printf("입력 오류\n");
            break;
        }

        int sel = atoi(option);

        if (sel == 1) {
            printf("거울 모드 텍스트: %s\n", texts[idx].mirrored);
            getInput("원래 텍스트를 입력하세요: ", answers[round].userInput, MAX_LEN);

            if (equalsIgnoreCase(answers[round].userInput, texts[idx].original)) {
                printf("정답입니다!\n");
                correctCount++;
            } else {
                printf("오답입니다.\n");
                failedIndices[failCount++] = idx;
            }
        } else if (sel == 2) {
            printf("원래 텍스트: %s\n", texts[idx].original);
            getInput("거울 모드 텍스트를 입력하세요: ", answers[round].userInput, MAX_LEN);

            if (equalsIgnoreCase(answers[round].userInput, texts[idx].mirrored)) {
                printf("정답입니다!\n");
                correctCount++;
            } else {
                printf("오답입니다.\n");
                failedIndices[failCount++] = idx;
            }
        } else {
            printf("잘못된 선택입니다. 이번 문제는 건너뜁니다.\n");
        }
    }

    printf("\n[결과]\n");
    printf("총 3문제 중 %d문제를 맞췄습니다.\n", correctCount);

    if (failCount > 0) {
        printf("틀린 문제 목록:\n");
        for (int i = 0; i < failCount; i++) {
            int fi = failedIndices[i];
            printf("원래 텍스트: %s\n", texts[fi].original);
            printf("거울 모드 텍스트: %s\n\n", texts[fi].mirrored);
        }
    }
}

// 메뉴 확장 예시
void menuStage5() {
    char input[16];
    while (1) {
        printf("\n[II. 훈련 > 5. 댄스 훈련]\n");
        printf("E. 마인드풀니스 호흡 (예시)\n");
        printf("F. 안무 스타일 퀴즈\n");
        printf("G. 거울 모드 연습\n");
        printf("Q. 종료\n");
        printf("선택: ");

        if (!fgets(input, sizeof(input), stdin))
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strcasecmp(input, "Q") == 0) {
            printf("프로그램을 종료합니다.\n");
            break;
        } else if (strcasecmp(input, "E") == 0) {
            printf("마인드풀니스 호흡 기능은 아직 구현되지 않았습니다.\n");
        } else if (strcasecmp(input, "F") == 0) {
            // 기존 문제5 함수 호출 (learnDanceStyle) 여기에 위치
        } else if (strcasecmp(input, "G") == 0) {
            PracticeMirrorMode();
        } else {
            printf("잘못된 선택입니다. 다시 입력해주세요.\n");
        }
    }
}

int main() {
    menuStage5();
    return 0;
}
