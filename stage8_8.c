#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debut.h"

// 동적 노드 생성 함수
static MemoryNode* createNode(const char* keyword, const char* content) {
    MemoryNode* node = (MemoryNode*)malloc(sizeof(MemoryNode));
    if (!node) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(EXIT_FAILURE);
    }
    strncpy(node->keyword, keyword, MAX_KEYWORD_LEN - 1);
    node->keyword[MAX_KEYWORD_LEN - 1] = '\0';
    strncpy(node->content, content, MAX_CONTENT_LEN - 1);
    node->content[MAX_CONTENT_LEN - 1] = '\0';
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 사용자 입력으로 추억 배열 생성
static void inputMemories(MemoryNode*** memories, int* count) {
    char keyword[MAX_KEYWORD_LEN];
    char content[MAX_CONTENT_LEN];
    int capacity = 10;

    *memories = (MemoryNode**)malloc(capacity * sizeof(MemoryNode*));
    *count = 0;

    printf("추억 키워드와 내용을 입력하세요 (종료는 빈 키워드 입력):\n");

    while (1) {
        printf("키워드: ");
        if (!fgets(keyword, sizeof(keyword), stdin)) break;
        keyword[strcspn(keyword, "\n")] = 0; // 개행 제거
        if (keyword[0] == '\0') break;

        printf("내용: ");
        if (!fgets(content, sizeof(content), stdin)) break;
        content[strcspn(content, "\n")] = 0;

        if (*count >= capacity) {
            capacity *= 2;
            *memories = (MemoryNode**)realloc(*memories, capacity * sizeof(MemoryNode*));
            if (!*memories) {
                fprintf(stderr, "메모리 재할당 실패\n");
                exit(EXIT_FAILURE);
            }
        }
        (*memories)[*count] = createNode(keyword, content);
        (*count)++;
    }
}

// 추억 목록 출력
static void printMemories(MemoryNode* memories[], int count) {
    printf("\n입력된 추억 목록:\n");
    for (int i = 0; i < count; i++) {
        printf("[%d] 키워드: %s\n", i + 1, memories[i]->keyword);
        printf("    내용: %s\n", memories[i]->content);
    }
}

// 재귀로 키워드 검색
static MemoryNode* findNode(MemoryNode* root, const char* keyword) {
    if (!root) return NULL;
    if (strcmp(root->keyword, keyword) == 0) return root;
    MemoryNode* found = findNode(root->left, keyword);
    if (found) return found;
    return findNode(root->right, keyword);
}

// 마인드맵 재귀 출력 (들여쓰기)
static void printMindmap(MemoryNode* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("- %s: %s\n", node->keyword, node->content);
    printMindmap(node->left, depth + 1);
    printMindmap(node->right, depth + 1);
}

// 부모 노드에 자식 연결 (좌우 한 개씩 최대 2개)
static int connectNodes(MemoryNode* parent, MemoryNode* child) {
    if (!parent || !child) return 0;
    if (parent->left == NULL) {
        parent->left = child;
        return 1;
    }
    else if (parent->right == NULL) {
        parent->right = child;
        return 1;
    }
    else {
        printf("부모 노드 '%s'에 이미 자식 노드가 2개 연결되어 있습니다.\n", parent->keyword);
        return 0;
    }
}

// 부모 찾기 도우미 재귀 함수
static void findParent(MemoryNode* current, MemoryNode* target, MemoryNode** result) {
    if (!current || *result != NULL) return;
    if (current->left == target || current->right == target) {
        *result = current;
        return;
    }
    findParent(current->left, target, result);
    findParent(current->right, target, result);
}

// 노드 위치 이동 (원래 부모와 연결 해제 후 새 부모에 연결)
static int moveNode(MemoryNode* root, MemoryNode* nodeToMove, MemoryNode* newParent) {
    if (!root || !nodeToMove || !newParent) return 0;
    if (nodeToMove == root) {
        printf("루트 노드는 이동할 수 없습니다.\n");
        return 0;
    }

    MemoryNode* parent = NULL;
    findParent(root, nodeToMove, &parent);
    if (!parent) {
        printf("노드의 부모를 찾을 수 없습니다.\n");
        return 0;
    }

    // 부모 연결 해제
    if (parent->left == nodeToMove) parent->left = NULL;
    else if (parent->right == nodeToMove) parent->right = NULL;

    // 새 부모에 연결 시도
    return connectNodes(newParent, nodeToMove);
}

