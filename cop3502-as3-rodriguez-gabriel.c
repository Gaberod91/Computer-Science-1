/*
This program is a program that uses 6 different sorting methods to sort n number of randomly
generated monster structures. We used the bubble sort, quick sort, selection sort,
insertion sort, merge sort, and merge-insertion sort methods. This template file is what contains
the functions and necessary code to implement each sorting method. It works in conjunction with
the main file that contains the code to randomly generate the monster structures and call all the
functions. The code outputs the sorting methods, the stats of the sorting methods, and if the list
is sorted.
*/
#include "monster-sorts.h"

void print_clocks(clock_t clocks) {
  printf("  %lfs CPU time used\n", ((double) clocks) / CLOCKS_PER_SEC);
}
// swap_monsters: swaps two data structures in memory.
/*This function uses memcpy to swap the position of two moonsters and all of the data of the structure.
The parameters are a pointer to monster called list and two ints, i and j, which are used to
use pointer arithmetic to get to the element of the array of the monsters to do the swap.
*/
void swap_monsters(monster *list, int i, int j)
{
  monster temp;

  memcpy(&temp, list + i, sizeof(monster));
  memcpy(list + i, list + j, sizeof(monster));
  memcpy(list + j, &temp, sizeof(monster));

  Swaps++;
}

/* compare_monsters: The core comparison function. */
/*Function takes the parameters of a pointer to monster m1 and m2, int use_height and int use_weight
This function compares two monsters by either weight or by name depending on the values of the two
int parameters. Only one value can be 1 and the other 0. Whichever int is 1 is the method we use
to compare the monsters. */
int compare_monsters(monster *m1, monster *m2, int use_name, int use_weight)
{
  Comparisons++;
   /* Here we use strcmp to lexicographically compare the the two monsters by name
   */
   if(use_name == 1)
  {
    if((strcmp(m1->name, m2->name) > 0))
    {
      return 1;
    }

    else if((strcmp(m1->name, m2->name) == 0))
    {
      return 0;
    }

    else
    {
      return -1;
    }
  }

  if(use_weight == 1)
  {
    if(m1->weight > m2->weight)
    {
      return 1;
    }

    else if(m1->weight == m2->weight)
    {
      return 0;
    }

    else
    {
      return -1;
    }
  }
}

/* repartition: Implement ascending quick sort. */
/* Function takes the parameterse of a pointer to monster call list, int low_index, int high_index
int use_name, and int use_weight. The ints use_name and use_Weight determine if we sort by either
name or weight. Whichever one contains the value of 1 is the method we use to sort. Both cannot be
the same value. One will always be 1 and the other 0. */
int repartition(monster *list, int low_index, int high_index, int use_name, int use_weight)
{
  monster pivot_value = list[high_index];

  int i = low_index;
  /* Here we use a for loop and call the compare monsters function and swamp_monsters function
  to put all the values lower than the pivot value to the left of it.
  */
  for(int j = low_index; j < high_index; j++)
  {
    if(compare_monsters(&list[j], &pivot_value, use_name, use_weight) < 0) {
      swap_monsters(list, i, j);
      i++;
    }
  }
  //We call swap monsters one more time to put the pivot value where it belongs.
  swap_monsters(list, i, high_index);
  return i;
}

/* quick_sort_recursive: Recursive function for quick sort. */
/* Function takes the parameterse of a pointer to monster call list, int low_index, int high_index
int use_name, and int use_weight. The ints use_name and use_Weight determine if we sort by either
name or weight. Whichever one contains the value of 1 is the method we use to sort. Both cannot be
the same value. One will always be 1 and the other 0. */
void quick_sort_recursive(monster *list, int low_index, int high_index, int use_name, int use_weight)
{
    {
  int pivot_index = repartition(list, low_index, high_index, use_name, use_weight);
  // Coming out of repartition, our pivot_index must already be in the right position.
  if(pivot_index-1 > low_index) quick_sort_recursive(list, low_index, pivot_index-1, use_name, use_weight);
  if(high_index > pivot_index+1) quick_sort_recursive(list, pivot_index+1, high_index, use_name, use_weight);
}
}

