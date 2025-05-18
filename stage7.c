// stage7.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Subject {
    char name[100];
    char details[1000];
    struct Subject *next;
} Subject;

Subject *head = NULL;

void initializeSubjects() {
    const char *subjectNames[] = {
        "Acting Theory", "Stage Theory", "Script Analysis",
        "Dialogue Interpretation and Emotional Expression",
        "Character Development", "Storytelling Theory", "Stage Movement and Poses"
    };

    Subject *prev = NULL;
    for (int i = 0; i < 7; i++) {
        Subject *newNode = (Subject *)malloc(sizeof(Subject));
        if (!newNode) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        strcpy(newNode->name, subjectNames[i]);
        newNode->details[0] = '\0';
        newNode->next = NULL;

        if (head == NULL)
            head = newNode;
        else
            prev->next = newNode;

        prev = newNode;
    }
}

void UpdateSubjectInfo(char *line) {
    Subject *current = head;
    while (current != NULL) {
        if (strstr(line, current->name)) {
            char *detailStart = strchr(line, ':');
            if (detailStart) {
                detailStart++; // skip colon
                while (*detailStart == ' ') detailStart++; // skip leading spaces
                // Append details in case of multiple lines
                if (strlen(current->details) + strlen(detailStart) < sizeof(current->details)) {
                    strcat(current->details, detailStart);
                    // Remove trailing newline if present
                    size_t len = strlen(current->details);
                    if (len > 0 && current->details[len - 1] == '\n') {
                        current->details[len - 1] = '\0';
                    }
                }
            }
            break;
        }
        current = current->next;
    }
}

void LoadContentFile() {
    FILE *file = fopen("perform_stage.txt", "r");
    if (!file) {
        printf("Error: Could not open file 'perform_stage.txt'\n");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        UpdateSubjectInfo(line);
    }

    fclose(file);
}

void PrintSubjectList() {
    Subject *current = head;
    int index = 1;
    while (current) {
        printf("%d. %s\n", index++, current->name);

        // print details line by line at each period
        char *copyDetails = strdup(current->details);
        if (!copyDetails) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        char *token = strtok(copyDetails, ".");
        while (token) {
            // Trim leading spaces
            while (*token == ' ') token++;
            if (strlen(token) > 0) {
                printf("    %s.\n", token);
            }
            token = strtok(NULL, ".");
        }
        free(copyDetails);

        current = current->next;
    }
}

void insertSubject(const char *name, const char *details) {
    Subject *newNode = malloc(sizeof(Subject));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    strcpy(newNode->name, name);
    strcpy(newNode->details, details);
    newNode->next = NULL;

    if (!head) {
        head = newNode;
        return;
    }

    Subject *curr = head;
    while (curr->next)
        curr = curr->next;

    curr->next = newNode;
}

void removeSubject(const char *name) {
    Subject *curr = head, *prev = NULL;

    while (curr != NULL && strcmp(curr->name, name) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) return; // Not found

    if (prev == NULL)
        head = curr->next;
    else
        prev->next = curr->next;

    free(curr);
}

void freeSubjects() {
    Subject *curr = head;
    while (curr) {
        Subject *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    head = NULL;
}

// The main function called from the menu
void learnPerformanceStageTheory() {
    freeSubjects();          // Free any existing list before initializing
    initializeSubjects();
    LoadContentFile();
    PrintSubjectList();
}

