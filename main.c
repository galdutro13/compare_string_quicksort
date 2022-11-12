/* Quick sort algorithm to sort strings */
#include <unistd.h>
#include <stdio.h>


#define ASCSUB -48
#define MEMORY_CAPACITY 1048576


int size_posb = 0;
int size_atob = 0;
unsigned int total_length = 0;
char *string = "8.3\n1.7\n3.1\n3.7\n5.4\n3084.72\n12.1\n\000";


unsigned int find_end(char* arr){
    unsigned int index = 0;

    while (*arr != '\n')
    {
        arr++;
        index++;
    }
    return index+1;
}

unsigned int find_start(char* arr, int length)
{
    unsigned int index = 1;

    while(index <= length && arr[length - index] != '\n')
        index++;

    return index-1;
}

unsigned int findsize(char* array)
{   char* tmp = array;
    int count = 0;

    while (*tmp)
    {
        tmp++;
        count++;
    }

    return count;
}
char* create_tmp(char* base, unsigned int size)
{
    unsigned int length_t;
    unsigned int index = 0;
    if(size == 0)
        length_t = findsize(base);
    else
        length_t = size;

    char tmpstr[length_t];


    while (index < length_t){
        tmpstr[index] = base[index];
        index++;
    }

    return tmpstr;

}

void  fmag(int value, int mag, int* mem)
{
    value = value + ASCSUB;
    value = value * mag;
    *mem = *mem + value;

}
void  fmag_simple(int value, int* mem)
{
    value = value + ASCSUB;
    *mem = *mem + value;
}

int __attribute__ ((hot, flatten)) lstrcmp(char *X, char *Y)
{

    const char *Xi = X, *Yi = Y;
    int memx = 0, memy = 0;
    int Xb = 1, Yb = 1;

    do{
        Xi++;
        Xb = Xb*10;
    }while (*Xi != '.');

    do{
        Yi++;
        Yb = Yb*10;
    }while(*Yi != '.');

    Xi = X, Yi = Y;
// ---------------------------------------
    while (*Xi != '.'){
        fmag(*Xi, Xb, &memx);
        Xb = Xb/10;
        Xi++;
    }

    while (*Yi != '.'){
        fmag(*Yi, Yb, &memy);
        Yb = Yb/10;
        Yi++;
    }

    if((memx != memy) || (X == Y))
        return (memx - memy);

    Xi++;
    while (*Xi && *Xi != ' '){
        fmag_simple(*Xi, &memx);
        Xi++;
    }

    Yi++;
    while (*Yi && *Yi != ' '){
        fmag_simple(*Yi, &memy);
        Yi++;
    }

    // return the ASCII difference after converting `char*` to `unsigned char*`
    return (memx - memy);
}

void swap(char *a, char *b) {
    char* tempa = a;
    char* tempb = b;
    unsigned int atob_size = (unsigned int) (b - a);

    unsigned int sizea = find_end(a);
    unsigned int sizeb = find_end(b);

    char* iso_a = create_tmp(a, sizea);
    char* iso_b = create_tmp(b, sizeb);
    char* iso_atob = create_tmp(a+sizea, atob_size);

    while(*a != '\n' && *b != '\n') {
        *a = *b;
        *b = tempa;
        a++;
        b++;
    }
}


void quicksort(char* arr, unsigned int length)
{
    unsigned int i = 0;
    unsigned int piv = 0;
    unsigned int startofend = 0;
    if(length <= 1)
        return;

    startofend = find_start(arr, length);

    do {

        if(arr[i] == '\n')
        {
            i++;
            continue;
        }
        if(lstrcmp(&arr[i], &arr[length - startofend]) < 0)
            swap(arr + i, arr + piv++);

        i++;
    }while(i < length);

    swap(arr + piv, arr + length - 1);
    quicksort(arr, piv++);
    quicksort(arr + piv, length - piv);
}

int main(int argc, char** argv) {
//	char const *arr[] = {"8.3", "1.7", "3.1", "3.7","3.9","32.17", "3029.81", "18.0", "13.0"};
//    int limit = sizeof(arr) / sizeof(*arr);
//    quicksort(arr, limit);


    int i;
    //char** arr_string = str_split(string);

    unsigned int limit = findsize(string);
    total_length = limit;

    //printf("\ntamanho do array %d\ntamanho do ponteiro %d\n", sizeof(arr_string),sizeof(*arr_string));

    /*quicksort(string, limit);
    for (i=0; i < limit; i++) {
        printf("%c ",string[i]);
        //puts(arr[i]);
    }*/


    char* newtemporary = "1496.74 \n";

    int vq = find_end(newtemporary);
    char* newvq = create_tmp(newtemporary, vq);

    printf("%d", vq);
}