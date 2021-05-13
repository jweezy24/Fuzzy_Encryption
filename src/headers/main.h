#include <galois.h>
#include <gauss.h>
#include <stdlib.h>
#include <math.h>

#define MESSAGE_SIZE 16
#define KEY_SIZE 8

//4 Bit
//unsigned int prim_poly = (unsigned int) 0b10011;
//int generator = (unsigned int)0b1011;

//6 Bit
// unsigned int prim_poly = (unsigned int) 0b1000011;
// int generator = (unsigned int)0b100101;

//8 Bit
// unsigned int prim_poly = (unsigned int) 0b100011101;
// int generator = (unsigned int)0b10000011;

//10 Bit
// unsigned int prim_poly = (unsigned int) 0b10000001001;
// int generator = (unsigned int)0b1000010001;


//16 bit 
// unsigned int prim_poly = (unsigned int) 0b10000001111011101;
// int generator = (unsigned int)0b1000000000000011;


poly* RSDecode(int t, poly* C, poly* key, poly* g);
poly* RSEncode(poly* g_x, poly* input, poly* key, poly* error);
void create_stream(char* s, int size);
void create_stream_with_error(char* s1, char* s, int size, int error);
void char_arr_to_poly(char* s, int size, poly* p);
int bin_to_int(char* buffer, int len);