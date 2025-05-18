// stage8.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stage8.h"

// Bad character heuristic for Boyer-Moore
static void buildBadCharTable(const char *pattern, int badCharTable[256]) {
    int len = (int)strlen(pattern);
    int i;
    for (i = 0; i < 256; i++) badCharTable[i] = -1;
    for (i = 0; i < len; i++) badCharTable[(unsigned char)pattern[i]] = i;
}

// Simple Boyer-Moore search with bad character rule only (for brevity)
static int boyerMoore(const char *text, const char *pattern) {
    int badCharTable[256];
    int n = (int)strlen(text);
    int m = (int)strlen(pattern);

    if (m == 0) return 0;
    if (m > n) return -1;

    buildBadCharTable(pattern, badCharTable);

    int s = 0; // shift of the pattern with respect to text
    while (s <= n - m) {
        int j = m - 1;

        while (j >= 0 && pattern[j] == text[s + j])
            j--;

        if (j < 0)
            return s; // match found

        else {
            int bcShift = j - badCharTable[(unsigned char)text[s + j]];
            s += (bcShift > 0) ? bcShift : 1;
        }
    }
    return -1; // no match
}

static void loadKeywords(const char *filename, Keywords *keywords) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening keyword file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LEN];
    keywords->negCount = 0;
    keywords->posCount = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Each line assumed format: NEG <word> or POS <word>
        char type[10], word[MAX_WORD_LEN];
        if (sscanf(line, "%s %s", type, word) != 2)
            continue;

        if (strcmp(type, "NEG") == 0 && keywords->negCount < MAX_KEYWORDS) {
            strcpy(keywords->negative[keywords->negCount++], word);
        } else if (strcmp(type, "POS") == 0 && keywords->posCount < MAX_KEYWORDS) {
            strcpy(keywords->positive[keywords->posCount++], word);
        }
    }
    fclose(fp);
}

void filterComment(void) {
    Keywords keywords;
    loadKeywords("bad_good_words.txt", &keywords);

    FILE *fp = fopen("fan_response.txt", "r");
    if (!fp) {
        perror("Error opening fan response file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LEN];
    int lineNum = 0;

    printf("=== 긍정 단어 포함 메시지 ===\n");
    while (fgets(line, sizeof(line), fp)) {
        lineNum++;

        // Remove trailing newline
        line[strcspn(line, "\r\n")] = 0;

        // Check for negative words
        int hasNegative = 0;
        for (int i = 0; i < keywords.negCount; i++) {
            if (boyerMoore(line, keywords.negative[i]) >= 0) {
                hasNegative = 1;
                break;
            }
        }
        if (hasNegative) continue; // skip line if negative word found

        // Check for positive words
        int hasPositive = 0;
        for (int i = 0; i < keywords.posCount; i++) {
            if (boyerMoore(line, keywords.positive[i]) >= 0) {
                hasPositive = 1;
                break;
            }
        }

        if (hasPositive)
            printf("%s\n", line);
    }
    fclose(fp);
}
