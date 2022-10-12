/*
Gabriel Rodriguez
PA1 Monster Assignment
This is a program that reads in an input file of monsters, regions, trainers, and dynamically
allocates memory for them. It then accesses that dynamically allocated memory to extract the
necessary information, perform calculations, and print to an output file the names of the trainers,
the regions they visit, and the amount of monsters that they will catch in each region.
COP3502
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cop3502_as1.h"
#include "leak_detector_c.h"


/* remove_crlf: traverse s and clobber any and all newline characters on its right
with nulls. s must be a properly allocated string. */
void remove_crlf(char *s)
{

    char *end = s + strlen(s);

    end--;

    while((end >= s) && (*end == '\n' || *end == '\r')) {
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
    } else {
        return 1;
    }

}


/*construct_monster: function is a function that dynamically allocates memory for a monster struct
and returns a pointer to the dynamically allocated struct. The parameters are a character pointer to
a name, a character pointer to an element, an integer population, and an integer id. The function
assigns these input parameters to the corresponding values in the dynamically allocated monster
struct.
**Constructs monsters**
*/
monster *construct_monster(char *name, char *element, int population, int id)
{
    /*Here I am using malloc to malloc the size of a monster struct an then I am using malloc
    to malloc the strings and strcpy to store the name and element of the monster into the struct.
    I also assign the id and total population, then I read the monster to the calling function.
    */
    monster *nmonster = malloc(sizeof(monster));

    nmonster->name = malloc((strlen(name) +1) * sizeof(char));

    strcpy(nmonster->name, name);

    nmonster->element = malloc((strlen(element) + 1) * sizeof(char));

    strcpy(nmonster->element, element);

    nmonster->population = population;

    nmonster->id = id;

return nmonster;
}


/*get_monster: function is a function that dynamically allocates memory for an array of pointers
to monster structs and reads in an input file using fgets via get_next_nonblank_line function and
uses sscanf to extract and temporarily store the information collected from the input file. It then
calls the monster constructor and passes the name, element, and population collected from the input.
This function returns a pointer to an array of pointers. The parameters of this function are the
pointer to the input file to be able to read the file and the monster_count collected in main in
order to know how many times to loop through calling the monster_constructor.
**Loop through monsters and builds array of pointers to monsters**
*/
monster **get_monster(FILE *ifp, int monster_count)
{
    char buf[255];
    char name[255];
    char element[255];
    int population, id = 1;

    /*Here we are using malloc to make an array of pointers and then I am using a for loop to
    read a line from the input file, use sscanf to extract the name, element, and population of
    the monster, and then I call my monster constructor function and pass that information I just
    read in. I then get back the monster that fills the corresponding element of my array of pointers
    per the cycle of the for loop that I am in. I then return my array of pointers to main.
    */
    monster **monster_list = malloc(monster_count * sizeof(monster*));

    for(int i=0; i<monster_count; i++)
    {
        get_next_nonblank_line(ifp, buf, 255);

        sscanf(buf,"%s %s %d", name, element, &population);

        monster_list[i] = construct_monster(name, element, population, id);

        id++;
   }
   return monster_list;
}


/*construct_region: function is a function that dynamically allocates memory for a region struct
and returns a pointer to the dynamically allocated struct. The parameters are a pointer to a file
that way we can read the input file, an int monster count to know how many times to loop through
the monsters in my main monster list as we figure out which monsters belong in my region monster
list by comparing the name read in from the input file to the name in the my main monster list. We
also have the main monster list monster **monster_list as a parameter, we need this to be able to
access the main monster list and construct our region monster list. The parameter int nmonsters is
to find how many monsters belong in each region based on what is read in from the input file. The
pointer to character name parameter is a name of a region that is read in from the input file and
allows us to know the name of the region that we are constructing.
**Constructs regions**
*/
region *construct_region(FILE *ifp, int monster_count, monster **monster_list, int nmonsters, char *name)
{
    char monster_name[255];
    char buf[255];
    int total_population = 0;

    /*Here I use malloc to dynamically allocate memory for the size of a region struct, I then malloc
    for the region name and I store the various parts of the region struct. I then use malloc again
    to dynamically allocate my array of pointers and I use a for loop and a nested for loop to
    read a name from the input file and compare that name to my monster list in order to find out
    which monster belongs in my region monster list. I also have a total_population variable
    that gets summed by the population of each monster in my region monster list and then I store
    the total population in the reigon total_population. I then return the region to the calling
    function.
    */
    region *nregion = malloc(sizeof(region));

    nregion->name = malloc((strlen(name) + 1) * sizeof(char));

        strcpy(nregion->name, name);

        nregion->nmonsters = nmonsters;

        nregion->monsters = malloc(nmonsters * sizeof(monster*));

        for(int i = 0; i<nmonsters; i++)
         {
             get_next_nonblank_line(ifp, buf, 255);

             sscanf(buf, "%s", monster_name);

             for(int j = 0; j< monster_count; j++)
             {
                 if(strcmp(monster_name,monster_list[j]->name) == 0)
                 {
                     nregion->monsters[i] = monster_list[j];

                     total_population += monster_list[j]->population;

                 }

              }
        }
        nregion->total_population = total_population;

        total_population = 0;

        return nregion;
}


