#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ROUTINES 100
#define MAX_LEN 100

typedef struct {
    char date[MAX_LEN];
    char weather[MAX_LEN];
    char timePeriod[MAX_LEN]; // before meal, after meal, pre-performance
    char exerciseType[MAX_LEN];
    int duration; // in minutes
    int caloriesBurned;
} WorkoutRoutine;

WorkoutRoutine routines[MAX_ROUTINES];
int routineCount = 0;

void setMyWorkoutRoutine() {
    int n;
    printf("Enter number of workout routines: ");
    scanf("%d", &n);
    getchar();

    for (int i = 0; i < n && routineCount < MAX_ROUTINES; i++) {
        printf("Workout #%d\n", i+1);
        printf("Date (YYYY-MM-DD): ");
        fgets(routines[routineCount].date, MAX_LEN, stdin);
        routines[routineCount].date[strcspn(routines[routineCount].date, "\n")] = 0;

        printf("Weather: ");
        fgets(routines[routineCount].weather, MAX_LEN, stdin);
        routines[routineCount].weather[strcspn(routines[routineCount].weather, "\n")] = 0;

        printf("Time period (before meal / after meal / pre-performance): ");
        fgets(routines[routineCount].timePeriod, MAX_LEN, stdin);
        routines[routineCount].timePeriod[strcspn(routines[routineCount].timePeriod, "\n")] = 0;

        printf("Exercise type: ");
        fgets(routines[routineCount].exerciseType, MAX_LEN, stdin);
        routines[routineCount].exerciseType[strcspn(routines[routineCount].exerciseType, "\n")] = 0;

        printf("Duration (minutes): ");
        scanf("%d", &routines[routineCount].duration);

        printf("Calories burned: ");
        scanf("%d", &routines[routineCount].caloriesBurned);
        getchar();

        routineCount++;
    }

    printf("\nSaved Workout Routines:\n");
    for (int i = 0; i < routineCount; i++) {
        printf("%d. %s | %s | %s | %s | %d min | %d cal\n",
            i+1,
            routines[i].date,
            routines[i].weather,
            routines[i].timePeriod,
            routines[i].exerciseType,
            routines[i].duration,
            routines[i].caloriesBurned);
    }
}

void statsMyWorkoutRoutine() {
    char searchDate[MAX_LEN];
    char searchPeriod[MAX_LEN];

    printf("Enter date (YYYY-MM-DD): ");
    fgets(searchDate, MAX_LEN, stdin);
    searchDate[strcspn(searchDate, "\n")] = 0;
    printf("Enter time period: ");
    fgets(searchPeriod, MAX_LEN, stdin);
    searchPeriod[strcspn(searchPeriod, "\n")] = 0;

    int totalCaloriesPerDate[MAX_ROUTINES] = {0};
    char dateList[MAX_ROUTINES][MAX_LEN];
    int dateIndex = 0;

    char mostFrequentExercise[MAX_LEN] = "";
    int maxCount = 0;

    char exerciseTypes[MAX_ROUTINES][MAX_LEN];
    int exerciseCounts[MAX_ROUTINES] = {0};
    int exerciseIndex = 0;

    char mostWorkedDate[MAX_LEN] = "";
    int maxDuration = 0;

    printf("\nWorkouts on %s (%s):\n", searchDate, searchPeriod);
    for (int i = 0; i < routineCount; i++) {
        if (strcmp(routines[i].date, searchDate) == 0 && strcmp(routines[i].timePeriod, searchPeriod) == 0) {
            printf("- %s: %d minutes\n", routines[i].exerciseType, routines[i].duration);
        }

        // Aggregate calories
        int found = 0;
        for (int j = 0; j < dateIndex; j++) {
            if (strcmp(dateList[j], routines[i].date) == 0) {
                totalCaloriesPerDate[j] += routines[i].caloriesBurned;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(dateList[dateIndex], routines[i].date);
            totalCaloriesPerDate[dateIndex] = routines[i].caloriesBurned;
            dateIndex++;
        }

        // Most frequent exercise type
        found = 0;
        for (int j = 0; j < exerciseIndex; j++) {
            if (strcmp(exerciseTypes[j], routines[i].exerciseType) == 0) {
                exerciseCounts[j]++;
                if (exerciseCounts[j] > maxCount) {
                    maxCount = exerciseCounts[j];
                    strcpy(mostFrequentExercise, exerciseTypes[j]);
                }
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(exerciseTypes[exerciseIndex], routines[i].exerciseType);
            exerciseCounts[exerciseIndex] = 1;
            if (exerciseCounts[exerciseIndex] > maxCount) {
                maxCount = exerciseCounts[exerciseIndex];
                strcpy(mostFrequentExercise, routines[i].exerciseType);
            }
            exerciseIndex++;
        }

        // Most workout time per date
        int dayDuration = 0;
        for (int j = 0; j < routineCount; j++) {
            if (strcmp(routines[j].date, routines[i].date) == 0) {
                dayDuration += routines[j].duration;
            }
        }
        if (dayDuration > maxDuration) {
            maxDuration = dayDuration;
            strcpy(mostWorkedDate, routines[i].date);
        }
    }

    printf("\nCalories Burned per Date:\n");
    for (int i = 0; i < dateIndex; i++) {
        printf("%s: %d cal\n", dateList[i], totalCaloriesPerDate[i]);
    }

    printf("\nMost Frequent Exercise Type: %s\n", mostFrequentExercise);
    printf("Day with Longest Workout Time: %s (%d min)\n", mostWorkedDate, maxDuration);
}  


