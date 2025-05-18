#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "debut.h"

static Media mediaList[MAX_MEDIA] = {
    {"BBC"}, {"JTBC"}, {"KBS"}, {"MBC"}, {"MTN"},
    {"OhmyNews"}, {"SBS"}, {"SamProTV"}, {"Hankyoreh"}, {"YTN"}
};

static Trainee trainees[MAX_TRAINEES];
static int traineeCount = 0;

// 알파벳 ID 생성 (기본)
void generateTraineeIDs(int count, char ids[][2]) {
    for (int i = 0; i < count; i++) {
        ids[i][0] = 'A' + i;
        ids[i][1] = '\0';
    }
}

// 연습생 닉네임 파일에서 읽기 (보너스)
int loadTraineesFromFile(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("연습생 파일 열기 실패");
        return 0;
    }
    traineeCount = 0;
    while (fgets(trainees[traineeCount].nickname, MAX_NAME_LEN, fp)) {
        // 개행 문자 제거
        char* nl = strchr(trainees[traineeCount].nickname, '\n');
        if (nl) *nl = '\0';
        if (strlen(trainees[traineeCount].nickname) > 0)
            traineeCount++;
        if (traineeCount >= MAX_TRAINEES) break;
    }
    fclose(fp);
    return traineeCount;
}

// 무작위 인덱스 선택 (중복 없이 count 개 선택)
void selectRandomTrainees(int count, int selectedIndices[]) {
    int selected = 0;
    int used[MAX_TRAINEES] = {0};

    while (selected < count) {
        int r = rand() % traineeCount;
        if (!used[r]) {
            used[r] = 1;
            selectedIndices[selected++] = r;
        }
    }
}

// 사다리 가로줄 입력 및 스왑 적용
void ladderGame(int numPeople, int numHorizontal, char ladder[][2]) {
    printf("가로줄 수: %d\n", numHorizontal);
    for (int i = 0; i < numHorizontal; i++) {
        printf("가로줄 %d (시작 세로줄 알파벳, 끝 세로줄 알파벳): ", i+1);
        scanf(" %c %c", &ladder[i][0], &ladder[i][1]);
        // 대문자 변환
        if (ladder[i][0] >= 'a' && ladder[i][0] <= 'z')
            ladder[i][0] = ladder[i][0] - 'a' + 'A';
        if (ladder[i][1] >= 'a' && ladder[i][1] <= 'z')
            ladder[i][1] = ladder[i][1] - 'a' + 'A';

        // 입력 유효성 검사: 인접 세로줄만 가능
        if (ladder[i][1] != ladder[i][0] + 1) {
            printf("잘못된 가로줄입니다. 인접 세로줄만 입력하세요.\n");
            i--;
            continue;
        }
        if (ladder[i][0] < 'A' || ladder[i][0] >= 'A' + numPeople - 1) {
            printf("세로줄 범위를 벗어났습니다.\n");
            i--;
        }
    }
}

// 사다리 진행 후 최종 위치 반환
void runLadder(int numPeople, int numHorizontal, char ladder[][2], int resultPositions[]) {
    // 시작 위치: 0..numPeople-1
    for (int i = 0; i < numPeople; i++)
        resultPositions[i] = i;

    for (int h = 0; h < numHorizontal; h++) {
        char left = ladder[h][0];
        int leftIndex = left - 'A';

        // 해당 가로줄에서 왼쪽과 오른쪽 위치 스왑
        int rightIndex = leftIndex + 1;
        int temp = resultPositions[leftIndex];
        resultPositions[leftIndex] = resultPositions[rightIndex];
        resultPositions[rightIndex] = temp;
    }
}

void interactionMedia(void) {
    int numPeople;

    srand((unsigned int)time(NULL));

    // 연습생 정보 불러오기 (보너스)
    int useFile = 0;
    printf("연습생 정보를 파일에서 불러오겠습니까? (0: 아니오, 1: 예): ");
    scanf("%d", &useFile);

    if (useFile) {
        int loaded = loadTraineesFromFile("trainees.txt");
        if (loaded < 10) {
            printf("연습생 정보가 10명 이상 필요합니다. 현재 %d명\n", loaded);
            printf("기본 알파벳 ID 사용으로 진행합니다.\n");
            useFile = 0;
        }
    }

    // 세로줄 수 입력 4~6 제한
    do {
        printf("세로 줄 수 (4~6): ");
        scanf("%d", &numPeople);
        if (numPeople < 4 || numPeople > 6)
            printf("4에서 6 사이의 값을 입력하세요.\n");
    } while (numPeople < 4 || numPeople > 6);

    // 연습생 ID 또는 닉네임 선정
    char traineeIDs[6][2];   // 최대 6명까지
    int selectedIndices[6];  // 파일에서 무작위 선택 시 인덱스 저장

    if (useFile) {
        selectRandomTrainees(numPeople, selectedIndices);
        printf("선택된 연습생 닉네임:\n");
        for (int i = 0; i < numPeople; i++) {
            printf("%c: %s\n", 'A' + i, trainees[selectedIndices[i]].nickname);
        }
    } else {
        generateTraineeIDs(numPeople, traineeIDs);
        printf("연습생 ID: ");
        for (int i = 0; i < numPeople; i++)
            printf("%s ", traineeIDs[i]);
        printf("\n");
    }

    // 미디어 무작위 선택 (numPeople개)
    Media selectedMedia[6];
    int mediaIndices[MAX_MEDIA] = {0};
    for (int i = 0; i < MAX_MEDIA; i++) mediaIndices[i] = i;

    // 미디어 인덱스 섞기 (Fisher-Yates)
    for (int i = MAX_MEDIA - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = mediaIndices[i];
        mediaIndices[i] = mediaIndices[j];
        mediaIndices[j] = tmp;
    }
    for (int i = 0; i < numPeople; i++) {
        selectedMedia[i] = mediaList[mediaIndices[i]];
    }

    // 가로줄 수 입력
    int numHorizontal;
    printf("가로 줄 수 입력: ");
    scanf("%d", &numHorizontal);

    // 가로줄 배열: 각 줄마다 (시작, 끝) 세로줄 알파벳 저장
    char ladder[numHorizontal][2];
    ladderGame(numPeople, numHorizontal, ladder);

    // 사다리 진행
    int resultPositions[6];  // 결과 위치 저장
    runLadder(numPeople, numHorizontal, ladder, resultPositions);

    // 결과 출력
    printf("\n===== 사다리 결과 =====\n");
    for (int i = 0; i < numPeople; i++) {
        int finalPos = resultPositions[i];
        if (useFile) {
            printf("%c: %s -> %s\n", 'A' + i, trainees[selectedIndices[i]].nickname, selectedMedia[finalPos].name);
        } else {
            printf("%c -> %s\n", traineeIDs[i][0], selectedMedia[finalPos].name);
        }
    }
    printf("=======================\n");
}
