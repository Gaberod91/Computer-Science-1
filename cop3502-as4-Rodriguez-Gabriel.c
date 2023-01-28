/*
This program is a program that reads information from an input file and prints it to an output file.
The information that is read in is used to dynamically allocate memory for a binary search tree. The
program then reads in commands from the input file and manipulates the binary search tree with different
functions. The program then outputs to an output the file these commands as they happen in the program.
This program works with binary search trees.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


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


typedef struct tree_name_node_struct
{
  char treeName[32];
  struct tree_name_node_struct *parent, *left, *right;
  struct item_node_struct *theTree;
} tree_name_node;


typedef struct item_node_struct
{
  char name[32];
  int count;
  struct item_node_struct *parent, *left, *right;
  struct tree_name_node_struct *parent_tree;
}item_node;

/*add_to_tree_of_trees: add trees to the tree of trees
This function takes in the parameters of two tree_name_node structs. The first struct is the root
of the tree and the other struct is being added to the tree of trees. This function works by checking
the lexicograhpic size and if the tree being added is smaller, it goes to the left, if it is bigger,
it goes to the right and repeatedly does that until it finds it's home. This is a void function and
does not return anyting.
*/
void add_to_tree_of_trees(tree_name_node *tree1, tree_name_node *tree)
{
    tree_name_node *finder = tree1;

    while(finder != NULL)
    {   //While loop and if else conditios for each case
        if(strcmp(finder->treeName, tree->treeName) > 0)
        {
            if(finder->left == NULL)
            {
                finder->left = tree;
                tree->parent = finder;
                finder = NULL;
            }

            else
            {
                finder = finder->left;
                tree->parent = finder;
            }
        }
        else
        {
            if(finder->right == NULL)
            {
                finder->right = tree;
                tree->parent = finder;
                finder = NULL;
            }

            else
            {
                finder = finder->right;
                tree->parent = finder;
            }
        }
        }
    }

/*create_tree_of_tree: This function creates all the trees.
This function takes in the parameters of the file pointer and an int num_trees. This function loops
through for the number of num_trees times and reads from the file and creates each tree. It then calls
the add function to add the tree to the tree of trees. This functin returns a tree_name_node struct
which is the root of the tree of trees.
*/
tree_name_node *create_tree_of_tree(FILE *ifp, int num_trees)
{
    char name[32];
    char buf[255];

        get_next_nonblank_line(ifp, buf, 255);
        sscanf(buf, "%s", name);

        tree_name_node *tree1 = malloc(sizeof(tree_name_node));

        strcpy(tree1->treeName, name);
        tree1->parent = NULL;
        tree1->left = NULL;
        tree1->right = NULL;
        tree1->theTree = NULL;
        //For loop to loop through the num trees and read in from the input file and create each
        //tree_name_node struct.
        for(int i = 0; i < num_trees-1; i++)
        {
            get_next_nonblank_line(ifp, buf, 255);
            sscanf(buf, "%s", name);

            tree_name_node *tree = malloc(sizeof(tree_name_node));

            strcpy(tree->treeName, name);
            tree->parent = NULL;
            tree->left = NULL;
            tree->right = NULL;
            tree->theTree = NULL;

            add_to_tree_of_trees(tree1, tree);
            }

            return tree1;
}

/*search_tree: is a function that uses recursion to find a matching tree.
This function takes in the parameters of a tree_name_node struct tree and a char pointer name.
The function uses recursion to go through the tree of trees and uses strcmp to find the matching
struct. Once found, the function returns that tree. This function returns a pointer to a tree_name_node
struct.
*/
tree_name_node *search_tree(tree_name_node *tree, char *name)
{
    tree_name_node *node_match;

    if(tree == NULL)
    {
        return 0;
    }
    //uses strcmp to determine when to return the tree once found
    if(strcmp(tree->treeName, name) == 0)
    {
        return tree;
    }
    node_match = search_tree(tree->right, name);

        if(node_match != NULL)
        {
            return node_match;
        }
    node_match = search_tree(tree->left, name);
    if(node_match != NULL)
        {
            return node_match;
        }

    return node_match;


}

