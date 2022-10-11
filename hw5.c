#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void addf(char add1, char add2, char ** saved, char * var) {
    int x,y;
    for (x = 0; x < 3; ++x) {
        if (var[x] == add1) break;
    }
    for (y = 0; y < 3; ++y) {
        if (var[y] == add2) break;
    }
    printf("add $t0,%s,%s\n", saved[x], saved[y]);
}
void multf(char mult1, int mult2, char ** saved, char * var, int neg, int * power2, int i) {
    int x,y;
    y = mult2;
    if (y > 1) {
        int sums[3];
        int z = 0;
        for (x = 0; x < 10; ++x) {
            if (power2[x] > y) {
                y -= power2[x-1];
                sums[z] = x;
                if (y <= 1) break;
                z += 1;
                x = 0;
            }
        }
        for (x = 0; x < 3; ++x) {
            if (var[x] == mult1) break;
        }
        printf("sll $t0,%s,%d\n", saved[x], sums[0]);
        printf("move $t1,$t0\n");
        for (z = 1; z < 3; ++z) {
            printf("sll $t0,%s,%d\n", saved[x], sums[z]);
            printf("add $t1,$t1,$t0\n");
        }
        if (y == 1) {
            printf("add $t1,$t1,%s\n", saved[x]);
        }
        if (neg == 1) {
            printf("sub %s,$zero,$t1\n", saved[i]);
        } else {
            printf("move %s,$t1\n", saved[i]);
        }
    } else if (y == 1) {
        for (x = 0; x < 3; ++x) {
            if (var[x] == mult1) break;
        }
        if (neg == 1) {
            printf("move $t1,%s\n", saved[x]);
            printf("sub %s,$zero,$t1\n", saved[i]);
        } else {
            printf("move $t0,%s\n", saved[x]);
            printf("move %s,$t0\n", saved[i]);
        }
    }
}
void subf(char sub1, char sub2, char ** saved, char * var) {
    int x,y;
    for (x = 0; x < 3; ++x) {
        if (var[x] == sub1) break;
    }
    for (y = 0; y < 3; ++y) {
        if (var[y] == sub2) break;
    }
    printf("sub $t0,%s,%s\n", saved[x], saved[y]);

}
void divf(char div1, int div2, char ** saved, char * var, int neg, int * power2, int i) {
    int x,y;
    for (x = 0; x < 3; ++x) {
        if (var[x] == div1) break;
    }
    if (div2 == 1) {
        if (neg == 0) {
            printf("move %s,%s\n", saved[i], saved[x]);
        } else {
            printf("sub %s,$zero,%s", saved[i], saved[x]);
        }
    } else {
        int sums;
        for (x = 0; x < 10; ++x) {
            if (power2[x] > y) {
                y -= power2[x-1];
                sums = x;
                if (y <= 1) break;
                x = 0;
            }
        }
        int z = x-1;
        printf("bltz %s,L0\n", saved[i]);
        printf("srl %s,%s,%d\n", saved[i], saved[x], sums);
        if (neg == 1) {
            printf("sub %s,$zero,%s\n", saved[i], saved[i]);
        }
        printf("j L1\n");
        printf("L0:\n");
        if (neg == 0) {
            printf("li $t0,%d\n", power2[z]);
        } else {
            printf("li $t0,-%d\n", power2[z]);
        }
        printf("div %s,$t0\n", saved[x]);
        printf("mflo %s\n", saved[i]);
        printf("L1:\n");
    }
}

