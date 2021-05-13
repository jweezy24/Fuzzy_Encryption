#include <galois.h>
#include <gauss.h>

unsigned int *gflog, *gfilog;

/*
This method initalizes a polynomial struct.
input:
    size = how many coeffiecnts are present in the polynomial
    p = a polynomial pointer 
*/
void create_poly(poly* p, int size){
    p->coeffs = malloc(sizeof(unsigned int)*size);
    p->size = size;
    for(int i =0; i < size; i++){
        p->coeffs[i] = 0;   
    }
}

/*
This method copies a polynomial to another.
input:
    a = source polynomial
    b = destination polynomial
*/
void dup_poly(poly* a, poly* b){
    b->size = a->size;
    b->coeffs = malloc(sizeof(int)*a->size);
    for(int i = 0; i < a->size; i++){
        b->coeffs[i] = a->coeffs[i];
    }
}

/*
This method frees another polynomial.
input:
    p = polynomial we want to free
*/
void free_poly(poly* p){
    free(p->coeffs);
    free(p);
}

void free_synd(synd* p){
    free_poly(p->p);
    free(p);
}


/*
This method will resize a polynomial removing leading zeros left over by operations.
input:
    p = polynomial we want resize
*/
void resize_poly(poly* p){
    int size = 0;
    for(int i = p->size-1; i >= 0; i--){
        if(p->coeffs[i] != 0){
            size = i;
            break;
        }
    }
    p->coeffs = realloc(p->coeffs, (size+1)*sizeof(int));
    p->size = size+1;

}

/*
This method will get the degree of the polynomial.
input:
    p = polynomial we the degree of.
output:
    integer representing the degree of the polynomial
*/
int deg(poly* p){
    for(int i = p->size-1; i >= 0; i--){
        if(p->coeffs[i] != 0){
            return i;
        }
    }
    return 0;
}

/*
Debugging method to print out a polynomial
input:
    p = polynomial we would like to print.
*/

void print_poly(poly* p){

    for(int i = 0; i < p->size; i++){
        printf("%d*x^%d +\t", p->coeffs[i], i);
    }
    printf("\n");

}

/*
This initializer method will create the galois field mappings with a global (unsigned int)GENERATOR.
*/
void setup_tables(){
    //inits
    unsigned int x_to_w=0; 
    unsigned int b=0; 
    unsigned int log = 0;
    
    //NW == 256
    x_to_w =  NW;
    // Inits of lists defined in the header
    gflog = malloc(sizeof(unsigned int) * (x_to_w));
    gfilog = malloc(sizeof(unsigned int) * (x_to_w));

    //Starting point for our inits
    b = 1;
    for(log = 0; log < x_to_w; log++){
        //All code below creates the Galois field where integers are thought as polynomials.
        gflog[b] = (unsigned int)log;
        gfilog[log] = (unsigned int) b;
        b = b << 1;
        if(b & (x_to_w)){
            b = (b ^ (unsigned int)PRIME_POLY);
        }

    }

}


/*
This method below defines a multiplication operations between two integers in our galois field.
input:
    a = fisrt number we would like to multiply
    b = second number we would like to multiply
output:
    a*b
*/
int gf_mult(int a, int b){
    int sum_log;
   
    if(a == 0 || b == 0) return 0;
    // if(a >= NW || b >= NW) return 0;
    sum_log = (int)gflog[a] + (int)gflog[b];
   
    if(sum_log >= NW-1){
        sum_log = sum_log - (NW-1);
    }
    
    return gfilog[sum_log];
}

/*
The method below multiplies polynomials together.
input:
    a = first polynomial we would like to multiply
    b = second polynomial we would like to multiply
ouput:
    a*b as a polynomial struct
*/
poly* gf_mult_poly(poly* a, poly* b){
    
    if(a->size == 1 && b->size== 1){
        poly* ret = malloc(sizeof(poly));
        create_poly(ret,2);
        ret->coeffs[0] = gf_mult(a->coeffs[0], b->coeffs[0]);
        return ret;
    }



    int new_poly_size = a->size + b->size - 1;

    poly* new_poly = malloc(sizeof(poly));
    create_poly(new_poly, new_poly_size);

    if(a->size == 1){
        for(int j = b->size-1; j >= 0; j--){
            
            new_poly->coeffs[j] ^= (unsigned int)gf_mult(a->coeffs[0], b->coeffs[j]);
        }
    }

    if(b->size == 1){
        for(int j = a->size-1; j >= 0; j--){
            
            new_poly->coeffs[j] ^= (unsigned int)gf_mult(a->coeffs[j], b->coeffs[0]);
        }
    }


    for(int i = a->size-1; i >= 0; i--){
        for(int j = b->size-1; j >= 0; j--){
            
            new_poly->coeffs[i+j] ^= (unsigned int)gf_mult(a->coeffs[i], b->coeffs[j]);
        }
    }

    resize_poly(new_poly);

    return new_poly;
}

/*
This method represents integer division in our Galois field.
input:
    a = numerator
    b = denominator
output:
    a*b^-1
*/

