#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SONG_COUNT 10
#define GENRE_COUNT 10
#define MAX_ATTEMPTS 2
#define MAX_INPUT 100

typedef struct {
    char title[50];
    char genre[50];
    char artist[50];
} Song;

const char *genres[GENRE_COUNT] = {
    "댄스 팝", "발라드", "힙합", "R&B", "록(Rock)",
    "일렉트로니카", "트로트", "레게", "퓨전", "밴드"
};

Song songs[SONG_COUNT] = {
    {"Call Me Baby", "댄스 팝", "EXO"},
    {"Through the Night", "발라드", "IU"},
    {"Mic Drop", "힙합", "BTS"},
    {"Eyes, Nose, Lips", "R&B", "Taeyang"},
    {"Shoot Me", "록(Rock)", "Day6"},
    {"Electric Shock", "일렉트로니카", "f(x)"},
    {"Love Tonight", "트로트", "홍진영"},
    {"Island", "레게", "WINNER"},
    {"Dumb Litty", "퓨전", "KARD"},
    {"I'm Sorry", "밴드", "CNBLUE"}
};

void shuffleArray(int *array, int size) {
    for (int i = size - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void showHint(const char *artist) {
    int len = strlen(artist);
    int hintLen = 1;
    if (len >= 4 && len <= 5) hintLen = 2;
    else if (len > 5) hintLen = 3;
    printf("힌트: %.*s...\n", hintLen, artist);
}

void learnSongJenre() {
    srand((unsigned int)time(NULL));
    int index = rand() % SONG_COUNT;
    Song *selected = &songs[index];
    char input[MAX_INPUT];
    int attempt = 0;
    int hintUsed = 0;

    printf("노래 제목: %s\n", selected->title);

    while (attempt < MAX_ATTEMPTS) {
        printf("장르를 입력하세요 (힌트를 원하면 '힌트' 입력): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "힌트") == 0 && !hintUsed) {
            showHint(selected->artist);
            hintUsed = 1;
            continue;
        } else if (strcmp(input, selected->genre) == 0) {
            printf("정답입니다! 축하합니다 🎉\n");
            return;
        } else {
            attempt++;
            if (attempt < MAX_ATTEMPTS) {
                printf("틀렸습니다. 다시 시도하세요.\n");
            }
        }
    }
    printf("아쉽게도 모두 틀렸습니다. 정답은 '%s' 입니다.\n", selected->genre);
}

void genreMatchingGame() {
    int songOrder[SONG_COUNT], genreOrder[GENRE_COUNT];
    for (int i = 0; i < SONG_COUNT; ++i) {
        songOrder[i] = i;
        genreOrder[i] = i;
    }

    shuffleArray(songOrder, SONG_COUNT);
    shuffleArray(genreOrder, GENRE_COUNT);

    char input[MAX_INPUT];
    int correct = 0;

    for (int i = 0; i < SONG_COUNT; ++i) {
        Song *song = &songs[songOrder[i]];
        printf("노래: %-25s | 장르 목록 중 선택: \n", song->title);
        for (int j = 0; j < GENRE_COUNT; ++j) {
            printf("%d. %s\n", j + 1, genres[genreOrder[j]]);
        }
        printf("선택 번호: ");
        fgets(input, sizeof(input), stdin);
        int choice = atoi(input);

        if (choice >= 1 && choice <= GENRE_COUNT && strcmp(song->genre, genres[genreOrder[choice - 1]]) == 0) {
            printf("정답!\n\n");
            correct++;
        } else {
            printf("오답. 정답은: %s\n\n", song->genre);
        }
    }
    printf("총 %d/%d개 정답을 맞췄습니다!\n", correct, SONG_COUNT);
}

int main() {
    int choice;
    do {
        printf("\n[보컬 훈련 > D. 장르 학습]\n");
        printf("1. 장르 퀴즈\n");
        printf("2. 장르 매칭 게임\n");
        printf("0. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);
        getchar();  // flush newline

        switch (choice) {
            case 1:
                learnSongJenre();
                break;
            case 2:
                genreMatchingGame();
                break;
            case 0:
                printf("종료합니다.\n");
                break;
            default:
                printf("잘못된 선택입니다.\n");
        }
    } while (choice != 0);

    return 0;
}
