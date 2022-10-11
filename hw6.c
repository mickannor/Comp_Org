#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CPU "CPU Cycles ===>\t1\t2\t3\t4\t5\t6\t7\t8\t9\n"

void print_no_hazards(int i,char * print[5][9],char inst[5][25],char * stages[5]) {
    int x,y,z;
    printf("%s", CPU);
    y = 1;
    for (x = 0; x <= y; ++x) {
        print[x][y] = stages[y-x];
    }
    for (x = 0; x < i; ++x) {
        printf("%s\t",inst[x]);
        for(z = 0; z < 9; ++z) {
            printf("%s", print[x][z]);
        }
        printf("\n");
    }
    printf("\n");

    printf("%s", CPU);
    y += 1;
    for (x = 0; x <= y; ++x) {
        print[x][y] = stages[y-x];
    }
    for (x = 0; x < i; ++x) {
        printf("%s\t",inst[x]);
        for (z = 0; z < 9; ++z) {
            printf("%s", print[x][z]);
        }
        printf("\n");
    }
    printf("\n");

    printf("%s", CPU);
    y += 1;
    for (x = 0; x <= y; ++x) {
        print[x][y] = stages[y-x];
    }
    for (x = 0; x < i; ++x) {
        printf("%s\t",inst[x]);
        for (z = 0; z < 9; ++z) {
            printf("%s", print[x][z]);
        }
        printf("\n");
    }
    printf("\n");

    printf("%s", CPU);
    y += 1;
    for (x = 0; x <= y; ++x) {
        print[x][y] = stages[y-x];
    }
    for (x = 0; x < i; ++x) {
        printf("%s\t",inst[x]);
        for (z = 0; z < 9; ++z) {
            printf("%s", print[x][z]);
        }
        printf("\n");
    }
    printf("\n");

    printf("%s", CPU);
    y += 1;
    for (x = 1; x <= y; ++x) {
        print[x][y] = stages[y-x];
    }
    for (x = 0; x < i; ++x) {
        printf("%s\t",inst[x]);
        for (z = 0; z < 9; ++z) {
            printf("%s", print[x][z]);
        }
        printf("\n");
    }
    printf("\n");

    printf("%s", CPU);
    y += 1;
    for (x = 2; x < 5; ++x) {
        print[x][y] = stages[y-x];
    }
    for (x = 0; x < i; ++x) {
        printf("%s\t",inst[x]);
        for (z = 0; z < 9; ++z) {
            printf("%s", print[x][z]);
        }
        printf("\n");
    }
    printf("\n");

    printf("%s", CPU);
    y += 1;
    for (x = 3; x < 5; ++x) {
        print[x][y] = stages[y-x];
    }
    for (x = 0; x < i; ++x) {
        printf("%s\t",inst[x]);
        for (z = 0; z < 9; ++z) {
            printf("%s", print[x][z]);
        }
        printf("\n");
    }
    printf("\n");

    printf("%s", CPU);
    print[4][8] = stages[4];
    for (x = 0; x < i; ++x) {
        printf("%s\t",inst[x]);
        for (z = 0; z < 9; ++z) {
            printf("%s", print[x][z]);
        }
        printf("\n");
    }
}

