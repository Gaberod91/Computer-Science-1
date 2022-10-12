/*
Gabriel Rodriguez
COP3502
Assignment 5 Heapify
This function reads from an input file and collects a total amount n of numbers and a value for
each n number. It dynamically allocates memory for an array the size of the value from the input
file and fills the elements of that array with the valeus from the input file. It recursively calls
a heapify function to organize a max heap in the array. It then removes the root of the max heap and
calls the heapify function over and over again until the array is empty. It prints each step of this
process.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* remove_crlf: traverse s and clobber any and all newline characters on its right
with nulls. s must be a properly allocated string. */
void remove_crlf(char *s)
{
    char *end = s + strlen(s);

    end--;

    while((end >= s) && (*end == '\n' || *end == '\r'))
    {
        *end = '\0';

        end--;
    }
}


/*get_next_nonblank_line: function is a function that takes in parameters of a pointer to an input
file, a character pointer buf, and an int max_length. The function uses a while loop to read in
strings from a file and the while loop breaks if we are either at the end of the file or the first
element of the array of characters is not a null character. This means that we do not read any blank
lines from the input file. This function returns a string of input from the file that is not blank.
*/
int get_next_nonblank_line(FILE *ifp, char *buf, int max_length)
{
    buf[0] = '\0';

    while(!feof(ifp) && (buf[0] == '\0'))
    {
        fgets(buf, max_length, ifp);

        remove_crlf(buf);
    }

    if(buf[0] == '\0')
    {
        return 0;
    }
     else
    {
        return 1;
    }
}


/*print_heap: prints the values of the array
function takes in the parameters of a pointer to an integer array heap_array and int num_count
and uses a for loop to print each value in the array.
*/
void print_heap(FILE *ofp, int *heap_array, int num_count)
{
    for(int i = 0; i < num_count; i++)
    {
        fprintf(ofp, "%d ", heap_array[i]);
    }

    fprintf(ofp, "\n");
}


/*swap: function takes in to pointers to ints and swaps their values
This function takes in two pointers to integers and swaps their values
*/
void swap(int *a, int *b)
{
    //temp to hold value of an int to swap values of two ints
    int temp;

    temp = *a;

    *a = *b;

    *b = temp;
}


/*heapify: function uses if conditions to replace the value of largest and if condition to recursively
call the heapify function to heapify the array.
This function takes in the parameters of a pointer to int heap_array and int num_count and int i.
The function has if conditions if the position left or right in the array is larger than position
largest and if it is then largest holds that position. In these cases it swaps the values at the
positions in the array and recursively calls heapify.
*/
void heapify(int *heap_array, int num_count, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if(left < num_count && heap_array[left] > heap_array[largest])
    {
        largest = left;
    }

    if(right < num_count && heap_array[right] > heap_array[largest])
    {
        largest = right;
    }

    if(largest != i)
    {
        swap(&heap_array[i], &heap_array[largest]);

        heapify(heap_array, num_count, largest);
    }
}


/*down_heapify: function uses a for loop to get to the leaf nodes and call heapify and print heap
This function takes in the parameters of a pointer to int heap_array and int num_count. It creates
the start value by dividing the int num count by 2 and subtracting 1 which is used for the loop.
It uses a for loop to call heapify and print_heap.
*/
void down_heapify(FILE *ofp, int *heap_array, int num_count)
{
    int start = (num_count/2) - 1;

    for(int i = start; i >= 0; i--)
    {
        heapify(heap_array, num_count, i);
        print_heap(ofp, heap_array, num_count);
    }

}


/*delete_heap: deletes elements of the array and readjusts the size of the array and prints the array
takes parameters of pointer ot int heap_array and int num_ count and loops through the size of the array
to print the value at the element being removed, swaps the value being removed with the last element
in the array, it reallocs the size of the array for num_count - 1 and calls heapify again and
prints the array.
*/
void delete_heap(FILE *ofp, int *heap_array, int num_count)
{
    int loop = num_count;

    for(int i = 0; i < loop; i++)
    {
    num_count--;

    fprintf(ofp, "%d\n", heap_array[0]);

    swap(&heap_array[0], &heap_array[num_count]);

    heap_array = realloc(heap_array, num_count * sizeof(int));

    heapify(heap_array, num_count, 0);

    print_heap(ofp, heap_array, num_count);

    }
    }


void read()
{
    char buf[255];
    int num_count;
    int num;

    FILE *ifp = fopen("cop3502-as5-input.txt", "r");
    FILE *ofp = fopen("cop3502-as5-output-rodriguez-gabriel.txt", "w");

    get_next_nonblank_line(ifp, buf, 255);
    sscanf(buf, "%d", &num_count);
    //malloc the array for size of num_count from input file.
    int *heap_array  = malloc(num_count * sizeof(int));
    //For loop to loop through num_count times and fill elements of the array with values from input
    for(int i = 0; i < num_count; i++)
    {
        get_next_nonblank_line(ifp, buf, 255);
        sscanf(buf, "%d", &num);
        heap_array[i] = num;
    }
    //Function calls
    print_heap(ofp, heap_array, num_count);

    down_heapify(ofp, heap_array, num_count);

    print_heap(ofp, heap_array, num_count);

    delete_heap(ofp, heap_array, num_count);
}


int main()
{
    read();

    return 0;
}
