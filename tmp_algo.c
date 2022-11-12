//
// Created by galdu on 12/11/2022.
//


int __attribute__((hot, flatten)) strcmp(const char *X, const char *Y)
{

}

unsigned int inline find_start(char const* arr, int length)
{
    unsigned int index = 1;

    while(arr[length - index] != ' ')
        index++;

    return index-1;
}
void quicksort(char const* arr, unsigned int length)
{
    unsigned int i = 0;
    unsigned int piv = 0;
    unsigned int startofend = 0;
    if(length <= 1)
        return;

    startofend = find_start(arr, length);

    do {

        if(arr[i] == ' ' || arr[i] == '\n')
        {
            i++;
            continue;
        }
        if(strcmp(&arr[i], &arr[length - startofend]) < 0)
            swap(arr + i, arr + length -1);

        i++;
    }while(i < length);

    swap(arr + piv, arr + length - 1);
    quicksort(arr, piv++);
    quicksort(arr + piv, length - piv);
}