/*add_to_tree: Adds the sub_trees to the tree of trees.
This function takes in the parameters of a tree_name_node struct and an item node struct. The function
uses a while loop and strcmp to lexicographically compare the names and add them to their corresponding
tree of trees. This function is void and does not return anything.
*/
void add_to_tree(tree_name_node *tree1, item_node *tree)
{
    if(tree1->theTree == NULL)
    {
        tree1->theTree = tree;
        tree->parent_tree = tree1;
    }

    else
    {
    item_node *finder = tree1->theTree;

    while(finder != NULL)
    {
        if(strcmp(finder->name, tree->name) > 0)
        {
            if(finder->left == NULL)
            {
                finder->left = tree;
                tree->parent_tree = tree1;
                tree->parent = finder;
                finder = NULL;
            }

            else
            {
                finder = finder->left;
                tree->parent = finder;
            }
        }
        else
        {
            if(finder->right == NULL)
            {
                finder->right = tree;
                tree->parent_tree = tree1;
                tree->parent = finder;
                finder = NULL;
            }

            else
            {
                finder = finder->right;
                tree->parent = finder;
            }
        }
        }
    }
}

/*create_tree: constructor for each item_node struct.
This function is the constructor for each item node that gets created. It takes in the parameters
of the input file pointer, a tree_name_node struct tree_of_tree and int num_items. It loops through
num_items amount of times and constructs each item_node struct. It then calls the add function to add
it to the corresponding tree of trees. This function is void and does not return anyting.
*/
void *create_tree(FILE *ifp, tree_name_node *tree_of_tree, int num_items)
{
    char buf[255];
    //For loop to loop through the num_items and dynamically allocate memory for them and calls
    //add function to add it to the binary search tree
    for(int i = 0; i < num_items; i++)
    {
        char tree[32];
        char name[32];
        int data;

        get_next_nonblank_line(ifp, buf, 255);
        sscanf(buf, "%s %s %d", tree, name, &data);

        item_node *t = malloc(sizeof(item_node));

        strcpy(t->name, name);
        t->count = data;
        t->left = NULL;
        t->right = NULL;
        t->parent = NULL;
        t->parent_tree = NULL;

        tree_name_node *match = search_tree(tree_of_tree, tree);

        add_to_tree(match, t);

    }
}

/*search_tree_of_tree_print: recursively prints tree of tree names.
This function takes in the parameters of an output file pointer and a tree_name_node struct pointer.
The function uses recursion to go through the binary tree lexicographically and print each component
of the tree of trees. This function is a void function and does not return anything.
*/
void search_tree_of_tree_print(FILE *ofp, tree_name_node *tree)
{
    if(tree == NULL)
    {
        return;
    }

    search_tree_of_tree_print(ofp, tree->left);

    fprintf(ofp, "%s ", tree->treeName);

    search_tree_of_tree_print(ofp, tree->right);
}

/*search_tree_print: Prints each item_node struct tree
This function takes in the parameters of an output file pointer and a tree_name_node struct pointer.
The function uses recursion to go through the binary tree lexicographically and print each component
of the tree. This function is a void function and does not return anything.
*/
void search_tree_print(FILE *ofp, item_node *tree)
{
    if(tree == NULL)
    {
        return;
    }

    search_tree_print(ofp, tree->left);

    fprintf(ofp, "%s ", tree->name);

    search_tree_print(ofp, tree->right);
}

/*search_tree_of_tree_of_tree_print: This function prints the lines and spacing and calls everything
to be printed.
This function takes in the parameters of a pointer to an output file and a tree_name_node struct tree.
The function calls the recursive functions to print the tree of trees and each of their sub trees.
It also prints out the spacing and lines. This function is void an does not return anything.
*/
void search_tree_of_tree_of_tree_print(FILE *ofp, tree_name_node *tree)
{
    if(tree == NULL)
    {
        return;
    }

    search_tree_of_tree_of_tree_print(ofp, tree->left);
    fprintf(ofp, "===%s=== \n", tree->treeName);
    search_tree_print(ofp, tree->theTree);
    fprintf(ofp, "\n");

    search_tree_of_tree_of_tree_print(ofp, tree->right);
}

/*dispose_tree_node: disposes a tree node
This function takes in a parameter of an item_node tree and its purpose is to free each item_node
struct that it is passed. It does not return anything because the function is void.
*/
void dispose_tree_node(item_node *tree) {
    free(tree);
}

/*dispose_tree: uses recursion to dispose a tree.
This function takes in a parameter of an item_node tree and its purpose is to free each item_node
struct that it is passed. It does not return anything because the function is void.
*/
void dispose_tree(item_node *tree) {
    if(tree->left) {
        dispose_tree(tree->left);
    }
    if(tree->right) {
        dispose_tree(tree->right);
    }
    dispose_tree_node(tree);
}

