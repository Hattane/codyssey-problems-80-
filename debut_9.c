#include "debut.h"
#include <string.h>

// 노드 생성
Node* createNode(Reflection data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// 이진 탐색 트리에 삽입 (날짜 기준 오름차순)
Node* insertNode(Node* root, Reflection data) {
    if (root == NULL) {
        return createNode(data);
    }
    int cmp = strcmp(data.date, root->data.date);
    if (cmp < 0) {
        root->left = insertNode(root->left, data);
    } else if (cmp > 0) {
        root->right = insertNode(root->right, data);
    } else {
        printf("같은 날짜의 기록이 이미 존재합니다.\n");
    }
    return root;
}

// 날짜로 노드 탐색
Node* searchNode(Node* root, const char* date) {
    if (root == NULL) return NULL;
    int cmp = strcmp(date, root->data.date);
    if (cmp == 0) return root;
    else if (cmp < 0) return searchNode(root->left, date);
    else return searchNode(root->right, date);
}

// 최소값 노드 찾기 (삭제시 활용)
Node* findMinNode(Node* root) {
    Node* current = root;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// 노드 삭제 (날짜 기준)
Node* deleteNode(Node* root, const char* date) {
    if (root == NULL) return root;
    int cmp = strcmp(date, root->data.date);
    if (cmp < 0) {
        root->left = deleteNode(root->left, date);
    } else if (cmp > 0) {
        root->right = deleteNode(root->right, date);
    } else {
        // 삭제할 노드 발견
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        // 두 자식 노드가 있는 경우
        Node* temp = findMinNode(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data.date);
    }
    return root;
}

// 날짜 목록을 오름차순으로 출력 (중위 순회)
void inorderPrintDates(Node* root) {
    if (root == NULL) return;
    inorderPrintDates(root->left);
    printf("%s\n", root->data.date);
    inorderPrintDates(root->right);
}

// 자기 성찰 출력
void printReflection(Node* node) {
    if (!node) return;
    printf("공연 날짜: %s\n", node->data.date);
    printf("공연 장소: %s\n", node->data.location);
    printf("공연 명: %s\n", node->data.show_name);
    printf("자기 성찰 내용: %s\n", node->data.reflection);
    printf("감사의 글: %s\n", node->data.thanks);
}

// 트리 메모리 해제
void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// 보너스: 현재 월과 일치하는 공연의 모든 자기 성찰 출력
void printReflectionsByMonth(Node* root, const char* current_month) {
    if (root == NULL) return;
    printReflectionsByMonth(root->left, current_month);

    // 날짜: YYYYMMDD 중 MM = substring[4..5]
    if (strncmp(root->data.date + 4, current_month, 2) == 0) {
        printReflection(root);
        printf("------\n");
    }
    printReflectionsByMonth(root->right, current_month);
}
