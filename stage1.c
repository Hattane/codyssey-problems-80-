#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMBERS 10
#define MAX_NAME 50

typedef struct {
    char name[MAX_NAME];
    float height, weight;
    int age;
} PhysicalCondition;

typedef struct {
    char day[10];
    char workout[100];
} WorkoutRoutine;

typedef struct {
    char day[10];
    char meal[100];
    int calories;
    int veganFriendly;
} DietPlan;

PhysicalCondition members[MAX_MEMBERS];
WorkoutRoutine workouts[MAX_MEMBERS][7];
DietPlan diets[MAX_MEMBERS][7];
int memberCount = 0;

void enterCondition() {
    if (memberCount >= MAX_MEMBERS) return;
    printf("Name: "); fgets(members[memberCount].name, MAX_NAME, stdin);
    members[memberCount].name[strcspn(members[memberCount].name, "\n")] = 0;
    printf("Height (cm): "); scanf("%f", &members[memberCount].height);
    printf("Weight (kg): "); scanf("%f", &members[memberCount].weight);
    printf("Age: "); scanf("%d", &members[memberCount].age);
    getchar();  // consume newline
    memberCount++;
}

void viewCondition() {
    for (int i = 0; i < memberCount; i++) {
        printf("[%d] %s - %.1fcm, %.1fkg, %d yrs\n", i+1,
            members[i].name, members[i].height, members[i].weight, members[i].age);
    }
}

void setWorkout() {
    int id; printf("Select member (0-%d): ", memberCount-1); scanf("%d", &id); getchar();
    for (int i = 0; i < 7; i++) {
        printf("Day %d workout: ", i+1);
        fgets(workouts[id][i].workout, 100, stdin);
        workouts[id][i].workout[strcspn(workouts[id][i].workout, "\n")] = 0;
    }
}

void viewWorkout() {
    int id; printf("Select member (0-%d): ", memberCount-1); scanf("%d", &id); getchar();
    for (int i = 0; i < 7; i++) {
        printf("Day %d: %s\n", i+1, workouts[id][i].workout);
    }
}

void setDiet() {
    int id; printf("Select member (0-%d): ", memberCount-1); scanf("%d", &id); getchar();
    for (int i = 0; i < 7; i++) {
        printf("Day %d meal: ", i+1);
        fgets(diets[id][i].meal, 100, stdin);
        diets[id][i].meal[strcspn(diets[id][i].meal, "\n")] = 0;
        printf("Calories: "); scanf("%d", &diets[id][i].calories);
        printf("Vegan friendly? (1-Yes, 0-No): "); scanf("%d", &diets[id][i].veganFriendly);
        getchar();  // consume newline
    }
}

void viewDiet() {
    int id; printf("Select member (0-%d): ", memberCount-1); scanf("%d", &id); getchar();
    for (int i = 0; i < 7; i++) {
        printf("Day %d: %s (%d cal, %s)\n", i+1, diets[id][i].meal,
            diets[id][i].calories,
            diets[id][i].veganFriendly ? "Vegan" : "Non-Vegan");
    }
}

void menu() {
    int choice;
    do {
        printf("\n1. Enter Physical Condition\n2. View Physical Condition\n3. Set Workout\n4. View Workout\n5. Set Diet\n6. View Diet\n0. Exit\n> ");
        scanf("%d", &choice); getchar();  // clear newline
        switch (choice) {
            case 1: enterCondition(); break;
            case 2: viewCondition(); break;
            case 3: setWorkout(); break;
            case 4: viewWorkout(); break;
            case 5: setDiet(); break;
            case 6: viewDiet(); break;
        }
    } while (choice != 0);
}

int main() {
    menu();
    return 0;
}
