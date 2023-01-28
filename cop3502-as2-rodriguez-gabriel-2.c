This program is a program that reads from an input and dynamically allocates memory for failfish
structs and failfish_queue structs and stores all the information read in from the file in these
dynamically allocated structs. The program uses a circular doubly linked list to store the failfish
structs. Inside of each failfish_queue struct is one of these linked lists and the failfish_queue
structs themselves are organized in an array of pointers. The program uses the values num_fish,
eating_counter, and threshold to systematically eat the fish in the first phase of eating. In the
second phase of eating, the program implements a queue method to find the highest sequence number
out of all the head pointers of the ponds and once found the whole linked list is eaten in order. The
program prints the status of the ponds and eating process throughout to be able to follow what is
happening. At the end of the program, the last fish standing is printed to the output file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"


typedef struct failfish
{
    int sequence_number;
    struct failfish *next;
    struct failfish *prev;

} failfish;


typedef struct failfish_queue
{
    char *name;
    int pond_id;
    int num_fish;
    int eating_counter;
    int threshold;
    failfish *head;
    failfish *tail;

} failfish_queue;


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


/* Adder for failfish list.  p and f must be an allocated failfish_queue and failfish.
   failfish must not already be part of a failfish_queue. */
/* This function adds f to the *tail* of p. */
void enqueue(failfish_queue *p, failfish *f)
{    /* Either we're adding to an empty list, or we're not. */
    if(p->head == NULL) {
    /* We're adding to an empty list.  Set both head and tail to our new
singlet. */
        p->head = f;
        p->tail = f;
        f->prev = f;
        f->next = f;

    } else {
    /* We're adding to a list with at least one item in it.  SET:
           * Our new item's next pointer to the head
           * Our new item's prev pointer to the tail
           * Our head's prev pointer to the new item
           * Our old tail's next pointer to the new item
           * The tail to our new item */
        f->next = p->head;
        f->prev = p->tail;
        p->head->prev = f;
        p->tail->next = f;
        p->tail = f;
    }
}


/* create_failfish: is a function that takes a int id as a parameter and it dynamically
allocates memory for a failfish struct. It then stores the fish parameter id in that struct
and sets both the next and prev pointers to null. This function returns a pointer to the failfish
struct. This function will fail if anything but an int is passed to the function.
*/
failfish *create_failfish(int id)
{
    failfish *f = malloc(sizeof(failfish));

    f->sequence_number = id;
    f->next = NULL;
    f->prev = NULL;

    return f;
}


/* failfish_queue: is a function that takes in parameters of a pointer to an input file, a pointer
to an array of characters name, an int pond_id, an int num_fish, an int threshold, and an int
eating_counter. This function dynamically allocates memory for a failfish_queue struct and uses
all the parameters passed to store those values in this struct. It also calls the create_failfish
function and enqueue function for the num_fish amount of times. This functions returns a pointer
to the failfish_queue struct that it creates. The function will fail if anything but the specified
parameters are passed.
*/
failfish_queue *create_pond(FILE *ifp, char *name, int pond_id, int num_fish, int threshold, int eating_counter)
{
    int sequence_number = 1;

    failfish_queue *p = malloc(sizeof(failfish_queue));

    p->pond_id = pond_id;
    p->name = strdup(name);
    p->num_fish = num_fish;
    p->eating_counter = eating_counter;
    p->threshold = threshold;
    p->head = NULL;
    p->tail = NULL;
    for(int i = 0; i < num_fish; i ++)
    {
        enqueue(p,create_failfish(sequence_number));
        sequence_number++;
    }

    return p;
}

/*free_failfish: is a void function that takes a pointer to a failfish struct as a parameter
and frees the memory. Fails if anything but a pointer to a falfish struct is passed to function */
void free_failfish(failfish *f)
    {
        free(f);
    }


/* Remove f from p.  p must be an allocated failfish list.  Fails silently if f
isn't in p. */
failfish  *dequeue(failfish_queue *p, failfish *f) {
      /* There are four cases of removal in a doubly-linked list with head/tail
pointers.
       Case 0: Trying to remove from an empty list.
       Case 1: Emptying the list.
       Case 2: Removing the head from a non-singlet list.
       Case 3: Removing the tail from a non-singlet list.
       Case 4: Removing an interior element. */

    if(p->head == NULL) {

        /* Case 0: If we're trying to remove from an empty list, don't do that. */
        ; // Do nothing.

    } else if(p->head == p->tail) {
        /* Case 1: Emptying the list. */
        if(f == p->head) {
            p->head = NULL;
            p->tail = NULL;
            f->prev = NULL;
            f->next = NULL;
        }
    } else if(f == p->head) {
    /* Case 2: Removing the head of the list. */
        p->head = p->head->next;
        p->head->prev = p->tail;
        p->tail->next = p->head;
        f->prev = NULL;
        f->next = NULL;
    } else if(f == p->tail) {
    /* Case 3: Removing the tail of the list. */
        p->tail = p->tail->prev;
        p->tail->next = p->head;
        f->prev = NULL;
        f->next = NULL;
    } else {
        f->prev->next = f->next;
        f->next->prev = f->prev;
        f->prev = NULL;
        f->next = NULL;
    }

    return(f);
}