int main(int argc, char* argv[]) {
    char * filename = argv[1];
    FILE * data = fopen(filename, "r");
    char line[256];
    char *saved[3] = {"$s0", "$s1", "$s2"};
    char var[3] = {'\0', '\0', '\0'};
    int power2[10] = {2,4,8,16,32,64,128,256,512,1024};
    int i,j;
    while (fgets(line, 256, data) != NULL) {
        printf("# %s", line);
        char * split1 = strtok(line, "=");
        char * split2 = strtok(NULL, "=");
        for (i = 0; i < 3; ++i) {
            if (var[i] == '\0') {
                var[i] = split1[0];
                break;
            }
        }
        // LOAD IMMEDIATE
        if (strlen(split2) <= 7) {
            printf("li %s,", saved[i]);
            for (j = 1; j < strlen(split2)-2; j++) {
                printf("%c",split2[j]);
            }
            printf("\n");
        } else {
            int x,y;
            char * add = strchr(split2, '+');
            char * div = strchr(split2, '/');
            char * sub = strchr(split2, '-');
            char * mult = strchr(split2, '*');
            char * mod = strchr(split2, '%');
            if (add == NULL) {
                if (mult == NULL) {
                    if (div == NULL) {
                        char sub1 = split2[sub-split2-2];
                        char sub2 = split2[sub-split2+2];
                        for (x = 0; x < 3; ++x) {
                            if (var[x] == sub1) break;
                        }
                        if (isdigit(sub2) == 0) {
                            for (y = 0; y < 3; ++y) {
                                if (var[y] == sub2) break;
                            }
                            printf("sub %s,%s,%s\n", saved[i], saved[x], saved[y]);
                        } else {
                            printf("addi %s,%s,-%c\n", saved[i], saved[x], sub2);
                        }
                    } else {
                        //DIVISION
                        char div1 = split2[div-split2-2];
                        char div2[2];
                        int neg;
                        if (split2[div-split2+2] == '-') {
                            div2[0] = split2[div-split2+3];
                            neg = 1;
                            if (split2[div-split2+4] != ';') {
                                div2[1] = split2[div-split2+4];
                            }
                        } else {
                            neg = 0;
                            div2[0] = split2[div-split2+2];
                            if (split2[div-split2+3] != ';') {
                                div2[1] = split2[div-split2+3];
                            }
                        }
                        y = atoi(div2);
                        divf(div1, y, saved, var, neg, power2, i);
                    }
                //MULTIPLICATION
                } else {
                    //MULTIPLYING BY A NUMBER
                    if (div == NULL && mod == NULL) {
                        char mult1 = split2[mult-split2-2];
                        char mult2[2];
                        if (split2[mult-split2+2] == '-') {
                            mult2[0] = split2[mult-split2+3];
                            mult2[1] = split2[mult-split2+4];
                            y = atoi(mult2);
                            int neg = 1;
                            multf(mult1, y, saved, var, neg, power2, i);
                        } else {
                            mult2[0] = split2[mult-split2+2];
                            mult2[1] = split2[mult-split2+3];
                            y = atoi(mult2);
                            int neg = 0;
                            multf(mult1, y, saved, var, neg, power2, i);
                        }
                    } else {
                        if (div != NULL) {
                            if (mult-split2 < div-split2) {
                                char mult1 = split2[mult-split2-2];
                                char mult2 = split2[mult-split2+2];
                                for (x = 0; x < 3; ++x) {
                                    if (var[x] == mult1) break;
                                }
                                for (y = 0; y < 3; ++y) {
                                    if (var[y] == mult2) break;
                                }
                                printf("mult %s,%s\n", saved[x], saved[y]);
                                if (isdigit(split2[div-split2+2]) == 0 && split2[div-split2+2] != '-') {
                                    printf("mflo $t0\n");
                                    char div1 = split2[div-split2+2];
                                    for (x = 0; x < 3; ++x) {
                                        if (var[x] == div1) break;
                                    }
                                    printf("div $t0,%s\n", saved[x]);
                                    printf("mflo %s\n", saved[i]);
                                } else if (isdigit(split2[mult-split2+2]) == 1) {
                                    printf("mflo $t1\n");
                                    char div1[3];
                                    div1[0] = split2[div-split2+2];
                                    div1[1] = split2[div-split2+3];
                                    div1[2] = split2[div-split2+4];
                                    y = atoi(div1);
                                    printf("bltz $t1,L2\n");
                                    int sums;
                                    for (x = 0; x < 10; ++x) {
                                        if (power2[x] == y) {
                                            sums = x;
                                        }
                                    }
                                    int z = x-1;
                                    printf("srl %s,%s,%d\n", saved[i], saved[x], sums);
                                    printf("j L3\n");
                                    printf("L2:\n");
                                    printf("li $t2,%d\n", power2[z]);
                                    printf("div $t1,$t2\n");
                                    printf("mflo %s", saved[i]);
                                    printf("L3:\n");
                                }
                            } else {
                                char div1 = split2[div-split2-2];
                                    for (x = 0; x < 3; ++x) {
                                    if (var[x] == div1) break;
                                }
                                if (isdigit(split2[div-split2+2]) == 0) {
                                    char div2[2];
                                    div2[0] = split2[div-split2+3];
                                    div2[1] = split2[div-split2+4];
                                    y = atoi(div2);
                                    printf("li $t0,-%d\n", y);
                                    printf("div %s,$t0\n", saved[x]);
                                    printf("mflo $t1\n");
                                    char mult2 = split2[mult-split2+2];
                                    for (x = 0; x < 3; ++x) {
                                        if (var[x] == mult2) break;
                                    }
                                    printf("mult $t1,%s\n", saved[x]);
                                    printf("mflo $t2\n");
                                    printf("li %s,0\n", saved[i]);
                                } else {
                                    char div2[2];
                                    div[0] = split2[div-split2+2];
                                    y = atoi(div2);
                                    if (y == 1) {
                                        printf("move %s,%s\n", saved[i], saved[x]);
                                    }
                                }
                            }
                        } else {
                            char mult1 = split2[mult-split2-2];
                            char mult2 = split2[mult-split2+2];
                            for (x = 0; x < 3; ++x) {
                                if (var[x] == mult1) break;
                            }
                            for (y = 0; y < 3; ++y) {
                                if (var[y] == mult2) break;
                            }
                            printf("mult %s,%s\n", saved[x], saved[y]);
                            printf("mflo $t0\n");
                            mult = strrchr(split2, '*');
                            mult2 = split2[mult-split2+2];
                            for (y = 0; y < 3; ++y) {
                                if (var[y] == mult2) break;
                            }
                            printf("mult $t0,%s\n", saved[y]);
                            printf("mflo $t1\n");
                            char mod1 = split2[mult-split2+2];
                            for (x = 0; x < 3; ++x) {
                                if (var[x] == mod1) break;
                            }
                            printf("div $t1,%s", saved[x]);
                            printf("mfhi %s", saved[i]);
                        }
                    }
                }
                //ADDITION
            } else {
                //ADDITION BEFORE SUBTRACTION
                if (add-split2 < sub-split2) {
                    char add1 = split2[(add-split2-2)];
                    char add2 = split2[(add-split2+2)];
                    addf(add1, add2, saved, var);
                    printf("addi %s,$t0,-",saved[i]);
                    char * sub = strchr(split2, '-');
                    for (j = (sub-split2+2); j < strlen(split2)-2; ++j) {
                        printf("%c",split2[j]);
                    }
                    printf("\n");
                //SUBTRACTION BEFORE ADDITION
                } else {
                    char sub1 = split2[sub-split2-2];
                    char sub2 = split2[sub-split2+2];
                    subf(sub1, sub2, saved, var);
                    char add1[2];
                    add1[0] = split2[add-split2+2];
                    add1[1] = split2[add-split2+3];
                    printf("addi $t1,$t0,%s\n", add1);
                    add = strrchr(split2, '+');
                    char add2 = split2[add-split2+2];
                    for (x = 0; x < 3; ++x) {
                        if (var[x] == add2) break;
                    }
                    printf("add $t2,$t1,%s\n", saved[x]);
                    sub = strrchr(split2, '-');
                    sub1 = split2[sub-split2+2];
                    for (x = 0; x < 3; ++x) {
                        if (var[x] == sub1) break;
                    }
                    printf("sub %s,$t2,%s\n", saved[i], saved[x]);
                }
            }
        }
    }
    return 0;
}
