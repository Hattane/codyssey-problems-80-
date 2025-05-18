#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100
#define MAX_SONGS 10

typedef struct Schedule {
    char country[MAX_NAME];
    char region[MAX_NAME];
    int year, month, day, hour, minute;
    char concertName[MAX_NAME];
    int songCount;
    char songs[MAX_SONGS][MAX_NAME];
    struct Schedule* next;
} Schedule;

typedef struct StageList {
    char name[MAX_NAME];
    struct StageList* next;
} StageList;

Schedule* head = NULL;
StageList* domesticStages = NULL;
StageList* overseasStages = NULL;

void addStage(StageList** list, const char* name) {
    StageList* newNode = (StageList*)malloc(sizeof(StageList));
    strcpy(newNode->name, name);
    newNode->next = *list;
    *list = newNode;
}

void printSchedule(Schedule* s) {
    printf("국가: %s, 지역: %s, 날짜: %d-%02d-%02d %02d:%02d\n",
        s->country, s->region, s->year, s->month, s->day, s->hour, s->minute);
    printf("공연명: %s\n", s->concertName);
    printf("노래 목록:\n");
    for (int i = 0; i < s->songCount; i++) {
        printf("  %d. %s\n", i + 1, s->songs[i]);
    }
    printf("-----------------------\n");
}

int compareDate(Schedule* a, Schedule* b) {
    if (a->year != b->year) return a->year - b->year;
    if (a->month != b->month) return a->month - b->month;
    if (a->day != b->day) return a->day - b->day;
    if (a->hour != b->hour) return a->hour - b->hour;
    return a->minute - b->minute;
}

void addSchedule() {
    Schedule* newNode = (Schedule*)malloc(sizeof(Schedule));
    printf("국가 입력: ");
    scanf("%s", newNode->country);
    printf("지역 입력: ");
    scanf("%s", newNode->region);
    printf("년 월 일 입력 (예: 2025 5 18): ");
    scanf("%d %d %d", &newNode->year, &newNode->month, &newNode->day);
    printf("시간 분 입력 (예: 20 30): ");
    scanf("%d %d", &newNode->hour, &newNode->minute);
    printf("공연명 입력: ");
    getchar();  // 버퍼 비우기
    fgets(newNode->concertName, MAX_NAME, stdin);
    newNode->concertName[strcspn(newNode->concertName, "\n")] = 0; // 개행 제거

    printf("노래 수 입력 (최대 %d): ", MAX_SONGS);
    scanf("%d", &newNode->songCount);
    if(newNode->songCount > MAX_SONGS) newNode->songCount = MAX_SONGS;

    getchar(); // 버퍼 비우기
    for (int i = 0; i < newNode->songCount; i++) {
        printf("노래 %d 제목 입력: ", i + 1);
        fgets(newNode->songs[i], MAX_NAME, stdin);
        newNode->songs[i][strcspn(newNode->songs[i], "\n")] = 0; // 개행 제거
    }

    // 날짜 기준으로 정렬해서 연결 리스트에 삽입
    if (!head || compareDate(newNode, head) < 0) {
        newNode->next = head;
        head = newNode;
    } else {
        Schedule* p = head;
        while (p->next && compareDate(newNode, p->next) >= 0) {
            p = p->next;
        }
        newNode->next = p->next;
        p->next = newNode;
    }
    printf("일정 추가 완료!\n");
}

