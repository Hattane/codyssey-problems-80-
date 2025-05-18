#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CHARACTERS 8
#define SELECT_COUNT 4

typedef struct Character {
    char name[64];
    char description[256];
    int isNegative; // 1: 악당 or 사이코패스 등 부정적, 0: 긍정적
} Character;

typedef struct CharacterNode {
    Character data;
    int ratio; // 비율 (합 100)
    struct CharacterNode *prev;
    struct CharacterNode *next;
} CharacterNode;

// 파일에서 캐릭터 읽기
int loadCharacters(const char *filename, Character arr[], int max) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("character.txt 파일 열기 실패");
        return 0;
    }
    int count = 0;
    char line[256];
    while (count < max) {
        if (!fgets(line, sizeof(line), fp)) break;
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;
        strncpy(arr[count].name, line, sizeof(arr[count].name)-1);
        arr[count].name[sizeof(arr[count].name)-1] = '\0';

        if (!fgets(line, sizeof(line), fp)) break;
        line[strcspn(line, "\r\n")] = 0;
        strncpy(arr[count].description, line, sizeof(arr[count].description)-1);
        arr[count].description[sizeof(arr[count].description)-1] = '\0';

        if (!fgets(line, sizeof(line), fp)) break;
        arr[count].isNegative = atoi(line);

        count++;
    }
    fclose(fp);
    return count;
}

// 리스트 노드 생성
CharacterNode* createNode(const Character *c, int ratio) {
    CharacterNode *node = (CharacterNode*)malloc(sizeof(CharacterNode));
    if (!node) return NULL;
    node->data = *c;
    node->ratio = ratio;
    node->prev = node->next = NULL;
    return node;
}

// 이중 연결 리스트에 삽입 (비율 내림차순 정렬)
void insertSorted(CharacterNode **head, CharacterNode *newNode) {
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    CharacterNode *cur = *head;
    while (cur && cur->ratio >= newNode->ratio) {
        cur = cur->next;
    }
    if (cur == *head) {
        // 맨 앞 삽입
        newNode->next = *head;
        (*head)->prev = newNode;
        *head = newNode;
    } else if (cur == NULL) {
        // 맨 뒤 삽입
        CharacterNode *tail = *head;
        while (tail->next) tail = tail->next;
        tail->next = newNode;
        newNode->prev = tail;
    } else {
        // 중간 삽입
        newNode->next = cur;
        newNode->prev = cur->prev;
        cur->prev->next = newNode;
        cur->prev = newNode;
    }
}

// 리스트 출력
void printList(CharacterNode *head) {
    printf("\n[캐릭터 분석 결과]\n");
    CharacterNode *cur = head;
    while (cur) {
        printf("- %s (비율: %d%%)%s\n  설명: %s\n",
            cur->data.name, cur->ratio,
            (cur->data.isNegative ? " [부정적]" : ""),
            cur->data.description);
        cur = cur->next;
    }
}

