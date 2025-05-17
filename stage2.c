#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <wchar.h>

#define MAX_SONGS 7
#define MAX_MEMBERS 10

typedef struct {
    char title[100];
    char consonants[100];
    char lyricsHint[256]; // for bonus
} KpopSong;

typedef struct {
    char name[30];
    char nickname[30];
} Member;

KpopSong songs[MAX_SONGS];
int songCount = 0;
Member members[MAX_MEMBERS] = {
    {"홍길동", "길동이"},
    {"김영희", "희희"},
    {"박철수", "철이"},
    {"이수민", "수미"},
    {"정지훈", "지훈"}
};
int memberCount = 5;
int completed[MAX_MEMBERS] = {0};

void extractConsonant(const char* title, char* result) {
    int i;
    int len = strlen(title);
    int idx = 0;
    for (i = 0; i < len; i++) {
        unsigned char c = title[i];
        if (c >= 0xB0 && c <= 0xC8) {
            int code = (unsigned char)title[i] << 8 | (unsigned char)title[i + 1];
            int cho = (code - 0xAC00) / (21 * 28);
            result[idx++] = "ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ"[cho];
            i++; 
        } else {
            result[idx++] = c;
        }
    }
    result[idx] = '\0';
}

void inputKpopSongInitials() {
    printf("\n-- K-POP 노래 입력 --\n");
    while (songCount < MAX_SONGS) {
        printf("노래 제목 (그만하려면 빈 줄): ");
        fgets(songs[songCount].title, sizeof(songs[songCount].title), stdin);
        songs[songCount].title[strcspn(songs[songCount].title, "\n")] = 0;
        if (strlen(songs[songCount].title) == 0) break;

        printf("가사 힌트: ");
        fgets(songs[songCount].lyricsHint, sizeof(songs[songCount].lyricsHint), stdin);
        songs[songCount].lyricsHint[strcspn(songs[songCount].lyricsHint, "\n")] = 0;

        extractConsonant(songs[songCount].title, songs[songCount].consonants);
        songCount++;
    }
}

int checkKpopSongTitle(int index, const char* answer) {
    return strcmp(songs[index].title, answer) == 0;
}

int getBlackKnight(int currentIndex) {
    int i;
    srand(time(NULL));
    int selected;
    do {
        selected = rand() % memberCount;
    } while (selected == currentIndex);
    return selected;
}

void completeMission() {
    printf("\n-- 팀워크 미션 시작 --\n");
    int missionCompleted = 0;
    for (int i = 0; i < memberCount; ++i) completed[i] = 0;

    int completedCount = 0;
    while (completedCount < memberCount) {
        int memberIndex = -1;
        for (int i = 0; i < memberCount; ++i) {
            if (!completed[i]) {
                memberIndex = i;
                break;
            }
        }
        if (memberIndex == -1) break;

        int songIndex = rand() % songCount;
        char answer[100];

        printf("\n[%s]님, 다음 초성의 노래 제목을 맞춰보세요: %s\n",
               members[memberIndex].nickname, songs[songIndex].consonants);
        printf("가사 힌트: %s\n", songs[songIndex].lyricsHint);

        int attempts = 0, success = 0;
        while (attempts < 2) {
            printf("입력: ");
            fgets(answer, sizeof(answer), stdin);
            answer[strcspn(answer, "\n")] = 0;

            if (checkKpopSongTitle(songIndex, answer)) {
                printf("%s님, 미션을 성공적으로 완료했습니다!\n", members[memberIndex].name);
                completed[memberIndex] = 1;
                completedCount++;
                success = 1;
                break;
            } else {
                printf("틀렸습니다.\n");
            }
            attempts++;
        }

        if (!success) {
            int knight = getBlackKnight(memberIndex);
            printf("블랙나이트 [%s]가 대신 도전합니다!\n", members[knight].nickname);
            printf("입력: ");
            fgets(answer, sizeof(answer), stdin);
            answer[strcspn(answer, "\n")] = 0;

            if (checkKpopSongTitle(songIndex, answer)) {
                printf("%s님, 미션을 성공적으로 완료했습니다!\n", members[knight].name);
                completed[memberIndex] = 1;
                completedCount++;
            } else {
                printf("밀리웨이즈는 팀워크를 더 발전시켜서 다시 도전하세요!\n");
                return;
            }
        }
    }

    printf("\n밀리웨이즈의 팀워크는 훌륭합니다!\n");
}
