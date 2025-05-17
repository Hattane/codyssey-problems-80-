#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STYLES 100
#define MAX_STR_LEN 256

typedef struct {
    char styleName[MAX_STR_LEN];
    char song[MAX_STR_LEN];
    char description[MAX_STR_LEN];
} DanceStyle;

// HTML 파일에서 태그 안 텍스트 추출 도움 함수
// 예: <div class="style">힙합</div> 에서 "힙합" 추출
int extractTextInTag(const char *line, char *output, int maxLen) {
    const char *start = strchr(line, '>');
    if (!start) return 0;
    start++;
    const char *end = strchr(start, '<');
    if (!end) return 0;
    int len = (int)(end - start);
    if (len >= maxLen) len = maxLen - 1;
    strncpy(output, start, len);
    output[len] = '\0';
    return 1;
}

// dance_style.html 파싱 함수
int parseDanceStyleHTML(const char *filename, DanceStyle *styles, int maxCount) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Failed to open %s\n", filename);
        return 0;
    }

    char line[512];
    int count = 0;

    while (fgets(line, sizeof(line), fp)) {
        // 스타일명 부분 찾기 (예: <div class="style">)
        if (strstr(line, "class=\"style\"")) {
            if (count >= maxCount) break;

            if (!extractTextInTag(line, styles[count].styleName, MAX_STR_LEN)) continue;

            // 다음 줄에서 노래명
            if (!fgets(line, sizeof(line), fp)) break;
            if (strstr(line, "class=\"song\"")) {
                if (!extractTextInTag(line, styles[count].song, MAX_STR_LEN))
                    styles[count].song[0] = '\0';
            } else {
                styles[count].song[0] = '\0';
            }

            // 다음 줄에서 설명
            if (!fgets(line, sizeof(line), fp)) break;
            if (strstr(line, "class=\"desc\"")) {
                if (!extractTextInTag(line, styles[count].description, MAX_STR_LEN))
                    styles[count].description[0] = '\0';
            } else {
                styles[count].description[0] = '\0';
            }

            count++;
        }
    }

    fclose(fp);
    return count;
}

// 배열 셔플 (Fisher–Yates shuffle)
void shuffle(int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

// 무작위 퀴즈 함수
void learnDanceStyle() {
    DanceStyle styles[MAX_STYLES];
    int total = parseDanceStyleHTML("dance_style.html", styles, MAX_STYLES);
    if (total == 0) {
        printf("안무 스타일을 불러오지 못했습니다.\n");
        return;
    }

    int indices[MAX_STYLES];
    for (int i = 0; i < total; i++) indices[i] = i;

    srand((unsigned)time(NULL));
    shuffle(indices, total);

    int quizCount = 5;
    if (quizCount > total) quizCount = total;

    int correct = 0;
    int wrongIndices[quizCount];
    int wrongCount = 0;
    char input[MAX_STR_LEN];

    printf("\n[안무 스타일 퀴즈 시작]\n\n");

    for (int i = 0; i < quizCount; i++) {
        int idx = indices[i];
        printf("노래: \"%s\"\n이 노래에 맞는 안무 스타일을 입력하세요: ", styles[idx].song);

        if (!fgets(input, sizeof(input), stdin))
            break;

        input[strcspn(input, "\n")] = '\0'; // 개행 문자 제거

        if (strcmp(input, styles[idx].styleName) == 0) {
            printf("정답입니다!\n\n");
            correct++;
        } else {
            printf("오답입니다.\n\n");
            wrongIndices[wrongCount++] = idx;
        }
    }

    printf("총 %d문제 중 %d문제를 맞췄습니다.\n\n", quizCount, correct);

    if (wrongCount > 0) {
        printf("틀린 문제 정답과 설명:\n");
        for (int i = 0; i < wrongCount; i++) {
            int idx = wrongIndices[i];
            printf("노래: %s\n안무 스타일: %s\n설명: %s\n\n",
                   styles[idx].song,
                   styles[idx].styleName,
                   styles[idx].description);
        }
    }
}

// YAML 파일로 저장 (보너스)
void saveDanceStyleYAML(const char *filename, DanceStyle *styles, int count) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "YAML 파일을 열 수 없습니다: %s\n", filename);
        return;
    }

    fprintf(fp, "danceStyles:\n");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "  - 안무_스타일: \"%s\"\n", styles[i].styleName);
        fprintf(fp, "    노래: \"%s\"\n", styles[i].song);
        fprintf(fp, "    설명: \"%s\"\n", styles[i].description);
    }

    fclose(fp);
    printf("YAML 파일로 저장되었습니다: %s\n", filename);
}

// 메뉴 함수 예시
void menuStage5() {
    char input[16];
    while (1) {
        printf("\n[II. 훈련 > 5. 댄스 훈련]\n");
        printf("E. 마인드풀니스 호흡 (예시)\n");
        printf("F. 안무 스타일 퀴즈\n");
        printf("Y. 안무 스타일 YAML 저장 (보너스)\n");
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
            learnDanceStyle();
        } else if (strcasecmp(input, "Y") == 0) {
            DanceStyle styles[MAX_STYLES];
            int total = parseDanceStyleHTML("dance_style.html", styles, MAX_STYLES);
            if (total == 0) {
                printf("안무 스타일을 불러오지 못했습니다.\n");
            } else {
                saveDanceStyleYAML("dance_style.yaml", styles, total);
            }
        } else {
            printf("잘못된 선택입니다. 다시 입력해주세요.\n");
        }
    }
}

int main() {
    menuStage5();
    return 0;
}
