#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NAME_LEN 30
#define MAX_NICK_LEN 20
#define MAX_GIFT_LEN 100
#define MAX_BASE64_LEN 200

typedef struct {
    char name[MAX_NAME_LEN];
    char nickname[MAX_NICK_LEN];
} Member;

typedef struct {
    char senderName[MAX_NAME_LEN];
    char gift[MAX_GIFT_LEN];
    char encodedData[MAX_BASE64_LEN];
} ManitoInfo;

/* --- 문자열 함수 직접 구현 --- */
int my_strlen(const char *s) {
    int len = 0;
    while(s[len] != '\0') len++;
    return len;
}

int my_strcmp(const char *a, const char *b) {
    int i = 0;
    while(a[i] != '\0' && b[i] != '\0') {
        if(a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
        i++;
    }
    return (unsigned char)a[i] - (unsigned char)b[i];
}

/* --- Base64 인코딩 --- */
const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void base64_encode(const unsigned char *input, int len, char *output) {
    int i = 0, j = 0;
    unsigned char arr3[3];
    unsigned char arr4[4];

    while(len--) {
        arr3[i++] = *(input++);
        if(i == 3) {
            arr4[0] = (arr3[0] & 0xfc) >> 2;
            arr4[1] = ((arr3[0] & 0x03) << 4) + ((arr3[1] & 0xf0) >> 4);
            arr4[2] = ((arr3[1] & 0x0f) << 2) + ((arr3[2] & 0xc0) >> 6);
            arr4[3] = arr3[2] & 0x3f;

            for(i=0; i<4; i++) output[j++] = base64_table[arr4[i]];
            i = 0;
        }
    }

    if(i) {
        int k;
        for(k=i; k<3; k++) arr3[k] = '\0';

        arr4[0] = (arr3[0] & 0xfc) >> 2;
        arr4[1] = ((arr3[0] & 0x03) << 4) + ((arr3[1] & 0xf0) >> 4);
        arr4[2] = ((arr3[1] & 0x0f) << 2) + ((arr3[2] & 0xc0) >> 6);
        arr4[3] = arr3[2] & 0x3f;

        for(k=0; k<i+1; k++) output[j++] = base64_table[arr4[k]];
        while(i++ < 3) output[j++] = '=';
    }
    output[j] = '\0';
}

/* --- Base64 디코딩 보조 함수 --- */
int base64_char_value(char c) {
    if(c >= 'A' && c <= 'Z') return c - 'A';
    if(c >= 'a' && c <= 'z') return c - 'a' + 26;
    if(c >= '0' && c <= '9') return c - '0' + 52;
    if(c == '+') return 62;
    if(c == '/') return 63;
    return -1;
}

void base64_decode(const char *input, char *output) {
    int i = 0, j = 0;
    unsigned char arr4[4], arr3[3];
    int input_len = my_strlen(input);
    int in_idx = 0;

    while(input_len-- && input[in_idx] != '=') {
        int val = base64_char_value(input[in_idx]);
        if(val == -1) {
            in_idx++;
            continue;
        }
        arr4[i++] = val;
        in_idx++;
        if(i ==4) {
            arr3[0] = (arr4[0] << 2) + ((arr4[1] & 0x30) >> 4);
            arr3[1] = ((arr4[1] & 0xf) << 4) + ((arr4[2] & 0x3c) >> 2);
            arr3[2] = ((arr4[2] & 0x3) << 6) + arr4[3];

            for(i=0; i<3; i++) output[j++] = arr3[i];
            i=0;
        }
    }

    if(i) {
        int k;
        for(k=i; k<4; k++) arr4[k] = 0;

        arr3[0] = (arr4[0] << 2) + ((arr4[1] & 0x30) >> 4);
        arr3[1] = ((arr4[1] & 0xf) << 4) + ((arr4[2] & 0x3c) >> 2);
        arr3[2] = ((arr4[2] & 0x3) << 6) + arr4[3];

        for(k=0; k<i-1; k++) output[j++] = arr3[k];
    }
    output[j] = '\0';
}

/* --- 멤버 닉네임으로 인덱스 찾기 --- */
int findMemberIndexByNickname(Member *members, int count, const char *nickname) {
    int i;
    for(i=0; i<count; i++) {
        if(my_strcmp(members[i].nickname, nickname) == 0) return i;
    }
    return -1;
}

/* --- 마니또 놀이 함수 --- */
void playManito(Member members[], int memberCount) {
    ManitoInfo info;
    char nickname[MAX_NICK_LEN];
    char inputEncoded[MAX_BASE64_LEN];
    int senderIndex, targetIndex;

    printf("[마니또 놀이 시작]\n");

    // 마니또 역할 닉네임 입력
    printf("마니또 역할 멤버 닉네임 입력: ");
    scanf("%s", nickname);
    senderIndex = findMemberIndexByNickname(members, memberCount, nickname);
    if(senderIndex == -1) {
        printf("멤버가 아닙니다.\n");
        return;
    }

    // 선물 입력
    printf("선물 내용 입력: ");
    getchar();  // 버퍼 개행 제거
    int i = 0;
    char ch;
    while(i < MAX_GIFT_LEN-1) {
        ch = getchar();
        if(ch == '\n' || ch == EOF) break;
        info.gift[i++] = ch;
    }
    info.gift[i] = '\0';

    // 원본 문자열 생성 (이름:선물)
    char original[MAX_NAME_LEN + MAX_GIFT_LEN + 2];
    int j = 0;
    for(i=0; members[senderIndex].name[i] != '\0'; i++) {
        original[j++] = members[senderIndex].name[i];
    }
    original[j++] = ':';
    for(i=0; info.gift[i] != '\0'; i++) {
        original[j++] = info.gift[i];
    }
    original[j] = '\0';

    // Base64 인코딩
    base64_encode((unsigned char*)original, my_strlen(original), info.encodedData);

    // 무작위 대상 뽑기 (본인 제외)
    srand((unsigned int)time(NULL));
    do {
        targetIndex = rand() % memberCount;
    } while(targetIndex == senderIndex);

    // 대상 닉네임 입력 및 확인 반복
    while(1) {
        char inputTarget[MAX_NICK_LEN];
        printf("대상 닉네임 입력: ");
        scanf("%s", inputTarget);
        if(my_strcmp(inputTarget, members[targetIndex].nickname) == 0) {
            printf("인코딩된 데이터: %s\n", info.encodedData);
            break;
        } else {
            printf("잘못된 대상입니다. 다시 시도하세요.\n");
        }
    }

    // 인코딩된 데이터 입력 후 디코딩 출력
    printf("인코딩된 데이터 입력: ");
    scanf("%s", inputEncoded);
    char decoded[MAX_NAME_LEN + MAX_GIFT_LEN + 2];
    base64_decode(inputEncoded, decoded);
    printf("디코딩된 데이터: %s\n", decoded);
}

/* --- 메인 함수 예시 --- */
int main(void) {
    Member members[4] = {
        {"아서", "arthur"},
        {"수잰", "susan"},
        {"마크", "mark"},
        {"엘리", "elly"}
    };

    playManito(members, 4);

    return 0;
}
