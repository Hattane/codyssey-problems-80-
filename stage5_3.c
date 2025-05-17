#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATTERNS 100

typedef struct {
    int id;
    char patternName[64];
    char description[256];
} BreathingPattern;

static BreathingPattern patterns[MAX_PATTERNS];
static int patternCount = 0;

// 문자열에서 특정 키의 값을 추출하는 단순 함수
// ex) extractValue(line, "id", buffer) -> buffer에 id 값 복사
int extractValue(const char *line, const char *key, char *buffer, size_t bufsize) {
    const char *p = strstr(line, key);
    if (!p) return 0;
    p = strchr(p, ':');
    if (!p) return 0;
    p++;
    while (*p == ' ' || *p == '\"') p++;
    size_t i = 0;
    while (*p && *p != '\"' && *p != ',' && *p != '\n' && i < bufsize - 1) {
        buffer[i++] = *p++;
    }
    buffer[i] = '\0';
    return 1;
}

void loadBreathingPatterns(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Cannot open %s\n", filename);
        return;
    }

    char line[512];
    BreathingPattern bp;
    int reading = 0;

    patternCount = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "{")) {
            memset(&bp, 0, sizeof(bp));
            reading = 1;
        } else if (strstr(line, "}")) {
            if (reading && patternCount < MAX_PATTERNS) {
                patterns[patternCount++] = bp;
                reading = 0;
            }
        } else if (reading) {
            char val[256];
            if (extractValue(line, "id", val, sizeof(val))) {
                bp.id = atoi(val);
            } else if (extractValue(line, "patternName", val, sizeof(val))) {
                strncpy(bp.patternName, val, sizeof(bp.patternName)-1);
            } else if (extractValue(line, "description", val, sizeof(val))) {
                strncpy(bp.description, val, sizeof(bp.description)-1);
            }
        }
    }

    fclose(fp);
}

void printPatternList() {
    printf("Available Breathing Patterns:\n");
    for (int i = 0; i < patternCount; i++) {
        printf("  ID: %d - %s\n", patterns[i].id, patterns[i].patternName);
    }
}

int findPatternIndexById(int id) {
    for (int i = 0; i < patternCount; i++) {
        if (patterns[i].id == id) return i;
    }
    return -1;
}

void saveFavoritePattern(const BreathingPattern *bp) {
    FILE *fp = fopen("mymindfulness.json", "a");
    if (!fp) {
        printf("Error: Cannot open mymindfulness.json for writing\n");
        return;
    }

    // JSON 형식으로 저장 (description -> how-to 로 키 이름 변경)
    fprintf(fp, "{\n");
    fprintf(fp, "  \"id\": %d,\n", bp->id);
    fprintf(fp, "  \"patternName\": \"%s\",\n", bp->patternName);
    fprintf(fp, "  \"how-to\": \"%s\"\n", bp->description);
    fprintf(fp, "}\n");

    fclose(fp);
    printf("Pattern saved to favorites (mymindfulness.json).\n");
}

void guideMindfulnessBreathing() {
    loadBreathingPatterns("mindfulness_breathing.json");

    if (patternCount == 0) {
        printf("No breathing patterns loaded.\n");
        return;
    }

    char input[64];
    while (1) {
        printPatternList();
        printf("Enter pattern ID to view details (or 'q' to quit): ");
        if (!fgets(input, sizeof(input), stdin)) break;

        // 개행 문자 제거
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "q") == 0 || strcmp(input, "quit") == 0) {
            printf("Exiting mindfulness breathing guide.\n");
            break;
        }

        int id = atoi(input);
        int idx = findPatternIndexById(id);
        if (idx == -1) {
            printf("Invalid ID. Try again.\n");
            continue;
        }

        BreathingPattern *bp = &patterns[idx];
        printf("Pattern: %s\nDescription: %s\n", bp->patternName, bp->description);

        // 보너스: 즐겨찾기 등록 여부
        printf("Add to favorites? (y/n): ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = '\0';
        if (input[0] == 'y' || input[0] == 'Y') {
            saveFavoritePattern(bp);
        }
    }
}
