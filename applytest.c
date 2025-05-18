#include <stdio.h>
#include <stdlib.h>  /* For rand, srand */
#include <time.h>    /* For seeding rand */
#include <math.h>    /* Allowed library */

#define CAT_COUNT 9

/* Category names */
const char categories_01_arr[CAT_COUNT][20] = {
    "Physical Strength",
    "Knowledge",
    "Self-Management",
    "Teamwork",
    "Korean Language",
    "Vocal",
    "Dance",
    "Visual",
    "Acting"
};

/* Exam dates (YYYYMMDD), 0 if not scheduled */
int exam_dates_01_arr[CAT_COUNT] = {0};
/* Passed status */
int exam_passed_01_arr[CAT_COUNT] = {0};

/* Training start date */
int training_start_date_01 = 0;

/* Utility: Extract year, month, day from YYYYMMDD */
void parse_date(int yyyymmdd, int *year, int *month, int *day)
{
    *year = yyyymmdd / 10000;
    *month = (yyyymmdd / 100) % 100;
    *day = yyyymmdd % 100;
}

/* Days per month (non-leap year) */
const int days_in_month_arr[12] = {
    31,28,31,30,31,30,31,31,30,31,30,31
};

/* Check leap year */
int is_leap_year(int year)
{
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        return 1;
    return 0;
}

/* Convert date to "days since 0000-01-01" */
int date_to_days(int yyyymmdd)
{
    int y, m, d;
    int i, days = 0;

    parse_date(yyyymmdd, &y, &m, &d);

    /* Add days for years */
    days = y * 365 + (y / 4) - (y / 100) + (y / 400);  /* Approx leap years */

    /* Add days for months */
    for (i = 1; i < m; i++) {
        days += days_in_month_arr[i - 1];
        if (i == 2 && is_leap_year(y))
            days += 1;
    }

    /* Add days */
    days += d;

    return days;
}

/* Calculate difference in days between date1 and date2 */
int days_diff(int date1, int date2)
{
    return date_to_days(date2) - date_to_days(date1);
}

/* Check if a category is Physical Strength or Knowledge */
int is_initial_required_category(int idx)
{
    return (idx == 0 || idx == 1);  /* indices 0 and 1 */
}

/* Check if prerequisites (Self-Management and Teamwork) are passed */
int prerequisites_passed(void)
{
    /* Self-Management index 2, Teamwork index 3 */
    return (exam_passed_01_arr[2] && exam_passed_01_arr[3]);
}

/* Display scheduled exams */
void display_exam_schedule(void)
{
    int i;
    printf("\nScheduled Certification Exams:\n");
    for (i = 0; i < CAT_COUNT; i++) {
        if (exam_dates_01_arr[i] != 0) {
            printf("  %s: %08d\n", categories_01_arr[i], exam_dates_01_arr[i]);
        }
    }
}

/* Input exam date with validation */
int input_exam_date(int category_index)
{
    int date;
    int diff;
    while (1) {
        printf("Enter exam date for %s (YYYYMMDD): ", categories_01_arr[category_index]);
        scanf("%d", &date);
        getchar();  /* consume newline */

        diff = days_diff(training_start_date_01, date);
        if (diff < 30) {
            printf("Exam date must be at least 30 days after training start date.\n");
        } else if (diff > 100) {
            printf("Exam date must be within 100 days from training start date.\n");
        } else {
            return date;
        }
    }
}

/* Schedule an exam */
void schedule_exam(int idx)
{
    int date = input_exam_date(idx);
    exam_dates_01_arr[idx] = date;
    printf("Exam for %s scheduled on %08d.\n", categories_01_arr[idx], date);
}

/* Random exam venue assignment on exam day */
void assign_exam_venue(int idx)
{
    /* 10 rooms numbered 1 to 10 */
    int room;
    /* Seed rand with exam date + category index */
    srand(exam_dates_01_arr[idx] + idx);
    room = (rand() % 10) + 1;
    printf("Exam venue for %s on %08d: Room %d\n", categories_01_arr[idx], exam_dates_01_arr[idx], room);
}

/* Check and print reminders and exam venues */
void check_reminders_and_venues(int current_date)
{
    int i, diff;
    for (i = 0; i < CAT_COUNT; i++) {
        if (exam_dates_01_arr[i] != 0) {
            diff = days_diff(current_date, exam_dates_01_arr[i]);
            if (diff >= 0 && diff <= 10) {
                printf("Reminder: Exam for %s is in %d day(s) on %08d.\n",
                    categories_01_arr[i], diff, exam_dates_01_arr[i]);
            }
            if (diff == 0) {
                assign_exam_venue(i);
            }
        }
    }
}

/* Main logic */
int main(void)
{
    int i, category_choice, current_date;
    int can_schedule;
    int all_initial_passed = 0;

    printf("Enter training start date (YYYYMMDD): ");
    scanf("%d", &training_start_date_01);
    getchar();

    /* For demo, current date input */
    printf("Enter current date (YYYYMMDD): ");
    scanf("%d", &current_date);
    getchar();

    printf("\nCertification Categories:\n");
    for (i = 0; i < CAT_COUNT; i++) {
        printf("%d. %s\n", i + 1, categories_01_arr[i]);
    }

    /* Initial scheduling: Must schedule Physical Strength and Knowledge first */
    printf("\nSchedule initial required exams:\n");
    for (i = 0; i < CAT_COUNT; i++) {
        if (is_initial_required_category(i)) {
            schedule_exam(i);
        }
    }

    /* Mark Physical Strength and Knowledge as passed for demo */
    exam_passed_01_arr[0] = 1;
    exam_passed_01_arr[1] = 1;

    /* Schedule Self-Management and Teamwork */
    printf("\nSchedule Self-Management and Teamwork exams:\n");
    schedule_exam(2);
    schedule_exam(3);

    /* Mark Self-Management and Teamwork as passed for demo */
    exam_passed_01_arr[2] = 1;
    exam_passed_01_arr[3] = 1;

    /* Now schedule remaining exams in any order */
    printf("\nSchedule remaining exams (any order):\n");
    for (i = 4; i < CAT_COUNT; i++) {
        schedule_exam(i);
    }

    /* Display all scheduled exams */
    display_exam_schedule();

    /* Check reminders and venues based on current date */
    check_reminders_and_venues(current_date);

    return 0;
}