void print_hazards(int i,char * print[5][9],char inst[5][25],char * stages[5],int nop_stage,int nop_location,int nop_amt) {
    int x,y,z;
    for (x = 2; x <= 9; x++) {
        char * nop[2][9];
        if (x < nop_stage) {
            printf("%s", CPU);
            for (y = 0; y <= x-1; ++y) {
                print[y][x-1] = stages[(x-1)-y];
            }
            for (y = 0; y < i; ++y) {
                printf("%s\t",inst[y]);
                for(z = 0; z < 9; ++z) {
                    printf("%s", print[y][z]);
                }
                printf("\n");
            }
            printf("\n");

        } else if (x == nop_stage) {
            printf("%s", CPU);
            for (y = 0; y <= x-1; ++y) {
                if (y < nop_location) {
                    print[y][x-1] = stages[(x-1)-y];
                } else {
                    print[y][x-1] = stages[(x-1)-(y+1)];
                }
            }
            if (nop_amt == 1) {
                for (y = 0; y < nop_location; ++y) {
                    nop[0][y] = ".\t";
                }
                nop[0][nop_location] = stages[0];
                nop[0][nop_location+1] = stages[1];
                nop[0][nop_location+2] = "*\t";
                for (y = nop_location + 3; y < 9; ++y) {
                    nop[0][y] = ".\t";
                }
                for (z = 0; z < i; ++z) {
                    if (z == nop_location) {
                        printf("nop  \t\t   ");
                        for (y = 0; y < 9; ++y) {
                            printf("%s", nop[0][y]);
                        }
                        printf("\n");
                    }
                    printf("%s\t", inst[z]);
                    for (y = 0; y < 9; y++) {
                        printf("%s", print[z][y]);
                    }
                    printf("\n");
                }
                printf("\n");
            } else {
                for (y = 0; y < nop_location; ++y) {
                    nop[0][y] = ".\t";
                    nop[1][y] = ".\t";
                }
                nop[0][nop_location] = stages[0];
                nop[1][nop_location] = stages[0];
                nop[0][nop_location+1] = stages[1];
                nop[1][nop_location+1] = stages[1];
                nop[0][nop_location+2] = "*\t";
                nop[1][nop_location+2] = stages[1];
                for (y = nop_location + 3; y < 9; ++y) {
                    nop[0][y] = ".\t";
                    nop[1][y] = ".\t";
                }
                for (z = 0; z < i; ++z) {
                    if (z == nop_location) {
                        printf("nop  \t\t   ");
                        for (y = 0; y < 9; ++y) {
                            printf("%s", nop[0][y]);
                        }
                        printf("\n");
                    }
                    printf("%s    ", inst[z]);
                    for (y = 0; y < 9; y++) {
                        printf("%s", print[z][y]);
                    }
                    printf("\n");
                }
                printf("\n");
            }
        } else if (x > nop_stage) {
            int j;
            printf("%s", CPU);
            if (nop_amt == 1) {
                if (x >= nop_stage+1 && x < nop_stage+3) {
                    nop[0][x-1] = "*\t";
                }
                for (y = x-5; y <= x-1; ++y) {
                    if (y >= nop_location) {
                        print[y][x-1] = stages[(x-1)-(y+1)];
                    } else {
                        print[y][x-1] = stages[(x-1)-(y)];
                    }
                }
                for (z = 0; z < i; ++z) {
                    if (z == nop_location) {
                        printf("nop  \t\t   ");
                        for (y = 0; y < 9; ++y) {
                            printf("%s", nop[0][y]);
                        }
                        printf("\n");
                    }
                    printf("%s\t", inst[z]);
                    for (y = 0; y < 9; y++) {
                        printf("%s", print[z][y]);
                    }
                    printf("\n");
                }
                printf("\n");
            } else {
                if (x >= nop_stage+1 && x < nop_stage+3) {
                    nop[0][x-1] = "*\t";
                    nop[1][x-1] = "*\t";
                }
                for (y = x-5; y <= x-1; ++y) {
                    if (y < nop_location) {
                        print[y][x-1] = stages[(x-1)-y];
                    } else  {
                        print[y][x-1] = stages[(x-1)-(y+1)];
                    }
                }
                for (z = 0; z < i; ++z) {
                    if (z == nop_location) {
                        for (j = 0; j < 2; ++j) {
                            printf("nop  \t\t   ");
                            for (y = 0; y < 9; ++y) {
                                printf("%s", nop[j][y]);
                            }
                            printf("\n");
                        }
                    }
                    printf("%s\t", inst[z]);
                    for (y = 0; y < 9; y++) {
                        printf("%s", print[z][y]);
                    }
                    printf("\n");
                }
                printf("\n");
            }
        }
    }
}

int main(int argc, char* argv[]) {
    char * stages[5] = {"IF\t","ID\t","EX\t","MEM\t","WB\t"};
    char * filename = argv[1];
    FILE * data = fopen(filename, "r");
    char line[25];
    char inst[5][25];
    char op[5][4];
    char reg[5][20];
    int i = 0;
    while (fgets(line,25,data) != NULL) {
        char * str = strtok(line,"\n");
        strcpy(inst[i],str);
        //char temp[25];
        //strcpy(temp,str);
        strcpy(op[i],strtok(str," "));
        strcpy(reg[i],strtok(NULL," "));
        i += 1;
    }
    char * print[5][9];
    int x,y;
    print[0][0] = stages[0];
    for (x = 0; x < 1; ++x) {
        for (y = 1; y < 8; ++y) {
            print[x][y] = ".\t";
        }
        for (y = 8; y < 9; ++y) {
            print[x][y] = ".";
        }
    }
    for (x = 1; x < 5; ++x) {
        for (y = 0; y < 8; ++y) {
            print[x][y] = ".\t";
        }
        for (y = 8; y < 9; ++y) {
            print[x][y] = ".";
        }
    }
    printf("START OF SIMULATION\n\n");
    printf("%s", CPU);
    for (x = 0; x < i; ++x) {
        printf("%s\t",inst[x]);
        for (y = 0; y < 9; ++y) {
            printf("%s", print[x][y]);
        }
        printf("\n");
    }
    printf("\n");

    if (i == 5) {
        print_no_hazards(i,print,inst,stages);
    } else {
        int hazards[4];
        int nop = 0;
        for (x = 0; x < i-1; ++x) {
            if (strcmp(op[x],"sw")!=0) {
                char search[5];
                strcpy(search,strtok(reg[x],","));
                for (y = x+1; y < i; ++y) {
                    if (strstr(reg[y], search) == NULL) {
                        hazards[x] = 0;
                    } else {
                        hazards[x] = y;
                        nop = 1;
                        break;
                    }
                }
            } else {
                char * str = strtok(reg[x],")");
                str = strtok(str,"(");
                char search[5];
                strcpy(search,strtok(NULL, "("));
                for (y = x+1; y < i; ++y) {
                    if (strstr(reg[y], search) == NULL) {
                        hazards[x] = 0;
                    } else {
                        hazards[x] = y;
                        nop = 1;
                    }
                }
            }
        }
        if (nop == 0) {
            print_no_hazards(i,print,inst,stages);
        } else {
            int nop_stage = 0;
            int nop_amt;
            int nop_location;
            for (x = 0; x < 4; ++x) {
                if (hazards[x] > 0 && hazards[x] < x + 3) {
                    nop_stage = hazards[x] + 3;
                    nop_location = hazards[x];
                    if (hazards[x] - x == 1) {
                        nop_amt = 2;
                    } else {
                        nop_amt = 1;
                    }
                }
            }
            if (nop_stage == 0) {
                print_no_hazards(i,print,inst,stages);
            } else {
                print_hazards(i,print,inst,stages,nop_stage,nop_location,nop_amt);
            }
        }
        printf("END OF SIMULATION\n");
    }
}
