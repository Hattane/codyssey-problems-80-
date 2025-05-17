#include <stdio.h>
#include <stdlib.h>

// 최대 상수 정의
#define MAX_ANSWER_LEN 41
#define NUM_QUESTIONS 7
#define MAX_NICK_LEN 20
#define MAX_MEMBERS 10

// 멤버 정보 (예시)
const char MEMBERS[MAX_MEMBERS][2][MAX_NICK_LEN + 1] = {
    {"Kim Minseo", "Mins"},
    {"Lee Jihun", "Jihun"},
    // ...
};
const int MEMBER_COUNT = 2;

// 질문 구조체 및 상수 질문
typedef struct {
    int id;
    char question[100];
} EmotionQuestion;

const EmotionQuestion QUESTIONS[NUM_QUESTIONS] = {
    {1, "최근에 어떤 일이 있었나요? 그 일에 대한 감정을 어떻게 느꼈나요?"},
    {2, "지난 몇 일 동안 기분은 어땠나요?"},
    {3, "현재 기분이 어떤가요? 기쁨, 슬픔, 분노, 불안 등 어떤 감정을 느끼고 있나요?"},
    {4, "어떤 상황에서 감정을 더 강하게 느끼나요?"},
    {5, "감정적인 변화나 스트레스 요인이 최근에 있었나요?"},
    {6, "감정을 표현하는 물리적인 증상이 있나요? (예: 눈물, 웃음, 심장이 빠르게 뛰는 소리 등)"},
    {7, "감정을 다루는 방식에 대해 어떤 생각이나 전략이 있나요?"}
};

// 감정 일기 구조체
typedef struct {
    char nickname[MAX_NICK_LEN + 1];
    char answers[NUM_QUESTIONS][MAX_ANSWER_LEN];
} EmotionDiary;

// 함수 원형 선언
int inputNickName(char *nickname);
void writeEmotionDiary(EmotionDiary *diary);
void printEmotionDiary(const EmotionDiary *diary);

// 문자열 함수 직접 구현 예시
int str_len(const char *str) {
    int len = 0;
    while(str[len] != '\0') len++;
    return len;
}

int str_cmp(const char *a, const char *b) {
    int i = 0;
    while(a[i] && b[i]) {
        if(a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
        i++;
    }
    return (unsigned char)a[i] - (unsigned char)b[i];
}

// 등등...

int main(void) {
    // 메뉴 호출 등 전체 프로그램 로직
    return 0;
}
