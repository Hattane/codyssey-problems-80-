FanMember* createFanMember(const char* name, int age, char gender) {
    FanMember* newMember = malloc(sizeof(FanMember));
    if (!newMember) return NULL;
    strncpy(newMember->name, name, sizeof(newMember->name)-1);
    newMember->name[sizeof(newMember->name)-1] = '\0';
    newMember->age = age;
    newMember->gender = gender;
    newMember->next = NULL;
    return newMember;
}

int addMember(const char* name, int age, char gender) {
    unsigned int idx = hashFunction(name);
    FanMember* cur = hashMap[idx];
    while (cur) {
        if (strcmp(cur->name, name) == 0) return 0; // 이미 존재
        cur = cur->next;
    }
    FanMember* newMember = createFanMember(name, age, gender);
    if (!newMember) return -1; // 메모리 오류
    newMember->next = hashMap[idx];
    hashMap[idx] = newMember;
    return 1;
}

FanMember* findMember(const char* name) {
    unsigned int idx = hashFunction(name);
    FanMember* cur = hashMap[idx];
    while (cur) {
        if (strcmp(cur->name, name) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

int deleteMember(const char* name) {
    unsigned int idx = hashFunction(name);
    FanMember* cur = hashMap[idx];
    FanMember* prev = NULL;
    while (cur) {
        if (strcmp(cur->name, name) == 0) {
            if (prev) prev->next = cur->next;
            else hashMap[idx] = cur->next;
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0; // 없음
}