void searchSchedule() {
    if (!head) {
        printf("일정이 없습니다.\n");
        return;
    }
    printf("검색 방법 선택:\n1. 날짜\n2. 공연명\n선택: ");
    int option; scanf("%d", &option);
    getchar();

    int found = 0;
    if (option == 1) {
        int y, m, d;
        printf("검색할 날짜 입력 (예: 2025 5 18): ");
        scanf("%d %d %d", &y, &m, &d);

        Schedule* p = head;
        while (p) {
            if (p->year == y && p->month == m && p->day == d) {
                printSchedule(p);
                found = 1;
            }
            p = p->next;
        }
    } else if (option == 2) {
        char name[MAX_NAME];
        printf("검색할 공연명 입력: ");
        fgets(name, MAX_NAME, stdin);
        name[strcspn(name, "\n")] = 0;

        Schedule* p = head;
        while (p) {
            if (strstr(p->concertName, name)) {
                printSchedule(p);
                found = 1;
            }
            p = p->next;
        }
    } else {
        printf("잘못된 선택입니다.\n");
        return;
    }
    if (!found) printf("검색 결과가 없습니다.\n");
}

void modifySchedule() {
    if (!head) {
        printf("일정이 없습니다.\n");
        return;
    }
    int y, m, d;
    printf("수정할 일정 날짜 입력 (예: 2025 5 18): ");
    scanf("%d %d %d", &y, &m, &d);
    getchar();

    Schedule* p = head;
    while (p) {
        if (p->year == y && p->month == m && p->day == d) {
            printf("현재 일정:\n");
            printSchedule(p);
            printf("수정할 항목 선택:\n");
            printf("1. 국가\n2. 지역\n3. 공연명\n4. 날짜 및 시간\n5. 노래 목록\n선택: ");
            int choice; scanf("%d", &choice);
            getchar();
            switch (choice) {
                case 1:
                    printf("새 국가 입력: ");
                    scanf("%s", p->country);
                    break;
                case 2:
                    printf("새 지역 입력: ");
                    scanf("%s", p->region);
                    break;
                case 3:
                    printf("새 공연명 입력: ");
                    fgets(p->concertName, MAX_NAME, stdin);
                    p->concertName[strcspn(p->concertName, "\n")] = 0;
                    break;
                case 4:
                    printf("새 날짜 입력 (년 월 일): ");
                    scanf("%d %d %d", &p->year, &p->month, &p->day);
                    printf("새 시간 입력 (시 분): ");
                    scanf("%d %d", &p->hour, &p->minute);
                    break;
                case 5:
                    printf("노래 수 입력 (최대 %d): ", MAX_SONGS);
                    scanf("%d", &p->songCount);
                    if(p->songCount > MAX_SONGS) p->songCount = MAX_SONGS;
                    getchar();
                    for (int i = 0; i < p->songCount; i++) {
                        printf("노래 %d 제목 입력: ", i + 1);
                        fgets(p->songs[i], MAX_NAME, stdin);
                        p->songs[i][strcspn(p->songs[i], "\n")] = 0;
                    }
                    break;
                default:
                    printf("잘못된 선택입니다.\n");
                    return;
            }
            printf("수정 완료!\n");
            return;
        }
        p = p->next;
    }
    printf("해당 날짜의 일정이 없습니다.\n");
}

void deleteSchedule() {
    if (!head) {
        printf("일정이 없습니다.\n");
        return;
    }
    int y, m, d;
    printf("삭제할 일정 날짜 입력 (예: 2025 5 18): ");
    scanf("%d %d %d", &y, &m, &d);

    Schedule* p = head;
    Schedule* prev = NULL;
    while (p) {
        if (p->year == y && p->month == m && p->day == d) {
            if (prev == NULL) head = p->next;
            else prev->next = p->next;
            free(p);
            printf("삭제 완료!\n");
            return;
        }
        prev = p;
        p = p->next;
    }
    printf("해당 날짜의 일정이 없습니다.\n");
}

void printScheduleByPeriod() {
    if (!head) {
        printf("일정이 없습니다.\n");
        return;
    }
    int year, startMonth, endMonth;
    printf("연도 입력: ");
    scanf("%d", &year);
    printf("시작 월 입력: ");
    scanf("%d", &startMonth);
    printf("종료 월 입력: ");
    scanf("%d", &endMonth);
    if (startMonth > endMonth || startMonth < 1 || endMonth > 12) {
        printf("잘못된 월 입력입니다.\n");
        return;
    }
    int found = 0;
    Schedule* p = head;
    while (p) {
        if (p->year == year && p->month >= startMonth && p->month <= endMonth) {
            printSchedule(p);
            found = 1;
        }
        p = p->next;
    }
    if (!found) printf("해당 기간 내 일정이 없습니다.\n");
}

