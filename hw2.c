#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>

int main() {
    char title[256];
    char column1[256];
    char column2[256];
    char buffer[256];
    char data[256][256];
    char name[256];
    int numbs[256];
    int numb;
    int stop = 0;

    int i;
    //READ IN THE TITLE
    printf("Enter a title for the data:\n");
    fgets(title, 256, stdin);
    for (i = 0; i < 256; i++) {
        if (title[i] == '\n') {
            title[i] = '\0';
        }
    }
    printf("You entered: %s\n\n", title);

    //READ IN THE COLUMN1 HEADER
    printf("Enter the column 1 header:\n");
    fgets(column1, 256, stdin);
    for (i = 0; i < 256; i++) {
        if (column1[i] == '\n') {
            column1[i] = '\0';
        }
    }
    printf("You entered: %s\n\n", column1);

    //READ IN THE COLUMN2 HEADER
    printf("Enter the column 2 header:\n");
    fgets(column2, 256, stdin);
    for (i = 0; i < 256; i++) {
        if (column2[i] == '\n') {
            column2[i] = '\0';
        }
    }
    printf("You entered: %s\n\n", column2);

    int j = 0;

    while (stop != 1) {
        printf("Enter a data point (-1 to stop input):\n");
        fgets(buffer, 256, stdin);
        //READ IN THE AUTHOR AND # OF NOVELS
        if (buffer[0] == '-') {
            printf("\n");
            stop = 1;
            break;
        }
        int count = 0;
        //CHECK THE NUMBER OF COMMAS
        for (i = 0; i < strlen(buffer); i++) {
            if (buffer[i] == ',') {
                count += 1;
            }
        }
        if (count == 0) {
            printf("Error: No comma in string.\n\n");
            continue;
        } else if (count > 1) {
            printf("Error: Too many commas in input.\n\n");
            continue;
        } else {
            char * temp1 = strtok(buffer, ",");
            char * temp2 = strtok(NULL, ",");
            strcpy(name, temp1);
            if (1 != sscanf(temp2, "%d", &numb) || numb == 0) {
                printf("Error: Comma not followed by an integer.\n\n");
                continue;
            }
            name[strlen(name)] = '\0';
            printf("Data string: %s\n", name);
            printf("Data integer: %d\n\n", numb);
            strcpy(data[j], name);
            numbs[j] = numb;
            j += 1;
        }

    }

    //PRINT THE DATA TABLE
    if (j == 0) {
        printf("%33s\n", title);
        printf("%-20s|%23s\n", column1, column2);
        char dash[45];
        memset(dash, '-', 45);
        dash[44] = '\0';
        printf("%s\n", dash);
        printf("\n");
    }

    if (j > 0) {
        printf("%33s\n", title);
        printf("%-20s|%23s\n", column1, column2);
        char dash[45];
        memset(dash, '-', 45);
        dash[44] = '\0';
        printf("%s\n", dash);

        for (i = 0; i < j; i++) {
            printf("%-20s|%23d\n", data[i], numbs[i]);
        }
        printf("\n");

        char star[256];
        for (i = 0; i < j; i++) {
            printf("%20s ", data[i]);
            memset(star, '*', numbs[i]);
            star[numbs[i]] = '\0';
            printf("%s\n", star);
        }
    }
}
