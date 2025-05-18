#include <stdio.h>
#include <stdlib.h>

#define MAX_LIGHTS 100

typedef struct {
    int number;     // 조명 번호
    int x1, y1;     // 직사각형 한 점
    int x2, y2;     // 직사각형 반대 점
} Light;

Light lights[MAX_LIGHTS];
int lightCount = 5;

// 좌표의 최소값 반환
int min(int a, int b) { return (a < b) ? a : b; }
// 좌표의 최대값 반환
int max(int a, int b) { return (a > b) ? a : b; }

// 조명 좌표 범위 출력 함수
void printLightingRange() {
    if (lightCount == 0) {
        printf("조명 정보가 없습니다.\n");
        return;
    }
    int minX = lights[0].x1, maxX = lights[0].x1;
    int minY = lights[0].y1, maxY = lights[0].y1;

    for (int i = 0; i < lightCount; i++) {
        minX = min(minX, min(lights[i].x1, lights[i].x2));
        maxX = max(maxX, max(lights[i].x1, lights[i].x2));
        minY = min(minY, min(lights[i].y1, lights[i].y2));
        maxY = max(maxY, max(lights[i].y1, lights[i].y2));
    }
    printf("위치 입력 범위: (%d,%d) ~ (%d,%d)\n", minX, minY, maxX, maxY);
}

// 점이 직사각형 영역 안에 있는지 확인
int isInside(Light* light, int x, int y) {
    int left = min(light->x1, light->x2);
    int right = max(light->x1, light->x2);
    int bottom = min(light->y1, light->y2);
    int top = max(light->y1, light->y2);

    return (x >= left && x <= right && y >= bottom && y <= top);
}

// 조명 정보 추가 입력 함수 (보너스)
void addLightingInfo() {
    if (lightCount >= MAX_LIGHTS) {
        printf("더 이상 조명을 추가할 수 없습니다.\n");
        return;
    }
    printf("조명 정보를 추가하시겠습니까? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    if (choice != 'y' && choice != 'Y') return;

    int x1, y1, x2, y2;
    printf("조명 좌표를 입력하세요 (x1,y1,x2,y2): ");
    if (scanf("%d,%d,%d,%d", &x1, &y1, &x2, &y2) != 4) {
        printf("잘못된 입력입니다.\n");
        while(getchar() != '\n'); // 입력 버퍼 정리
        return;
    }
    lights[lightCount].number = lightCount + 1;
    lights[lightCount].x1 = x1;
    lights[lightCount].y1 = y1;
    lights[lightCount].x2 = x2;
    lights[lightCount].y2 = y2;
    lightCount++;
    printf("조명 %d이(가) 추가되었습니다.\n", lightCount);
}

// 조명 정보 초기화
void initLights() {
    lights[0] = (Light){1, 1, 8, 4, 5};
    lights[1] = (Light){2, 3, 2, 13, 6};
    lights[2] = (Light){3, 7, 10, 12, 7};
    lights[3] = (Light){4, 8, 4, 5, 8};
    lights[4] = (Light){5, 9, 1, 2, 5};
}

// 메인 조명 처리 함수
void handleLighting() {
    initLights();

    while(1) {
        printLightingRange();

        int x, y;
        printf("현재 위치를 입력하세요 (X,Y): ");
        if (scanf("%d,%d", &x, &y) != 2) {
            printf("잘못된 입력입니다. 종료합니다.\n");
            break;
        }

        printf("위치 (%d,%d)가 영향을 받는 조명:\n", x, y);
        int found = 0;
        for (int i = 0; i < lightCount; i++) {
            if (isInside(&lights[i], x, y)) {
                printf("조명 %d: (%d,%d) ~ (%d,%d)\n",
                    lights[i].number,
                    lights[i].x1, lights[i].y1,
                    lights[i].x2, lights[i].y2);
                found = 1;
            }
        }
        if (!found) {
            printf("해당 위치에 영향을 주는 조명이 없습니다.\n");
        }

        addLightingInfo();

        printf("계속 하시겠습니까? (y/n): ");
        char cont;
        scanf(" %c", &cont);
        if (cont != 'y' && cont != 'Y') break;
    }
}

// 예제 메인 함수
int main() {
    handleLighting();
    return 0;
}