/*search_tree_item:Returns an item_node struct after it is located using strcmp on the name
This function takes in the parameters of an item_node tree, and a char pointer name. This function
uses recursion to traverse the tree and it uses strcmp to find the tree with the matching name. This
function returns an item_node struct.
*/
item_node *search_tree_item(item_node *tree, char *tree_name)
{
    if(tree == NULL)
    {
        return NULL;
    }

    else if(strcmp(tree->name, tree_name) == 0)
    {
        return tree;
    }

    else if(strcmp(tree->name, tree_name) == -1)
    {
        return search_tree_item(tree->right, tree_name);
    }

    else
    {
        return search_tree_item(tree->left, tree_name);
    }

}

/*tree_name_node: returns a tree_name_node struct after located in the tree
This function takes in the parameters of an tree_name_node struct tree, and a char pointer name. This function
uses recursion to traverse the tree and it uses strcmp to find the tree with the matching name. This
function returns a tree_name_node struct.
*/
tree_name_node *search_tree_of_tree(tree_name_node *tree, char *tree_of_tree_name)
{
    if(tree == NULL)
    {
        return NULL;
    }

    else if(strcmp(tree->treeName, tree_of_tree_name) == 0)
    {
        return tree;
    }

    else if(strcmp(tree->treeName, tree_of_tree_name) == -1)
    {
        return search_tree_of_tree(tree->right, tree_of_tree_name);
    }

    else
    {
        return search_tree_of_tree(tree->left, tree_of_tree_name);
    }
}

/*item_before: Returns the number of items before a node in the tree.
This function takes in the parameters of an item_node tree, a char pointer tree_name, and an int
count. The purpose of this function is to use recursion to calculate the number of nodes that come
before a specifided node per the input parameters. This function returns an int which is the number
of items that come before the node.
*/
int item_before(item_node *tree, char *tree_name, int count)
{
    if(tree == NULL)
    {
        return 0;
    }

    else if(strcmp(tree->name, tree_name) < 0)
    {
        return count = 1 + item_before(tree->left, tree_name, count) + item_before(tree->right, tree_name, count);

    }
    else
    {
        return count = item_before(tree->left, tree_name, count) + item_before(tree->right, tree_name, count);
    }
}

/*tree_height: finds how high an node is in a tree.
This function takes in the parameter of an item_node. The purpose of this function is to use
recursion to find the left and right height. It finds the height and returns it as an int to the
calling function.
*/
int tree_height(item_node *root) {
    // Get the height of the tree
    if (!root)
        return 0;
    else {
        // Find the height of both subtrees
        // and use the larger one
        int left_height = tree_height(root->left);
        int right_height = tree_height(root->right);
        //if else condition that determines which height to return
        if (left_height >= right_height)
            return left_height + 1;
        else
            return right_height + 1;
    }
}

/*height_balance: finds out if left and right side of tree are balanced
This function takes in the parameters of a pointer to an output file, a tree_name_node pointer, and
a char pointer. The purpose of this function is to call the recursive function tree_height and it
finds the height of both sides of a specific tree and prints the height of each side and the difference.
This function is void an does not return anything.
*/
void height_balance(FILE *ofp, tree_name_node *tree, char *tree_of_tree_name)
{
    if(tree == NULL)
    {
        return;
    }
    int left, right, difference;
    //Recursive calls for left and right side of the tree
        if(tree->theTree->left != NULL)
        {
        left = tree_height(tree->theTree->left);

        }
        else
        {
            left = 0;
        }

        if(tree->theTree->right != NULL)
        {
        right = tree_height(tree->theTree->right);

        }
        else
        {
            right = 0;
        }

        left--;
        right--;

        difference = abs(left - right);

        if(difference <= 1)
        {
        fprintf(ofp, "%s: left height %d, right height %d, difference %d, balanced\n", tree_of_tree_name, left, right, difference);
            }
            else
            {
                fprintf(ofp, "%s: left height %d, right height %d, difference %d, not balanced\n", tree_of_tree_name, left, right, difference);
            }
}

