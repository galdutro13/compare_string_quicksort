/* Quick sort algorithm to sort strings */
#include <unistd.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

#define ASCSUB -48
#define THRESHHOLD 16

void inline __attribute__((always_inline)) fmag(int value, int mag, int* mem)
{
    value = value + ASCSUB;
    value = value * mag;
    *mem = *mem + value;

}

#pragma omp declare simd
void inline __attribute__((always_inline)) swap(float* a, float* b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

void quicksort_serial(float* arr, unsigned int length) {
    unsigned int i, piv = 0;
    if (length <= 1)
        return;
#pragma omp simd
    for (i = 0; i < length; i++) {

        if ( arr[i] < arr[length -1])    //use string in last index as pivot
            swap(arr + i, arr + piv++);
    }
    //move pivot to "middle"
    swap(arr + piv, arr + length - 1);

    //recursively sort upper and lower

    quicksort_serial(arr, piv++);            //set length to current pvt and increase for next call

    //#pragma omp task default(none) shared(length) firstprivate(arr, piv)
    {
        quicksort_serial(arr + piv, length - piv);

    }
}
void quicksort_parallel(float* arr, unsigned int length, unsigned int depth) {
    unsigned int i, piv = 0;
    if (length <= 1)
        return;
#pragma omp simd
    for (i = 0; i < length; i++) {

        if ( arr[i] < arr[length -1])    //use string in last index as pivot
            swap(arr + i, arr + piv++);
    }
    //move pivot to "middle"
    swap(arr + piv, arr + length - 1);

    //recursively sort upper and lower

    if(depth < THRESHHOLD) {
        quicksort_parallel(arr, piv++, depth++);            //set length to current pvt and increase for next call

        #pragma omp task mergeable default(none) shared(length) firstprivate(arr, piv, depth)
        quicksort_parallel(arr + piv, length - piv, depth);
    } else{
        quicksort_serial(arr, piv++);

        quicksort_serial(arr + piv, length - piv);
    }


}

int findsize(char* array)
{   char* tmp = array;
    int count = 0;

    while (*tmp)
    {
        if (*tmp == '\n')
        {
            count++;

        }

        tmp++;
    }

    return count;
}


float * str_split(char* a_str)
{   float* resultf;
    int count     = 0;
    int strsize   = 0;
    char* tmp     = a_str;
    char delim[2];
    delim[0] = '\n';
    delim[1] = 0;

    //#pragma omp parallel default(none) firstprivate(tmp, count, strsize)
    while (*tmp)
    {
        if (*tmp == '\n' && *(tmp+1) != '\000')
        {
            count++;
        }
        strsize++;
        tmp++;
    }

    /* Add space for trailing token. */
    //count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;
    resultf = malloc(sizeof(float*) * count);


    if (resultf)
    {
        int magmask = 1;
        int wholepart = 0;
        float decimalpart = 0;
        float converted_value = 0;


        char* tmpPointer = a_str;

        for(int i = 0; i < count; i++)
        {

            do
            {
                tmpPointer++;
                if(*tmpPointer == '.' || *tmpPointer == ' ') //Colocando a condicional aqui para ter o mesmo comportamento 'do -> while'
                    break;             //Ou seja, a condição ser verificada após a operação do ponteiro
                magmask = magmask * 10; //magmask só mudará de casa, caso a parte inteira não tenha terminado
            } while (*tmpPointer != '\n' && *tmpPointer != ' ');



            while (*a_str != '.')
            {

                fmag((int) *a_str, magmask, &wholepart);
                magmask = magmask / 10;
                if(*a_str == '\n' || *a_str == ' ')
                    break;

            a_str++;
            }

            converted_value = (float) wholepart;

            if(*a_str == ' ')
            {
                a_str++; //eliminate ' '
                a_str++; //eliminate '\n'
                /*encaminha a execução para um codigo que resetará as variáveis do forloop */
                goto next;
            } else if (*a_str == '\n'){
                a_str++; //eliminate '\n'
                /*encaminha a execução para um codigo que resetará as variáveis do forloop */
                goto next;
            }

            a_str++;
            magmask = 1;

            while(*a_str != ' ' && *a_str != '\n')
            {
                magmask = magmask * 10;
                decimalpart +=  ((float)(*a_str + ASCSUB)) / (float)magmask;
                a_str++;

            }

            converted_value = converted_value + decimalpart;
            a_str++;
            a_str++;

            /*AQUI ESTÁ O GOTO*/
            next:
                resultf[i] = converted_value; //coloca o valor no array a ser ordenado
                magmask = 1; //Reseta o magmask
                wholepart = 0; //reseta o wholepart
                decimalpart = 0; //reseta o decimalpart
                while (*a_str == ' ') //Corrige para bungs em que o número iniciase com espaço
                    a_str++;
                tmpPointer = a_str; //Realinha os ponteiros em relação a referencia
        }

    }

    return resultf;
}

char* readfile(const char* filename){

    char *string_of_numbers;
    long size = 0;

    FILE* entrada = fopen(filename, "r");

    if(fseek(entrada, 0, SEEK_END) != 0)
        exit(fprintf(stderr, "Erro ao fazer o seek do fim do arquivo.\n"));

    size = ftell(entrada);

    if(fseek(entrada, 0, SEEK_SET) != 0)
        exit(fprintf(stderr, "Erro ao fazer o seek-set para a leitura do arquivo\n"));

    string_of_numbers = malloc(size + 1);


    if(fread(string_of_numbers, size, 1, entrada) != 1)
        exit(fprintf(stderr, "Não foi possivel ler até o fim do arquivo\n"));

    fclose(entrada);

    return string_of_numbers;

}

int main() {

        char* string = readfile("dados_10000000.txt");
        unsigned int i = 0;
        float* arr_string;



        arr_string = str_split(string);

        int limit = findsize(string);

        clock_t t_start = clock();

#pragma omp parallel default(shared) firstprivate(arr_string) num_threads(8)
#pragma omp single
        quicksort_parallel(arr_string, limit,0);


        clock_t t_ends = clock() - t_start;
        long double time_taken = ((long double) t_ends) / CLOCKS_PER_SEC;

#pragma omp parallel default(shared) num_threads(8)
#pragma omp single
       do {
#pragma omp task default(shared)
           {
               float c = arr_string[i];
               unsigned int j = 0;
               printf("%f;  ", c);
           }

            i++;
        } while (arr_string[i]);

        printf("\nThe program took %Lf seconds to execute", time_taken);
}