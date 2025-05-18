#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LookNode {
    char expression[64];
    char description[256];
    struct LookNode *prev;
    struct LookNode *next;
} LookNode;

// 파일에서 표정 데이터를 읽어 이중 연결 리스트 생성
LookNode* loadLooks(const char* filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open look.txt");
        return NULL;
    }

    LookNode *head = NULL, *tail = NULL;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        // 한 줄에서 개행문자 제거
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        // 표정 명 읽기
        char expression[64];
        strncpy(expression, line, sizeof(expression));
        expression[sizeof(expression)-1] = '\0';

        // 다음 줄에서 설명 읽기
        if (!fgets(line, sizeof(line), fp)) break;
        line[strcspn(line, "\r\n")] = 0;
        char description[256];
        strncpy(description, line, sizeof(description));
        description[sizeof(description)-1] = '\0';

        LookNode *node = (LookNode*)malloc(sizeof(LookNode));
        if (!node) {
            perror("Memory allocation failed");
            fclose(fp);
            // 메모리 정리 함수 호출 가능
            return head;
        }
        strcpy(node->expression, expression);
        strcpy(node->description, description);
        node->prev = tail;
        node->next = NULL;

        if (tail) tail->next = node;
        else head = node;

        tail = node;
    }

    fclose(fp);
    return head;
}

// 리스트 전체 출력
void printLooks(LookNode *head) {
    LookNode *cur = head;
    int i = 1;
    while (cur) {
        printf("%d. %s\n%s\n\n", i++, cur->expression, cur->description);
        cur = cur->next;
    }
}

// 키워드로 설명 검색 후 출력
void searchLook(LookNode *head, const char *keyword) {
    LookNode *cur = head;
    int found = 0;
    while (cur) {
        if (strstr(cur->description, keyword) != NULL) {
            printf("Found:\n%s\n%s\n\n", cur->expression, cur->description);
            found = 1;
        }
        cur = cur->next;
    }
    if (!found) {
        printf("No expression found with keyword \"%s\".\n", keyword);
    }
}

// 인접 노드 교환 (pos부터 pos+1번째 노드 교환)
// pos는 1부터 시작, 리스트 노드 개수보다 작아야 함
void swapAdjacent(LookNode **head, int pos) {
    if (!head || !(*head) || pos < 1) return;

    LookNode *cur = *head;
    int i = 1;

    // pos 위치 노드 찾기
    while (cur && i < pos) {
        cur = cur->next;
        i++;
    }

    // cur와 cur->next가 존재해야 교환 가능
    if (!cur || !cur->next) {
        printf("Swap position invalid or out of range.\n");
        return;
    }

    LookNode *first = cur;
    LookNode *second = cur->next;
    LookNode *prev = first->prev;
    LookNode *next = second->next;

    // 교환
    if (prev) prev->next = second;
    else *head = second;

    second->prev = prev;
    second->next = first;

    first->prev = second;
    first->next = next;

    if (next) next->prev = first;

    printf("Swapped expressions at positions %d and %d.\n", pos, pos + 1);
    printLooks(*head);
}

// 리스트 메모리 해제
void freeLooks(LookNode *head) {
    LookNode *cur = head;
    while (cur) {
        LookNode *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

// 메뉴 함수 - haveALook()
void haveALook(void) {
    LookNode *head = loadLooks("look.txt");
    if (!head) {
        printf("Failed to load expressions.\n");
        return;
    }

    int choice;
    char keyword[256];
    int pos;

    while (1) {
        printf("\n[표정 관리 메뉴]\n");
        printf("1. 전체 표정 출력\n");
        printf("2. 키워드로 표정 검색\n");
        printf("3. 인접 표정 교환\n");
        printf("4. 종료\n");
        printf("선택: ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                printLooks(head);
                break;
            case 2:
                printf("검색할 키워드 입력: ");
                scanf(" %255[^\n]", keyword);
                searchLook(head, keyword);
                break;
            case 3:
                printf("교환할 첫 번째 표정 위치 입력 (1부터 시작): ");
                if (scanf("%d", &pos) != 1) {
                    printf("잘못된 입력입니다.\n");
                    break;
                }
                swapAdjacent(&head, pos);
                break;
            case 4:
                freeLooks(head);
                return;
            default:
                printf("잘못된 선택입니다.\n");
        }
    }

    freeLooks(head);
}
