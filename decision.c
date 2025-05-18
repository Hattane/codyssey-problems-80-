/* decision.c */

#include <stdio.h>
#include <math.h>

#define NUM_CATEGORIES 9
#define MIN_SCORE 1
#define MAX_SCORE 11  // Scale of 11 points (0-10)
#define AUTO_DEBUT_MIN 95
#define AUTO_DEBUT_MAX 100

/* Enum for categories */
typedef enum {
    PHYSICAL_STRENGTH = 0,
    KNOWLEDGE,
    SELF_MANAGEMENT,
    TEAMWORK,
    KOREAN_LANGUAGE,
    VOCAL,
    DANCE,
    VISUAL,
    ACTING
} Category;

/* Function to get category name by enum */
const char* get_category_name(Category c) {
    switch (c) {
        case PHYSICAL_STRENGTH: return "Physical Strength";
        case KNOWLEDGE: return "Knowledge";
        case SELF_MANAGEMENT: return "Self-Management";
        case TEAMWORK: return "Teamwork";
        case KOREAN_LANGUAGE: return "Korean Language";
        case VOCAL: return "Vocal";
        case DANCE: return "Dance";
        case VISUAL: return "Visual";
        case ACTING: return "Acting";
        default: return "Unknown";
    }
}

/* Function to find three lowest scoring categories */
void find_three_lowest(int scores[], int* idx1, int* idx2, int* idx3) {
    int i;
    int first = -1, second = -1, third = -1;
    int min1 = 1000, min2 = 1000, min3 = 1000;

    for (i = 0; i < NUM_CATEGORIES; i++) {
        int score = scores[i];
        if (score < min1) {
            min3 = min2; third = second;
            min2 = min1; second = first;
            min1 = score; first = i;
        } else if (score < min2) {
            min3 = min2; third = second;
            min2 = score; second = i;
        } else if (score < min3) {
            min3 = score; third = i;
        }
    }
    *idx1 = first;
    *idx2 = second;
    *idx3 = third;
}

int main(void) {
    int num_members, i, j;
    int scores[NUM_CATEGORIES]; /* Store average team scores per category */
    int member_scores[NUM_CATEGORIES]; /* Temporary for input */
    int min_individual_score;

    /* Set team passing average and minimum individual scores */
    int team_passing_average;
    int automatic_debut_score;

    printf("Enter number of team members: ");
    if (scanf("%d", &num_members) != 1 || num_members < 1) {
        printf("Invalid number of members.\n");
        return 1;
    }

    printf("Set the team's average passing score (1-11): ");
    if (scanf("%d", &team_passing_average) != 1 || team_passing_average < 1 || team_passing_average > 11) {
        printf("Invalid team average passing score.\n");
        return 1;
    }

    printf("Set the minimum individual score (1-11): ");
    if (scanf("%d", &min_individual_score) != 1 || min_individual_score < 1 || min_individual_score > 11) {
        printf("Invalid minimum individual score.\n");
        return 1;
    }

    printf("Set the automatic debut score (95-100): ");
    if (scanf("%d", &automatic_debut_score) != 1 || automatic_debut_score < 95 || automatic_debut_score > 100) {
        printf("Invalid automatic debut score.\n");
        return 1;
    }

    /* Initialize scores array to zero */
    for (i = 0; i < NUM_CATEGORIES; i++) {
        scores[i] = 0;
    }

    /* For each category, input scores for each member */
    for (i = 0; i < NUM_CATEGORIES; i++) {
        int sum_scores = 0;
        printf("\nEnter scores for %s (each member score 0-10):\n", get_category_name(i));
        for (j = 0; j < num_members; j++) {
            printf(" Member %d: ", j + 1);
            if (scanf("%d", &member_scores[j]) != 1 || member_scores[j] < 0 || member_scores[j] > 10) {
                printf("Invalid score input.\n");
                return 1;
            }
            /* Check minimum individual score */
            if (member_scores[j] < min_individual_score) {
                printf("\nMinimum required score is %d.\n", min_individual_score);
                printf("All members must retake %s.\n", get_category_name(i));
                return 0; /* Terminate the program */
            }
            sum_scores += member_scores[j];
        }
        /* Calculate average and truncate decimal */
        scores[i] = sum_scores / num_members;
    }

    /* Calculate total score (sum of 9 categories) */
    int total_score = 0;
    for (i = 0; i < NUM_CATEGORIES; i++) {
        total_score += scores[i];
    }

    /* Ensure minimum total score is at least 1 */
    if (total_score < MIN_SCORE) {
        total_score = MIN_SCORE;
    }

    printf("\nTeam total score: %d / 100\n", total_score);

    /* Decision using switch and conditions */
    if (total_score >= automatic_debut_score && total_score <= AUTO_DEBUT_MAX) {
        printf("Congratulations! Your stage debut is confirmed!\n");
    } else if (total_score >= 90 && total_score < automatic_debut_score) {
        printf("The Debut Certification Team will hold a final evaluation meeting to make a decision.\n");
    } else if (total_score >= 80 && total_score < 90) {
        int idx1, idx2, idx3;
        find_three_lowest(scores, &idx1, &idx2, &idx3);
        printf("Retake the three lowest-scoring categories (%s, %s, %s) after 30 days.\n",
            get_category_name(idx1), get_category_name(idx2), get_category_name(idx3));
    } else {
        printf("Scores are below the threshold. Please review training.\n");
    }

    return 0;
}
