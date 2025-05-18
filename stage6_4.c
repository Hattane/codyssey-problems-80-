#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STYLE 100
#define MAX_NAME 100

typedef struct {
    char gender[10];
    char name[MAX_NAME];
} HairStyle;

typedef struct {
    char name[MAX_NAME];
    int external_score;
    int internal_score;
    double total_score;
} Suitability;

HairStyle styles[MAX_STYLE];
int totalStyles = 0;

// 파일에서 헤어 스타일 읽기
void loadHairStyles(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("hair_style.txt 파일을 열 수 없습니다");
        exit(1);
    }

    while (fscanf(fp, "%[^,],%[^\n]\n", styles[totalStyles].gender, styles[totalStyles].name) == 2) {
        totalStyles++;
        if (totalStyles >= MAX_STYLE) break;
    }

    fclose(fp);
}

// 버블 정렬
void bubbleSort(Suitability arr[], int n) {
    for (int i = 0; i < n-1; ++i)
        for (int j = 0; j < n-i-1; ++j)
            if (arr[j].total_score < arr[j+1].total_score) {
                Suitability temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
}

// 삽입 정렬
void insertionSort(Suitability arr[], int n) {
    for (int i = 1; i < n; ++i) {
        Suitability key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].total_score < key.total_score) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

// 퀵 정렬
void quickSort(Suitability arr[], int left, int right) {
    if (left >= right) return;

    double pivot = arr[(left + right) / 2].total_score;
    int i = left, j = right;

    while (i <= j) {
        while (arr[i].total_score > pivot) i++;
        while (arr[j].total_score < pivot) j--;
        if (i <= j) {
            Suitability temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++; j--;
        }
    }

    if (left < j) quickSort(arr, left, j);
    if (i < right) quickSort(arr, i, right);
}

void testHairStyles() {
    char gender[10];
    printf("성별을 입력하세요 (man / woman): ");
    scanf("%s", gender);

    int n;
    do {
        printf("무작위로 선택할 헤어스타일 개수 (5~10): ");
        scanf("%d", &n);
    } while (n < 5 || n > 10);

    // 헤어스타일 필터링
    HairStyle filtered[MAX_STYLE];
    int count = 0;
    for (int i = 0; i < totalStyles; ++i) {
        if (strcmp(styles[i].gender, gender) == 0) {
            filtered[count++] = styles[i];
        }
    }

    if (count < n) {
        printf("헤어스타일이 부족합니다.\n");
        return;
    }

    Suitability selected[10];
    srand(time(NULL));
    int used[100] = {0};
    for (int i = 0; i < n; ++i) {
        int idx;
        do {
            idx = rand() % count;
        } while (used[idx]);
        used[idx] = 1;

        int external = 50 + rand() % 51;
        int internal = 50 + rand() % 51;
        double suitability = external * 0.7 + internal * 0.3;

        strcpy(selected[i].name, filtered[idx].name);
        selected[i].external_score = external;
        selected[i].internal_score = internal;
        selected[i].total_score = suitability;
    }

    printf("\n[정렬 방식 선택]\n1. 버블 정렬\n2. 삽입 정렬\n3. 퀵 정렬\n입력: ");
    int sortMethod;
    scanf("%d", &sortMethod);

    switch (sortMethod) {
        case 1:
            bubbleSort(selected, n);
            break;
        case 2:
            insertionSort(selected, n);
            break;
        case 3:
            quickSort(selected, 0, n - 1);
            break;
        default:
            printf("잘못된 선택입니다. 퀵 정렬을 사용합니다.\n");
            quickSort(selected, 0, n - 1);
    }

    printf("\n[적합도 평가 결과]\n");
    for (int i = 0; i < n; ++i) {
        printf("헤어스타일: %s, 타인: %d, 자기: %d, 적합도: %.2f\n",
               selected[i].name,
               selected[i].external_score,
               selected[i].internal_score,
               selected[i].total_score);
    }
}
