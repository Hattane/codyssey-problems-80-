#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANDIDATE_COUNT 6
#define SCORE_COUNT 5
#define MAX_JUDGE_NAME 50
#define MAX_EXPERTISE 30

typedef struct {
    char name[50];
    int id;
} Candidate;

typedef struct {
    char judgeName[MAX_JUDGE_NAME];
    char expertise[MAX_EXPERTISE];
    int scores[CANDIDATE_COUNT][SCORE_COUNT + 2]; 
    // [candidate][0]: candidate id, [1..5]: scores, [6]: total score
} JudgeScores;

Candidate candidates[CANDIDATE_COUNT] = {
    {"Jiyeon Park", 123456},
    {"Ethan Smith", 234567},
    {"Helena Silva", 345678},
    {"Liam Wilson", 456789},
    {"Sophia Kim", 567890},
    {"Noah Lee", 678901}
};

const char *categories[SCORE_COUNT] = {
    "Music Proficiency",
    "Dance",
    "Vocal",
    "Visual",
    "Expression"
};

void inputScores(JudgeScores *js);
void printScores(JudgeScores *js);
int findCandidateIndex(const char *name, int id);
void modifyScores(JudgeScores *js);

int main() {
    JudgeScores js;
    int submitted = 0;

    while (!submitted) {
        inputScores(&js);
        printScores(&js);

        printf("Would you like to submit? (Y/N): ");
        char yn;
        scanf(" %c", &yn);
        getchar(); 

        if (yn == 'Y' || yn == 'y') {
            printf("***Final submission completed.***\n");
            submitted = 1;
        } else if (yn == 'N' || yn == 'n') {
            modifyScores(&js);
        } else {
            printf("Invalid input. Please enter Y or N.\n");
        }
    }

    return 0;
}

void inputScores(JudgeScores *js) {
    printf("####################################\n");
    printf("#     Audition Evaluation Entry    #\n");
    printf("####################################\n");

    printf("> Judge Name: ");
    fgets(js->judgeName, MAX_JUDGE_NAME, stdin);
    js->judgeName[strcspn(js->judgeName, "\n")] = 0; 

    printf("> Expertise: ");
    fgets(js->expertise, MAX_EXPERTISE, stdin);
    js->expertise[strcspn(js->expertise, "\n")] = 0;

    for (int i = 0; i < CANDIDATE_COUNT; i++) {
        js->scores[i][0] = candidates[i].id;
        int total = 0;

        printf("++++++++++++++++++++++++++++++++++++\n");
        printf("Candidate: %s\n", candidates[i].name);

        for (int j = 0; j < SCORE_COUNT; j++) {
            int score;
            while (1) {
                printf("%s: ", categories[j]);
                if (scanf("%d", &score) != 1) {
                    while(getchar() != '\n'); 
                    printf("Invalid input. Please enter a number between 10 and 100.\n");
                    continue;
                }
                if (score < 10 || score > 100) {
                    printf("Score must be between 10 and 100. Please re-enter.\n");
                } else {
                    break;
                }
            }
            js->scores[i][j+1] = score;
            total += score;
        }
        js->scores[i][SCORE_COUNT + 1] = total;
        while(getchar() != '\n'); 
        printf("------------------------------------\n");
    }
}

void printScores(JudgeScores *js) {
    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("Submission completed.\nPlease review your input!\n");
    printf("------------------------------------\n");

    for (int i = 0; i < CANDIDATE_COUNT; i++) {
        printf("%s: ", candidates[i].name);
        for (int j = 0; j < SCORE_COUNT; j++) {
            printf("%d ", js->scores[i][j+1]);
        }
        printf(" | Total: %d\n", js->scores[i][SCORE_COUNT+1]);
    }
}

int findCandidateIndex(const char *name, int id) {
    for (int i = 0; i < CANDIDATE_COUNT; i++) {
        if (strcmp(candidates[i].name, name) == 0 && candidates[i].id == id) {
            return i;
        }
    }
    return -1;
}

void modifyScores(JudgeScores *js) {
    char name[50];
    int id;

    while (1) {
        printf("Enter candidate name to modify (or type 'exit' to finish): ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
        if (strcmp(name, "exit") == 0) break;

        printf("Enter candidate 6-digit ID: ");
        if (scanf("%d", &id) != 1) {
            while(getchar() != '\n');
            printf("Invalid ID input.\n");
            continue;
        }
        while(getchar() != '\n');

        if (id < 100000 || id > 999999) {
            printf("ID must be 6-digit number.\n");
            continue;
        }

        int idx = findCandidateIndex(name, id);
        if (idx == -1) {
            printf("Candidate not found. Please try again.\n");
            continue;
        }

        int total = 0;
        printf("Modifying scores for %s (ID: %06d)\n", candidates[idx].name, candidates[idx].id);
        for (int j = 0; j < SCORE_COUNT; j++) {
            int score;
            while (1) {
                printf("%s: ", categories[j]);
                if (scanf("%d", &score) != 1) {
                    while(getchar() != '\n');
                    printf("Invalid input. Please enter a number between 10 and 100.\n");
                    continue;
                }
                if (score < 10 || score > 100) {
                    printf("Score must be between 10 and 100. Please re-enter.\n");
                } else {
                    break;
                }
            }
            js->scores[idx][j+1] = score;
            total += score;
        }
        js->scores[idx][SCORE_COUNT+1] = total;
        while(getchar() != '\n');

        printScores(js);
    }
    printf("No more modifications. Returning to submission prompt.\n");
}
