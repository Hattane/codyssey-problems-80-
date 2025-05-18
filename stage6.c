#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_QUEUE_SIZE 4
#define MAX_LINE_LEN 256

typedef struct {
    char group[100];
    char concept[100];
    char description[256];
} Concept;

typedef struct Node {
    char data[100];
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
    int size;
} Queue;

typedef struct {
    Node* top;
    int size;
} Stack;

Concept* allConcepts = NULL;
int totalConcepts = 0;

// Queue functions
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

void enqueue(Queue* q, const char* data) {
    if (q->size >= MAX_QUEUE_SIZE) return;
    Node* node = (Node*)malloc(sizeof(Node));
    strcpy(node->data, data);
    node->next = NULL;
    if (q->rear) q->rear->next = node;
    else q->front = node;
    q->rear = node;
    q->size++;
}

char* dequeue(Queue* q) {
    if (!q->front) return NULL;
    Node* temp = q->front;
    char* data = strdup(temp->data);
    q->front = temp->next;
    if (!q->front) q->rear = NULL;
    free(temp);
    q->size--;
    return data;
}

void freeQueue(Queue* q) {
    while (q->front) free(dequeue(q));
    free(q);
}

// Stack functions
Stack* createStack() {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->top = NULL;
    s->size = 0;
    return s;
}

void push(Stack* s, const char* data) {
    if (s->size >= MAX_QUEUE_SIZE) return;
    Node* node = (Node*)malloc(sizeof(Node));
    strcpy(node->data, data);
    node->next = s->top;
    s->top = node;
    s->size++;
}

char* pop(Stack* s) {
    if (!s->top) return NULL;
    Node* temp = s->top;
    char* data = strdup(temp->data);
    s->top = temp->next;
    free(temp);
    s->size--;
    return data;
}

void freeStack(Stack* s) {
    while (s->top) free(pop(s));
    free(s);
}

// Utility
void shuffle(int* indices, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }
}

void loadConcepts(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file.\n");
        return;
    }

    allConcepts = (Concept*)malloc(sizeof(Concept) * 100);
    totalConcepts = 0;

    while (!feof(file) && totalConcepts < 100) {
        fgets(allConcepts[totalConcepts].group, sizeof(allConcepts[totalConcepts].group), file);
        fgets(allConcepts[totalConcepts].concept, sizeof(allConcepts[totalConcepts].concept), file);
        fgets(allConcepts[totalConcepts].description, sizeof(allConcepts[totalConcepts].description), file);

        allConcepts[totalConcepts].group[strcspn(allConcepts[totalConcepts].group, "\n")] = 0;
        allConcepts[totalConcepts].concept[strcspn(allConcepts[totalConcepts].concept, "\n")] = 0;
        allConcepts[totalConcepts].description[strcspn(allConcepts[totalConcepts].description, "\n")] = 0;

        totalConcepts++;
    }

    fclose(file);
}

void defineConcept() {
    int mode;
    printf("Select mode:\n1. Queue\n2. Stack\n> ");
    scanf("%d", &mode);

    srand(time(NULL));
    loadConcepts("concept.txt");

    if (totalConcepts == 0) {
        printf("No concept data loaded.\n");
        return;
    }

    int extractCount;
    printf("How many data sets to extract? (max %d): ", totalConcepts);
    scanf("%d", &extractCount);
    if (extractCount > totalConcepts) extractCount = totalConcepts;

    int indices[totalConcepts];
    for (int i = 0; i < totalConcepts; i++) indices[i] = i;
    shuffle(indices, totalConcepts);

    char** groupArr = (char**)malloc(sizeof(char*) * extractCount);
    char** conceptArr = (char**)malloc(sizeof(char*) * extractCount);

    for (int i = 0; i < extractCount; i++) {
        groupArr[i] = strdup(allConcepts[indices[i]].group);
        conceptArr[i] = strdup(allConcepts[indices[i]].concept);
    }

    shuffle(indices, extractCount);
    if (mode == 1) {
        Queue* groupQ = createQueue();
        Queue* conceptQ = createQueue();

        for (int i = 0; i < extractCount; i++) {
            enqueue(groupQ, groupArr[i]);
            enqueue(conceptQ, conceptArr[indices[i]]);
        }

        printf("## Mismatched Group and Concept ##\n");
        for (int i = 0; i < extractCount; i++) {
            char* g = dequeue(groupQ);
            char* c = dequeue(conceptQ);
            printf("%s: %s\n", g, c);
            free(g);
            free(c);
        }

        freeQueue(groupQ);
        freeQueue(conceptQ);
    } else if (mode == 2) {
        Stack* groupS = createStack();
        Stack* conceptS = createStack();

        for (int i = 0; i < extractCount; i++) {
            push(groupS, groupArr[i]);
            push(conceptS, conceptArr[indices[i]]);
        }

        printf("## Mismatched Group and Concept (Stack) ##\n");
        for (int i = 0; i < extractCount; i++) {
            char* g = pop(groupS);
            char* c = pop(conceptS);
            printf("%s: %s\n", g, c);
            free(g);
            free(c);
        }

        freeStack(groupS);
        freeStack(conceptS);
    }

    for (int i = 0; i < extractCount; i++) {
        free(groupArr[i]);
        free(conceptArr[i]);
    }
    free(groupArr);
    free(conceptArr);
    free(allConcepts);
    allConcepts = NULL;
}
