#include <stdio.h>
#include <stdlib.h>
#include "debut.h"

// 큐 함수 구현
void initQueue(Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int isEmpty(Queue* q) {
    return (q->front == NULL);
}

void enqueue(Queue* q, Installment item) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    newNode->data = item;
    newNode->next = NULL;

    if (isEmpty(q)) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
}

Installment dequeue(Queue* q) {
    Installment ret = {0,0,0,0,0};
    if (isEmpty(q)) {
        printf("큐가 비어있습니다.\n");
        return ret;
    }
    Node* temp = q->front;
    ret = temp->data;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
    q->size--;
    return ret;
}

// 납부정보 출력
void printInstallment(Installment* inst) {
    printf("납부 월 수: %d\n", inst->month);
    printf("할부 잔액: %.2f\n", inst->remaining);
    printf("할부 원금: %.2f\n", inst->principal);
    printf("할부 수수료: %.2f\n", inst->fee);
    printf("월 납부액: %.2f\n\n", inst->payment);
}

void studyMoney(void) {
    char productName[100];
    double principal, feeRate;
    int months;

    printf("구매 상품 명: ");
    scanf(" %[^\n]", productName);
    printf("원금 (원): ");
    scanf("%lf", &principal);
    printf("할부 개월: ");
    scanf("%d", &months);
    printf("할부 수수료율 (예: 0.05 = 5%%): ");
    scanf("%lf", &feeRate);

    double totalFee = principal * feeRate;
    double totalAmount = principal + totalFee;
    double monthlyPayment = totalAmount / months;

    Queue queue;
    initQueue(&queue);

    printf("\n월별 납부 정보 계산 및 큐에 저장 중...\n\n");

    for (int i = 1; i <= months; i++) {
        Installment inst;
        inst.month = i;
        inst.principal = principal / months;
        inst.fee = totalFee / months;
        inst.payment = monthlyPayment;
        inst.remaining = totalAmount - monthlyPayment * i;

        if (inst.remaining < 0)
            inst.remaining = 0;

        enqueue(&queue, inst);
    }

    // 납부 시뮬레이션: 매달 납부 실행 및 출력
    printf("=== 납부 시뮬레이션 시작 ===\n\n");
    while (!isEmpty(&queue)) {
        Installment current = dequeue(&queue);
        printInstallment(&current);
        printf("다음 달 납부를 진행하려면 Enter 키를 누르세요...");
        getchar(); // 앞서 scanf 남은 개행 제거용
        getchar();
    }
    printf("모든 할부 납부가 완료되었습니다.\n");
}