int gf_div(int a, int b){
    int diff_log;

    if(a == 0) return 0;
    if(b == 0) return 0;
    diff_log = (int)gflog[a] - (int)gflog[b];
    if(diff_log < 0) diff_log = diff_log + (NW-1);
    return gfilog[diff_log];
}

/*
This method adds two polynomials together.
input:
    a = first polynomial the user wants to add together
    b = second polynomial the user wants to add together
*/
poly* gf_poly_add(poly* a, poly* b){
    int max = 0;
    int min = 0;
    if(a->size > b->size){
        max = a->size;
        min = b->size;
    }else{
        max = b->size;
        min = a->size;
    }

    poly* p = malloc(sizeof(poly));
    create_poly(p, max);

    for(int i =0; i < max; i++){
        if(i >= min){
            if(min == b->size){
                p->coeffs[i] = a->coeffs[i];
            }else{
                p->coeffs[i] = b->coeffs[i];
            }
        }else{
            p->coeffs[i] = a->coeffs[i] ^ b->coeffs[i];
        }
    }
    return p;
}

/*
This method is polynomial division within our Galois field.
input:
    a = numerator polynomial
    b = denominator polynomial
    remainder = 1 or 0, if 1 we return the remainder 0 we will return the dividend
output:
    remainder or dividend depending on the remainder argument

Explaination for remainder argument:
    Some cases require a remainder over the dividend.
    Therefore, returning the remainder as a argument makes the code cleaner.
*/
poly* gf_div_poly(poly* a, poly* b, int remainder){
    
    poly* zero = malloc(sizeof(poly));
    create_poly(zero, 2);
    zero->coeffs[0] = 0;
    if(a->size == 0 || b->size == 0) return zero;

    if(a->size <= 2){
        int div = a->coeffs[0];
        for(int i = 0; i < b->size; i++){
            b->coeffs[i] = gf_div(div, b->coeffs[i]);
        }
        return b;
    }
    
    if(b->size <= 2){
        for(int i = 0; i < a->size; i++){
            a->coeffs[i] = gf_div(a->coeffs[i], b->coeffs[0]); 
        }
        return a;
    }

    int pos = 0;


    poly* divisor = malloc(sizeof(poly));
    dup_poly(b, divisor);

    poly* dividend = malloc(sizeof(poly));
    dup_poly(a, dividend);

    poly* q = malloc(sizeof(poly));
    create_poly(q,a->size);

    int i = a->size-1;

    while(i >= b->size-1){
        int num =  dividend->coeffs[i];
        int test = 0;
        int coeff = 0;

        for(int j = 1; j < NW; j++){
            test = gf_mult(j,divisor->coeffs[b->size-1]);
            int test2 = test^num;
            if(test2 == 0){
                coeff = j;
                break;
            }
        }

        q->coeffs[(i-b->size+1)] = coeff;

        if(coeff != 0){
            int tmp_pos= 0;
            for(int j = b->size-1; j >= 0;j--){
                if (divisor->coeffs[j] != 0){
                    dividend->coeffs[i-tmp_pos] ^= gf_mult(divisor->coeffs[j], coeff);
                }
                tmp_pos++;
            }
            
        }
        pos++;
        i--;

    }
    resize_poly(q);
    resize_poly(dividend);

    if(remainder == 1){
        free_poly(divisor);
        free_poly(zero); 
        return dividend;
    }else{
        free_poly(zero);
        free_poly(divisor);
        free_poly(dividend);
        return q;
    }

}

/*
This method will create the g(x) of our Galois field.
g(x) divides everything of less degree than it within the galois field
input:
    t = half the size of g(x)
output:
    g(x)
*/
poly* g(int t){

   
    poly* tmp_1 = malloc(sizeof(poly));
    create_poly(tmp_1,2);
    tmp_1->coeffs[0] = (unsigned int)GENERATOR;
    tmp_1->coeffs[1] = 1;
    for(int i = 2; i <= 2*t; i++){
        poly* tmp_2 = malloc(sizeof(poly)); 
        create_poly(tmp_2, 2);
        tmp_2->coeffs[0] = gf_pow((unsigned int)GENERATOR, i);
        tmp_2->coeffs[1] = 1;
        poly* holder = gf_mult_poly(tmp_1, tmp_2);
        free(tmp_1);
        tmp_1 = malloc(sizeof(poly));
        create_poly(tmp_1,holder->size);
        for(int j =0; j < holder->size; j++){
            tmp_1->coeffs[j] = holder->coeffs[j];
        }
        free(tmp_2);
    }

    return tmp_1;
}

/*
This method will evaluate a polynomial within the galois field.
input:
    p = polynomial we want to evaluate
    x = the value we want to compute p at. (p(x))
output:
    integer result of p(x) within our galois field
*/
int eval_poly(poly* p, int x){
    int ret = p->coeffs[0];
    for(int i = 1; i < p->size; i++ ){
        ret ^= gf_mult(p->coeffs[i], gf_pow(x, i));
       
    }
    return ret;
}

