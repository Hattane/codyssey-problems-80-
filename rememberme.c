#include <stdio.h>
#include <string.h>

#define MAX_CANDIDATES 6
#define MAX_MESSAGES 20
#define MAX_NAME_LEN 50
#define MAX_MSG_LEN 200
#define MAX_EMAIL_LEN 50

/* Failed candidates data: name, audition, email (comma separated) */
/* Immutable */
const char memorial_01_arr[MAX_CANDIDATES][200] = {
    "Alice Kim,milliways,alice.kim@example.com",
    "Bob Lee,milliways,bob.lee@example.com",
    "Charlie Park,milliways,charlie.park@example.com",
    "Diana Choi,milliways,diana.choi@example.com",
    "Evan Jo,milliways,evan.jo@example.com",
    "Fiona Shin,milliways,fiona.shin@example.com"
};

/* Messages to failed candidates */
/* For simplicity, store recipient names separately */
char recipients_01_arr[MAX_MESSAGES][MAX_NAME_LEN] = {
    "Alice Kim", "Bob Lee", "Alice Kim", "Diana Choi", "Charlie Park"
};

char rollingpp_01_arr[MAX_MESSAGES][MAX_MSG_LEN] = {
    "{\"Simba\": \"Keep your head up!\"}",
    "{\"Belle\": \"You did great!\"}",
    "{\"Ariel\": \"Hope to see you again!\"}",
    "{\"Aladdin\": \"Don't give up!\"}",
    "{\"Simba\": \"You'll come back stronger!\"}"
};

int message_count = 5;

void list_failed_candidates(void)
{
    int i;
    printf("Failed Candidates:\n");
    for (i = 0; i < MAX_CANDIDATES; i++) {
        char name[MAX_NAME_LEN] = {0};
        /* Extract name from memorial */
        sscanf(memorial_01_arr[i], "%[^,]", name);
        printf("%d. %s\n", i + 1, name);
    }
}

int find_failed_candidate_index(const char *name)
{
    int i;
    char cand_name[MAX_NAME_LEN] = {0};
    for (i = 0; i < MAX_CANDIDATES; i++) {
        sscanf(memorial_01_arr[i], "%[^,]", cand_name);
        if (strcmp(cand_name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void print_messages_for_candidate(const char *candidate)
{
    int i, found = 0;
    printf("Messages for %s:\n", candidate);
    for (i = 0; i < message_count; i++) {
        if (strcmp(recipients_01_arr[i], candidate) == 0) {
            printf("  %s\n", rollingpp_01_arr[i]);
            found = 1;
        }
    }
    if (!found)
        printf("  No messages found.\n");
}

void print_email_and_confirm(const char *candidate)
{
    int idx = find_failed_candidate_index(candidate);
    if (idx == -1) {
        printf("Candidate not found.\n");
        return;
    }
    char email[MAX_EMAIL_LEN] = {0};
    /* Extract email from memorial */
    sscanf(memorial_01_arr[idx], "%*[^,],%*[^,],%s", email);

    printf("\nPreparing to send email to: %s\n", email);
    printf("Email content:\n");
    print_messages_for_candidate(candidate);

    printf("\nSend email? (y/n): ");
    char ans = getchar();
    while (getchar() != '\n');  /* Clear input buffer */
    if (ans == 'y' || ans == 'Y') {
        printf("Email has been sent.\n");
    } else {
        printf("Email sending cancelled.\n");
    }
}

int main(void)
{
    char candidate_name[MAX_NAME_LEN] = {0};
    int idx;

    list_failed_candidates();

    printf("\nEnter candidate name to view messages: ");
    fgets(candidate_name, sizeof(candidate_name), stdin);
    candidate_name[strcspn(candidate_name, "\n")] = 0;  /* Remove newline */

    idx = find_failed_candidate_index(candidate_name);
    if (idx == -1) {
        printf("Candidate not found.\n");
        return 1;
    }

    print_messages_for_candidate(candidate_name);

    /* Bonus: send email */
    print_email_and_confirm(candidate_name);

    return 0;
}
