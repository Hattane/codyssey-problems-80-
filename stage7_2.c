// stage7.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct BookList {
    int order;                   // 읽는 순번
    char title[256];             // 책 제목
    char author[256];            // 저자 정보
    struct BookList *next;
} BookList;

BookList *readingHead = NULL;    // 읽을 책 리스트 (독서 목록)
BookList *readHead = NULL;       // 읽은 책 리스트 (보너스)

// --- 유틸리티 함수 ---
void freeList(BookList **head) {
    BookList *curr = *head;
    while (curr) {
        BookList *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    *head = NULL;
}

// 순번 오름차순 정렬 (단순 삽입 정렬, 리스트가 크지 않으므로 충분)
void sortListByOrder(BookList **head) {
    if (*head == NULL || (*head)->next == NULL) return;

    BookList *sorted = NULL;

    while (*head) {
        BookList *curr = *head;
        *head = (*head)->next;

        if (!sorted || curr->order < sorted->order) {
            curr->next = sorted;
            sorted = curr;
        } else {
            BookList *p = sorted;
            while (p->next && p->next->order < curr->order) {
                p = p->next;
            }
            curr->next = p->next;
            p->next = curr;
        }
    }
    *head = sorted;
}

// 순번 재조정: 1부터 순차적으로 다시 맞춤
void reorderList(BookList *head) {
    int num = 1;
    while (head) {
        head->order = num++;
        head = head->next;
    }
}

// --- 독서 목록 출력 (공통 함수) ---
void printBookList(BookList *head) {
    if (!head) {
        printf("독서 목록이 비어있습니다.\n");
        return;
    }
    BookList *curr = head;
    printf("순번\t책 제목\t저자\n");
    printf("-------------------------------\n");
    while (curr) {
        printf("%d\t%s\t%s\n", curr->order, curr->title, curr->author);
        curr = curr->next;
    }
}

// --- 파일에서 독서 목록 읽기 ---
void loadReadingList() {
    FILE *fp = fopen("book_list.txt", "r");
    if (!fp) {
        printf("파일 열기 실패: book_list.txt\n");
        return;
    }

    freeList(&readingHead);

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        int order;
        char title[256];
        char author[256];

        // 형식: 순번,책 제목,저자 (콤마로 구분된다고 가정)
        if (sscanf(line, "%d,%255[^,],%255[^\n]", &order, title, author) == 3) {
            BookList *newNode = malloc(sizeof(BookList));
            if (!newNode) {
                fprintf(stderr, "메모리 할당 실패\n");
                fclose(fp);
                return;
            }
            newNode->order = order;
            strcpy(newNode->title, title);
            strcpy(newNode->author, author);
            newNode->next = NULL;

            // 리스트 맨 뒤에 추가
            if (!readingHead) {
                readingHead = newNode;
            } else {
                BookList *p = readingHead;
                while (p->next) p = p->next;
                p->next = newNode;
            }
        }
    }
    fclose(fp);

    // 혹시 순서가 섞였을 수도 있으니 정렬 후 순번 재조정
    sortListByOrder(&readingHead);
    reorderList(readingHead);
}

// --- 순번에 맞게 새 책 추가 ---
void insertBook(int order, const char *title, const char *author) {
    if (order < 1) {
        printf("순번은 1 이상의 값이어야 합니다.\n");
        return;
    }

    BookList *newNode = malloc(sizeof(BookList));
    if (!newNode) {
        fprintf(stderr, "메모리 할당 실패\n");
        return;
    }
    newNode->order = order;
    strncpy(newNode->title, title, sizeof(newNode->title) - 1);
    newNode->title[sizeof(newNode->title) - 1] = '\0';
    strncpy(newNode->author, author, sizeof(newNode->author) - 1);
    newNode->author[sizeof(newNode->author) - 1] = '\0';
    newNode->next = NULL;

    if (!readingHead || order == 1) {
        newNode->next = readingHead;
        readingHead = newNode;
    } else {
        BookList *prev = NULL;
        BookList *curr = readingHead;
        while (curr && curr->order < order) {
            prev = curr;
            curr = curr->next;
        }
        newNode->next = curr;
        prev->next = newNode;
    }

    // 이후 노드들의 순번 +1 증가
    BookList *p = newNode->next;
    while (p) {
        p->order++;
        p = p->next;
    }

    reorderList(readingHead);

    // 보너스: 읽은 목록에도 동기화 (추가된 순번 이후 재조정)
    sortListByOrder(&readHead);
    reorderList(readHead);
}

// --- 특정 순번 도서 제거 ---
void removeBook(int order) {
    if (!readingHead) {
        printf("독서 목록이 비어있습니다.\n");
        return;
    }
    BookList *curr = readingHead;
    BookList *prev = NULL;

    while (curr && curr->order != order) {
        prev = curr;
        curr = curr->next;
    }

    if (!curr) {
        printf("순번 %d에 해당하는 도서가 없습니다.\n", order);
        return;
    }

    if (!prev) {
        readingHead = curr->next;
    } else {
        prev->next = curr->next;
    }
    free(curr);

    reorderList(readingHead);

    // 보너스: 읽은 목록에서도 동일 순번 제거 후 재조정
    curr = readHead;
    prev = NULL;
    while (curr && curr->order != order) {
        prev = curr;
        curr = curr->next;
    }
    if (curr) {
        if (!prev) readHead = curr->next;
        else prev->next = curr->next;
        free(curr);
        reorderList(readHead);
    }
}