/*get_region is a function that takes in parameters of a pointer to a file, an integer for the
monster_count and a pointer to an array of pointers called monster_list. This functions purpose
is to read in strings from an input file and scan the contents of those string into temporary
variables as we loop through calling our region constructor and building our regions. The region
constructor returns a pointer which gets stored into the corresponding element of my region_list
array of pointers. This function returns a pointer to an array of pointers.
**Loops through regions and fills array of pointers to regions**
*/
region **get_region(FILE *ifp, int region_count, int monster_count, monster **monster_list)
{
    char buf[255];
    char name[255];
    char monster_name[255];
    int nmonsters;
    int total_population = 0;

    /*Here I am dynamically allocating memory for my array of pointers. I then am reading in lines
    from the input file and using sscanf to extract the name of the reigon and the number of monsters
    in that region. I then call my region constructor function and pass it those values. My region
    constructor function returns me the pointer to a region struct that fills the corresponding
    element of my array of pointers per the cycle of the for loop that I am in. I then return my
    array of pointers to region structs to my main.
    */
    region **region_list = malloc(region_count * sizeof(region*));

    for(int i = 0; i<region_count; i++)
    {
        get_next_nonblank_line(ifp, buf, 255);

        sscanf(buf,"%s", name);

        get_next_nonblank_line(ifp, buf, 255);

        sscanf(buf,"%d %s", &nmonsters, buf);

        region_list[i] = construct_region(ifp, monster_count, monster_list, nmonsters, name );

    }

    return region_list;
}


/*construct_itinerary: is a function that takes in parameters of an pointer to an input file, an integer
trainer_count to count how many trainers we have based on the input file, an integer region count to
count how many regions we have based on the input file, a pointer to an array of pointers called
region_list to access our list of regions, an integer captures, and an integer nregions so that
based on the values read in from the input file, we know how many captures we have per trainer, and
how many regions we have in an itinerary. This function constructs an itinerary and returns a pointer
to the itinerary struct.
**Constructs itineraries**
*/
itinerary *construct_itinerary(FILE *ifp, int trainer_count, int region_count, region **region_list, int captures, int nregions)
{
    char name[255];
    char buf[255];
    char temp_buf[255];
    char region_name[255];

    /*Here I am dynamically allocating memory for the size of an itinerary struct. I then am filling
    the values of that struct and I also dynamically allocate memory for the array of pointers that
    will contain pointers to each of the regions in this itinerary. I use a for loop and a nested for
    loop to read in a name of a region from the file and compare it to my region list to figure out
    which region belongs in my itinerary region list. I then return this itinerary to the calling
    function.
    */
    itinerary *nitinerary = malloc(sizeof(itinerary));

    nitinerary->nregions = nregions;

    nitinerary->captures = captures;

    nitinerary->regions = malloc(nregions * sizeof(region*));

    for(int i = 0; i< nregions; i++)
    {
         get_next_nonblank_line(ifp, buf, 255);

            sscanf(buf, "%s", region_name);

            for(int j = 0; j<region_count; j++)
            {
                if(strcmp(region_name, region_list[j]->name)==0)
                {
                    nitinerary->regions[i] = region_list[j];
                }
            }
        }
    return nitinerary;
}


/*get_trainer: function takes in parameters of a pointer to a file, integer trainer count to know
how many trainers we need loop through and construct, integer region count to pass to our itinerary
constructor, and a pointer to an array of pointers called region_list that we pass to our itinerary
constructor. The purpose of this function is to dynamically allocated memory for an array of trainer
structs and to read an input file, temporarily store values in variables as we construct our trainer
and also calls our itinerary constructor to construct our itinerary and finish building our trainer.
This function returns a pointer to an array of trainer structs.
**Constructs trainers**
*/
trainer *construct_trainer(FILE *ifp, int trainer_count, int region_count, region **region_list)
{
    char temp_buf[255];
    char trainer_name[255];
    char buf[255];
    int nregions;
    int captures;

    /*Here I am dynamically allocating memory for an array of trainer structs where I am reading
    from the input file and using sscanf to etract and store the values that I read in. I then call
    my intinerary constructor which will return me the specific itinerary that belongs in the element
    of my trainer list per the cycle of the for loop that I am in. I then return this pointer to this
    list of trainer structs to my main.
    */
    trainer *ntrainer = malloc(trainer_count * sizeof(trainer));

    for(int i = 0; i<trainer_count;i++)
    {

        get_next_nonblank_line(ifp, buf, 255);

        sscanf(buf,"%s", trainer_name);

        get_next_nonblank_line(ifp, buf, 255);

        sscanf(buf,"%d %s", &captures, temp_buf);

        get_next_nonblank_line(ifp, buf, 255);

        sscanf(buf,"%d %s", &nregions, temp_buf);

        itinerary *nitinerary = construct_itinerary(ifp, trainer_count, region_count, region_list,captures, nregions);

        ntrainer[i].name = malloc((strlen(trainer_name) + 1) * sizeof(char));

        strcpy(ntrainer[i].name, trainer_name);

        ntrainer[i].visits = nitinerary;
    }
    return ntrainer;
}


