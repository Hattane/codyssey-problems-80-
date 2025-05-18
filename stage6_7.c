#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TICKETS 1000
#define AIRPORT_CODE_LENGTH 4

// 구조체 정의
typedef struct {
    char from[AIRPORT_CODE_LENGTH];
    char to[AIRPORT_CODE_LENGTH];
    bool used;
} Ticket;

typedef struct {
    Ticket *tickets;
    int size;
    int capacity;
} Tickets;

typedef struct {
    char **path;
    int size;
    int capacity;
} TravelPath;

// 함수 선언
Tickets *createTickets();
void addTicket(Tickets *t, const char *from, const char *to);
void freeTickets(Tickets *t);
void sortTickets(Tickets *t);

TravelPath *createTravelPath();
void addAirportToPath(TravelPath *tp, const char *airport);
void removeLastAirport(TravelPath *tp);
void freeTravelPath(TravelPath *tp);
void printTravelPath(const TravelPath *tp);

bool dfs(Tickets *t, TravelPath *tp, int depth);
void planOverseasTraining();

// 문자열 비교를 위한 정렬 함수
int compareTickets(const void *a, const void *b) {
    Ticket *t1 = (Ticket *)a;
    Ticket *t2 = (Ticket *)b;
    if (strcmp(t1->from, t2->from) == 0) {
        return strcmp(t1->to, t2->to);
    }
    return strcmp(t1->from, t2->from);
}

Tickets *createTickets() {
    Tickets *t = (Tickets *)malloc(sizeof(Tickets));
    t->size = 0;
    t->capacity = MAX_TICKETS;
    t->tickets = (Ticket *)malloc(sizeof(Ticket) * t->capacity);
    return t;
}

void addTicket(Tickets *t, const char *from, const char *to) {
    strcpy(t->tickets[t->size].from, from);
    strcpy(t->tickets[t->size].to, to);
    t->tickets[t->size].used = false;
    t->size++;
}

void freeTickets(Tickets *t) {
    free(t->tickets);
    free(t);
}

void sortTickets(Tickets *t) {
    qsort(t->tickets, t->size, sizeof(Ticket), compareTickets);
}

TravelPath *createTravelPath() {
    TravelPath *tp = (TravelPath *)malloc(sizeof(TravelPath));
    tp->size = 0;
    tp->capacity = MAX_TICKETS + 1;
    tp->path = (char **)malloc(sizeof(char *) * tp->capacity);
    return tp;
}

void addAirportToPath(TravelPath *tp, const char *airport) {
    tp->path[tp->size] = (char *)malloc(sizeof(char) * AIRPORT_CODE_LENGTH);
    strcpy(tp->path[tp->size], airport);
    tp->size++;
}

void removeLastAirport(TravelPath *tp) {
    if (tp->size > 0) {
        free(tp->path[tp->size - 1]);
        tp->size--;
    }
}

void freeTravelPath(TravelPath *tp) {
    for (int i = 0; i < tp->size; ++i) {
        free(tp->path[i]);
    }
    free(tp->path);
    free(tp);
}

void printTravelPath(const TravelPath *tp) {
    for (int i = 0; i < tp->size; ++i) {
        printf("%s\n", tp->path[i]);
    }
}

bool dfs(Tickets *t, TravelPath *tp, int depth) {
    if (depth == t->size) {
        return true;
    }

    for (int i = 0; i < t->size; ++i) {
        if (!t->tickets[i].used && strcmp(tp->path[tp->size - 1], t->tickets[i].from) == 0) {
            t->tickets[i].used = true;
            addAirportToPath(tp, t->tickets[i].to);

            if (dfs(t, tp, depth + 1)) {
                return true;
            }

            t->tickets[i].used = false;
            removeLastAirport(tp);
        }
    }

    return false;
}

void planOverseasTraining() {
    FILE *fp = fopen("tickets.txt", "r");
    if (!fp) {
        perror("파일 열기 실패");
        return;
    }

    Tickets *tickets = createTickets();
    char from[AIRPORT_CODE_LENGTH], to[AIRPORT_CODE_LENGTH];
    while (fscanf(fp, "%3s,%3s\n", from, to) == 2) {
        addTicket(tickets, from, to);
    }
    fclose(fp);

    sortTickets(tickets);

    TravelPath *path = createTravelPath();
    addAirportToPath(path, "ICN");

    if (dfs(tickets, path, 0)) {
        printTravelPath(path);
    } else {
        printf("유효한 경로를 찾을 수 없습니다.\n");
    }

    freeTickets(tickets);
    freeTravelPath(path);
}

