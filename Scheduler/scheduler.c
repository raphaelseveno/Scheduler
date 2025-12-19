#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLASSES 50
#define MAX_NAME_LEN 100
#define MAX_TIME_LEN 20
#define MAX_SCHEDULE 20

typedef struct {
    char name[MAX_NAME_LEN];
    char time[MAX_TIME_LEN];
    char day[MAX_TIME_LEN];
} Class;

typedef struct {
    char className[MAX_NAME_LEN];
    char firstChoice[MAX_TIME_LEN];
    char secondChoice[MAX_TIME_LEN];
} Preference;

typedef struct {
    char className[MAX_NAME_LEN];
    char time[MAX_TIME_LEN];
    char day[MAX_TIME_LEN];
    int isFirstChoice;
} ScheduledClass;

/*
 * Reads class data from CSV file.
 * Expected format: name,day,time
 *
 * Returns: Number of classes read, or -1 on error
 */
int readClasses(const char *filename, Class classes[]) {
    FILE *file = fopen(filename, "r");

    if (!file) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        return -1;
    }

    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) && count < MAX_CLASSES) {
        if (line[0] == '#' || line[0] == '\n') continue;

        char *token = strtok(line, ",");
        if (token) {
            strncpy(classes[count].name, token, MAX_NAME_LEN - 1);
            classes[count].name[strcspn(classes[count].name, "\n")] = 0;

            token = strtok(NULL, ",");
            if (token) {
                strncpy(classes[count].day, token, MAX_TIME_LEN - 1);
                classes[count].day[strcspn(classes[count].day, "\n")] = 0;

                token = strtok(NULL, ",");
                if (token) {
                    strncpy(classes[count].time, token, MAX_TIME_LEN - 1);
                    classes[count].time[strcspn(classes[count].time, "\n")] = 0;
                    count++;
                }
            }
        }
    }

    fclose(file);
    return count;
}

/*
 * Reads user preferences from CSV file.
 * Expected format: className,firstChoice,secondChoice
 *
 * Returns: Number of preferences read, or -1 on error
 */
int readPreferences(const char *filename, Preference prefs[]) {
    FILE *file = fopen(filename, "r");

    if (!file) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", filename);
        return -1;
    }

    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) && count < MAX_SCHEDULE) {
        if (line[0] == '#' || line[0] == '\n') continue;

        char *token = strtok(line, ",");
        if (token) {
            strncpy(prefs[count].className, token, MAX_NAME_LEN - 1);
            prefs[count].className[strcspn(prefs[count].className, "\n")] = 0;

            token = strtok(NULL, ",");
            if (token) {
                strncpy(prefs[count].firstChoice, token, MAX_TIME_LEN - 1);
                prefs[count].firstChoice[strcspn(prefs[count].firstChoice, "\n")] = 0;

                token = strtok(NULL, ",");
                if (token) {
                    strncpy(prefs[count].secondChoice, token, MAX_TIME_LEN - 1);
                    prefs[count].secondChoice[strcspn(prefs[count].secondChoice, "\n")] = 0;
                    count++;
                }
            }
        }
    }

    fclose(file);
    return count;
}

/*
 * Checks for scheduling conflicts.
 *
 * Returns: 1 if conflict exists, 0 otherwise
 */
int hasConflict(ScheduledClass schedule[], int scheduleSize, const char *day, const char *time) {
    for (int i = 0; i < scheduleSize; i++) {
        if (strcmp(schedule[i].day, day) == 0 && strcmp(schedule[i].time, time) == 0) {
            return 1;
        }
    }
    return 0;
}

/*
 * Finds a class by name and time.
 *
 * Returns: Pointer to matching class, or NULL if not found
 */
Class* findClass(Class classes[], int classCount, const char *name, const char *time) {
    for (int i = 0; i < classCount; i++) {
        if (strcmp(classes[i].name, name) == 0 && strcmp(classes[i].time, time) == 0) {
            return &classes[i];
        }
    }
    return NULL;
}

/*
 * Generates schedule from user preferences with conflict resolution.
 * Attempts first choice, falls back to second choice if conflict detected.
 * Updates scheduleSize with number of successfully scheduled classes.
 */
