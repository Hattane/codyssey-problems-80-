#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_POSES 100

typedef struct Pose {
    char name[50];
    char description[256];
} Pose;

typedef struct Node {
    Pose pose;
    struct Node *prev;
    struct Node *next;
} Node;

// 포즈 리스트 로딩 함수
int loadPoses(const char *filename, Pose *poses, int maxPoses) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open pose file");
        return -1;
    }
    int count = 0;
    while (count < maxPoses && fscanf(fp, "%49s %[^\n]", poses[count].name, poses[count].description) == 2) {
        count++;
    }
    fclose(fp);
    return count;
}

// 노드 생성 함수
Node* createNode(const Pose *pose) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;
    node->pose = *pose;
    node->prev = node->next = NULL;
    return node;
}

// 이중 연결 리스트에 노드 추가 (맨 뒤)
void appendNode(Node **head, Node **tail, Node *newNode) {
    if (*head == NULL) {
        *head = *tail = newNode;
    } else {
        (*tail)->next = newNode;
        newNode->prev = *tail;
        *tail = newNode;
    }
}

// 랜덤 중복 없는 포즈 선택 후 리스트 생성
Node* selectRandomPoses(Pose *poses, int totalPoses, int selectCount) {
    if (selectCount > totalPoses) return NULL;

    int *selected = calloc(totalPoses, sizeof(int));
    if (!selected) return NULL;

    Node *head = NULL, *tail = NULL;
    srand((unsigned)time(NULL));
    int chosen = 0;
    while (chosen < selectCount) {
        int idx = rand() % totalPoses;
        if (!selected[idx]) {
            selected[idx] = 1;
            Node *node = createNode(&poses[idx]);
            if (!node) {
                // 메모리 해제 처리 생략
                free(selected);
                return NULL;
            }
            appendNode(&head, &tail, node);
            chosen++;
        }
    }
    free(selected);
    return head; // tail 포인터는 필요 시 따로 저장
}

// 포즈 출력 함수
void printPose(const Pose *pose) {
    printf("포즈 이름: %s\n", pose->name);
    printf("설명: %s\n\n", pose->description);
}

// 소문자 변환 함수
void toLowerStr(char *str) {
    for (; *str; ++str) *str = (char)tolower(*str);
}

// 포즈 연습 함수
void practicePoses(Node *head) {
    if (!head) return;
    Node *current;
    char input[10];
    int round;

    for (round = 0; round < 2; round++) {
        printf("=== 연습 라운드 %d ===\n", round + 1);
        current = head;
        while (current) {
            printPose(&current->pose);
            printf("이 포즈를 했나요? (yes/no): ");
            if (fgets(input, sizeof(input), stdin) == NULL) return;

            // 입력 정리
            input[strcspn(input, "\n")] = 0;
            toLowerStr(input);

            while (strcmp(input, "yes") != 0 && strcmp(input, "no") != 0) {
                printf("잘못된 입력입니다. yes 또는 no를 입력하세요: ");
                if (fgets(input, sizeof(input), stdin) == NULL) return;
                input[strcspn(input, "\n")] = 0;
                toLowerStr(input);
            }
            current = current->next;
        }
    }
    printf("포즈 연습이 끝났습니다.\n");
}

// 리스트 메모리 해제
void freePosesList(Node *head) {
    Node *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

// (보너스) 원형 이중 연결 리스트 변환 함수
void makeCircularList(Node **head, Node **tail) {
    if (!*head || !*tail) return;
    (*tail)->next = *head;
    (*head)->prev = *tail;
}

// doPosePractice 함수(메뉴에서 호출할 함수 예시)
void takePose() {
    Pose poses[MAX_POSES];
    int totalPoses = loadPoses("pose.txt", poses, MAX_POSES);
    if (totalPoses < 3) {
        printf("포즈 데이터가 부족합니다.\n");
        return;
    }

    int practiceCount;
    printf("연습할 포즈 수를 입력하세요 (3~5): ");
    scanf("%d", &practiceCount);
    while (practiceCount < 3 || practiceCount > 5) {
        printf("3에서 5 사이의 숫자를 입력하세요: ");
        scanf("%d", &practiceCount);
    }
    getchar(); // 버퍼 정리

    Node *head = selectRandomPoses(poses, totalPoses, practiceCount);
    if (!head) {
        printf("포즈 리스트 생성 실패\n");
        return;
    }

    // 보너스: 원형 리스트 만들기 위해 tail 찾기
    Node *tail = head;
    while (tail->next) tail = tail->next;
    makeCircularList(&head, &tail);

    // 포즈 연습 실행
    practicePoses(head);

    // 메모리 해제 (원형 리스트일 때 주의 필요 - 임시로 비원형으로 변환)
    tail->next = NULL;
    head->prev = NULL;
    freePosesList(head);
}
