//
// Created by galdu on 12/11/2022.
//



void quicksort(char const* arr, unsigned int length)
{
    unsigned int i = 0;
    unsigned int piv = 0;

    if(length <= 1)
        return;

    do {

        if(arr[i] == ' ')
        {
            i++;
            continue;
        }
        if(strcmp(arr[i], arr[length - 1]) < 0)
            swap(arr + i, arr + length -1);

        i++;
    }while(i < length);
}