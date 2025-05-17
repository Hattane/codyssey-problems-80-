#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TUTORS 10
#define MAX_MEMBERS 10

// Vocal Tutor Structure
typedef struct {
    char name[20];
    char skill[100];
    int assigned; // 0 if available, 1 if assigned
} Tutor;

// Member Structure
typedef struct {
    char name[20];
    Tutor *assignedTutor;
} Member;

Tutor tutors[MAX_TUTORS] = {
    {"Aria", "Wide vocal range and strong tone"},
    {"Retro", "Versatile with vintage and modern styles"},
    {"Magic", "Magical expression through music"},
    {"Symphony", "Orchestral harmonization skills"},
    {"Siren", "Mysterious and emotional vocals"},
    {"Rune", "Imaginative and touching music"},
    {"Future", "Forward-looking musical guidance"},
    {"Massive", "Strong voice that empowers students"},
    {"Echo", "Conveys emotion and messages in music"}
};

Member members[MAX_MEMBERS] = {
    {"Alice", NULL},
    {"Bob", NULL},
    {"Charlie", NULL},
    {"Diana", NULL},
    {"Eddie", NULL},
    {"Fay", NULL},
    {"George", NULL},
    {"Hannah", NULL},
    {"Ivan", NULL},
    {"Jane", NULL}
};

void assignTutorsRandomly() {
    int assignedCount = 0;
    while (assignedCount < MAX_MEMBERS) {
        int tutorIndex = rand() % MAX_TUTORS;
        int memberIndex = assignedCount;
        if (!tutors[tutorIndex].assigned) {
            members[memberIndex].assignedTutor = &tutors[tutorIndex];
            tutors[tutorIndex].assigned = 1;
            assignedCount++;
        }
    }
}

void printAssignments() {
    printf("\n[Member - Tutor Assignments]\n");
    for (int i = 0; i < MAX_MEMBERS; i++) {
        if (members[i].assignedTutor)
            printf("%s -> %s\n", members[i].name, members[i].assignedTutor->name);
        else
            printf("%s -> No Tutor Assigned\n", members[i].name);
    }
}

void deleteRandomTutor() {
    int idx = rand() % MAX_TUTORS;
    printf("\n[Tutor %s has left the program]\n", tutors[idx].name);
    for (int i = 0; i < MAX_MEMBERS; i++) {
        if (members[i].assignedTutor == &tutors[idx]) {
            members[i].assignedTutor = NULL;
        }
    }
    tutors[idx].assigned = 0;
    strcpy(tutors[idx].name, "");
    strcpy(tutors[idx].skill, "");
}

void reassignTutors() {
    for (int i = 0; i < MAX_MEMBERS; i++) {
        if (members[i].assignedTutor == NULL) {
            for (int j = 0; j < MAX_TUTORS; j++) {
                if (!tutors[j].assigned && strlen(tutors[j].name) > 0) {
                    members[i].assignedTutor = &tutors[j];
                    tutors[j].assigned = 1;
                    break;
                }
            }
        }
    }
}

void adminVocalTuition() {
    srand(time(NULL));
    assignTutorsRandomly();
    printAssignments();
    deleteRandomTutor();
    reassignTutors();
    printAssignments();
}

void stage4() {
    int choice;
    while (1) {
        printf("\n[Training Menu]\n");
        printf("1. Diet Planning\n");
        printf("2. Dance Training\n");
        printf("3. Self Reflection\n");
        printf("4. Vocal Genre Training\n");
        printf("5. Vocal Tutoring\n");
        printf("0. Exit\n");
        printf("Select an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // callDietPlanning();
                break;
            case 2:
                // callDanceTraining();
                break;
            case 3:
                // callSelfReflection();
                break;
            case 4:
                // learnSongGenre();
                break;
            case 5:
                adminVocalTuition();
                break;
            case 0:
                return;
            default:
                printf("Invalid option. Try again.\n");
        }
    }
}

int main() {
    stage4();
    return 0;
}
