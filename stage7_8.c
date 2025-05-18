#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALBUM_COUNT 5
#define MAX_STR_LEN 512
#define MAX_NAME_LEN 64
#define MAX_HINT_LEN 32
#define MAX_INPUT_RETRY 2

typedef struct Hint {
    char word1[MAX_HINT_LEN];
    char word2[MAX_HINT_LEN];
} Hint;

typedef struct WorldView {
    char albumName[MAX_NAME_LEN];
    char content[MAX_STR_LEN];
    Hint hint;  // 2번째 앨범부터 사용
    struct WorldView* prev;
    struct WorldView* next;
} WorldView;

// 문자열 소문자 변환 (비교용)
void strToLower(char *str) {
    while (*str) {
        *str = (char)tolower((unsigned char)*str);
        str++;
    }
}

// 문자열이 content 안에 포함되는지 확인 (대소문자 무시)
int containsWordIgnoreCase(const char *content, const char *word) {
    char contentLower[MAX_STR_LEN];
    char wordLower[MAX_HINT_LEN];
    strncpy(contentLower, content, MAX_STR_LEN);
    strncpy(wordLower, word, MAX_HINT_LEN);
    contentLower[MAX_STR_LEN - 1] = 0;
    wordLower[MAX_HINT_LEN - 1] = 0;

    strToLower(contentLower);
    strToLower(wordLower);

    return strstr(contentLower, wordLower) != NULL;
}

// 파일에서 앨범 이름과 세계관 내용 읽기
int loadWorldViewFile(const char* filename, WorldView* arr, int maxCount) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("파일 열기 실패: %s\n", filename);
        return 0;
    }

    char line[MAX_STR_LEN];
    int count = 0;

    while (count < maxCount && fgets(line, sizeof(line), fp)) {
        // 앨범 이름 (줄바꿈 제거)
        line[strcspn(line, "\r\n")] = 0;
        strncpy(arr[count].albumName, line, MAX_NAME_LEN);
        arr[count].albumName[MAX_NAME_LEN - 1] = 0;

        // 세계관 내용 읽기 (한 줄)
        if (!fgets(line, sizeof(line), fp)) break;
        line[strcspn(line, "\r\n")] = 0;
        strncpy(arr[count].content, line, MAX_STR_LEN);
        arr[count].content[MAX_STR_LEN - 1] = 0;

        // 초기화
        arr[count].prev = NULL;
        arr[count].next = NULL;
        arr[count].hint.word1[0] = 0;
        arr[count].hint.word2[0] = 0;

        count++;
    }

    fclose(fp);

    if (count != maxCount) {
        printf("앨범 정보를 모두 읽지 못했습니다. 읽은 개수: %d\n", count);
        return 0;
    }
    return 1;
}