/* quick_sort: Shell function for quick sort. */
/* Function takes the parameterse of a pointer to monster call list, int n,
int use_name, and int use_weight. The ints use_name and use_Weight determine if we sort by either
name or weight. Whichever one contains the value of 1 is the method we use to sort. Both cannot be
the same value. One will always be 1 and the other 0. */
void quick_sort(monster *list, int n, int use_name, int use_weight)
{
  printf("Quick sort %d monsters by %s...\n", n, use_name ? "name" : "weight");

  clear_stats();
  quick_sort_recursive(list, 0, n-1, use_name, use_weight);
  print_stats();
}

/* bubble_sort: Implement ascending bubble sort. */
/* Function takes the parameterse of a pointer to monster call list, int n,
int use_name, and int use_weight. The ints use_name and use_Weight determine if we sort by either
name or weight. Whichever one contains the value of 1 is the method we use to sort. Both cannot be
the same value. One will always be 1 and the other 0. */
void bubble_sort(monster *list, int n, int use_name, int use_weight)
{
  int i;
  int j;
  printf("Bubble sort %d integers...\n", n);
  clear_stats();
  /*Here we use a nested for loop, the first one moves us through the array as we actually sort the
  array. The second for loop is what loops through the array and does our comparisions and swaps
  calling the respective functions.
  */
  for(i = n-1; i >= 0; i--)
  {
    for(j = 0; j < i; j++)
    {
      if(compare_monsters(&list[j], &list[j+1], use_name, use_weight) > 0)
      {
        swap_monsters(list, j, j+1);
      }
    }
  }
  print_stats();
}

/* find_highest: Highest-value finder for selection sort. */
/* Function takes the parameterse of a pointer to monster call list, int n,
int use_name, and int use_weight. The ints use_name and use_Weight determine if we sort by either
name or weight. Whichever one contains the value of 1 is the method we use to sort. Both cannot be
the same value. One will always be 1 and the other 0. */
int find_highest(monster *list, int n, int use_name, int use_weight)
{
  monster highest_monster = list[n];
  int highest_loc = n;
  int i;
  /*For loop that calls the compare_monsters function and if we find a bigger monster than the current
  we set that monster as the highest. Once we find the highest location, we return that location.
  */
  for(i = 0; i <= n; i++)
  {
    if(compare_monsters(&list[i], &highest_monster, use_name, use_weight) > 0)
    {
      highest_loc = i;
      highest_monster = list[i];
    }
  }
  return highest_loc;
}

/* selection_sort:Implement ascending selection sort. */
/* Function takes the parameterse of a pointer to monster call list, int n,
int use_name, and int use_weight. The ints use_name and use_Weight determine if we sort by either
name or weight. Whichever one contains the value of 1 is the method we use to sort. Both cannot be
the same value. One will always be 1 and the other 0. */
void selection_sort(monster *list, int n, int use_name, int use_weight)
{
  int i;
  int highest;
  printf("Selection sort %d integers...\n", n);
  clear_stats();
  /*For loop that calls the find_highest function and swaps the monsters if we find a higher value
  */
  for(i = n-1; i > 0; i--)
  {
    highest = find_highest(list, i, use_name, use_weight);
    if(highest != i)
    {
      swap_monsters(list, highest, i);
    }
  }
  print_stats();
}

/* Find position for insertion sort. */

int insertion_sort_find_position(monster *list, int low_index, int high_index, monster *k, int use_name, int use_weight)
{
  /* Here we use a while loop to compare the monsters and decrement low index to determine the
  position that the key belongs in the main array. We return the low_index + 1 whih is where the key
  belongs. Block_copies only gets incremented if we decrement low_index.
  */
  while(low_index >= 0 && compare_monsters(&list[low_index], k, use_name, use_weight) > 0)
  {
    low_index--;
  }

  if(high_index != low_index +1)
  {
    Block_Copies++;
  }

  return low_index + 1;
}

