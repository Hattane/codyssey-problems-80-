#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_WORD_LEN 32
#define MAX_LINE_LEN 256

typedef struct Word {
    char word[MAX_WORD_LEN];
} Word;

typedef struct WordNode {
    char word[MAX_WORD_LEN];
    int count;
    struct WordNode* next;
} WordNode;

// 함수 원형 (구현 필요)
int loadWordPool(const char* filename, Word** pool, int* poolSize);
void selectRandomWords(Word* pool, int poolSize, Word* selected, int selectCount, int minLen, int maxLen);
void printSelectedWords(Word* selected, int count);
char** readWritingInput(int lineCount);
void countWordUsage(Word* selected, int selectCount, char** lines, int lineCount, WordNode** head);
int checkAllWordsUsed(WordNode* head, int expectedCount);
void saveWritingToFile(const char* filename, char** lines, int lineCount);
void freeWordNodes(WordNode* head);
void freeWritingLines(char** lines, int lineCount);

void doCreativeWriting() {
    Word* wordPool = NULL;
    int poolSize = 0;
    int selectCount = 7;
    int minLen = 2, maxLen = 7; // 보너스 반영 시 사용자 입력으로 대체 가능

    if (loadWordPool("writing_words.txt", &wordPool, &poolSize) != 0) {
        printf("단어 풀 파일을 불러오는 데 실패했습니다.\n");
        return;
    }

    Word selected[20]; // 최대 20단어 선택 가능
    srand((unsigned int)time(NULL));

    // 무작위로 단어 선택
    selectRandomWords(wordPool, poolSize, selected, selectCount, minLen, maxLen);

    printSelectedWords(selected, selectCount);

    char** lines = NULL;
    WordNode* head = NULL;
    int allUsed = 0;

    do {
        if (lines != NULL) {
            freeWritingLines(lines, selectCount);
            freeWordNodes(head);
            head = NULL;
        }

        printf("\n글쓰기를 시작하세요 (줄 수: %d):\n", selectCount);
        lines = readWritingInput(selectCount);

        countWordUsage(selected, selectCount, lines, selectCount, &head);
        allUsed = checkAllWordsUsed(head, selectCount);

        if (!allUsed) {
            printf("모든 단어를 한 번 이상 사용해야 합니다. 다시 작성하세요.\n");
        }

    } while (!allUsed);

    saveWritingToFile("camelcase.txt", lines, selectCount);

    printf("작성 완료! 단어 사용 횟수:\n");
    WordNode* cur = head;
    while (cur != NULL) {
        printf("단어: %s, 사용 횟수: %d\n", cur->word, cur->count);
        cur = cur->next;
    }

    freeWritingLines(lines, selectCount);
    freeWordNodes(head);
    free(wordPool);
}
