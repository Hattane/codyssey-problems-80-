#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 후보자 기본 데이터 구조체 */
typedef struct {
    char name[30];
    char nickname[20];
    char education[20];
    double height;  // m 단위
    double weight;  // kg 단위, 초기값 0
    char blood_type;
    char allergy[20];
    char hobby[50];
    char sns[50];
    int id;         // 6자리 고유 ID
    int passed;     // 합격 여부 1:합격, 0:불합격
} candidate_t;

/* 밀리웨이즈 멤버 추가 정보 구조체 */
typedef struct {
    char name[30];
    char nickname[20];
    char education[20];
    double height;
    double weight;
    char blood_type;
    char allergy[20];
    char hobby[50];
    char sns[50];
    int id;
    // 추가 인터뷰 데이터 예: (임의로 추가)
    char interview_answer[100];
    double bmi;
} milliways_t;

/* BMI 상태 출력 함수 */
void print_bmi_status(double bmi) {
    int category;
    if (bmi < 18.5) {
        category = 0;  // 저체중
    } else if (bmi < 25.0) {
        category = 1;  // 정상
    } else if (bmi < 30.0) {
        category = 2;  // 과체중
    } else {
        category = 3;  // 비만
    }

    switch (category) {
    case 0: printf("저체중\n"); break;
    case 1: printf("정상\n"); break;
    case 2: printf("과체중\n"); break;
    case 3: printf("비만\n"); break;
    default: printf("정보 없음\n"); break;
    }
}

int main(void) {
    /* 문제 3, 5 후보자 데이터 (예시) */
    candidate_t candidate_arr[6] = {
        {"Jiyeon Park", "Ariel", "고1중퇴", 1.68, 0.0, 'A', "유제품", "댄스 연습, 작곡", "Instagram - @Ariel_Jiyeon", 123456, 1},
        {"Ethan Smith", "Simba", "중3중퇴", 1.78, 0.0, 'O', "땅콩", "노래 작곡, 헬스 트레이닝", "Twitter - @Simba_Ethan", 234567, 1},
        {"Helena Silva", "Belle", "중졸", 1.63, 0.0, 'B', "생선", "노래 부르기, 그림 그리기", "Instagram - @Belle_Helena", 345678, 1},
        {"Liam Wilson", "Aladdin", "중2중퇴", 1.75, 0.0, 'AB', "갑각류", "춤추기, 음악 프로듀싱", "Instagram - @Aladdin_Liam", 456789, 1},
        {"Mia Chen", "Mimi", "고3재학", 1.60, 0.0, 'A', "없음", "노래, 작곡", "Twitter - @Mimi_Chen", 567890, 0},
        {"Noah Lee", "Leo", "고2재학", 1.70, 0.0, 'B', "계란", "댄스, 음악", "Instagram - @Leo_Noah", 678901, 0}
    };

    /* 합격자만 저장할 배열 */
    milliways_t milliways_arr[4];
    int milliways_count = 0;
    int i;

    printf("밀리웨이즈 정규 멤버 인터뷰 추가 데이터 입력\n\n");

    /* 합격자 데이터 복사 및 인터뷰 입력 */
    for (i = 0; i < 6; i++) {
        if (candidate_arr[i].passed) {
            milliways_t *m = &milliways_arr[milliways_count];

            /* 기존 정보 복사 */
            strcpy(m->name, candidate_arr[i].name);
            strcpy(m->nickname, candidate_arr[i].nickname);
            strcpy(m->education, candidate_arr[i].education);
            m->height = candidate_arr[i].height;
            m->weight = 0.0;  // 나중 계산
            m->blood_type = candidate_arr[i].blood_type;
            strcpy(m->allergy, candidate_arr[i].allergy);
            strcpy(m->hobby, candidate_arr[i].hobby);
            strcpy(m->sns, candidate_arr[i].sns);
            m->id = candidate_arr[i].id;

            /* 인터뷰 추가 데이터 입력 */
            printf("%s 인터뷰 답변 입력: ", m->name);
            fgets(m->interview_answer, sizeof(m->interview_answer), stdin);
            /* fgets 개행 문자 제거 */
            size_t len = strlen(m->interview_answer);
            if (len > 0 && m->interview_answer[len-1] == '\n') {
                m->interview_answer[len-1] = '\0';
            }

            /* BMI 입력 */
            double bmi;
            do {
                printf("%s BMI 값 입력 (예: 22.5): ", m->name);
                if (scanf("%lf", &bmi) != 1) {
                    while(getchar() != '\n'); // 입력 버퍼 비우기
                    bmi = -1.0;
                }
                if (bmi < 10.0 || bmi > 50.0) {
                    printf("유효하지 않은 BMI 값입니다. 다시 입력하세요.\n");
                }
            } while (bmi < 10.0 || bmi > 50.0);
            while(getchar() != '\n'); // 개행 제거

            m->bmi = bmi;
            m->weight = bmi * m->height * m->height; /* 몸무게 계산 */

            milliways_count++;
        }
    }

    /* 인터뷰 및 BMI 결과 출력 */
    printf("\n================== 밀리웨이즈 정규 멤버 리스트 ==================\n");
    for (i = 0; i < milliways_count; i++) {
        milliways_t *m = &milliways_arr[i];
        printf("이름: %s (닉네임: %s)\n", m->name, m->nickname);
        printf("학력: %s, 키: %.2fm, 몸무게: %.2fkg, 혈액형: %c\n", m->education, m->height, m->weight, m->blood_type);
        printf("알러지: %s\n", m->allergy);
        printf("취미: %s\n", m->hobby);
        printf("SNS: %s\n", m->sns);
        printf("ID: %06d\n", m->id);
        printf("인터뷰 답변: %s\n", m->interview_answer);
        printf("BMI: %.2f - 상태: ", m->bmi);
        print_bmi_status(m->bmi);
        printf("-----------------------------------------\n");
    }

    return 0;
}
