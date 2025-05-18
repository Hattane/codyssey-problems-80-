#define MAX_WORDS 14
#define MAX_LINE_LEN 256
#define MAX_RESULTS 100

extern char *misunderstoodWords[MAX_WORDS];
extern int resultCount;

typedef struct {
    int lineNumber;
    char sentence[MAX_LINE_LEN];
} Result;

extern Result results[MAX_RESULTS];

void initMisunderstoodWords() {
    misunderstoodWords[0] = "항상";
    misunderstoodWords[1] = "절대";
    // ... 생략 ...
}

void computeLPSArray(const char* pat, int M, int* lps) {
    // KMP 전처리 함수 구현
}

int KMPSearch(const char* pat, const char* txt) {
    // KMP 탐색 함수 구현, 패턴 발견 시 1 반환
}

void writingManners() {
    initMisunderstoodWords();
    // 파일 열고 한 줄씩 읽기
    // 각 줄마다 misunderstoodWords 배열 반복하여 KMPSearch 실행
    // 발견 시 결과 저장
    // 저장된 결과 출력
}
