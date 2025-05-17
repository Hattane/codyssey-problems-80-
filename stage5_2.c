#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LYRICS 100
#define MAX_LYRIC_LEN 256
#define MAX_CONCEPT_LEN 100
#define MAX_DANCE_LEN 100
#define MAX_ANS_LEN 100

typedef struct {
    char lyric[MAX_LYRIC_LEN];
    char concept[MAX_CONCEPT_LEN];
    char danceSymbol[MAX_DANCE_LEN];
} SongWithDance;

typedef struct {
    char userDanceSymbol[MAX_ANS_LEN];
} MyAnswer;

static SongWithDance songData[MAX_LYRICS];
static MyAnswer userAnswers[MAX_LYRICS];
static int songCount = 0;
static int displayDuration = 30; // 초 기본값

// 화면 지우기 (크로스 플랫폼 최소화, ANSI escape code 사용 가능)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 파일에서 (안무 컨셉)과 {안무 표현} 추출
// 형식: 가사(안무컨셉){안무표현}
int parseLine(const char *line, SongWithDance *out) {
    const char *pConceptStart = strchr(line, '(');
    const char *pConceptEnd = strchr(line, ')');
    const char *pDanceStart = strchr(line, '{');
    const char *pDanceEnd = strchr(line, '}');

    if (!pConceptStart || !pConceptEnd || !pDanceStart || !pDanceEnd)
        return 0;

    // 가사: line 시작 ~ '(' 앞까지 (공백 제거)
    int lyricLen = (int)(pConceptStart - line);
    while (lyricLen > 0 && (line[lyricLen - 1] == ' ' || line[lyricLen - 1] == '\t'))
        lyricLen--;
    if (lyricLen >= MAX_LYRIC_LEN) lyricLen = MAX_LYRIC_LEN - 1;
    strncpy(out->lyric, line, lyricLen);
    out->lyric[lyricLen] = '\0';

    // 안무 컨셉 복사
    int conceptLen = (int)(pConceptEnd - pConceptStart - 1);
    if (conceptLen >= MAX_CONCEPT_LEN) conceptLen = MAX_CONCEPT_LEN - 1;
    strncpy(out->concept, pConceptStart + 1, conceptLen);
    out->concept[conceptLen] = '\0';

    // 안무 표현 복사
    int danceLen = (int)(pDanceEnd - pDanceStart - 1);
    if (danceLen >= MAX_DANCE_LEN) danceLen = MAX_DANCE_LEN - 1;
    strncpy(out->danceSymbol, pDanceStart + 1, danceLen);
    out->danceSymbol[danceLen] = '\0';

    return 1;
}

// song-with-dance.txt 파일에서 노래와 안무 데이터 로드
int LoadFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: 파일 '%s'을(를) 열 수 없습니다.\n", filename);
        return 0;
    }

    char line[512];
    songCount = 0;
    while (fgets(line, sizeof(line), fp)) {
        // 개행 제거
        line[strcspn(line, "\r\n")] = 0;
        if (line[0] == '\0') continue; // 빈 줄 무시

        if (songCount >= MAX_LYRICS) {
            printf("경고: 최대 가사 수 초과, 추가 읽기 중단\n");
            break;
        }

        if (!parseLine(line, &songData[songCount])) {
            printf("경고: 형식 오류 - %s\n", line);
            continue;
        }
        songCount++;
    }
    fclose(fp);
    return 1;
}

// 가사, 안무 컨셉, 안무 표현 출력 후 유지시간 대기 및 화면 지우기
void DisplayHint(int seconds) {
    printf("\n=== 가사와 안무 힌트 ===\n\n");
    for (int i = 0; i < songCount; i++) {
        printf("%d) %s (%s) {%s}\n", i + 1, songData[i].lyric, songData[i].concept, songData[i].danceSymbol);
    }
    printf("\n%d초간 유지 후 화면을 지웁니다...\n", seconds);
    fflush(stdout);

    time_t start = time(NULL);
    while ((time(NULL) - start) < seconds) {
        // 대기
    }
    clearScreen();
}