/* insertion_sort_internal: Implement insertion sort. */
/* Function takes the parameterse of a pointer to monster call list, int n,
int use_name, and int use_weight. The ints use_name and use_Weight determine if we sort by either
name or weight. Whichever one contains the value of 1 is the method we use to sort. Both cannot be
the same value. One will always be 1 and the other 0. */
void insertion_sort_internal(monster *list, int n, int use_name, int use_weight)
{
  monster k;
  /*Here we have a for loop that calls the find position function and passes it a key. We are
  returned the position in the array that the key belongs and we used memmove to move the key into
  that position of the main array.
  */
  for (int i = 1; i < n; i++)
  {
    k = list[i];
    int low_index = i - 1;
    int high_index = i;

    int position = insertion_sort_find_position(list, low_index, high_index, &k, use_name, use_weight);

      while(low_index >= 0 && low_index + 1 > position)
  {
    memmove(&list[low_index + 1], &list[low_index], sizeof(monster));
    low_index--;
    Copies++;
  }

  memmove(&list[position], &k, sizeof(monster));

  }
}

/* insertion_sort: Shell for insertion sort. */
/* Function takes the parameterse of a pointer to monster call list, int n,
int use_name, and int use_weight. The ints use_name and use_Weight determine if we sort by either
name or weight. Whichever one contains the value of 1 is the method we use to sort. Both cannot be
the same value. One will always be 1 and the other 0. */
void insertion_sort(monster *list, int n, int use_name, int use_weight)
{
  printf("Insertion sort %d monsters by %s...\n", n, use_name ? "name" : "weight");
  clear_stats();

  insertion_sort_internal(list, n, use_name, use_weight);

  print_stats();
  return;
}

/* merge_sort_merge:Merge function for merge sort.  Merges sub-arrays [l1..h1] and [l2..h2].
   Must have l2 = l1 + 1 or bad things will happen. */
/* Function takes the parameterse of a pointer to monster call list, int l1, int h1, int l2, int h2
int use_name, and int use_weight. The ints use_name and use_Weight determine if we sort by either
name or weight. Whichever one contains the value of 1 is the method we use to sort. Both cannot be
the same value. One will always be 1 and the other 0. */
void merge_sort_merge(monster *list, int l1, int h1, int l2, int h2,
                      int use_name, int use_weight)
{
    int left_length = h1 - l1 + 1;
    int right_length = h2 - h1;
    int i = 0, j = 0, k = l1;

    monster *arr_left = malloc(left_length * sizeof(monster));
    monster *arr_right = malloc(right_length * sizeof(monster));
    Mallocs++;
        /*Here we have just used malloc to allocate memory for two sub-arrays and we are using
        two for loops using memmove to fill the elements of these sub-arrays with the specified
        positions from the l1 l2 h1 h2 variables which are give using the elements to copy from
        the main array that is being sorted.
        */
        for(int i = 0; i < left_length; i++)
        {
            memmove(&arr_left[i], &list[i + l1], sizeof(monster));
            Copies++;
        }
        Block_Copies++;

        for(int j = 0; j < right_length; j++)
        {
            memmove(&arr_right[j], &list[j + l2], sizeof(monster));
            Copies++;
        }
        Block_Copies++;
    /* Here we are using a while loop with if conditions to compare the sub-arrays and sort them
    back into the main array. We use i to keep track of left sub-array, j to keep track of right
    sub-array, and k to keep track of the position in main array
    */
    while(i < left_length && j < right_length)
    {
        if(compare_monsters(&arr_left[i], &arr_right[j], use_name, use_weight) < 0)
        {
            memmove(&list[k], &arr_left[i], sizeof(monster));
            i++;
            Copies++;
        }

        else
        {
            memmove(&list[k], &arr_right[j], sizeof(monster));
            j++;
            Copies++;
        }

        k++;
    }

    while(i < left_length)
    {
        memmove(&list[k], &arr_left[i], sizeof(monster));
        i++;
        k++;
        Copies++;
    }

    while(j < right_length)
    {
        memmove(&list[k], &arr_right[j], sizeof(monster));
        j++;
        k++;
        Copies++;
    }
    /* Here we free the memory for the sub-arrays that we malloced
    */
    free(arr_left);
    free(arr_right);
}

