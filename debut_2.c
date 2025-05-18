#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct City {
    char name[20];
    int x, y;
    struct City* next;
} City;

#define CITY_COUNT 5
#define MID_CITY_COUNT 3
#define MAX_PATHS 6

// 도시 정보
City cities[CITY_COUNT] = {
    {"서울", 12697, 3756, NULL},
    {"구미", 12834, 3611, NULL},
    {"대전", 12738, 3635, NULL},
    {"춘천", 12773, 3788, NULL},
    {"부산", 12907, 3518, NULL}
};

// 대전, 구미, 춘천 인덱스 (순서 중요X)
int midCities[MID_CITY_COUNT] = {2, 1, 3}; // 대전(2), 구미(1), 춘천(3)

// 경로와 총 거리 저장 구조체
typedef struct {
    City* pathHead;    // 경로 연결 리스트 머리
    double totalDist;  // 총 이동 거리
} PathInfo;

PathInfo paths[MAX_PATHS];
int pathCount = 0;

// 함수 선언
double calculateDistance(City* a, City* b);
void swap(int* a, int* b);
void permute(int* arr, int l, int r);
City* createPathList(int order[]);
void freePathList(City* head);
void printPathsSorted(void);

int main() {
    printf("=== 뮤직 비디오 촬영 최적 경로 ===\n");
    // 3개 중간 도시 순열 생성 및 경로 생성
    permute(midCities, 0, MID_CITY_COUNT - 1);
    // 결과 출력
    printPathsSorted();

    // 할당된 경로 리스트 메모리 해제
    for (int i = 0; i < pathCount; i++) {
        freePathList(paths[i].pathHead);
    }

    return 0;
}

// 두 도시 사이 거리 계산 함수
double calculateDistance(City* a, City* b) {
    double dx = (double)(a->x - b->x);
    double dy = (double)(a->y - b->y);
    return sqrt(dx*dx + dy*dy);
}

// 배열 원소 스왑
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// 순열 생성 (대전, 구미, 춘천의 모든 순열 6가지)
void permute(int* arr, int l, int r) {
    if (l == r) {
        if (pathCount >= MAX_PATHS) return;
        // 순열로 경로 생성 및 저장
        City* pathList = createPathList(arr);
        if (!pathList) return;

        // 총 거리 계산
        double totalDist = 0.0;
        City* cur = pathList;
        while (cur->next != NULL) {
            totalDist += calculateDistance(cur, cur->next);
            cur = cur->next;
        }
        // 저장
        paths[pathCount].pathHead = pathList;
        paths[pathCount].totalDist = totalDist;
        pathCount++;
    } else {
        for (int i = l; i <= r; i++) {
            swap(&arr[l], &arr[i]);
            permute(arr, l + 1, r);
            swap(&arr[l], &arr[i]); // 복원
        }
    }
}

// 주어진 순서대로 경로 연결 리스트 생성 (서울 -> mid cities 순서 -> 부산)
City* createPathList(int order[]) {
    City* head = NULL;
    City* tail = NULL;

    // 출발지 서울 추가
    City* start = (City*)malloc(sizeof(City));
    if (!start) return NULL;
    *start = cities[0]; // 서울
    start->next = NULL;
    head = tail = start;

    // 중간 도시 순서대로 추가
    for (int i = 0; i < MID_CITY_COUNT; i++) {
        City* c = (City*)malloc(sizeof(City));
        if (!c) {
            freePathList(head);
            return NULL;
        }
        *c = cities[order[i]];
        c->next = NULL;
        tail->next = c;
        tail = c;
    }

    // 도착지 부산 추가
    City* end = (City*)malloc(sizeof(City));
    if (!end) {
        freePathList(head);
        return NULL;
    }
    *end = cities[4]; // 부산
    end->next = NULL;
    tail->next = end;
    tail = end;

    return head;
}

// 연결 리스트 메모리 해제
void freePathList(City* head) {
    City* cur = head;
    while (cur) {
        City* next = cur->next;
        free(cur);
        cur = next;
    }
}

// 거리 기준 오름차순 정렬 및 경로 출력
void printPathsSorted(void) {
    // 간단한 버블 정렬
    for (int i = 0; i < pathCount - 1; i++) {
        for (int j = 0; j < pathCount - 1 - i; j++) {
            if (paths[j].totalDist > paths[j + 1].totalDist) {
                PathInfo tmp = paths[j];
                paths[j] = paths[j + 1];
                paths[j + 1] = tmp;
            }
        }
    }

    for (int i = 0; i < pathCount; i++) {
        printf("%d. ", i + 1);
        // 경로 이름 출력
        City* cur = paths[i].pathHead;
        while (cur) {
            printf("%s", cur->name);
            if (cur->next) printf(" -> ");
            cur = cur->next;
        }
        printf("\n   ");

        // 구간별 거리 출력
        cur = paths[i].pathHead;
        while (cur && cur->next) {
            double dist = calculateDistance(cur, cur->next);
            printf("%.1f", dist);
            if (cur->next->next) printf(" + ");
            cur = cur->next;
        }
        printf(" : %.1f\n", paths[i].totalDist);
    }
}