// --- 독서 순번 변경 ---
void changeOrder(int oldOrder, int newOrder) {
    if (oldOrder == newOrder) {
        printf("기존 순번과 변경 순번이 같습니다.\n");
        return;
    }
    if (newOrder < 1) {
        printf("새 순번은 1 이상의 값이어야 합니다.\n");
        return;
    }

    BookList *prev = NULL, *curr = readingHead;
    while (curr && curr->order != oldOrder) {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) {
        printf("순번 %d에 해당하는 도서가 없습니다.\n", oldOrder);
        return;
    }

    // 리스트에서 curr 제거
    if (!prev) readingHead = curr->next;
    else prev->next = curr->next;

    // 새 순번에 맞게 다시 삽입
    curr->order = newOrder;
    curr->next = NULL;

    if (!readingHead || newOrder == 1) {
        curr->next = readingHead;
        readingHead = curr;
    } else {
        BookList *p = readingHead;
        BookList *pprev = NULL;
        while (p && p->order < newOrder) {
            pprev = p;
            p = p->next;
        }
        if (!pprev) {
            curr->next = readingHead;
            readingHead = curr;
        } else {
            curr->next = p;
            pprev->next = curr;
        }
    }

    reorderList(readingHead);

    // 보너스: 읽은 목록 순번도 변경 (동일 책이 있으면)
    BookList *readCurr = readHead;
    while (readCurr) {
        if (strcmp(readCurr->title, curr->title) == 0 && strcmp(readCurr->author, curr->author) == 0) {
            readCurr->order = curr->order;
            break;
        }
        readCurr = readCurr->next;
    }
    sortListByOrder(&readHead);
    reorderList(readHead);
}

// --- 보너스: 읽을 책 선택해서 읽은 책 목록에 추가 ---
void markBookAsRead(int order) {
    BookList *curr = readingHead;
    while (curr && curr->order != order) {
        curr = curr->next;
    }
    if (!curr) {
        printf("순번 %d에 해당하는 도서가 없습니다.\n", order);
        return;
    }

    // 이미 읽은 목록에 있는지 확인
    BookList *readCurr = readHead;
    while (readCurr) {
        if (strcmp(readCurr->title, curr->title) == 0 && strcmp(readCurr->author, curr->author) == 0) {
            printf("이미 읽은 목록에 있는 도서입니다.\n");
            return;
        }
        readCurr = readCurr->next;
    }

    BookList *newNode = malloc(sizeof(BookList));
    if (!newNode) {
        fprintf(stderr, "메모리 할당 실패\n");
        return;
    }
    newNode->order = curr->order;
    strcpy(newNode->title, curr->title);
    strcpy(newNode->author, curr->author);
    newNode->next = NULL;

    // 읽은 책 목록에 삽입 (순번 오름차순)
    if (!readHead || newNode->order < readHead->order) {
        newNode->next = readHead;
        readHead = newNode;
    } else {
        BookList *p = readHead;
        while (p->next && p->next->order < newNode->order) {
            p = p->next;
        }
        newNode->next = p->next;
        p->next = newNode;
    }
    reorderList(readHead);
    printf("'%s' 책을 읽은 목록에 추가했습니다.\n", newNode->title);
}

// --- doReadingList() 메인 함수 ---
void doReadingList() {
    int choice;
    char buffer[512];

    loadReadingList();

    while (1) {
        printf("\n[독서 목록 관리]\n");
        printf("1. 독서 목록 출력\n");
        printf("2. 새 도서 추가\n");
        printf("3. 독서 순번 변경\n");
        printf("4. 도서 제거\n");
        printf("5. 도서 읽음 표시 (읽은 목록에 추가)\n");
        printf("6. 읽은 도서 목록 출력\n");
        printf("0. 종료\n");
        printf("선택: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        choice = atoi(buffer);

        if (choice == 0) break;

        switch (choice) {
            case 1:
                printf("\n-- 독서 목록 --\n");
                printBookList(readingHead);
                break;
            case 2: {
                int order;
                char title[256], author[256];

                printf("추가할 순번: ");
                if (!fgets(buffer, sizeof(buffer), stdin)) break;
                order = atoi(buffer);
                if (order < 1) {
                    printf("유효한 순번을 입력하세요.\n");
                    break;
                }

                printf("책 제목: ");
                if (!fgets(title, sizeof(title), stdin)) break;
                title[strcspn(title, "\n")] = 0; // 개행 제거

                printf("저자: ");
                if (!fgets(author, sizeof(author), stdin)) break;
                author[strcspn(author, "\n")] = 0; // 개행 제거

                insertBook(order, title, author);
                printf("도서가 추가되었습니다.\n");
                break;
            }
            case 3: {
                int oldOrder, newOrder;
                printf("변경할 기존 순번: ");
                if (!fgets(buffer, sizeof(buffer), stdin)) break;
                oldOrder = atoi(buffer);
                printf("새 순번: ");
                if (!fgets(buffer, sizeof(buffer), stdin)) break;
                newOrder = atoi(buffer);
                changeOrder(oldOrder, newOrder);
                printf("순번 변경 완료.\n");
                break;
            }
            case 4: {
                int order;
                printf("제거할 도서 순번: ");
                if (!fgets(buffer, sizeof(buffer), stdin)) break