// 힌트 단어 입력 및 검증 함수
int inputHintWords(WorldView* current, WorldView* next) {
    char w1[MAX_HINT_LEN], w2[MAX_HINT_LEN];
    int tries = 0;

    while (tries < MAX_INPUT_RETRY) {
        printf("앨범 '%s'의 세계관에 포함된 다음 앨범 '%s'의 힌트 단어 2개를 입력하세요 (공백 구분):\n", current->albumName, next->albumName);
        if (scanf("%31s %31s", w1, w2) != 2) {
            printf("입력 형식이 올바르지 않습니다. 다시 시도하세요.\n");
            // 입력 버퍼 비우기
            int c; while ((c = getchar()) != '\n' && c != EOF);
            tries++;
            continue;
        }

        // 힌트 단어가 현재 앨범의 세계관 내용에 포함되어 있는지 검사
        int w1_found = containsWordIgnoreCase(current->content, w1);
        int w2_found = containsWordIgnoreCase(current->content, w2);

        if (!w1_found || !w2_found) {
            printf("힌트 단어 중 하나 이상이 현재 앨범 세계관에 포함되어 있지 않습니다. 다시 시도하세요. (%d/%d)\n", tries + 1, MAX_INPUT_RETRY);
            tries++;
            // 입력 버퍼 비우기
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        // 성공
        strncpy(next->hint.word1, w1, MAX_HINT_LEN);
        next->hint.word1[MAX_HINT_LEN - 1] = 0;
        strncpy(next->hint.word2, w2, MAX_HINT_LEN);
        next->hint.word2[MAX_HINT_LEN - 1] = 0;

        // 입력 버퍼 비우기
        int c; while ((c = getchar()) != '\n' && c != EOF);

        return 1;
    }

    printf("힌트 단어 입력 실패로 상위 메뉴로 돌아갑니다.\n");
    return 0;
}

// 연결 리스트 생성 함수 (배열 기반으로 연결)
WorldView* createLinkedList(WorldView* arr, int count) {
    for (int i = 0; i < count - 1; i++) {
        arr[i].next = &arr[i+1];
        arr[i+1].prev = &arr[i];
    }
    arr[0].prev = NULL;
    arr[count-1].next = NULL;
    return &arr[0];
}

// 전체 세계관 출력
void printAllWorldViews(WorldView* head) {
    printf("\n전체 앨범 세계관 내용 출력\n-------------------------\n");
    WorldView* cur = head;
    while (cur) {
        printf("앨범: %s\n", cur->albumName);
        printf("세계관: %s\n", cur->content);
        if (cur->hint.word1[0] != 0 && cur->hint.word2[0] != 0) {
            printf("힌트 단어: %s, %s\n", cur->hint.word1, cur->hint.word2);
        }
        printf("\n");
        cur = cur->next;
    }
}

// 노드 스왑 함수 (포인터 재연결)
void swapNodes(WorldView** headRef, WorldView* a, WorldView* b) {
    if (a == b) return;

    WorldView* prevA = a->prev;
    WorldView* nextA = a->next;
    WorldView* prevB = b->prev;
    WorldView* nextB = b->next;

    // a와 b가 인접한 경우 처리
    if (a->next == b) {
        a->next = nextB;
        if (nextB) nextB->prev = a;
        b->prev = prevA;
        if (prevA) prevA->next = b;
        b->next = a;
        a->prev = b;
    }
    else if (b->next == a) {
        swapNodes(headRef, b, a);
        return;
    }
    else {
        // 일반적인 경우
        if (prevA) prevA->next = b;
        if (nextA) nextA->prev = b;
        if (prevB) prevB->next = a;
        if (nextB) nextB->prev = a;

        a->prev = prevB;
        a->next = nextB;
        b->prev = prevA;
        b->next = nextA;
    }

    // head 변경 체크
    if (*headRef == a) {
        *headRef = b;
    }
    else if (*headRef == b) {
        *headRef = a;
    }
}

// 정렬 함수 (오름차순 ascending=1, 내림차순 ascending=0)
WorldView* sortWorldViews(WorldView* head, int ascending) {
    if (!head) return NULL;

    int swapped;
    WorldView* ptr1;
    WorldView* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            int cmp = strcmp(ptr1->albumName, ptr1->next->albumName);
            int needSwap = ascending ? (cmp > 0) : (cmp < 0);

            if (needSwap) {
                swapNodes(&head, ptr1, ptr1->next);
                swapped = 1;
                // swapNodes에서 head 변경 가능하므로 다시 시작
                break;
            } else {
                ptr1 = ptr1->next;
            }
        }
        lptr = ptr1;
    } while (swapped);

    return head;
}

// developWorldView 함수: 문제 수행 핵심 함수
void developWorldView() {
    WorldView albums[ALBUM_COUNT];
    if (!loadWorldViewFile("world_view.txt", albums, ALBUM_COUNT)) {
        printf("세계관 파일을 읽는데 실패했습니다.\n");
        return;
    }

    // 1번째 앨범은 힌트 단어 없음
    // 2~5번째 앨범에 대해 힌트 단어 입력 및 검증
    for (int i = 1; i < ALBUM_COUNT; i++) {
        if (!inputHintWords(&albums[i-1], &albums[i])) {
            // 힌트 단어 입력 실패 시 함수 종료 (상위 메뉴 복귀)
            return;
        }
    }

    WorldView* head = createLinkedList(albums, ALBUM_COUNT);

    printAllWorldViews(head);

    // 보너스: 정렬 기능
    int sortChoice = 0;
    printf("세계관 앨범을 정렬하시겠습니까? (1: 오름차순, 2: 내림차순, 0: 정렬 안함): ");
    if (scanf("%d", &sortChoice) != 1) {
        printf
