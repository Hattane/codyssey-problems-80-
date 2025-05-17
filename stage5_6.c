#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_EMOTIONS 10
#define MAX_WORDS 20

typedef struct {
    char basicKor[32];
    char basicEng[32];
    char similarKor[32];
    char similarEng[32];
    char bodyExpression[256];
} Emotion;

typedef struct {
    char kor[32];
    char eng[32];
} EmotionWord;

int loadEmotionData(const char *filename, Emotion emotions[], int maxCount) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("파일을 열 수 없습니다: %s\n", filename);
        return -1;
    }

    char line[512];
    int count = 0;
    Emotion temp = {0};

    while (fgets(line, sizeof(line), fp)) {
        // 기본 감정 한국어
        if (strstr(line, "기본 감정:")) {
            // 다음 줄부터 읽기
            fgets(line, sizeof(line), fp);
            sscanf(line, "    한국어: %31[^\n]", temp.basicKor);
            fgets(line, sizeof(line), fp);
            sscanf(line, "    영어: %31[^\n]", temp.basicEng);
        }
        else if (strstr(line, "유사 감정:")) {
            fgets(line, sizeof(line), fp);
            sscanf(line, "    한국어: %31[^\n]", temp.similarKor);
            fgets(line, sizeof(line), fp);
            sscanf(line, "    영어: %31[^\n]", temp.similarEng);
        }
        else if (strstr(line, "몸 동작:")) {
            fgets(line, sizeof(line), fp);
            sscanf(line, "    표현: %255[^\n]", temp.bodyExpression);

            if (count < maxCount) {
                emotions[count++] = temp;
            }
            memset(&temp, 0, sizeof(Emotion));
        }
    }
    fclose(fp);
    return count;
}

void printEmotion(const Emotion *e) {
    printf("기본 감정: %s (%s)\n", e->basicKor, e->basicEng);
    printf("유사 감정: %s (%s)\n", e->similarKor, e->similarEng);
    printf("몸 동작: %s\n", e->bodyExpression);
}

void practiceEmotionWithBody() {
    Emotion emotions[MAX_EMOTIONS];
    int count = loadEmotionData("emotion.yaml", emotions, MAX_EMOTIONS);
    if (count <= 0) {
        printf("감정 데이터를 불러오는데 실패했습니다.\n");
        return;
    }

    EmotionWord words[MAX_WORDS];
    int wordCount = 0;

    // 단어 자료구조 초기화 (basicEng, similarEng)
    for (int i = 0; i < count; i++) {
        if (wordCount < MAX_WORDS) {
            strcpy(words[wordCount].kor, emotions[i].basicKor);
            strcpy(words[wordCount].eng, emotions[i].basicEng);
            wordCount++;
        }
        if (wordCount < MAX_WORDS) {
            strcpy(words[wordCount].kor, emotions[i].similarKor);
            strcpy(words[wordCount].eng, emotions[i].similarEng);
            wordCount++;
        }
    }

    // 문제 출제 - 무작위 5문제 맞힐 때까지 진행
    int correctCount = 0;
    int askedIndices[5] = {-1, -1, -1, -1, -1};
    srand((unsigned)time(NULL));

    while (correctCount < 5) {
        int idx = rand() % wordCount;

        // 중복 문제 제외
        int duplicate = 0;
        for (int i = 0; i < correctCount; i++) {
            if (askedIndices[i] == idx) {
                duplicate = 1;
                break;
            }
        }
        if (duplicate) continue;

        printf("다음 영어 감정 단어의 한국어 뜻은? : %s\n", words[idx].eng);

        char answer[64];
        printf("정답 입력: ");
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = '\0';

        if (strcmp(answer, words[idx].kor) == 0) {
            printf("정답입니다!\n");
            askedIndices[correctCount++] = idx;
        } else {
            printf("오답입니다. 다시 시도하세요.\n");
        }
    }

    printf("\n모든 문제를 맞혔습니다! 결과:\n");
    for (int i = 0; i < correctCount; i++) {
        int idx = askedIndices[i];
        // 해당 감정 출력 (몸동작까지)
        // 몸동작은 emotions 배열에서 찾아 출력해야 함
        // words[idx]가 어떤 감정인지 찾기
        for (int j = 0; j < count; j++) {
            if ((strcmp(words[idx].kor, emotions[j].basicKor) == 0 && strcmp(words[idx].eng, emotions[j].basicEng) == 0) ||
                (strcmp(words[idx].kor, emotions[j].similarKor) == 0 && strcmp(words[idx].eng, emotions[j].similarEng) == 0)) {
                printEmotion(&emotions[j]);
                break;
            }
        }
        printf("\n");
    }
}
