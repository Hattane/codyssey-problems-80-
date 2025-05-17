#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LYRICS 100
#define MAX_PARTS 7
#define MAX_MEMBERS 10
#define MAX_NAME_LEN 50

// Song parts
const char *PARTS[MAX_PARTS] = {
    "Intro", "Verse 1", "Pre-Chorus", "Chorus", "Verse 2", "Bridge", "Final Chorus"
};

// Sample BTS "봄 날" lyrics (shortened example, replace with actual lyrics or load from file)
const char *SPRING_DAY_LYRICS[] = {
    "보고 싶다",
    "이렇게 말하니까 더 보고 싶다",
    "너희 사진을 보고 있어도 보고 싶다",
    "시간이 마치 멈춘 것처럼 느껴져",
    "너에게 가는 이 길이 모두 막혔으면",
    "하루하루가 너무 더디 흘러가",
    "마치 네가 없는 것처럼 너무나도 낯설어",
    "눈 감으면 네 모습이 보여",
    "내 손을 잡아줘",
    "그게 네가 나를 사랑하는 방법이니까",
    "하늘에 별을 따다 줄게",
    "다시 만날 때까지 날 지켜줘",
    "봄이 온다",
    "꽃잎이 날리면",
    "너에게 닿을 거야"
};
const int SPRING_DAY_LYRIC_COUNT = sizeof(SPRING_DAY_LYRICS) / sizeof(SPRING_DAY_LYRICS[0]);

// Structures

typedef struct {
    int startIndex;
    int endIndex;
} PartRange;

typedef struct {
    char songTitle[MAX_NAME_LEN];
    const char **lyrics;
    int lyricCount;
    PartRange parts[MAX_PARTS];
} AnalyzedSong;

typedef struct {
    char memberName[MAX_NAME_LEN];
    PartRange assignedParts[MAX_PARTS]; // Which indices assigned to this member per part (simplified)
    int partCount[MAX_PARTS]; // number of lyrics assigned per part to this member
} MemberAssignment;

typedef struct {
    char groupName[MAX_NAME_LEN];
    int memberCount;
    MemberAssignment members[MAX_MEMBERS];
} Group;

// Function prototypes
void printLyrics(const char **lyrics, int count);
void analyzSong(const char **lyrics, int lyricCount, AnalyzedSong *analyzedSong);
void printAnalyzedSong(const AnalyzedSong *analyzedSong);
void assignSongPart(Group *group, const AnalyzedSong *analyzedSong);
void printPartLyrics(const AnalyzedSong *analyzedSong, const Group *group);
void clearAnalyzedSong(AnalyzedSong *analyzedSong);
void clearGroup(Group *group);

void understandSongs() {
    AnalyzedSong analyzedSong = {0};
    Group group = {0};
    
    strncpy(analyzedSong.songTitle, "봄 날", MAX_NAME_LEN - 1);
    analyzedSong.lyrics = SPRING_DAY_LYRICS;
    analyzedSong.lyricCount = SPRING_DAY_LYRIC_COUNT;

    clearAnalyzedSong(&analyzedSong);
    clearGroup(&group);

    printf("==== Team Song Analysis ====\n");
    printf("Song Title: %s\n\n", analyzedSong.songTitle);

    printLyrics(analyzedSong.lyrics, analyzedSong.lyricCount);

    analyzSong(analyzedSong.lyrics, analyzedSong.lyricCount, &analyzedSong);

    printAnalyzedSong(&analyzedSong);

    assignSongPart(&group, &analyzedSong);

    printPartLyrics(&analyzedSong, &group);
}

// Print all lyrics with indices
void printLyrics(const char **lyrics, int count) {
    printf("Lyrics:\n");
    for (int i = 0; i < count; i++) {
        printf("[%2d] %s\n", i, lyrics[i]);
    }
    printf("\n");
}

// Initialize part ranges to -1
void clearAnalyzedSong(AnalyzedSong *analyzedSong) {
    for (int i = 0; i < MAX_PARTS; i++) {
        analyzedSong->parts[i].startIndex = -1;
        analyzedSong->parts[i].endIndex = -1;
    }
}

// Clear group data
void clearGroup(Group *group) {
    group->memberCount = 0;
    memset(group->members, 0, sizeof(group->members));
}

// Analyze song parts by inputting start and end indices
void analyzSong(const char **lyrics, int lyricCount, AnalyzedSong *analyzedSong) {
    printf("Please assign the song parts by entering start and end indices of lyrics.\n");
    printf("If a part has no assigned lyrics, enter -1 -1.\n\n");

    for (int i = 0; i < MAX_PARTS; i++) {
        int start, end;
        while (1) {
            printf("%s (start index end index): ", PARTS[i]);
            if (scanf("%d %d", &start, &end) != 2) {
                while (getchar() != '\n'); // flush input
                printf("Invalid input. Please enter two integers.\n");
                continue;
            }
            if ((start == -1 && end == -1) ||
                (start >= 0 && end >= start && end < lyricCount)) {
                analyzedSong->parts[i].startIndex = start;
                analyzedSong->parts[i].endIndex = end;
                break;
            } else {
                printf("Invalid range. Please try again.\n");
            }
        }
    }
    printf("\nSong parts assigned successfully.\n\n");
}

