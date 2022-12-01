/* Quick sort algorithm to sort strings */
#include <unistd.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

#define ASCSUB -48
#define MEMORY_CAPACITY 8388608

typedef struct free_block {
    int size;
    struct free_block* next;
} free_block;

static free_block free_block_list_head = { 0, 0 };


static const int align_to = 16;
void __attribute__ ((optimize("toplevel-reorder")))*mov_sbrk(int increment)
{
    static char global_mem[MEMORY_CAPACITY] = {0};
    static char *p_break = global_mem;

    char *const limit = global_mem + MEMORY_CAPACITY;
    char *const original = p_break;

    if (increment < global_mem - p_break  || //pbreak initially equals global_mem
    increment >= limit - p_break) //limit - p_break equals MEMORY_CAPACITY
    {
        return (void*)-1;
    }
    p_break += increment;

    return original;
}
void* __attribute__ ((flatten, malloc, optimize("toplevel-reorder"))) malloc(int size) {
    size = (size + sizeof(free_block) + (align_to - 1)) & ~ (align_to - 1);
    free_block* block = free_block_list_head.next;
    free_block** head = &(free_block_list_head.next);


    while (block != 0) {
        if (block->size >= size) {
            *head = block->next;
            return ((char*)block) + sizeof(free_block);
        }
        head = &(block->next);
        block = block->next;
    }

    block = (free_block*)mov_sbrk(size);
    block->size = size;

    return ((char*)block) + sizeof(free_block);
}


void free(void* ptr) {
    free_block* block = (free_block*)(((char*)ptr) - sizeof(size_t));
    block->next = free_block_list_head.next;
    free_block_list_head.next = block;
}


void inline fmag(int value, int mag, int* mem)
{
    value = value + ASCSUB;
    value = value * mag;
    *mem = *mem + value;

}
void inline fmag_simple(int value, int* mem)
{
    value = value + ASCSUB;
    *mem = *mem + value;
}

int __attribute__ ((hot, flatten)) strcmp(const char *X, const char *Y)
{

    const char *Xi = X, *Yi = Y;
    int memx = 0, memy = 0;
    int Xb = 1, Yb = 1;

    #pragma omp ordered
    do{
        Xi++;
        Xb = Xb*10;
    }while (*Xi != '.');

    #pragma omp ordered
    do{
        Yi++;
        Yb = Yb*10;
    }while(*Yi != '.');

    Xi = X, Yi = Y;
// ---------------------------------------
    #pragma omp ordered
    while (*Xi != '.'){
        fmag(*Xi, Xb, &memx);
        Xb = Xb/10;
        Xi++;
    }

    #pragma omp ordered
    while (*Yi != '.'){
        fmag(*Yi, Yb, &memy);
        Yb = Yb/10;
        Yi++;
    }

    if((memx != memy) || (X == Y))
        return (memx - memy);

    Xi++;
    Yi++;


    while (*Xi && *Yi){
        if(*Xi > *Yi) {
            fmag_simple(*Xi, &memx);
            break;
        }
        else if(*Yi > *Xi) {
            fmag_simple(*Yi, &memy);
            break;
        }
        Xi++;
        Yi++;
    }

    // return the ASCII difference after converting `char*` to `unsigned char*`
    return  (memx - memy);
}

void inline swap(char **a, char **b) {
    #pragma omp critical
    {
        const char *temp = *a;
        *a = *b;
        *b = temp;
    }
}

void quicksort(char const ** arr, unsigned int length) {
    unsigned int i, piv = 0;
    if (length <= 1)
        return;
    #pragma omp parallel for num_threads(8)
    for (i = 0; i < length; i++) {

        if (strcmp(arr[i], arr[length -1]) < 0) 	//use string in last index as pivot
            swap(arr + i, arr + piv++);
    }
    //move pivot to "middle"
    swap(arr + piv, arr + length - 1);

    //recursively sort upper and lower
    quicksort(arr, piv++);			//set length to current pvt and increase for next call
    quicksort(arr + piv, length - piv);
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
char** str_split(char* a_str)
{
    char** result;
    int count     = 0;
    int strsize   = 0;
    char* tmp        = a_str;
    __attribute__((unused)) char* last_comma;
    char delim[2];
    delim[0] = '\n';
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (*tmp == '\n')
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

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        __attribute__((unused)) int idx;
        int tokensize = 0;
        __attribute__((unused)) char currvalue = 0;
        char* newString;
        char* tmpPointer = a_str;
        //char* token = strtok(a_str, '\n');

        for(int i = 0; i < count; i++)
        {
            while (*tmpPointer)
            {
                if(*tmpPointer == '\n')
                {
                    tmpPointer++;
                    break;
                }
                tokensize++;
                tmpPointer++;
            }

            newString = malloc(sizeof(char) * tokensize);
            result[i] = newString;
            while (*a_str)
            {
                if(*a_str == '\n')
                {
                    a_str++;
                    break;
                }

                *newString = *a_str;
                a_str++;
                newString++;
            }

        }

    }

    return result;
}

int main(int argc, char** argv) {

    char *string = "86.022 \n5.7183 \n27.033 \n34.308 \n37.638 \n6.0957 \n45.662 \n46.794 \n79.383 \n66.569 \n33.24 \n12.5 \n15.75 \n33.29 \n3.28 \n12.178 \n";
    unsigned short j = 0;
    unsigned int i = 0;
    char **arr_string;

    clock_t t_start = clock();


    do {
        arr_string = str_split(string);
        int limit = findsize(string);

        //printf("\ntamanho do array %d\ntamanho do ponteiro %d\n", sizeof(arr_string), sizeof(*arr_string));

        quicksort(arr_string, limit);
        j++;
        if(j != 2048)
            free(arr_string);
    }while(j < 2048);



    clock_t t_ends = clock() - t_start;
    long double time_taken = ((long double) t_ends) / CLOCKS_PER_SEC;

    do{
        char* c = arr_string[i];
        unsigned int j = 0;
        do{
            printf("%c", c[j]);
            j++;
        }while(c[j]);

        i++;
    }while(*arr_string[i]);


    printf("The program took %Lf seconds to execute", time_taken);
}