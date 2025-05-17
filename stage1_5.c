#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define SUBJECTS 5

typedef struct {
    char name[50];
    char nickname[50];
    int age;
    char gender;
    int grade;
} Student;

typedef struct {
    char nickname[50];
    float scores[SUBJECTS];
    float weights[SUBJECTS];
    float average;
} Grade;

Student students[MAX_STUDENTS];
Grade grades[MAX_STUDENTS];
int student_count = 0;

char* subjects[SUBJECTS] = {"English", "Math", "Korean", "Social", "Science"};

void inputCommonLearningInfo() {
    int i, j;
    printf("\n[공통 교육 정보 입력]\n");
    printf("학생 수를 입력하세요: ");
    scanf("%d", &student_count);

    for (i = 0; i < student_count; i++) {
        printf("\n[%d번째 학생 정보]\n", i + 1);
        printf("이름: "); scanf("%s", students[i].name);
        printf("닉네임: "); scanf("%s", students[i].nickname);
        printf("나이: "); scanf("%d", &students[i].age);
        printf("성별(M/F): "); scanf(" %c", &students[i].gender);
        students[i].grade = students[i].age - 6;

        strcpy(grades[i].nickname, students[i].nickname);
        float total = 0;
        float weighted_sum = 0;
        printf("과목별 점수 및 가중치 입력 (예: 85 0.2):\n");
        for (j = 0; j < SUBJECTS; j++) {
            printf("%s: ", subjects[j]);
            scanf("%f %f", &grades[i].scores[j], &grades[i].weights[j]);
            weighted_sum += grades[i].scores[j] * grades[i].weights[j];
        }
        grades[i].average = (int)(weighted_sum * 10 + 0.5) / 10.0; // round to 1 decimal
    }
}

void printStudentInfo() {
    printf("\n[닉네임 기준 내림차순 출력]\n");
    for (int i = 0; i < student_count - 1; i++) {
        for (int j = i + 1; j < student_count; j++) {
            if (strcmp(students[i].nickname, students[j].nickname) < 0) {
                Student tmp = students[i]; students[i] = students[j]; students[j] = tmp;
            }
        }
    }
    for (int i = 0; i < student_count; i++) {
        printf("이름: %s, 닉네임: %s, 나이: %d, 성별: %c, 학년: %d\n",
               students[i].name, students[i].nickname,
               students[i].age, students[i].gender, students[i].grade);
    }
}

void printAscendAvgGrade() {
    printf("\n[평균 성적 기준 오름차순 출력]\n");
    for (int i = 0; i < student_count - 1; i++) {
        for (int j = i + 1; j < student_count; j++) {
            if (grades[i].average > grades[j].average) {
                Grade tmp = grades[i]; grades[i] = grades[j]; grades[j] = tmp;
            }
        }
    }
    for (int i = 0; i < student_count; i++) {
        printf("닉네임: %s, 평균 성적: %.1f\n", grades[i].nickname, grades[i].average);
    }
}

void printDescendHighWeightGrade() {
    printf("\n[가중치 높은 과목 기준 내림차순 출력]\n");
    for (int s = 0; s < SUBJECTS; s++) {
        printf("\n과목: %s\n", subjects[s]);
        for (int i = 0; i < student_count - 1; i++) {
            for (int j = i + 1; j < student_count; j++) {
                if (grades[i].weights[s] < grades[j].weights[s]) {
                    Grade tmp = grades[i]; grades[i] = grades[j]; grades[j] = tmp;
                }
            }
        }
        for (int i = 0; i < student_count; i++) {
            printf("닉네임: %s, 점수: %.1f, 가중치: %.2f\n",
                   grades[i].nickname, grades[i].scores[s], grades[i].weights[s]);
        }
    }
}

void printExcludeMaxMinGrade() {
    int s;
    printf("\n[과목 번호 입력 (0:English ~ 4:Science)]: ");
    scanf("%d", &s);

    float max = grades[0].scores[s], min = grades[0].scores[s];
    for (int i = 1; i < student_count; i++) {
        if (grades[i].scores[s] > max) max = grades[i].scores[s];
        if (grades[i].scores[s] < min) min = grades[i].scores[s];
    }
    printf("\n[%s 성적 (최대/최소 제외)]\n", subjects[s]);
    for (int i = 0; i < student_count; i++) {
        if (grades[i].scores[s] != max && grades[i].scores[s] != min) {
            printf("닉네임: %s, 점수: %.1f\n", grades[i].nickname, grades[i].scores[s]);
        }
    }
}

void obfuscateName(char* name) {
    printf("[이름 난독화]: ");
    for (int i = 0; name[i]; i++) {
        printf("%d ", name[i]);
    }
    printf("\n");
}

void obfuscateNickname(char* nickname) {
    printf("[닉네임 난독화]: ");
    for (int i = strlen(nickname) - 1; i >= 0; i--) {
        printf("%c", nickname[i]);
    }
    printf("\n");
}

void inquiryCommonLearningInfo() {
    printStudentInfo();
    printAscendAvgGrade();
    printDescendHighWeightGrade();
    printExcludeMaxMinGrade();

    // 보너스 난독화 예시 출력
    printf("\n[보너스: 난독화된 이름 및 닉네임]\n");
    for (int i = 0; i < student_count; i++) {
        obfuscateName(students[i].name);
        obfuscateNickname(students[i].nickname);
    }
}
