#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 200
#define MAX_CONTENT 100
#define MAX_COMPLETED 100

// ----------- STRUCT DEFINITIONS ------------

typedef struct {
    int id;
    char content[MAX_CONTENT];
} Bucket;

typedef struct {
    Bucket *buckets;
    int size;
    int capacity;
} BucketList;

typedef struct {
    Bucket completed[MAX_COMPLETED];
    int front;
    int rear;
    int count;
} CompletedQueue;

// ----------- QUEUE FUNCTIONS ------------

void initQueue(CompletedQueue *q) {
    q->front = 0;
    q->rear = 0;
    q->count = 0;
}

void enqueueCompleted(CompletedQueue *q, Bucket b) {
    if (q->count == MAX_COMPLETED) {
        printf("완료 버킷 큐가 가득 찼습니다.\n");
        return;
    }
    q->completed[q->rear] = b;
    q->rear = (q->rear + 1) % MAX_COMPLETED;
    q->count++;
}

// ----------- BUCKET LIST FUNCTIONS ------------

void initBucketList(BucketList *list) {
    list->size = 0;
    list->capacity = 10;
    list->buckets = malloc(sizeof(Bucket) * list->capacity);
    if (list->buckets == NULL) {
        perror("동적 메모리 할당 실패");
        exit(1);
    }
}

void resizeBucketList(BucketList *list) {
    list->capacity *= 2;
    list->buckets = realloc(list->buckets, sizeof(Bucket) * list->capacity);
    if (list->buckets == NULL) {
        perror("메모리 재할당 실패");
        exit(1);
    }
}

void loadBucketList(const char *filename, BucketList *list) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("버킷 파일 열기 실패");
        exit(1);
    }

    initBucketList(list);

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        if (list->size == list->capacity) {
            resizeBucketList(list);
        }

        Bucket b;
        char *token = strtok(line, ",");
        if (token) b.id = atoi(token);
        token = strtok(NULL, "\n");
        if (token) strncpy(b.content, token, MAX_CONTENT);

        list->buckets[list->size++] = b;
    }
    fclose(fp);
}

void printBucketList(BucketList *list) {
    printf("\n[버킷 리스트]\n");
    for (int i = 0; i < list->size; i++) {
        printf("%d: %s\n", list->buckets[i].id, list->buckets[i].content);
    }
}

int removeBucketByID(BucketList *list, int id, CompletedQueue *completed) {
    for (int i = 0; i < list->size; i++) {
        if (list->buckets[i].id == id) {
            enqueueCompleted(completed, list->buckets[i]);
            for (int j = i; j < list->size - 1; j++) {
                list->buckets[j] = list->buckets[j + 1];
            }
            list->size--;
            return 1;
        }
    }
    return 0;
}

void printCompletedBuckets(CompletedQueue *q) {
    printf("\n[완료된 버킷]\n");
    for (int i = 0, idx = q->front; i < q->count; i++, idx = (idx + 1) % MAX_COMPLETED) {
        printf("%d: %s\n", q->completed[idx].id, q->completed[idx].content);
    }
}

// ----------- BONUS: LINEAR SEARCH ------------

void searchBucketList(BucketList *list, const char *query) {
    printf("\n[검색 결과: \"%s\"]\n", query);
    int found = 0;
    for (int i = 0; i < list->size; i++) {
        if (strstr(list->buckets[i].content, query)) {
            printf("%d: %s\n", list->buckets[i].id, list->buckets[i].content);
            found = 1;
        }
    }
    if (!found) printf("검색 결과가 없습니다.\n");
}

// ----------- MAIN LOGIC ------------

void makeBucketList() {
    BucketList list;
    CompletedQueue completed;
    initQueue(&completed);

    loadBucketList("buckets.csv", &list);

    while (1) {
        printBucketList(&list);

        printf("\n완료한 버킷 ID를 입력하세요 (0: 종료, -1: 검색): ");
        int input;
        scanf("%d", &input);
        getchar(); // remove newline

        if (input == 0) break;
        else if (input == -1) {
            char keyword[MAX_CONTENT];
            printf("검색어를 입력하세요: ");
            fgets(keyword, sizeof(keyword), stdin);
            keyword[strcspn(keyword, "\n")] = '\0'; // remove newline
            searchBucketList(&list, keyword);
            continue;
        }

        if (!removeBucketByID(&list, input, &completed)) {
            printf("존재하지 않는 ID입니다.\n");
        }

        if (list.size == 0) {
            printf("모든 버킷을 완료했습니다!\n");
            break;
        }
    }

    printCompletedBuckets(&completed);

    if (list.size > 0) {
        printf("\n[남은 버킷]\n");
        for (int i = 0; i < list.size; i++) {
            printf("%d: %s\n", list.buckets[i].id, list.buckets[i].content);
        }
    }

    free(list.buckets);
}

// ----------- MENU STUB ------------

void showMenu() {
    printf("\n=== Codyssey Bucket List Menu ===\n");
    printf("G. 버킷리스트 관리\n");
    printf("Q. 종료\n");
    printf("메뉴 선택: ");
}

int main() {
    char choice;
    while (1) {
        showMenu();
        scanf(" %c", &choice);
        switch (choice) {
            case 'G':
                makeBucketList();
                break;
            case 'Q':
                printf("프로그램을 종료합니다.\n");
                return 0;
            default:
                printf("잘못된 선택입니다.\n");
        }
    }
}