/* Failfish_queue: this function takes in a pointer to an input file and an int num_ponds.
It dynamically allocates memory for an array of pointers. It then uses a for loop to loop through
the number of ponds and reads from the input file and calls the create_pond function and passes
the values scanned from the file. This function returns a pointer to the array of pointers. This
function could fail if anything other than the specified parameters are passed.
*/
failfish_queue **get_ponds(FILE *ifp, int num_ponds)
{
    char buf[255];
    char name[255];
    int pond_id;
    int num_fish;
    int eating_counter;
    int threshold;

    failfish_queue **pond_list = malloc(num_ponds * sizeof(failfish_queue*));
    // For loop for num_ponds and reads from the input file and passes those values to the called]
    // create_pond function to build the array of pointers.
    for(int i = 0; i < num_ponds; i++)
    {
        get_next_nonblank_line(ifp, buf, 255);
    sscanf(buf, "%d %s %d %d %d", &pond_id, name, &num_fish, &eating_counter, &threshold);
        pond_list[i] = create_pond(ifp, name, pond_id, num_fish, threshold, eating_counter);
    }

    return pond_list;
}

/* sort_ponds: is a void function that takes in an array of pointers to failfish_queue structs and
an int that is the number of ponds. The function uses a nested for loop to compare the pond_id of
the failfish_queue structs and rearrange the pointers' position in the array of pointers to be in order from
lowest to highest pond_id. */
void sort_ponds(failfish_queue **pond_list, int num_ponds)
{
    failfish_queue *temp;
    /* Nested for loop that has an if statement to compare one faiflish_queue id to the next
    failfish_queue id and store the larger of the 2 and also switches their position in the array
    */
    for(int i = 0; i < num_ponds; i++)
    {
        for(int j = 0; j < num_ponds; j++)
        {
            if(pond_list[i]->pond_id <= pond_list[j]->pond_id)
            {
                temp = pond_list[i];
                pond_list[i] = pond_list[j];
                pond_list[j] = temp;
            }
        }
    }
}


/* print_status: Is a void function that has three parameters which are an array of pointers to
failfish_queue structs, an int of the number of ponds, and a pointer to an output file. The function
uses a for loop and a do while loop to loop through each pond list and print the name of the pond
the pond id and prints the failfish queue which is the sequence numbers of all the failfish all to
an output file. The function does not return anything. */
void print_status(failfish_queue **pond_list, int num_ponds, FILE *ofp)
{   //For loop to loop through the number of ponds and print the id and name to an output file
    for(int i = 0; i < num_ponds; i++)
    {
        failfish *finder = pond_list[i]->head;

        fprintf(ofp, "%d %s ", pond_list[i]->pond_id, pond_list[i]->name);
        //Do while loop to loop through the failfish_queue and print each fish's sequence number
        do
        {
            fprintf(ofp, "%d ", finder->sequence_number);

            finder = finder->next;

             } while(finder != pond_list[i]->head);

             fprintf(ofp, "\n");
    }
    fprintf(ofp, "\n");
}

/* eating_time: is a function that takes in the parameters of a pointer to an array of pointers,
an int num_ponds, and a pointer to a file. It then uses for loops and a while loop to loop
through each pond and eat failfish based on each ponds eating_counter. The function fails if anything
other than the specified parameters are passed. This is a void function so nothing is returned.
*/
void eating_time(failfish_queue **pond_list, int num_ponds, FILE *ofp)
{
    failfish *finder;
    failfish *temp;

    fprintf(ofp, "First Course\n\n");
    //For loop to loop through the number of ponds
    for(int i = 0; i< num_ponds; i++)
    {
        finder = pond_list[i]->head;

        fprintf(ofp, "Pond %d: %s\n", pond_list[i]->pond_id, pond_list[i]->name);
        //While loop to loop through until the num_fish is the same as the threshold
        while(pond_list[i]->num_fish > pond_list[i]->threshold)
        {   //for loop to loop through eating counter and move finder to eat fish
            for(int j = 0; j < pond_list[i]->eating_counter -1; j++)
            {
                finder = finder->next;
            }
            temp = finder;
            finder = finder->next;
            temp = dequeue(pond_list[i], temp);

            fprintf(ofp, "Failfish %d eaten\n", temp->sequence_number);
            free_failfish(temp);

            pond_list[i]->num_fish--;

        }
        fprintf(ofp, "\n");
    }
}

/* total_fish: is a function that takes in a pointer to an array of pointers and an int num_ponds
to loop through each pond and sum the total number of fish in all the ponds. This function returns
an int which will be the number of all the fish in all the ponds.
*/
int total_fish(failfish_queue **pond_list, int num_ponds)
{
    int total_fish = 0;
    //For loop to loop through each pond and incrememnt total_fish by each ponds num_fish
    for(int i = 0; i < num_ponds; i++)
    {
       total_fish += pond_list[i]->num_fish;
    }
    return total_fish;
}