/*count: sums the payloads of each node in a tree
This function takes in an item node pointer. The purpose of this function is to use recursion to
traverse the tree and add each nodes payload to a sum. This function then returns an int which is
the sum of all the payloads in a tree.
*/
int count(item_node *tree)
{
    int payload;

    if(tree == NULL)
    {
        return 0;
    }

    return payload = tree->count + count(tree->left) + count(tree->right);
}

/*delete: deletes a node from a tree
This function takes in the parameters of a tree_name_node pointer and an item_node pointer. The
purpose of this function is to take in a item_node and delete it from its tree. This is a void
function and does not return anything. I have different conditions for if there are no children or
a leaf, one child, and two children.
*/
void delete(tree_name_node *tree, item_node *sub_tree)
{
    item_node *temp = sub_tree;

    if(tree == NULL)
    {
        return;
    }
    //Else statement for case of two children
    else if(sub_tree->left != NULL && sub_tree->right != NULL)
    {
        temp = temp->right;

        while(temp->left != NULL)
        {
            temp = temp->left;
        }

        temp->parent->left = NULL;
        //If statement for if we are at the root
        if(sub_tree == tree->theTree)
        {
            temp->left = sub_tree->left;
            temp->right = sub_tree->right;
            temp->parent = NULL;
            temp->parent_tree->theTree = temp;

            free(sub_tree);
        }

        else if(sub_tree->parent->left == sub_tree)
        {
            sub_tree->parent->left = temp;
            temp->left = sub_tree->left;
            temp->right = sub_tree->right;
            temp->parent = sub_tree->parent;

            free(sub_tree);
        }

        else if(sub_tree->parent->right == sub_tree)
        {
            sub_tree->parent->right = temp;
            temp->left = sub_tree->left;
            temp->right = sub_tree->right;
            temp->parent = sub_tree->parent;

            free(sub_tree);
        }
    }
    //Else statement if only one child
    else if(sub_tree->left != NULL && sub_tree->right == NULL)
    {
        temp = sub_tree->left;

        if(sub_tree == tree->theTree)
        {
            temp->parent = NULL;
            temp->parent_tree->theTree = temp;
            free(sub_tree);
        }

        else if(sub_tree->parent->left == sub_tree)
        {
            sub_tree->parent->left = temp;
            temp->parent = sub_tree->parent;
            free(sub_tree);
        }

        else if(sub_tree->parent->right == sub_tree)
        {
            sub_tree->parent->right = temp;
            temp->parent = sub_tree->parent;
            free(sub_tree);
        }
    }
    //Else statement if only one child
    else if(sub_tree->right != NULL && sub_tree->left == NULL)
    {
        temp = sub_tree->right;

        if(sub_tree == tree->theTree)
        {
            temp->parent = NULL;
            temp->parent_tree->theTree = temp;
            free(sub_tree);
        }

        else if(sub_tree->parent->left == sub_tree)
        {
            sub_tree->parent->left = temp;
            temp->parent = sub_tree->parent;
            free(sub_tree);
        }

        else if (sub_tree->parent->right == sub_tree)
        {
            sub_tree->parent->right = temp;
            sub_tree->parent = sub_tree->parent;
            free(sub_tree);
        }
    }
    //Else statement if no children
    else
    {
        if(sub_tree == tree->theTree)
        {
            temp->parent_tree->theTree = NULL;

            free(sub_tree);
        }

        else if(sub_tree->parent->left == sub_tree)
        {
            sub_tree->parent->left = NULL;
            free(sub_tree);
        }

        else if(sub_tree->parent->right = NULL)
        {
            sub_tree->parent->right = NULL;
            free(sub_tree);
        }
    }
}

