#include <stdio.h>

#define MAX_MEMBERS 10
#define MAX_SELECTED_SUB_MENUS 5
#define MAX_NICK_LEN 20

const char MEMBERS[MAX_MEMBERS][2][MAX_NICK_LEN + 1] = {
    {"Kim Minseo", "Mins"},
    {"Lee Jihun", "Jihun"}
};
int MEMBER_COUNT = 2;

const char SUB_MENUS[][2][20] = {
    {"A", "멘토링"},
    {"B", "트라우마 관리"},
    {"D", "자기 성찰"},
    {"E", "여정 기록"},
    {"F", "감정 일기"}
};
int SUB_MENU_COUNT = sizeof(SUB_MENUS)/sizeof(SUB_MENUS[0]);

typedef struct {
    char nickname[MAX_NICK_LEN + 1];
    char subMenuIDs[MAX_SELECTED_SUB_MENUS];
    int subMenuCount;
} MyDashboard;

MyDashboard myDashboards[MAX_MEMBERS];  // 멤버별 대시보드 저장
int myDashboardCount = 0;

// 문자열 함수 (비교, 길이 등) 직접 구현 필요

int str_len(const char *str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

int str_cmp(const char *a, const char *b) {
    int i = 0;
    while(a[i] && b[i]) {
        if (a[i] != b[i]) return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}

// 닉네임 입력 함수 - 기존 문제 재사용 가능
int inputNickName(char *nickname);

// 대시보드 편집 함수
void selectSubMenu(const char *nickname) {
    printf("하위 메뉴 목록:\n");
    for (int i = 0; i < SUB_MENU_COUNT; i++) {
        printf("%s. %s\n", SUB_MENUS[i][0], SUB_MENUS[i][1]);
    }
    printf("저장할 메뉴 ID를 복수로 입력하세요 (예: A B F):\n");

    char input[100];
    fgets(input, sizeof(input), stdin);

    // 입력에서 메뉴 ID만 추출하여 저장 (구현 필요)
    // 예) 입력 토큰화 및 대시보드 저장 처리

    // myDashboards에 해당 nickname이 있으면 업데이트, 없으면 새로 추가
}

// 대시보드 출력 함수
void displayDashboard(const MyDashboard *dashboard) {
    printf("=== 대시보드: %s ===\n", dashboard->nickname);

    for (int i = 0; i < dashboard->subMenuCount; i++) {
        char id = dashboard->subMenuIDs[i];
        switch(id) {
            case 'A':
                printf("[멘토링 데이터 출력]\n");
                break;
            case 'B':
                printf("[트라우마 관리 데이터 출력]\n");
                break;
            case 'D':
                printf("[자기 성찰 데이터 출력]\n");
                break;
            case 'E':
                printf("[여정 기록 데이터 출력]\n");
                break;
            case 'F':
                printf("[감정 일기 데이터 출력]\n");
                break;
            default:
                break;
        }
        printf("\n");
    }
}

// 대시보드 메뉴 진입 함수
void showDashboard() {
    char nickname[21];
    if (inputNickName(nickname) != 0) {
        printf("닉네임 입력 실패. 프로그램 종료.\n");
        return;
    }

    // 내 대시보드 찾기
    int foundIndex = -1;
    for (int i = 0; i < myDashboardCount; i++) {
        if (str_cmp(myDashboards[i].nickname, nickname) == 0) {
            foundIndex = i;
            break;
        }
    }

    printf("1. 대시보드 편집\n2. 대시보드 표시\n선택: ");
    int sel;
    scanf("%d", &sel);
    getchar(); // 버퍼 비우기

    if (sel == 1) {
        selectSubMenu(nickname);
    } else if (sel == 2) {
        if (foundIndex == -1 || myDashboards[foundIndex].subMenuCount == 0) {
            printf("저장된 대시보드가 없습니다.\n");
        } else {
            displayDashboard(&myDashboards[foundIndex]);
        }
    }
}

int main() {
    // 메뉴 호출 예시
    showDashboard();
    return 0;
}