/* peek: is a function that takes in a parameter of a pointer to a failfish_queue and finds the
sequence number of the fish that is at the head of the linked list. The sequence number of that
fish is returned from the function.
*/
int peek(failfish_queue *pond)
{
    int sequence_number = pond->head->sequence_number;

    return sequence_number;
}

/* is_empty: is a function that takes in a pointer to a failfish_queue as a parameter and determines
whether or not the head pointer of the pond is null or not. If it is null or empty it returns 0
(false) and if it is not empty it returns 1 or (true).
*/
int is_empty(failfish_queue *pond)
{
    if(pond->head == NULL)
    {
        return 0;
    }
    else{
        return 1;
    }
}


/* eating_time_again: This function takes in the parameters of a pointer to an array of pointers,
an int num_ponds, and a pointer to an output file. It uses while loops and a for loop to loop while
the total num of fish is greater than 1 and a for loop to loop through each pond and loop while
the head pointer is not null and it uses a queue method to eat the failfish queue with the highest
sequence number at its head. If there is a tie, the failfish_queue with the lowest pond_id is eaten.
*/
void eating_time_again(failfish_queue **pond_list, int num_ponds, FILE *ofp)
{
    int total_num_fish = total_fish(pond_list, num_ponds);
    int temp_id = 0;
    failfish *temp_fish;
    failfish *temp_fish2;
    failfish_queue *temp_pond;

    fprintf(ofp, "Second Course\n\n");
    //While loop while total_num_fish is greater than 1
    while(total_num_fish > 1)
    {
        temp_id = 0;
    //for loop to loop through num of ponds
    for(int i = 0; i < num_ponds; i++)
    {   //if statements to check if list is empty and find the highest sequence number and store it
        // and point to that fish and its failfish_queue
        if(is_empty(pond_list[i]))
        {
        if(peek(pond_list[i]) > temp_id)
        {
            temp_id = pond_list[i]->head->sequence_number;
            temp_fish = pond_list[i]->head;
            temp_pond = pond_list[i];
        }
        }
    }
    //While loop to dequeue the failfish_queue and print to the output file
    while(temp_pond->head != NULL && total_num_fish > 1)
    {
        temp_fish2 = temp_fish;
        temp_fish = temp_fish->next;
        temp_fish2 = dequeue(temp_pond,temp_fish2);

        fprintf(ofp,"Eaten: Failfish %d from pond %d\n", temp_fish2->sequence_number, temp_pond->pond_id);
        free_failfish(temp_fish2);

        total_num_fish--;
    }
    }
    fprintf(ofp, "\nFish %d from pond %d remains", temp_fish->sequence_number, temp_pond->pond_id);
    }


    /* free_failfish_queue: is a function that takes in a pointer to an array of pointers and an int
    num_ponds and it uses a for loop and while loop to make sure that we are freeing memory that
    we actually have allocated. It calls the dequeue function and frees all the memory allocated
    in the program.
    */
    void free_failfish_queue(failfish_queue **p, int num_ponds)
    {
        failfish *finder;
        failfish *temp;
        //For loop for number of ponds
        for(int i = 0; i < num_ponds; i++)
        {
            free(p[i]->name);

            if(p[i]->head != NULL)
            {
                finder = p[i]->head;
            //While loop to move finder and call dequeue function and free the tmep variable
            while(finder != NULL)
            {
                temp = finder;

                if(finder->next == p[i]->head)
                {
                    finder = NULL;
                }
                dequeue(p[i], temp);
                free(temp);
            }
        }
        free(p[i]);
    }
    free(p);
    }

    /* print_failfish_queue: is a function that takes a pointer to a failfish queue as a parameter
    and it uses a do while loop to print out the name of the pond, pond id, and each fish sequence
    number in the linked list. This is a void function that doesn't return anything. This function
    could fail if the faifish_queue that is passed doesn't exist or has already been eaten.
    */
    void print_failfish_queue(failfish_queue *q)
    {
         failfish *finder = q->head;

        printf("%d %s ", q->pond_id, q->name);
        //Do while loop to loop through the failfish_queue and print each fish's sequence number
        do
        {
            printf("%d ", finder->sequence_number);

            finder = finder->next;

             } while(finder != q->head);

             printf("\n");
    }


int main()
{
    atexit(report_mem_leak);

    char buf[255];
    int num_ponds;
    FILE *ifp = fopen("cop3502-as2-input.txt.txt", "r");
    FILE *ofp = fopen("cop3502-as2-output-rodriguez-gabriel.txt", "w");

    get_next_nonblank_line(ifp, buf, 255);
    sscanf(buf, "%d", &num_ponds);

    failfish_queue **pond_list = get_ponds(ifp, num_ponds);

    sort_ponds(pond_list, num_ponds);

    fprintf(ofp, "Initial Pond Status\n");
    print_status(pond_list, num_ponds, ofp);

    eating_time(pond_list, num_ponds, ofp);

    fprintf(ofp, "\nEnd Of First Course Pond Status\n");
    print_status(pond_list, num_ponds, ofp);

    eating_time_again(pond_list, num_ponds, ofp);

    free_failfish_queue(pond_list, num_ponds);

    fclose(ifp);
    fclose(ofp);

    return 0;
}
