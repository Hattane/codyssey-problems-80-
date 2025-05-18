// debut.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debut.h"

#define MAX_LINE_LEN 512

// 임시 전역: 작곡가별 곡 배열 (최대 3명)
static ComposerSongs composers[MAX_COMPOSERS] = {
    {"ComposerA", NULL},
    {"ComposerB", NULL},
    {"ComposerC", NULL}
};

// 노드 생성 헬퍼
Song* createSongNode(const char* title, const char* desc, int priority) {
    Song* newSong = (Song*)malloc(sizeof(Song));
    if (!newSong) return NULL;
    strncpy(newSong->title, title, MAX_TITLE_LEN);
    strncpy(newSong->description, desc, MAX_DESC_LEN);
    newSong->priority = priority;
    newSong->next = NULL;
    return newSong;
}

// music.txt 읽기 예시 (파일 포맷에 맞게 구현 필요)
void readMusicFile(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open music file");
        return;
    }
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), fp)) {
        char composerName[50], title[100], desc[256];
        // 파싱: composer|title|desc
        if (sscanf(line, "%49[^|]|%99[^|]|%255[^\n]", composerName, title, desc) != 3)
            continue;

        // 해당 작곡가 인덱스 찾기
        int idx = -1;
        for (int i=0; i<MAX_COMPOSERS; i++) {
            if (strcmp(composers[i].composerName, composerName) == 0) {
                idx = i;
                break;
            }
        }
        if (idx == -1) continue;

        // 곡 노드 생성 및 리스트에 추가 (우선순위는 0, 나중에 재배정)
        Song* newSong = createSongNode(title, desc, 0);
        if (!newSong) continue;

        // 리스트 맨 뒤에 추가
        if (!composers[idx].songList) {
            composers[idx].songList = newSong;
        } else {
            Song* temp = composers[idx].songList;
            while (temp->next) temp = temp->next;
            temp->next = newSong;
        }
    }
    fclose(fp);
}

// 선택 로직 (간단 예시: 앞에서부터 3곡 선택, 연속 2곡 제한 적용)
// 보너스 무작위 선택은 추가 구현 필요
void selectSongsByComposer(void) {
    int totalPriority = 1;
    for (int i=0; i<MAX_COMPOSERS; i++) {
        Song* cur = composers[i].songList;
        Song* selectedHead = NULL;
        Song* selectedTail = NULL;
        int selectedCount = 0;
        int consecutiveCount = 0;

        while (cur && selectedCount < 3) {
            // 선택 조건: 연속 2곡까지 허용
            if (consecutiveCount < 2) {
                // 우선순위 부여
                cur->priority = totalPriority++;
                // 선택 리스트에 옮기기 (깊은 복사 또는 포인터 재배치 중 하나)
                // 여기서는 기존 리스트를 우선순위로 마킹만
                selectedCount++;
                consecutiveCount++;
            } else {
                // 다음 작곡가로 넘기거나 중단 가능
                break;
            }
            cur = cur->next;
        }
    }
}

// 두 리스트 병합 후 우선순위 기준 정렬 (병합 정렬 등 구현 필요)
Song* mergeAndSortSongs(void) {
    // 모든 작곡가 리스트 병합
    Song* merged = NULL;
    Song* tail = NULL;

    for (int i=0; i<MAX_COMPOSERS; i++) {
        Song* cur = composers[i].songList;
        while (cur) {
            Song* newNode = createSongNode(cur->title, cur->description, cur->priority);
            if (!merged) {
                merged = tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
            cur = cur->next;
        }
    }

    // 우선순위 오름차순 정렬 (단순 선택정렬 예시)
    for (Song* p = merged; p; p = p->next) {
        for (Song* q = p->next; q; q = q->next) {
            if (p->priority > q->priority) {
                // swap data
                char tmpTitle[MAX_TITLE_LEN], tmpDesc[MAX_DESC_LEN];
                int tmpPriority;
                strcpy(tmpTitle, p->title);
                strcpy(tmpDesc, p->description);
                tmpPriority = p->priority;

                strcpy(p->title, q->title);
                strcpy(p->description, q->description);
                p->priority = q->priority;

                strcpy(q->title, tmpTitle);
                strcpy(q->description, tmpDesc);
                q->priority = tmpPriority;
            }
        }
    }

    return merged;
}

void printDebutAlbum(Song* albumList) {
    int count = 1;
    printf("=== Debut Album Songs ===\n");
    for (Song* cur = albumList; cur; cur = cur->next) {
        printf("%d. %s\n   %s\n", count++, cur->title, cur->description);
    }
}

void makeDebutAlbum(void) {
    readMusicFile("music.txt");
    selectSongsByComposer();
    Song* debutAlbum = mergeAndSortSongs();
    printDebutAlbum(debutAlbum);

    // 메모리 해제 구현 필요 (생략)
}