// Print analyzed song structure with lyrics
void printAnalyzedSong(const AnalyzedSong *analyzedSong) {
    printf("=== Analyzed Song Structure ===\n");
    for (int i = 0; i < MAX_PARTS; i++) {
        PartRange pr = analyzedSong->parts[i];
        printf("%s: ", PARTS[i]);
        if (pr.startIndex == -1) {
            printf("No lyrics assigned.\n");
        } else {
            for (int idx = pr.startIndex; idx <= pr.endIndex; idx++) {
                printf("%s ", analyzedSong->lyrics[idx]);
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Assign song parts to group members
void assignSongPart(Group *group, const AnalyzedSong *analyzedSong) {
    printf("Enter group name: ");
    fgets(group->groupName, MAX_NAME_LEN, stdin);
    // Remove newline
    group->groupName[strcspn(group->groupName, "\n")] = 0;

    printf("Enter number of group members (max %d): ", MAX_MEMBERS);
    scanf("%d", &group->memberCount);
    while(getchar() != '\n'); // flush newline

    if (group->memberCount > MAX_MEMBERS) group->memberCount = MAX_MEMBERS;

    for (int i = 0; i < group->memberCount; i++) {
        printf("Enter name of member %d: ", i+1);
        fgets(group->members[i].memberName, MAX_NAME_LEN, stdin);
        group->members[i].memberName[strcspn(group->members[i].memberName, "\n")] = 0;
        // Clear assignments
        for (int p = 0; p < MAX_PARTS; p++) {
            group->members[i].assignedParts[p].startIndex = -1;
            group->members[i].assignedParts[p].endIndex = -1;
            group->members[i].partCount[p] = 0;
        }
    }

    printf("\nAssign lyrics indices to members for each part.\n");
    printf("For each member and each part, input start and end indices of assigned lyrics.\n");
    printf("If no lyrics assigned for a part, enter -1 -1.\n\n");

    for (int i = 0; i < group->memberCount; i++) {
        printf("Assigning for member: %s\n", group->members[i].memberName);
        for (int p = 0; p < MAX_PARTS; p++) {
            int start, end;
            while (1) {
                printf("  %s (start index end index): ", PARTS[p]);
                if (scanf("%d %d", &start, &end) != 2) {
                    while (getchar() != '\n'); // flush input
                    printf("Invalid input. Try again.\n");
                    continue;
                }
                if ((start == -1 && end == -1) ||
                    (start >= 0 && end >= start && end < analyzedSong->lyricCount)) {
                    group->members[i].assignedParts[p].startIndex = start;
                    group->members[i].assignedParts[p].endIndex = end;
                    if (start == -1) group->members[i].partCount[p] = 0;
                    else group->members[i].partCount[p] = end - start + 1;
                    break;
                } else {
                    printf("Invalid range. Try again.\n");
                }
            }
        }
        printf("\n");
    }
}

// Print lyrics for each member by part
void printPartLyrics(const AnalyzedSong *analyzedSong, const Group *group) {
    printf("=== Member Part Lyrics ===\n");
    for (int i = 0; i < group->memberCount; i++) {
        printf("Member: %s\n", group->members[i].memberName);
        for (int p = 0; p < MAX_PARTS; p++) {
            PartRange pr = group->members[i].assignedParts[p];
            if (pr.startIndex != -1) {
                printf("  %s:\n", PARTS[p]);
                for (int idx = pr.startIndex; idx <= pr.endIndex; idx++) {
                    printf("    [%d] %s\n", idx, analyzedSong->lyrics[idx]);
                }
            }
        }
        printf("\n");
    }
}

// MAIN menu addition example
void stage4_menu() {
    int choice;
    do {
        printf("=== Stage 4 Menu ===\n");
        printf("1. Prepare Masked Stage (Problem 5)\n");
        printf("2. Vocal Health Checklist (Problem 6)\n");
        printf("3. Team Song Analysis (Problem 10)\n");
        printf("0. Exit\n");
        printf("Select option: ");
        scanf("%d", &choice);
        while(getchar() != '\n'); // flush newline
        switch(choice) {
            case 1:
                // prepareMaskedStage(); // Implemented elsewhere
                printf("Problem 5 not implemented in this snippet.\n");
                break;
            case 2:
                // checkList4VocalHealth(); // Implemented elsewhere
                printf("Problem 6 not implemented in this snippet.\n");
                break;
            case 3:
                understandSongs();
                break;
            case 0:
                printf("Exiting Stage 4 menu.\n");
                break;
            default:
                printf("Invalid option.\n");
        }
    } while(choice != 0);
}

int main() {
    stage4_menu();
    return 0;
}