// 리스트 메모리 해제
void freeList(CharacterNode *head) {
    CharacterNode *cur = head;
    while (cur) {
        CharacterNode *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

// 리스트에서 부정적 캐릭터(악당 또는 사이코패스) 찾기 및 제거
int removeNegative(CharacterNode **head) {
    int removed = 0;
    CharacterNode *cur = *head;
    while (cur) {
        if (cur->data.isNegative) {
            CharacterNode *toRemove = cur;
            // 연결 끊기
            if (toRemove->prev) toRemove->prev->next = toRemove->next;
            else *head = toRemove->next;
            if (toRemove->next) toRemove->next->prev = toRemove->prev;

            cur = toRemove->next;
            free(toRemove);
            removed = 1;
            // 문제 요구사항에 따르면 모든 부정적 캐릭터 제거, 혹은 1개만?  
            // 여기선 모두 제거
            continue;
        }
        cur = cur->next;
    }
    return removed;
}

// 선택된 4개 중 부정적 캐릭터가 포함되어 있는지 확인
int containsNegative(Character arr[], int indices[], int count) {
    for (int i = 0; i < count; i++) {
        if (arr[indices[i]].isNegative) return 1;
    }
    return 0;
}

// 4개 캐릭터 선택 (부정적 1개 반드시 포함)
void selectCharacters(Character arr[], int total, int indices[]) {
    int selected = 0;
    int tries = 0;
    while (1) {
        // 무작위 4개 인덱스 뽑기 (중복 없이)
        int picked[SELECT_COUNT] = {-1,-1,-1,-1};
        int count = 0;
        while (count < SELECT_COUNT) {
            int r = rand() % total;
            int duplicate = 0;
            for (int i=0; i<count; i++) {
                if (picked[i] == r) {
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate) {
                picked[count++] = r;
            }
        }
        // 부정적 캐릭터 포함 검사
        if (containsNegative(arr, picked, SELECT_COUNT)) {
            for (int i=0; i<SELECT_COUNT; i++) indices[i] = picked[i];
            break;
        }
        tries++;
        if (tries > 1000) { 
            // 무한루프 방지: 부정적 캐릭터가 없다면 그냥 선택
            for (int i=0; i<SELECT_COUNT; i++) indices[i] = i;
            break;
        }
    }
}

// 4개 비율 합 100 으로 랜덤 분배
void generateRatios(int ratios[], int count) {
    int total = 100;
    for (int i = 0; i < count - 1; i++) {
        // 남은 점수 중 랜덤 분배, 최소 1 이상 보장
        int maxVal = total - (count - 1 - i);
        ratios[i] = (rand() % maxVal) + 1;
        total -= ratios[i];
    }
    ratios[count-1] = total;
}

// 리스트 내 부정적 캐릭터 대체 (보너스)
void replaceNegative(CharacterNode **head, Character arr[], int total) {
    CharacterNode *cur = *head;
    while (cur) {
        if (cur->data.isNegative) {
            // 부정적 캐릭터 발견시 대체 후보 탐색
            for (int i = 0; i < total; i++) {
                int exists = 0;
                CharacterNode *check = *head;
                while (check) {
                    if (strcmp(check->data.name, arr[i].name) == 0) {
                        exists = 1;
                        break;
                    }
                    check = check->next;
                }
                if (!exists && arr[i].isNegative == 0) {
                    // 대체
                    strncpy(cur->data.name, arr[i].name, sizeof(cur->data.name)-1);
                    strncpy(cur->data.description, arr[i].description, sizeof(cur->data.description)-1);
                    cur->data.isNegative = 0;
                    break;
                }
            }
        }
        cur = cur->next;
    }
}

// main studyMyCharacter 함수
void studyMyCharacter(void) {
    Character characters[MAX_CHARACTERS];
    int totalChars = loadCharacters("character.txt", characters, MAX_CHARACTERS);
    if (totalChars < MAX_CHARACTERS) {
        printf("캐릭터 파일 로드 실패 또는 캐릭터 수 부족\n");
        return;
    }

    srand((unsigned int)time(NULL));

    int selectedIndices[SELECT_COUNT];
    selectCharacters(characters, totalChars, selectedIndices);

    int ratios[SELECT_COUNT];
    generateRatios(ratios, SELECT_COUNT);

    CharacterNode *head = NULL;

    // 선택된 캐릭터 노드 생성 후 리스트 삽입 (비율 높은 순 정렬)
    for (int i = 0; i < SELECT_COUNT; i++) {
        CharacterNode *node = createNode(&characters[selectedIndices[i]], ratios[i]);
        if (!node) {
            printf("메모리 할당 실패\n");
            freeList(head);
            return;
        }
        insertSorted(&head, node);
    }

    printf("== 초기 캐릭터 분석 결과 ==\n");
    printList(head);

    // 부정적 캐릭터 제거
    if (removeNegative(&head)) {
        printf("\n== 부정적 캐릭터 제거 후 분석 결과 ==\n");
        printList(head);

        // 보너스: 부정적 캐릭터를 다른 고유한 긍정적 캐릭터로 대체
        replaceNegative(&head, characters, totalChars);

        printf("\n== 부정적 캐릭터 대체 후 분석 결과 ==\n");
        printList(head);
    } else {
        printf("\n부정적 캐릭터가 없습니다.\n");
    }

    freeList(head);
}

// 테스트용 main
int main(void) {
    studyMyCharacter();
    return 0;
}