/*delete_tree: deletes a node from a tree
This function takes in the parameters of a tree_name_node pointer and another tree_name_node pointer. The
purpose of this function is to take in a item_node and delete it from its tree. This is a void
function and does not return anything. I have different conditions for if there are no children or
a leaf, one child, and two children.
*/
void delete_tree(tree_name_node *tree, tree_name_node *tree_of_tree_name)
{
    if(tree_of_tree_name == NULL)
    {
        return;
    }

    tree_name_node *temp = tree_of_tree_name;
        //If statement if no kids
        if(tree_of_tree_name->left == NULL && tree_of_tree_name->right == NULL)
        {
            if(tree == tree_of_tree_name)
            {
                dispose_tree(tree_of_tree_name->theTree);
                free(tree_of_tree_name);
            }

            else if(tree_of_tree_name->parent->left == tree_of_tree_name)
            {
                tree_of_tree_name->parent->left = NULL;
                dispose_tree(tree_of_tree_name->theTree);
                free(tree_of_tree_name);
            }
            else
            {
                tree_of_tree_name->parent->right = NULL;
                dispose_tree(tree_of_tree_name->theTree);
                free(tree_of_tree_name);
            }
        }
        //else statement if one child
        else if(tree_of_tree_name->left != NULL && tree_of_tree_name->right == NULL)
        {
            temp = tree_of_tree_name->left;

            if(tree_of_tree_name = tree)
            {
                temp->parent = NULL;
                dispose_tree(tree_of_tree_name->theTree);
                free(tree_of_tree_name);
            }

        else if(tree_of_tree_name->parent->left == tree_of_tree_name)
        {
            tree_of_tree_name->parent->left = temp;
            temp->parent = tree_of_tree_name->parent;
            dispose_tree(tree_of_tree_name->theTree);
            free(tree_of_tree_name);
        }

        else if(tree_of_tree_name->parent->right == tree_of_tree_name)
        {
            tree_of_tree_name->parent->right = temp;
            temp->parent = tree_of_tree_name->parent;
            dispose_tree(tree_of_tree_name->theTree);
            free(tree_of_tree_name);
        }
    }
    //Else statement if one child
    else if(tree_of_tree_name->right != NULL && tree_of_tree_name->left == NULL)
    {
        temp = tree_of_tree_name->right;

            if(tree_of_tree_name == tree)
            {
                temp->parent = NULL;
                dispose_tree(tree_of_tree_name->theTree);
                free(tree_of_tree_name);
            }

        else if(tree_of_tree_name->parent->left == tree_of_tree_name)
        {
            tree_of_tree_name->parent->left = temp;
            temp->parent = tree_of_tree_name->parent;
            dispose_tree(tree_of_tree_name->theTree);
            free(tree_of_tree_name);
        }

        else if(tree_of_tree_name->parent->right == tree_of_tree_name)
        {
            tree_of_tree_name->parent->right = temp;
            temp->parent = tree_of_tree_name->parent;
            dispose_tree(tree_of_tree_name->theTree);
            free(tree_of_tree_name);
        }
    }
        //Else statement if two children
        else if(tree_of_tree_name->left != NULL && tree_of_tree_name->right != NULL)
        {
        temp = temp->right;

        while(temp->left != NULL)
        {
            temp = temp->left;
        }

        temp->parent->left = NULL;

        if(tree_of_tree_name == tree)
        {
            temp->left = tree_of_tree_name->left;
            temp->right = tree_of_tree_name->right;
            temp->parent = NULL;

            dispose_tree(tree_of_tree_name->theTree);
            free(tree_of_tree_name);
        }

        else if(tree_of_tree_name->parent->left == tree_of_tree_name)
        {
            tree_of_tree_name->parent->left = temp;
            temp->left = tree_of_tree_name->left;
            temp->right = tree_of_tree_name->right;
            temp->parent = tree_of_tree_name->parent;

            dispose_tree(tree_of_tree_name->theTree);
            free(tree_of_tree_name);
        }

        else if(tree_of_tree_name->parent->right == tree_of_tree_name)
        {
            tree_of_tree_name->parent->right = temp;
            temp->left = tree_of_tree_name->left;
            temp->right = tree_of_tree_name->right;
            temp->parent = tree_of_tree_name->parent;

            dispose_tree(tree_of_tree_name->theTree);
            free(tree_of_tree_name);

        }
    }
}

