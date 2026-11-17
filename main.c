#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Log {
    char time[25];
    char user[30];
    char ip[20];
    char status[10];
};

int main() {
    FILE *fp;
    struct Log *logs;
    char line[150];
    int count = 0, i, j, failed;
    
    logs = malloc(100 * sizeof(struct Log));

    if (logs == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    fp = fopen("logs.txt", "r");

    if (fp == NULL) {
        printf("File not found\n");
        free(logs);
        return 1;
    }

    while (count < 100 && fgets(line, sizeof(line), fp) != NULL) {
        char *t = strtok(line, ",");
        char *u = strtok(NULL, ",");
        char *ip = strtok(NULL, ",");
        char *s = strtok(NULL, ",\r\n");

        if (t != NULL && u != NULL && ip != NULL && s != NULL) {
            if (strlen(t) < sizeof(logs[count].time) &&
                strlen(u) < sizeof(logs[count].user) &&
                strlen(ip) < sizeof(logs[count].ip) &&
                strlen(s) < sizeof(logs[count].status)) {

                strcpy(logs[count].time, t);
                strcpy(logs[count].user, u);
                strcpy(logs[count].ip, ip);
                strcpy(logs[count].status, s);
                count++;
            }
        }
    }

    fclose(fp);

    printf("Total records: %d\n", count);

    printf("\n--- Login Report ---\n");

    for (i = 0; i < count; i++) {
        printf("User: %s | IP: %s | Status: %s\n",
               logs[i].user, logs[i].ip, logs[i].status);
    }

    printf("\n--- Suspicious Login Report ---\n");

    for (i = 0; i < count; i++) {
        failed = 0;

        if (strcmp(logs[i].status, "FAILED") == 0) {
            for (j = 0; j < count; j++) {
                if (strcmp(logs[i].user, logs[j].user) == 0 &&
                    strcmp(logs[j].status, "FAILED") == 0) {
                    failed++;
                }
            }

            if (failed >= 3) {
                printf("Alert: %s has repeated failed logins\n",
                       logs[i].user);
                break;
            }
        }
    }

    free(logs);
    return 0;
}
