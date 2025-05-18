#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "debut.h"

static Fan fans[NUM_FANS];

// 유클리드 거리 계산 함수 (3차원 특징 벡터 기준)
static double distance(double* a, double* b, int dim) {
    double sum = 0.0;
    for (int i = 0; i < dim; i++) {
        double diff = a[i] - b[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

// 팬 특징 벡터 생성 (팬 유형별로 임의로 특징 부여)
static void initFanFeatures(Fan* fan) {
    // 찐팬: (1, 0, 0), 가짜팬: (0, 1, 0), 안티팬: (0, 0, 1)
    for (int i = 0; i < 3; i++) fan->features[i] = 0.0;
    fan->features[fan->type] = 1.0;
}

// K-means 클러스터링 수행
static void kMeansClustering(Fan fans[], int n, int k) {
    double centroids[k][3];     // 클러스터 중심 (3차원)
    int clusters[k];            // 각 클러스터에 속한 팬 수
    int changed;
    int maxIter = 100;

    // 1. 초기 중심: 첫 k개의 팬 특징을 중심으로 설정
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < 3; j++)
            centroids[i][j] = fans[i].features[j];
    }

    for (int iter = 0; iter < maxIter; iter++) {
        changed = 0;

        // 2. 각 팬을 가장 가까운 중심 클러스터에 할당
        for (int i = 0; i < n; i++) {
            double minDist = distance(fans[i].features, centroids[0], 3);
            int minIndex = 0;
            for (int c = 1; c < k; c++) {
                double dist = distance(fans[i].features, centroids[c], 3);
                if (dist < minDist) {
                    minDist = dist;
                    minIndex = c;
                }
            }
            if (fans[i].cluster != minIndex) {
                fans[i].cluster = minIndex;
                changed = 1;
            }
        }

        if (!changed) break; // 수렴

        // 3. 중심 재계산
        // 초기화
        for (int c = 0; c < k; c++) {
            clusters[c] = 0;
            for (int d = 0; d < 3; d++)
                centroids[c][d] = 0.0;
        }
        // 합산
        for (int i = 0; i < n; i++) {
            int c = fans[i].cluster;
            for (int d = 0; d < 3; d++) {
                centroids[c][d] += fans[i].features[d];
            }
            clusters[c]++;
        }
        // 평균
        for (int c = 0; c < k; c++) {
            if (clusters[c] == 0) continue; // 빈 클러스터 방지
            for (int d = 0; d < 3; d++)
                centroids[c][d] /= clusters[c];
        }
    }
}

// 팬 및 클러스터 결과 출력
static void printClusters(Fan fans[], int n) {
    const char* clusterNames[NUM_CLUSTERS] = {"찐팬", "가짜팬", "안티팬"};

    for (int c = 0; c < NUM_CLUSTERS; c++) {
        printf("=== 클러스터 %d (%s) ===\n", c+1, clusterNames[c]);
        for (int i = 0; i < n; i++) {
            if (fans[i].cluster == c) {
                printf("팬 ID: %d, 초기 유형: %s\n", fans[i].id, clusterNames[fans[i].type]);
            }
        }
        printf("\n");
    }
}

void classifyFan(void) {
    srand((unsigned int)time(NULL));

    // 1. 팬 랜덤 유형 할당 및 특징 생성
    for (int i = 0; i < NUM_FANS; i++) {
        fans[i].id = i + 1;
        fans[i].type = rand() % 3;  // 0~2 랜덤
        fans[i].cluster = -1;
        initFanFeatures(&fans[i]);
    }

    // 2. K-means 클러스터링 수행
    kMeansClustering(fans, NUM_FANS, NUM_CLUSTERS);

    // 3. 결과 출력 (클러스터별 팬 목록 포함)
    printClusters(fans, NUM_FANS);
}