void saveToFile() {
    FILE* fp = fopen("schedule.txt", "w");
    if (!fp) {
        printf("파일 열기 실패\n");
        return;
    }
    Schedule* p = head;
    while (p) {
        fprintf(fp, "%s|%s|%d|%d|%d|%d|%d|%s|%d\n",
            p->country, p->region, p->year, p->month, p->day,
            p->hour, p->minute, p->concertName, p->songCount);
        for (int i = 0; i < p->songCount; i++) {
            fprintf(fp, "%s\n", p->songs[i]);
        }
        p = p->next;
    }
    fclose(fp);
    printf("파일 저장 완료.\n");
}

void loadFromFile() {
    FILE* fp = fopen("schedule.txt", "r");
    if (!fp) {
        printf("파일이 없습니다.\n");
        return;
    }
    // 기존 리스트 삭제
    Schedule* p = head;
    while (p) {
        Schedule* tmp = p;
        p = p->next;
        free(tmp);
    }
    head = NULL;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        Schedule* newNode = (Schedule*)malloc(sizeof(Schedule));
        char* token = strtok(line, "|");
        if (token) strcpy(newNode->country, token);
        token = strtok(NULL, "|");
        if (token) strcpy(newNode->region, token);
        token = strtok(NULL, "|");
        if (token) newNode->year = atoi(token);
        token = strtok(NULL, "|");
        if (token) newNode->month = atoi(token);
        token = strtok(NULL, "|");
        if (token) newNode->day = atoi(token);
        token = strtok(NULL, "|");
        if (token) newNode->hour = atoi(token);
        token = strtok(NULL, "|");
        if (token) newNode->minute = atoi(token);
        token = strtok(NULL, "|");
        if (token) strcpy(newNode->concertName, token);
        token = strtok(NULL, "\n");
        if (token) newNode->songCount = atoi(token);

        for (int i = 0; i < newNode->songCount; i++) {
            if (fgets(line, sizeof(line), fp)) {
                line[strcspn(line, "\n")] = 0; // 개행 제거
                strcpy(newNode->songs[i], line);
            }
        }
        newNode->next = NULL;
        if (!head) head = newNode;
        else {
            Schedule* tmp = head;
            while (tmp->next) tmp = tmp->next;
            tmp->next = newNode;
        }
    }
    fclose(fp);
    printf("파일 불러오기 완료.\n");
}

void printMenu() {
    printf("\n--- 공연 일정 관리 ---\n");
    printf("1. 일정 추가\n");
    printf("2. 일정 검색\n");
    printf("3. 일정 수정\n");
    printf("4. 일정 삭제\n");
    printf("5. 일정 기간별 출력\n");
    printf("6. 파일 저장\n");
    printf("7. 파일 불러오기\n");
    printf("8. 종료\n");
    printf("선택: ");
}

int main() {
    int choice;
    while (1) {
        printMenu();
        scanf("%d", &choice);
        getchar(); // 버퍼 비우기
        switch (choice) {
            case 1: addSchedule(); break;
            case 2: searchSchedule(); break;
            case 3: modifySchedule(); break;
            case 4: deleteSchedule(); break;
            case 5: printScheduleByPeriod(); break;
            case 6: saveToFile(); break;
            case 7: loadFromFile(); break;
            case 8:
                printf("프로그램 종료.\n");
                // 메모리 해제
                while (head) {
                    Schedule* tmp = head;
                    head = head->next;
                    free(tmp);
                }
                return 0;
            default:
                printf("잘못된 선택입니다.\n");
        }
    }
    return 0;
}