/*print_output: function takes in parameters of a pointer to an output file, an integer trainer count
to loop through the trainers, a pointer to an array of structs called trainer to access each trainer,
a pointer to an array of pointers called trainer_list to access each trainer. This function loops through each element of trainer, region,
and monster in order to print the correct output per the assignment file. The function also has a
couple of calculations that determine the number of monsters that are caught in each region based on
relative population, number of captures, per trainer, and that number is rounded up with the round
function. This is a void function that only serves the purpose of printing to an output file.
**Does math calculations and prints output**
*/
void print_output(FILE *ofp, int trainer_count, trainer *trainer_list)
{
    ofp = fopen("cop3502-as1-output-gabriel-rodriguez.txt", "w");

    /*Here I am using nested for loops to loop through my list of trainers, my list of regions,
    and my list of monsters to print out the necessary trainer name, the regions they visit, and the
    amount of which monsters they will catch. I also have a couple calculations where I divide
    the specific monster population by the total region population, I then multiply that number by
    the amount of captures that the specific trainer has. I then use a round function to round that
    number up to a whole number and that is number of monsters they will catch for that monster. I
    also use a pointer to an output file to use print statements to print to an output file.
    */
    for(int i = 0; i<trainer_count; i++)
    {
        fprintf(ofp,"%s\n", trainer_list[i].name);

        for(int j = 0; j<trainer_list[i].visits->nregions; j++)
        {
            fprintf(ofp, "%s\n", trainer_list[i].visits->regions[j]->name);


            for(int k = 0; k<trainer_list[i].visits->regions[j]->nmonsters; k++)
            {
                int population = trainer_list[i].visits->regions[j]->monsters[k]->population;

                double relative_population = (double) population  / trainer_list[i].visits->regions[j]->total_population;

                double catch_probability = relative_population * trainer_list[i].visits->captures;

                int amount_caught = round(catch_probability);

                if(amount_caught != 0)
                {
                    fprintf(ofp,"%d %s\n", amount_caught, trainer_list[i].visits->regions[j]->monsters[k]->name);
                }
             }

        }
        fprintf(ofp,"\n\n");
    }
    fclose(ofp);
}


/*free_lists: is a void function that takes in the parameters pointer to an array of pointers
monster_list, a pointer to an array of pointers called region_list, a pointer to an array of structs
called trainer_list, integers mosnter_count, region_count, and trainer_count in order to loop
through each list for the number of items in that list. The purpose of this function is to go through
each list a free all the memory that we previously dynamically allocated. This is a void function
so it only serves the purpose of freeing the memory and does not return anything.
**Frees memory**
*/
void free_lists(monster **monster_list, region **region_list, trainer *trainer_list, int monster_count, int region_count, int trainer_count)
{
    /*Here I am using for loops to free each of the parts of memory that I previously dynamically
    allocated. I start with the first things that I dynamically allocated which would be monsters,
    then I move to freeing the regions, then lastly I free my trainers.
    */
    for(int i = 0; i<monster_count; i++)
    {
        free(monster_list[i]->name);
        free(monster_list[i]->element);
        free(monster_list[i]);
    }

        free(monster_list);

    for(int i = 0; i<region_count; i++)
    {
        free(region_list[i]->name);
        free(region_list[i]->monsters);
        free(region_list[i]);
    }

        free(region_list);

    for(int i = 0; i<trainer_count; i++)
    {
        free(trainer_list[i].name);
        free(trainer_list[i].visits->regions);
        free(trainer_list[i].visits);
    }

        free(trainer_list);
}


/*main: function calls atexit to check for memory leaks and calls all my functions throughout
the program. Main opens and closes the input file.
*/
int main() {

atexit(report_mem_leak);

FILE *ifp;
FILE *ofp;

char temp_buf[255];
char buf[255];
int monster_count;
int region_count;
int trainer_count;

    ifp = fopen("cop3502-as1-input.txt", "r");

    get_next_nonblank_line(ifp, buf, 255);

    sscanf(buf,"%d %s", &monster_count, temp_buf);

    monster **monster_list = get_monster(ifp, monster_count);

    get_next_nonblank_line(ifp, buf, 255);

    sscanf(buf, "%d %s", &region_count, temp_buf);

    region **region_list = get_region(ifp, region_count, monster_count, monster_list);

    get_next_nonblank_line(ifp, buf, 255);

    sscanf(buf,"%d %s", &trainer_count, temp_buf);

    trainer *trainer_list = construct_trainer(ifp, trainer_count, region_count, region_list);

    print_output(ofp, trainer_count, trainer_list);

    free_lists(monster_list, region_list, trainer_list, monster_count, region_count, trainer_count);

    fclose(ifp);

    return 0;
}