#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

/*******************************************************************************/
/*                                 FUNCTIONS                                   */
/*******************************************************************************/

BIT not_gate(BIT A) {
  if (A)
    return FALSE;
  else
    return TRUE;
}

BIT or_gate(BIT A, BIT B) {
  // TODO: implement logical OR
  if (A || B)
    return TRUE;
  else
    return FALSE;
}

BIT and_gate(BIT A, BIT B) {
  // TODO: implement logical AND
  if (A && B)
    return TRUE;
  else
    return FALSE;
}

BIT xor_gate(BIT A, BIT B) {
  // TODO: implement logical XOR
  if (A ^ B)
    return TRUE;
  else
    return FALSE;
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3) {
  // TO DO: implement a 2-input decoder
  *O0 = and_gate(not_gate(I1), not_gate(I0));
  *O1 = and_gate(I1, not_gate(I0));
  *O2 = and_gate(not_gate(I1), I0);
  *O3 = and_gate(I1, I0);

  return;
}

BIT multiplexor2(BIT S, BIT I0, BIT I1) {
  // TODO: implement a 2-input multiplexor
  BIT x0 = and_gate(not_gate(S), I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3) {
  // TODO: implement a 4-input multiplexor
  BIT x0, x1, x2, x3 = FALSE;
  decoder2(S0, S1, &x0, &x1, &x2, &x3);

  BIT y0 = and_gate(x0, I0);
  BIT y1 = and_gate(x1, I1);
  BIT y2 = and_gate(x2, I2);
  BIT y3 = and_gate(x3, I3);

  BIT z0 = or_gate(y0, y1);
  BIT z1 = or_gate(y2, y3);

  return or_gate(z0, z1);
}

void adder1(BIT* A, BIT* B, int Index, BIT* CarryIn, BIT* P, BIT* G, BIT* Result) {
    BIT x0 = xor_gate(A[Index], B[Index]);
    Result[Index] = xor_gate(CarryIn[Index+1], x0);
    P[Index] = or_gate(A[Index],B[Index]);
    G[Index] = and_gate(A[Index],B[Index]);
}

void CarryLookAhead(BIT* CarryIn, BIT* P, BIT* G, int Index) {
    CarryIn[Index] = or_gate( G[Index], and_gate(P[Index], CarryIn[Index+1]));
}

void adder4(BIT* A, BIT* B, int Index, BIT* CarryIn, BIT* P, BIT* G, BIT* Result) {
    adder1(A, B, Index, CarryIn, P, G, Result);
    CarryLookAhead(CarryIn,P,G,Index);

    adder1(A, B, Index-1, CarryIn, P, G, Result);
    CarryLookAhead(CarryIn,P,G,Index-1);

    adder1(A, B, Index-2, CarryIn, P, G, Result);
    CarryLookAhead(CarryIn,P,G,Index-2);

    adder1(A, B, Index-3, CarryIn, P, G, Result);
    CarryLookAhead(CarryIn,P,G,Index-3);
}

void SLL(BIT* A) {
    int x;
    for (x = 0; x < 16-1; ++x) {
        A[x] = A[x+1];
    }
    A[15] = 0;
}

void SRL(BIT* A) {
    int x;
    for (x = 15; x > 0; --x) {
        A[x] = A[x-1];
    }
    A[0] = 0;
}

void divider(BIT* dividend, BIT* divisor, BIT* Result) {
    int x;
    for (x = 0; x < 16; ++x) {
        Result[x] = FALSE;
    }
    BIT remainder[16];
    strcpy(remainder,dividend);
    BIT prop[16];
    BIT gen[16];
    BIT divisor_cpy[16];
    strcpy(divisor_cpy,divisor);
    BIT neg_divisor[16];
    BIT Binvert = 1;
    BIT cin[17];

    for (x = 0; x < 17; ++x) {
        // REM - DIVISOR
        int i;
        for (i = 0; i < 16; ++i) {
            neg_divisor[i] = multiplexor2(Binvert, divisor_cpy[x], not_gate(divisor_cpy[x]));
        }
        cin[0] = 0;
            adder4(remainder, neg_divisor, 15, cin, prop, gen, remainder);
            adder4(remainder, neg_divisor, 11, cin, prop, gen, remainder);
            adder4(remainder, neg_divisor, 7, cin, prop, gen, remainder);
            adder4(remainder, neg_divisor, 3, cin, prop, gen, remainder);
        // REM >= 0
        if (remainder[0] != 1) {
            SLL(Result);
            Result[15] = 1;
        // REM < 0
        } else {
            SLL(Result);
            strcpy(remainder,dividend);
            Result[15] = 0;
        }
        // SHIFT DIVISOR RIGHT
        SRL(divisor_cpy);
    }
}



void ALU(BIT* A, BIT* B, BIT* CarryIn, BIT* P, BIT* G, BIT Op0, int Index, BIT* Result) {
    if (Op0 == 0) {
        adder4(A, B, Index, CarryIn, P, G, Result);
        Index -= 4;
        adder4(A, B, Index, CarryIn, P, G, Result);
        Index -= 4;
        adder4(A, B, Index, CarryIn, P, G, Result);
        Index -= 4;
        adder4(A, B, Index, CarryIn, P, G, Result);
    } else {
        divider(A,B,Result);
    }
}

void convert_to_binary_char(int a, BIT* A, int length) {
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? TRUE : FALSE);
      a /= 2;
    }
  } else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? TRUE : FALSE);
      a /= 2;
    }
  }
  int x;
  for (x = 0; x < 16; ++x) {
  }
}

void convert_to_binary(int a, BIT* A) {
  // TODO: convert integer to 2's complement BIT representation
  // Note: A[0] is least significant bit and A[31] is most significant bit
  // For this function ONLY: you're allowed to use logical if-else statements
  convert_to_binary_char(a, A, 16);
  int x;
  BIT temp[16];
  for (x = 0; x < 16; ++x) {
    temp[x] = A[16-x-1];
  }
  for (x = 0; x < 16; ++x) {
    A[x] = temp[x];
  }
}


/*******************************************************************************/
/*                                   MAIN                                      */
/*******************************************************************************/
int main(int argc,char * argv[]) {
    char sign;
    int operand1;
    int operand2;
    scanf("%d %c %d",&operand1,&sign,&operand2);
    BIT P[16];
    BIT G[16];
    BIT C[17];
    C[16] = 0;
    BIT bin1[16];
    BIT bin2[16];
    BIT result[16];
    BIT Op[3] = {1};
    if (sign == '+') {
        Op[0] = 0;
    }
    convert_to_binary(operand1,bin1);
    int x;
    for (x = 0; x < 16; ++x) {
        printf("%d", bin1[0]);
    }
    for (x = 0; x < 16; ++x) {
        printf("%d", bin1[x]);
    }
    printf("\n");
    if (sign == '/') {
        printf("/\n");
    } else {
        printf("+\n");
    }
    convert_to_binary(operand2,bin2);
    for (x = 0; x < 16; ++x) {
        printf("%d", bin2[0]);
    }
    for (x = 0; x < 16; ++x) {
        printf("%d", bin2[x]);
    }
    printf("\n=\n");
    ALU(bin1, bin2, C, P, G, Op[0], 15, result);
    for (x = 0; x < 16; ++x) {
        printf("%d", result[0]);
    }
    for (x = 0; x < 16; ++x) {
        printf("%d", result[x]);
    }
    printf("\n");
    return 0;
}
