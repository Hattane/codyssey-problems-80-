#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_GROUP_NAME_LEN 50
#define MAX_MEMBERS 50
#define MAX_TESTS_PER_DAY 3

typedef struct {
    int bpm;            // 60,80,120
    char rhythm[5];     // "2/4", "3/4", "4/4", "6/8"
} TempoRhythm;

typedef struct {
    char memberName[MAX_NAME_LEN];
    char testDate[11];  // YYMMDDHH format
    int targetBeatCount; 
    double elapsedTime; // seconds
} BeatTestRecord;

typedef struct {
    char groupName[MAX_GROUP_NAME_LEN];
    char members[MAX_MEMBERS][MAX_NAME_LEN];
    int memberCount;
} TraineeGroup;

// 불변 템포와 리듬 데이터
const TempoRhythm TEMPO_RHYTHMS[] = {
    {60, "2/4"}, {60, "3/4"}, {60, "4/4"}, {60, "6/8"},
    {80, "2/4"}, {80, "3/4"}, {80, "4/4"}, {80, "6/8"},
    {120, "2/4"}, {120, "3/4"}, {120, "4/4"}, {120, "6/8"},
};
const int TEMPO_RHYTHM_COUNT = sizeof(TEMPO_RHYTHMS) / sizeof(TEMPO_RHYTHMS[0]);

// 현재 날짜시간 YYMMDDHH 형식 생성
void getCurrentDateYYMMDDHH(char *buffer, size_t size) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    snprintf(buffer, size, "%02d%02d%02d%02d",
        (tm_info->tm_year + 1900) % 100,
        tm_info->tm_mon + 1,
        tm_info->tm_mday,
        tm_info->tm_hour);
}