/*
This method preforms power operations within our galois field
input:
    a = base
    b = exponent
output:
    a**b in our galois field
*/
int gf_pow(unsigned int a, int b){
    unsigned int ret = 1;
    if(a == 0 && b == 0){
        return 1;
    }
    if(a == 0){
        return 0;
    }
    for (int i =0; i < b; i++){
        ret = (unsigned int) gf_mult(ret, a);
    }
    return ret;
}

/*
Preforms the syndome calculation using the division method
input:
    C = a polynomial that is a codeword in the context of reed-solomon
    g = g(x) as defined earlier
    t = half the size of g(x)
output:
    the syndrome of C with resepect of g
*/

synd* syndome_calculator_division(poly* C, poly* g, int t){

    poly* s = gf_div_poly(C, g,1);
    poly* S = malloc(sizeof(poly));
    create_poly(S,2*t);
    synd* sy = malloc(sizeof(synd));
    
    int iter = 1;
    sy->p = S;
    sy->synds = 0;
    for(int i = 1; i <= 2*t; i++){
        S->coeffs[i-1] = eval_poly(s, gf_pow((unsigned int)GENERATOR, i));
        for(int j = 0; j < i; j++){
            if(S->coeffs[i-1] == S->coeffs[j] && i-1 != j){
                iter = 0;
            }
        }
        if(iter == 1){
            sy->synds += 1;
        }
        iter = 1; 
    }

    resize_poly(S);
    free_poly(s);

    return sy;
}

/*
This method computes the 
input:
    s = a 

*/
poly* sigma_r(poly* s){
    int pos = 0;
    int size = s->size;
    poly* sigma_r = malloc(sizeof(poly)); 
    create_poly(sigma_r,size);
    for(int i =size-1; i >=0; i--){
        sigma_r->coeffs[pos] = s->coeffs[i];
        pos++;
        
    }
    return sigma_r;
}


poly* roots_of_poly(poly* sigma_r, int t, int n){
    poly* zeros_ = malloc(sizeof(poly));
    create_poly(zeros_,sigma_r->size);
    int zeros = 0;
    unsigned int num = 0;

    for(int i=0; i < NW-1; i++){
        num = (unsigned int) eval_poly(sigma_r, gf_pow((unsigned int)GENERATOR, i));

        if(num == 0){
            zeros_->coeffs[zeros] = i;
            zeros++;

        }

    }


    resize_poly(zeros_);

    return zeros_;
}

poly* error_correction(poly* roots, poly* S){
    
    mat* matrix = create_matrix(S->size, roots->size+1);

    for(int i = 0; i < S->size; i++){
        int* tmp = malloc(sizeof(int)* (roots->size+1));
        for(int j =0; j < roots->size+1; j++){
            if(j < roots->size){
                tmp[j] = gf_pow(gf_pow((unsigned int) GENERATOR, roots->coeffs[j]), i+1);
            }else{
                tmp[j] = S->coeffs[i];
            }
        }
        matrix->matrix[i] = tmp;    
    }

    
    poly* errors = gauss_elim(matrix);


    return errors;

}

poly* berlecamp_table(poly* S, int t){

    poly* C = malloc(sizeof(poly)); 
    create_poly(C, 2);
    C->coeffs[0]  = 1;
    poly* B = malloc(sizeof(poly)); 
    create_poly(B, 2);
    B->coeffs[0] = 1;

    

    int m = 1;

    int L = 0;

    int b = 1;

    int n, d;

    
    if(t <= (S->size)-2){
        t = S->size;
    }
    //printf("t=%d\n", t);

    for(n = 0; n < t; n++){
        d = S->coeffs[n];
        for(int i =1; i <= L; i++){

            d ^= gf_mult(C->coeffs[i], S->coeffs[n-i]);
        }
        
        if (d == 0){
            m = m+1;
        }else if(2*L <= n+1){
            poly* T = C;
            int coeff = gf_mult(d,gf_inverse(b));

            poly* tmp = malloc(sizeof(poly)); 
            create_poly(tmp, m+1);
            tmp->coeffs[m] = coeff;

            

            C = gf_poly_add(C, gf_mult_poly(tmp, B));
            L = n + 1 - L;
            B = T;
            b = d;
            m = 1;
        }else{
            int coeff = gf_mult(d,gf_inverse(b));;
            poly* tmp = malloc(sizeof(poly)); 
            create_poly(tmp, m+1);
            tmp->coeffs[m] = coeff;
            C = gf_poly_add(C, gf_mult_poly(tmp, B));
            m=m+1;
        }
        
    }

    if(L == 0){
        return 0;
    }

    return C;




}

void reassemble_message(poly* errors, poly* locations, poly* M){
    int location = 0;
    int error = 0;

    for(int i =0; i < M->size; i++){
        for(int j = 0; j < errors->size; j++){
            location = locations->coeffs[j];
            error = errors->coeffs[j];
            if(location == i){
                M->coeffs[location] ^= error;
                break;
            }
        }
    }

}