/* merge_sort_recursive: Recursive function for merge sort. */
/* Function takes the parameterse of a pointer to monster call list, int low_index, int high_index
int use_name, and int use_weight. The ints use_name and use_Weight determine if we sort by either
name or weight. Whichever one contains the value of 1 is the method we use to sort. Both cannot be
the same value. One will always be 1 and the other 0. */
void merge_sort_recursive(monster *list, int low_index, int high_index,
                          int use_name, int use_weight)
{
  if(low_index < high_index)
    {
      /*This calculation mid keeps dividing the list in half and uses pointer arithmetic to pass
        the half of the list recursively to the two recursive functions. It recursively calls the
        lower half and then works its way back up the higher half.*/
        int mid = (high_index + low_index)/2;

        merge_sort_recursive(list, low_index, mid, use_name, use_weight);
        merge_sort_recursive(list, mid + 1, high_index, use_name, use_weight);
        //Once we get to the if condition breaking and we only have 1 item, we work our way
        //backwards recursively calling the merge_sort_merge on the left and right halves
        merge_sort_merge(list, low_index, mid, mid + 1, high_index, use_name, use_weight);
    }
}

/* merge_sort: Implement merge sort. */
/* Function takes the parameters of a pointer to monster struct called list, int n, use_name, and
use_weight. n is the number of items to be sorted and use_name and use_weight are what we use to
determine the method of sorting. Only use_name or use_weight can be 1 and the other must be 0.
Whichever is 1 is the method we use to sort when the function is called.
*/
void merge_sort(monster *list, int n, int use_name, int use_weight)
{
  printf("Merge sort %d monsters...\n", n);

  clear_stats();
  merge_sort_recursive(list, 0, n-1, use_name, use_weight);
  print_stats();
}

/* merge_insertion_sort_recursive: Recursive function for merge-insertion sort. */
/* Function takes the parameterse of a pointer to monster call list, int low_index, int high_index
int use_name, and int use_weight. The ints use_name and use_Weight determine if we sort by either
name or weight. Whichever one contains the value of 1 is the method we use to sort. Both cannot be
the same value. One will always be 1 and the other 0.
*/
void merge_insertion_sort_recursive(monster *list, int low_index, int high_index,
                                    int use_name, int use_weight)
{
        /*This calculation mid keeps dividing the list in half and uses pointer arithmetic to pass
        the half of the list recursively to the two recursive functions. It recursively calls the
        lower half and then works its way back up the higher half.
        */
        int mid = (high_index + low_index)/2;
        if(high_index - low_index <=25)
        {
          insertion_sort_internal(list + low_index, high_index + 1 - low_index, use_name, use_weight);
        }
        else{


        merge_insertion_sort_recursive(list, low_index, mid, use_name, use_weight);
        merge_insertion_sort_recursive(list, mid + 1, high_index, use_name, use_weight);
        //Once we get to the if condition breaking and we only have 1 item, we work our way
        //backwards recursively calling the merge_sort_merge on the left and right halves
        merge_sort_merge(list, low_index, mid, mid + 1, high_index, use_name, use_weight);
        }
}

/* merge_insertion_sort: Implement merge-insertion sort. */
/* Function takes the parameters of a pointer to monster struct called list, int n, use_name, and
use_weight. n is the number of items to be sorted and use_name and use_weight are what we use to
determine the method of sorting. Only use_name or use_weight can be 1 and the other must be 0.
Whichever is 1 is the method we use to sort when the function is called.
*/
void merge_insertion_sort(monster *list, int n, int use_name, int use_weight)
{
  clear_stats();
  printf("Merge-insertion sort %d monsters...\n", n);

  merge_insertion_sort_recursive(list, 0, n-1, use_name, use_weight);
  print_stats();
}
