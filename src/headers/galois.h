#include <stdio.h>
#include <stdlib.h>

typedef struct polynomial{
    unsigned int* coeffs;
    int size;
}poly;
    
typedef struct syndrome{
    poly* p;
    int synds;
}synd;


int pow_2 = 8;
int p = 256;
unsigned int NW = 1 << 8;
unsigned int *gflog, *gfilog;
int start_size = 30;
int start_message = 9;

//4 Bit
//unsigned int prim_poly = (unsigned int) 0b10011;
//int generator = (unsigned int)0b1011;

//6 Bit
// unsigned int prim_poly = (unsigned int) 0b1000011;
// int generator = (unsigned int)0b100101;

//8 Bit
unsigned int prim_poly = (unsigned int) 0b100011101;
int generator = (unsigned int)0b10000011;

//10 Bit
// unsigned int prim_poly = (unsigned int) 0b10000001001;
// int generator = (unsigned int)0b1000010001;


//16 bit 
// unsigned int prim_poly = (unsigned int) 0b10000001111011101;
// int generator = (unsigned int)0b1000000000000011;