// trainee.csv에서 멤버 존재 여부 확인
int isValidTrainee(const char *name) {
    FILE *fp = fopen("trainee.csv", "r");
    if (!fp) {
        printf("Error: trainee.csv 파일을 열 수 없습니다.\n");
        return 0;
    }
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",\n\r");
        if (token && strcmp(token, name) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// newtrainees.csv에서 그룹 이름에 맞는 멤버 목록 불러오기
int loadTraineeGroup(const char *groupName, TraineeGroup *group) {
    FILE *fp = fopen("newtrainees.csv", "r");
    if (!fp) {
        printf("Error: newtrainees.csv 파일을 열 수 없습니다.\n");
        return 0;
    }
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",\n\r");
        if (token && strcmp(token, groupName) == 0) {
            strncpy(group->groupName, groupName, MAX_GROUP_NAME_LEN - 1);
            group->memberCount = 0;
            while ((token = strtok(NULL, ",\n\r")) != NULL && group->memberCount < MAX_MEMBERS) {
                strncpy(group->members[group->memberCount], token, MAX_NAME_LEN - 1);
                group->memberCount++;
            }
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// 하루에 테스트 몇 번 했는지 계산 (beat_test_result.txt 분석)
int countTestsToday(const char *memberName) {
    FILE *fp = fopen("beat_test_result.txt", "r");
    if (!fp) {
        return 0;
    }
    char line[200];
    int count = 0;
    char now[11];
    char today[7];
    getCurrentDateYYMMDDHH(now, sizeof(now));
    strncpy(today, now, 6);
    today[6] = '\0';

    while (fgets(line, sizeof(line), fp)) {
        char fileName[MAX_NAME_LEN];
        char fileDate[11];
        if (sscanf(line, "%[^|]|%10[^|]|%*d|%*lf", fileName, fileDate) == 2) {
            if (strcmp(fileName, memberName) == 0 && strncmp(fileDate, today, 6) == 0) {
                count++;
            }
        }
    }
    fclose(fp);
    return count;
}

// 리듬과 템포로 60초 동안 세야할 박자 수 계산
int calculateTargetBeatCount(const TempoRhythm *tr) {
    int numerator = 0, denominator = 1;
    if (sscanf(tr->rhythm, "%d/%d", &numerator, &denominator) != 2) {
        numerator = 4; denominator = 4;
    }
    double beatsPerSecond = (double)tr->bpm * ((double)numerator / denominator) / 60.0;
    return (int)(beatsPerSecond * 60.0 + 0.5);
}

// 연습 기록을 파일에 저장
void writeFile(const BeatTestRecord *record) {
    FILE *fp = fopen("beat_test_result.txt", "a");
    if (!fp) {
        printf("Error: beat_test_result.txt 파일을 열 수 없습니다.\n");
        return;
    }
    fprintf(fp, "%s|%s|%d|%.2lf\n", record->memberName, record->testDate,
            record->targetBeatCount, record->elapsedTime);
    fclose(fp);
}

// 결과 출력
void printResult(const BeatTestRecord *record) {
    double expectedTime = 60.0; // 60초 기준
    double diff = record->elapsedTime - expectedTime;

    printf("\n=== 테스트 결과 ===\n");
    printf("대상 박자 수: %d\n", record->targetBeatCount);
    printf("걸린 시간: %.2lf 초\n", record->elapsedTime);
    if (diff > 0.1) {
        printf("초과: %.2lf 초 초과하셨습니다.\n", diff);
    } else if (diff < -0.1) {
        printf("미달: %.2lf 초 부족하셨습니다.\n", -diff);
    } else {
        printf("박자 감각이 정확합니다!\n");
    }
}

// 연습생 이름 입력 (trainee.csv에서 확인)
int inputTrainee(char *memberName) {
    printf("연습생 이름을 입력하세요: ");
    if (!fgets(memberName, MAX_NAME_LEN, stdin)) return 0;
    memberName[strcspn(memberName, "\n")] = 0;

    if (!isValidTrainee(memberName)) {
        printf("등록된 연습생 이름이 아닙니다. 다시 시도하세요.\n");
        return 0;
    }
    return 1;
}

// 박자 테스트 수행
void testBeat(const char *memberName) {
    if (countTestsToday(memberName) >= MAX_TESTS_PER_DAY) {
        printf("하루 최대 3회 테스트가 완료되었습니다.\n");
        return;
    }

    srand((unsigned int)time(NULL));
    int idx = rand() % TEMPO_RHYTHM_COUNT;
    const TempoRhythm *tr = &TEMPO_RHYTHMS[idx];
    int targetBeats = calculateTargetBeatCount(tr);

    printf("\n=== 박자 테스트 ===\n");
    printf("템포: %d BPM, 리듬: %s\n", tr->bpm, tr->rhythm);
    printf("60초 동안 세야할 박자 수: %d\n", targetBeats);
    printf("테스트를 시작하려면 Enter 키를 누르세요...");
    getchar();

    time_t start = time(NULL);

    printf("대상 박자 수를 모두 세었다면 Enter 키를 누르세요.\n");
    getchar();

    time_t end = time(NULL);
    double elapsed = difftime(end, start);

    BeatTestRecord record;
    strncpy(record.memberName, memberName, MAX_NAME_LEN - 1);
    getCurrentDateYYMMDDHH(record.testDate, sizeof(record.testDate));
    record.targetBeatCount = targetBeats;
    record.elapsedTime = elapsed;

    writeFile(&record);
    printResult(&record);
}

// 그룹 선택 후 멤버 선택 함수 (보너스)
int selectMemberByGroup(char *selectedMember) {
    char groupName[MAX_GROUP_NAME_LEN];
    printf("연습생 그룹 이름을 입력하세요: ");
    if (!fgets(groupName, MAX_GROUP_NAME_LEN, stdin)) return 0;
    groupName[strcspn(groupName, "\n")] = 0;

    TraineeGroup group;
    if (!loadTraineeGroup(groupName, &group)) {
        printf("해당 그룹이 존재하지 않습니다.\n");
        return 0;
    }

    printf("그룹 [%s] 멤버 목록:\n", group.groupName);
    for (int i = 0; i < group.memberCount; i++) {
        printf("%d. %s\n", i + 1, group.members[i]);
    }

    int choice = 0;
    printf("테스트할 멤버 번호를 선택하세요 (1-%d): ", group.memberCount);
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > group.memberCount) {
        printf("잘못된 선택입니다.\n");
        while (getchar() != '\n'); // 입력 버퍼 클리어
        return 0;
    }
    while (getchar() != '\n'); // 버퍼 비우기

    strncpy(selectedMember, group.members[choice - 1], MAX_NAME_LEN - 1);
    return 1;
}

// 개선된 박자 감각 향상 기능 메인
void improveSenseOfBeat(void) {
    printf("=== 밀리웨이즈 박자 감각 향상 프로그램 ===\n");
    printf("1. 직접 연습생 이름 입력\n");
    printf("2. 연습생 그룹 선택 후 멤버 선택\n");
    printf("선택하세요(1 or 2): ");

    int mode = 0;
    if (scanf("%d", &mode) != 1 || (mode != 1 && mode != 2)) {
        printf("잘못된 선택입니다.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n'); // 입력 버퍼 비우기

    char memberName[MAX_NAME_LEN] = {0};

    if (mode == 1) {
        if (!inputTrainee(memberName)) {
            return;
        }
    } else {
        if (!selectMemberByGroup(memberName)) {
            return;
        }
        // 그룹 멤버는 trainee.csv와 동일한지 확인하는 추가 로직 필요시 추가 가능
        if (!isValidTrainee(member