/*commands: This function calls the functions to execute each command per the input file
This function takes in the parameters of an input file pointer, output file pointer, a tree_name_node
pointer and an int num_commands. This function use if else statements and strcmp to call the correct
functions to execute each command per the input file. This function is void and does not return
anything.
*/
void commands(FILE *ifp, FILE *ofp, tree_name_node *tree_of_tree, int num_commands)
{
    char buf[255];
    char command[255];
    char tree_of_tree_name[255];
    char tree_name[255];
    char csearch[] = "search";
    char citem_before[] = "item_before";
    char cheight_balance[] = "height_balance";
    char ccount[] = "count";
    char cdelete[] = "delete";
    char cdelete_tree[] = "delete_tree";

    fprintf(ofp, "====Processing Commands===\n");

    for(int i = 0; i < num_commands; i++)
    {
        get_next_nonblank_line(ifp, buf, 255);
        sscanf(buf, "%s %s %s", command, tree_of_tree_name, tree_name);

        tree_name_node *tree = search_tree_of_tree(tree_of_tree, tree_of_tree_name);

        if(strcmp(command, csearch) == 0)
        {
            if(tree == NULL)
            {
                fprintf(ofp, "%s does not exist\n", tree_of_tree_name);
            }

            else
            {
                item_node *item = search_tree_item(tree->theTree, tree_name);

                if(item == NULL)
                {
                    fprintf(ofp, "%s not found in %s\n", tree_name, tree_of_tree_name);
                }

                else
                {
                    fprintf(ofp, "%d %s found in %s\n", item->count, tree_name, tree_of_tree_name);
                }
            }
        }

        else if(strcmp(command, citem_before) == 0)
        {
            int count_before = item_before(tree->theTree, tree_name, 0);

             fprintf(ofp, "item before %s: %d\n", tree_name, count_before);
        }

        else if(strcmp(command, cheight_balance) == 0)
        {
            height_balance(ofp, tree, tree_of_tree_name);
        }

        else if(strcmp(command, ccount) == 0)
        {
            int payload = count(tree->theTree);
            fprintf(ofp, "%s count %d\n", tree_of_tree_name, payload);
        }

        else if(strcmp(command, cdelete) == 0)
        {
            item_node *found_item = search_tree_item(tree->theTree, tree_name);
            if(found_item == NULL)
            {
                fprintf(ofp, "%s not found in %s\n", tree_name, tree->theTree);
            }
            else
            {
            delete(tree, found_item);
            fprintf(ofp, "%s deleted from %s\n", tree_name, tree_of_tree_name);
            }
        }

        else if(strcmp(command, cdelete_tree) == 0)
        {
            delete_tree(tree_of_tree, tree);
            fprintf(ofp, "%s deleted", tree_of_tree_name);
        }
    }
}

/*dispose_tree_node: disposes a tree node
This function takes in a parameter of an item_node tree and its purpose is to free each item_node
struct that it is passed. It does not return anything because the function is void.
*/
void dispose_tree_node_all(item_node *tree) {
    free(tree);
}

/*dispose_tree_node: disposes a tree node
This function takes in a parameter of an item_node tree and its purpose is to free each item_node
struct that it is passed. It does not return anything because the function is void.
*/
void dispose_tree_all(item_node *tree) {
    if(tree->left) {
        dispose_tree_all(tree->left);
    }
    if(tree->right) {
        dispose_tree_all(tree->right);
    }
    dispose_tree_node_all(tree);
}

/*dispose_tree_node: disposes a tree node
This function takes in a parameter of an item_node tree and its purpose is to free each tree_name_node
struct that it is passed. It does not return anything because the function is void.
*/
void dispose_tree_name_node_all(tree_name_node *tree) {
    dispose_tree_all(tree->theTree);
    free(tree);
}

/*dispose_tree_node: disposes a tree node
This function takes in a parameter of an tree_name_node tree and its purpose is to free each item_node
struct that it is passed. It does not return anything because the function is void.
*/
void dispose_tree_of_tree_all(tree_name_node *tree)
{
    if(tree->left) {
        dispose_tree_of_tree_all(tree->left);
    }
    if(tree->right) {
        dispose_tree_of_tree_all(tree->right);
    }
    dispose_tree_name_node_all(tree);
}


int main()
{
    char buf[255];

    int num_trees;
    int num_items;
    int num_commands;

    FILE *ifp = fopen("cop3502-as4-input.txt", "r");
    FILE *ofp = fopen("cop3502-as4-output-rodriguez-gabriel.txt", "w");

    get_next_nonblank_line(ifp, buf, 255);
    sscanf(buf, "%d %d %d", &num_trees, &num_items, &num_commands);

    tree_name_node *tree_of_tree = create_tree_of_tree(ifp, num_trees);

    create_tree(ifp, tree_of_tree, num_items);

    search_tree_of_tree_print(ofp, tree_of_tree);
    fprintf(ofp, "\n");

    search_tree_of_tree_of_tree_print(ofp, tree_of_tree);


    commands(ifp, ofp, tree_of_tree, num_commands);

    dispose_tree_of_tree_all(tree_of_tree);

    fclose(ifp);
    fclose(ofp);

    return 0;
}
