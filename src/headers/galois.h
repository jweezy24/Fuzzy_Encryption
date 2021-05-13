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


#define POW_2 8
#define NW  256
#define START_SIZE 30
#define START_MESSAGE  9

#define PRIME_POLY 0b100011101
#define GENERATOR 0b10000011


//utils
void bin(unsigned n);
void print_arr(int* a, int len );
unsigned int str_int(char* str);
void print_mat(int** mat, int row, int col);
int find_coeff_row_reduction(int a, int b);
void display_message(poly* M);
int bin_num(unsigned n);
int check_range(int* range, int num);
int cardinality(int x);
void setup_tables();


//polynomial data structure functions
void create_poly(poly* p, int size);
void print_poly(poly* p);
poly* gf_poly_add(poly* a, poly* b);
poly* gf_mult_poly(poly* a, poly* b);
int gf_mult(int a, int b);
void dup_poly(poly* a, poly* b);
poly* gf_div_poly(poly* a, poly* b, int remainder);
int gf_div(int a, int b);
int gf_inverse(int num);
void resize_poly(poly* p);
int deg(poly* p);
poly* g(int t);
poly* m_(int n, int k, int t, int bits_start, int tracker, char* bits);
synd* syndome_calculator_division(poly* C, poly* g, int t);
poly* euclid_alg(poly* S, int t);
poly** euclid_alg_rec(poly* a, poly* b, poly* q, poly* r, int t);
poly* sigma_r(poly* s);
int eval_poly(poly* p, int x);
poly* roots_of_poly(poly* sigma_r, int t, int n);
poly* berlecamp_table(poly* S, int t);
poly* inverse_poly(poly* p);
poly* error_correction(poly* roots, poly* S);
void reassemble_message(poly* errors, poly* locations, poly* M);
int poly_eq(poly* p1, poly* p2);
void free_poly(poly* p);
void free_synd(synd* p);
poly* copy_poly(poly* p1);
poly* g_2(int t, int generator);
poly* berlecamp_table(poly* S, int t);
int gf_pow(unsigned int a, int b);