void reminisceMindmap(void) {
    MemoryNode** memories = NULL;
    int count = 0;

    inputMemories(&memories, &count);

    if (count == 0) {
        printf("입력된 추억이 없습니다.\n");
        free(memories);
        return;
    }

    printMemories(memories, count);

    int rootIndex = -1;
    while (1) {
        printf("\n루트 노드(시작 추억) 번호를 선택하세요 (1~%d): ", count);
        if (scanf("%d", &rootIndex) != 1) {
            while (getchar() != '\n');
            printf("잘못된 입력입니다.\n");
            continue;
        }
        if (rootIndex >= 1 && rootIndex <= count) break;
        printf("범위 내 번호를 입력하세요.\n");
    }

    MemoryNode* root = memories[rootIndex - 1];

    printf("\n루트 노드 제외, 각 노드를 부모 노드에 연결하세요.\n");
    for (int i = 0; i < count; i++) {
        if (i == rootIndex - 1) continue;
        int parentIndex = -1;
        while (1) {
            printf("'%s'의 부모 노드 번호 (1~%d): ", memories[i]->keyword, count);
            if (scanf("%d", &parentIndex) != 1) {
                while (getchar() != '\n');
                printf("잘못된 입력입니다.\n");
                continue;
            }
            if (parentIndex >= 1 && parentIndex <= count) {
                MemoryNode* parent = memories[parentIndex - 1];
                if (connectNodes(parent, memories[i])) break;
                else printf("부모 노드에 연결 실패. 다른 부모 노드를 선택하세요.\n");
            }
            else {
                printf("범위 내 번호를 입력하세요.\n");
            }
        }
    }

    while (getchar() != '\n'); // 입력 버퍼 정리

    printf("\n=== 전체 추억 마인드 맵 ===\n");
    printMindmap(root, 0);

    // 검색 및 노드 이동 기능
    while (1) {
        printf("\n1. 추억 검색\n2. 노드 위치 이동\n0. 종료\n선택: ");
        int sel;
        if (scanf("%d", &sel) != 1) {
            while (getchar() != '\n');
            printf("잘못된 입력입니다.\n");
            continue;
        }
        while (getchar() != '\n');

        if (sel == 0) break;
        else if (sel == 1) {
            char searchKey[MAX_KEYWORD_LEN];
            printf("검색할 키워드 입력: ");
            if (!fgets(searchKey, sizeof(searchKey), stdin)) continue;
            searchKey[strcspn(searchKey, "\n")] = 0;
            MemoryNode* found = findNode(root, searchKey);
            if (found) {
                printf("검색 결과: %s - %s\n", found->keyword, found->content);
            }
            else {
                printf("'%s' 키워드를 가진 추억이 없습니다.\n", searchKey);
            }
        }
        else if (sel == 2) {
            char moveKey[MAX_KEYWORD_LEN];
            char newParentKey[MAX_KEYWORD_LEN];
            printf("이동할 노드 키워드: ");
            if (!fgets(moveKey, sizeof(moveKey), stdin)) continue;
            moveKey[strcspn(moveKey, "\n")] = 0;

            printf("새 부모 노드 키워드: ");
            if (!fgets(newParentKey, sizeof(newParentKey), stdin)) continue;
            newParentKey[strcspn(newParentKey, "\n")] = 0;

            MemoryNode* nodeToMove = findNode(root, moveKey);
            MemoryNode* newParent = findNode(root, newParentKey);

            if (!nodeToMove) {
                printf("'%s' 노드를 찾을 수 없습니다.\n", moveKey);
                continue;
            }
            if (!newParent) {
                printf("'%s' 부모 노드를 찾을 수 없습니다.\n", newParentKey);
                continue;
            }
            if (moveNode(root, nodeToMove, newParent)) {
                printf("노드 이동 성공\n");
                printf("\n=== 전체 추억 마인드 맵 ===\n");
                printMindmap(root, 0);
            }
            else {
                printf("노드 이동 실패\n");
            }
        }
        else {
            printf("잘못된 선택입니다.\n");
        }
    }

    // 메모리 해제(트리 전체 해제 생략)
    free(memories);
}
