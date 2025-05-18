void understandFashion() {
    Fashion *maleStyles = NULL, *femaleStyles = NULL;
    int maleCount = 0, femaleCount = 0;
    loadFashionStyles("fashion.txt", &maleStyles, &maleCount, &femaleStyles, &femaleCount);

    Member *members = NULL;
    int memberCount = 0;
    loadMembers("milliways_member.csv", &members, &memberCount);

    char nickname[50];
    printf("닉네임을 입력하세요: ");
    scanf("%s", nickname);

    char* gender = findGenderByNickname(members, memberCount, nickname);
    if (!gender) {
        printf("해당 닉네임의 멤버를 찾을 수 없습니다.\n");
        return;
    }

    Fashion* styles = strcmp(gender, "남") == 0 ? maleStyles : femaleStyles;
    int count = strcmp(gender, "남") == 0 ? maleCount : femaleCount;

    for (int i = 0; i < count; i++)
        printf("%d. %s - %s\n", i + 1, styles[i].style, styles[i].reason);

    // 무작위 기준 스타일 선택
    int center = rand() % count;
    printf("기준 패션 스타일: %s\n", styles[center].style);

    printf("앞에 채울까요(1) 아니면 뒤에 채울까요(2)? ");
    int choice;
    scanf("%d", &choice);

    Deque* dq = createDeque(7);
    if (choice == 1) {
        for (int i = center - 1; i >= 0; i--) addFront(dq, &styles[i]);
        addRear(dq, &styles[center]);
        for (int i = center + 1; i < count && !isFull(dq); i++) addRear(dq, &styles[i]);
    } else {
        for (int i = center + 1; i < count; i++) addRear(dq, &styles[i]);
        addFront(dq, &styles[center]);
        for (int i = center - 1; i >= 0 && !isFull(dq); i--) addFront(dq, &styles[i]);
    }

    printf("🎉 덱이 완성되었습니다! 다음은 채운 결과입니다:\n");
    printDeque(dq);

    // Clean up
    freeDeque(dq);
    free(maleStyles); free(femaleStyles); free(members);
}
