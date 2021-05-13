#include <main.h>

void main(){
    setup_tables();
    char* stream1 = malloc(sizeof(char)*65);
    char* stream2 = malloc(sizeof(char)*65);
    char* key_bits = malloc(sizeof(char)*129);
    char* error_bits = malloc(sizeof(char)*129);
    time_t ti;
    int n = MESSAGE_SIZE;
    int k = KEY_SIZE;
    int t = (int)((n-k)/2);
    poly* g_x = g(t);
    poly* p1 = malloc(sizeof(poly));
    poly* error = malloc(sizeof(poly));
    poly* key = malloc(sizeof(poly));

    //16 = 128/8 because each coeff is 8 bits
    create_poly(p1, k);
    create_poly(error, n);
    create_poly(key, n);

    //Randomizer init
    srand((unsigned) time(&ti));
    create_stream(stream1, 64);
    create_stream(key_bits, 128);

    //Creates first stream without error
    char_arr_to_poly(stream1, 64, p1);
    // printf("Poly 1 = \n");
    // print_poly(p1);

    //Create second bit stream with 10% error
    create_stream_with_error(key_bits, error_bits, 128, 5);


    char_arr_to_poly(error_bits, 128, error);
    // printf("error = %s\n", error_bits);
    // print_poly(error);

    char_arr_to_poly(key_bits, 128, key);
    // printf("key = \n");
    // print_poly(key);

}

void create_stream(char* s, int size){
    int i;
    int n;

    for(i =0; i < size; i++){
        n = rand()%2;
        s[i] = n+'0';
    }
    s[i] = '\0';

}

//error should be 0-100, 100 being every bit is wrong. 0 is every bit matches
void create_stream_with_error(char* s1, char* s, int size, int error){
    int i;
    int n;

    for(i =0; i < size; i++){
        n = rand()%100;
        char bit = s1[i];
        if(n > error){
            s[i] = '0';
        }else{
            s[i] = '1';
        }
    }
    s[i] = '\0';

}

poly* RSEncode(poly* g_x, poly* input, poly* key, poly* error){
    poly* C = gf_mult_poly(g_x, input);
    C = gf_poly_add(C, key);
    C = gf_poly_add(C, error);
    return C;
}


poly* RSDecode(int t, poly* C, poly* key, poly* g){

    C = gf_poly_add(key,C);

    synd* S = syndome_calculator_division(C, g, t);

    poly* sig = berlecamp_table(S->p, S->synds);
    if(sig != 0){
        poly* s_r = sigma_r(sig);

        poly* roots = roots_of_poly(s_r,t,NW-1);

        poly* errors = error_correction(roots,  S->p);
        reassemble_message(errors, roots, C);

    

        poly* M2 = gf_div_poly(C, g, 0);
        
        free_poly(sig);
        free_synd(S);
        free_poly(roots);
        free_poly(errors);
        return M2;
    }else{
        poly* message = gf_div_poly(C, g, 0);
        printf("FAILED MESSAGE DECODE\n");
        return message;
    }

}

void char_arr_to_poly(char* s, int size, poly* p){

    char buffer[9];
    int i = 0;
    int count = 0;

    for(i=0; i < size; i++){
        if(i%8 == 0 && i > 0){
            p->coeffs[count] = bin_to_int(buffer, 7);
            count+=1; 
        }else{
            buffer[i%8] = s[i];
        }
    }

}


int bin_to_int(char* buffer, int len){
    int i;
    int ret = 0;

    for(i = 0; i < len; i++){
        char c = buffer[i];
        if(c == '1'){
            ret += (int)pow(2,len-i);
        }
    }

    return ret;

}