void generateSchedule(Class classes[], int classCount, Preference prefs[], int prefCount,
                     ScheduledClass schedule[], int *scheduleSize) {
    *scheduleSize = 0;

    for (int i = 0; i < prefCount; i++) {
        Class *selectedClass = NULL;
        int isFirst = 0;

        selectedClass = findClass(classes, classCount, prefs[i].className, prefs[i].firstChoice);

        if (selectedClass && !hasConflict(schedule, *scheduleSize, selectedClass->day, selectedClass->time)) {
            isFirst = 1;
        } else {
            selectedClass = findClass(classes, classCount, prefs[i].className, prefs[i].secondChoice);

            if (selectedClass && hasConflict(schedule, *scheduleSize, selectedClass->day, selectedClass->time)) {
                selectedClass = NULL;
            }
        }

        if (selectedClass) {
            strcpy(schedule[*scheduleSize].className, selectedClass->name);
            strcpy(schedule[*scheduleSize].day, selectedClass->day);
            strcpy(schedule[*scheduleSize].time, selectedClass->time);
            schedule[*scheduleSize].isFirstChoice = isFirst;
            (*scheduleSize)++;
        } else {
            printf("Attention: Impossible de programmer %s (conflits détectés)\n", prefs[i].className);
        }
    }
}

/*
 * Prints formatted schedule to stdout, organized by day.
 */
void printSchedule(ScheduledClass schedule[], int scheduleSize) {
    printf("\n========== VOTRE HORAIRE GÉNÉRÉ ==========\n\n");

    if (scheduleSize == 0) {
        printf("Aucun cours programmé.\n");
        return;
    }

    const char *days[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi"};
    int dayCount = 5;

    for (int d = 0; d < dayCount; d++) {
        int hasClasses = 0;

        for (int i = 0; i < scheduleSize; i++) {
            if (strcmp(schedule[i].day, days[d]) == 0) {
                hasClasses = 1;
                break;
            }
        }

        if (hasClasses) {
            printf("%s:\n", days[d]);

            for (int i = 0; i < scheduleSize; i++) {
                if (strcmp(schedule[i].day, days[d]) == 0) {
                    printf("  %-30s %s %s\n",
                           schedule[i].className,
                           schedule[i].time,
                           schedule[i].isFirstChoice ? "[1er choix]" : "[2ème choix]");
                }
            }
            printf("\n");
        }
    }

    printf("=============================================\n");
}

/*
 * Writes formatted schedule to file, organized by day.
 */
void saveSchedule(ScheduledClass schedule[], int scheduleSize, const char *filename) {
    FILE *file = fopen(filename, "w");

    if (!file) {
        printf("Erreur: Impossible de créer le fichier de sortie %s\n", filename);
        return;
    }

    fprintf(file, "========== VOTRE HORAIRE GÉNÉRÉ ==========\n\n");

    const char *days[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi"};
    int dayCount = 5;

    for (int d = 0; d < dayCount; d++) {
        int hasClasses = 0;

        for (int i = 0; i < scheduleSize; i++) {
            if (strcmp(schedule[i].day, days[d]) == 0) {
                hasClasses = 1;
                break;
            }
        }

        if (hasClasses) {
            fprintf(file, "%s:\n", days[d]);

            for (int i = 0; i < scheduleSize; i++) {
                if (strcmp(schedule[i].day, days[d]) == 0) {
                    fprintf(file, "  %-30s %s %s\n",
                           schedule[i].className,
                           schedule[i].time,
                           schedule[i].isFirstChoice ? "[1er choix]" : "[2ème choix]");
                }
            }
            fprintf(file, "\n");
        }
    }

    fprintf(file, "=============================================\n");
    fclose(file);
    printf("Horaire sauvegardé dans %s\n", filename);
}

/*
 * Entry point. Orchestrates file parsing, schedule generation, and output.
 * Usage: ./scheduler <classes_file> <preferences_file>
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <fichier_cours> <fichier_préférences>\n", argv[0]);
        printf("Exemple: %s classes.txt preferences.txt\n", argv[0]);
        return 1;
    }

    Class classes[MAX_CLASSES];
    Preference prefs[MAX_SCHEDULE];
    ScheduledClass schedule[MAX_SCHEDULE];
    int scheduleSize = 0;

    printf("Lecture des cours disponibles depuis %s...\n", argv[1]);
    int classCount = readClasses(argv[1], classes);

    if (classCount < 0) {
        return 1;
    }

    printf("Chargé %d options de cours.\n", classCount);
    printf("Lecture de vos préférences depuis %s...\n", argv[2]);

    int prefCount = readPreferences(argv[2], prefs);

    if (prefCount < 0) {
        return 1;
    }

    printf("Chargé %d préférences.\n", prefCount);
    printf("\nGénération de l'horaire...\n");

    generateSchedule(classes, classCount, prefs, prefCount, schedule, &scheduleSize);
    printSchedule(schedule, scheduleSize);
    saveSchedule(schedule, scheduleSize, "mon_horaire.txt");

    return 0;
}