// 무작위 소절 출력하고 안무 표현 입력 받아 MyAnswer에 저장
void InputDanceSymbol(void) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < songCount; i++) {
        int idx = rand() % songCount;
        printf("가사: %s\n", songData[idx].lyric);
        printf("안무 표현 입력: ");
        if (fgets(userAnswers[idx].userDanceSymbol, MAX_ANS_LEN, stdin) == NULL) {
            userAnswers[idx].userDanceSymbol[0] = '\0';
        } else {
            // 개행 제거
            userAnswers[idx].userDanceSymbol[strcspn(userAnswers[idx].userDanceSymbol, "\r\n")] = 0;
        }
    }
}

// 기준 데이터와 입력 데이터 비교, 일치율 계산
void EvalMatchingRatio(void) {
    int matched = 0;
    printf("\n=== 평가 결과 ===\n");
    for (int i = 0; i < songCount; i++) {
        if (strcmp(songData[i].danceSymbol, userAnswers[i].userDanceSymbol) == 0) {
            matched++;
        }
    }
    int percent = (songCount == 0) ? 0 : (matched * 100 / songCount);
    printf("제시된 가사에 대한 당신의 안무가 일치한 비율은 %d%% 입니다.\n", percent);

    if (matched < songCount) {
        printf("일치하지 않은 가사 목록:\n");
        for (int i = 0; i < songCount; i++) {
            if (strcmp(songData[i].danceSymbol, userAnswers[i].userDanceSymbol) != 0) {
                printf("- %s\n", songData[i].lyric);
            }
        }
    }
}

// 출력 유지시간 설정 함수 (보너스)
void SetDisplayDuration(void) {
    printf("가사와 안무 출력 유지 시간을 초 단위로 입력하세요 (현재: %d초): ", displayDuration);
    int inputSec = 0;
    if (scanf("%d", &inputSec) == 1 && inputSec > 0) {
        displayDuration = inputSec;
        printf("유지 시간이 %d초로 설정되었습니다.\n", displayDuration);
    } else {
        printf("잘못된 입력입니다. 유지 시간이 변경되지 않았습니다.\n");
    }
    while (getchar() != '\n'); // 버퍼 비우기
}

// [II. 훈련 > 5. 댄스 훈련 > D. 노래와 안무 기억]
void RememberSongWithDance(void) {
    if (!LoadFile("song-with-dance.txt")) {
        printf("파일 로드 실패로 인해 기능을 실행할 수 없습니다.\n");
        return;
    }

    DisplayHint(displayDuration);
    InputDanceSymbol();
    EvalMatchingRatio();
}

// 문제3번 메뉴 확장 예제
void MenuDanceTraining(void) {
    while (1) {
        printf("\n=== 댄스 훈련 메뉴 ===\n");
        printf("1. 가사와 안무 기억하기\n");
        printf("2. 출력 유지 시간 설정\n");
        printf("0. 종료\n");
        printf("선택: ");

        int choice = 0;
        if (scanf("%d", &choice) != 1) {
            printf("잘못된 입력입니다.\n");
            while (getchar() != '\n'); // 입력버퍼 비우기
            continue;
        }
        while (getchar() != '\n'); // 입력버퍼 비우기

        switch (choice) {
            case 1:
                RememberSongWithDance();
                break;
            case 2:
                SetDisplayDuration();
                break;
            case 0:
                return;
            default:
                printf("올바른 메뉴를 선택하세요.\n");
                break;
        }
    }
}

// main() 예시 (실제 프로그램의 일부로 통합 필요)
int main(void) {
    // 문제3번 메뉴에 댄스 훈련 메뉴를 연결했다고 가정
    MenuDanceTraining();

    return 0;
}
