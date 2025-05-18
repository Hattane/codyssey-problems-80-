#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_TYPE 30
#define MAX_USAGE 100
#define MAX_QUEUE_SIZE 10

typedef struct {
    char name[MAX_NAME];
    char type[MAX_TYPE];
    char usage[MAX_USAGE];
} Accessory;

typedef struct {
    Accessory* queue[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int size;
} CircularQueue;

CircularQueue* createQueue() {
    CircularQueue* q = (CircularQueue*)malloc(sizeof(CircularQueue));
    q->front = 0;
    q->rear = 0;
    q->size = 0;
    return q;
}

int isFull(CircularQueue* q) {
    return q->size == MAX_QUEUE_SIZE;
}

int isEmpty(CircularQueue* q) {
    return q->size == 0;
}

void enqueue(CircularQueue* q, Accessory* acc) {
    if (isFull(q)) {
        printf("⚠️ 큐가 가득 찼습니다. 더 이상 추가할 수 없습니다.\n");
        return;
    }
    q->queue[q->rear] = acc;
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->size++;
}

void printQueue(CircularQueue* q) {
    if (isEmpty(q)) {
        printf("📭 큐가 비어 있습니다.\n");
        return;
    }
    printf("\n🧢 선택한 액세서리 목록:\n");
    for (int i = 0; i < q->size; i++) {
        int idx = (q->front + i) % MAX_QUEUE_SIZE;
        Accessory* acc = q->queue[idx];
        printf("- %s (%s): %s\n", acc->name, acc->type, acc->usage);
    }
}

int loadAccessories(const char* filename, Accessory** list) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("파일 열기 실패");
        return 0;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) count++;

    rewind(fp);
    *list = (Accessory*)malloc(sizeof(Accessory) * count);

    int idx = 0;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%[^"]", (*list)[idx].name, (*list)[idx].type, (*list)[idx].usage);
        idx++;
    }
    fclose(fp);
    return count;
}

void applyAccessory() {
    Accessory* allAccessories = NULL;
    int total = loadAccessories("accessory.txt", &allAccessories);
    if (total == 0) return;

    CircularQueue* queue = createQueue();

    printf("👜 사용 가능한 액세서리 목록 (%d개):\n", total);
    for (int i = 0; i < total; i++) {
        printf("%d. %s (%s) - %s\n", i + 1, allAccessories[i].name, allAccessories[i].type, allAccessories[i].usage);
    }

    int selectCount;
    do {
        printf("\n선택할 액세서리 개수 (4~6개): ");
        scanf("%d", &selectCount);
    } while (selectCount < 4 || selectCount > 6);

    for (int i = 0; i < selectCount; i++) {
        int index;
        do {
            printf("추가할 액세서리 번호 #%d: ", i + 1);
            scanf("%d", &index);
        } while (index < 1 || index > total);
        enqueue(queue, &allAccessories[index - 1]);
    }

    printf("\n✅ 액세서리 큐가 구성되었습니다!\n");
    printQueue(queue);

    free(queue);
    free(allAccessories);
}

// 테스트용 main 함수
int main() {
    applyAccessory();
    return 0;
}
