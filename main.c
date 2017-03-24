#define MAX_NUM_HEAP 100 // the max number of the max-heap
#define STRING_MAX 512   // number of max string
#define INF 0x7fffffff;   // Number of infinite(max size of signed int)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Structure **/
// stack
typedef struct _linked_stack {
    int value;
    struct _linked_stack* next;
} LinkedStack, *pLinkedStack;

// max-heap
typedef struct _heap_node {
    int key;
    LinkedStack* pStack;
} HeapNode, *pHeapArr;

// binary search tree
typedef struct _node {
    int key;
    int index;
    struct _node* pLlink;
    struct _node* pRlink;
} Node, *pNode;

/** Heap & Linked Stack **/
/* This function insert a node to pHA. The node contains two fields, one for a key, the other for a stack.
The key value is the sum of the item values in stack. If the stack is empty, the key value is 0 */
void heap_insert(pHeapArr pHA, LinkedStack *stack);

/* This function deletes the root node of pHA. */
void heap_delete(pHeapArr pHA);

/* These two functions update the max-heap when there are some changes about the key values of nodes. */
/* max_heapify is max heap with descending index, and max_heapify_ascend is max heap with ascending index */
void max_heapify(pHeapArr *pHA, int index);
void max_heapify_ascend(pHeapArr *pHA, int index, int size_heap);

/* This function pops an item from the stack of the root node in pHA. The key value of the root node
should be changed. */
int stack_pop(pHeapArr pHA, int *save_value);

/* This function pushes an item to the stack of the root node in pHA. The key value of the root node
should be changed. */
void stack_push(pHeapArr pHA, int value);

/* This function shows the current values of the max-heap and their stack. */
void heap_print_status(pHeapArr pHA);

/** Binary Search Tree **/
/* This function inserts a node of Tree_Ptr type to the BST from an integer taken from the standard input.
Each node contains three fields, one for an integer and the other two for links to the left/right child. This
function should not insert a node with a key which is already in the BST. */
void BST_insert(pNode head, pNode pnode, int key, int index);

/* This function searches and returns the pointer of the node that contains the data. This function returns
NULL if the data does not exists in the BST. */
int BST_search(pNode pnode, int key);

/* This function deletes a node that contains the key. Be aware that the deletion procedure for a leaf node
and an internal node in BST is different. */
int BST_delete(pNode pnode, int key);

 /* This function traverses the BST in preorder. */
void BST_preOrder(pNode pnode);

/* This function traverses the BST in inorder. */
void BST_inOrder(pNode pnode);

/* This function traverses the BST in postorder. */
void BST_postOrder(pNode pnode);

/** Function for interface **/
/* Basic function about MaxHeap */
void MaxHeap();

/* Basic function about Binary Search */
void BinarySearch();

/* Sending text message for interface */
void InitMessage(int type);

/* Get information whether users really want to quit current menu or program */
int EndMessage();

/** Start of Writing function */

int main(void) {
    /* Initial Settings */
    int type_of_hw = 0;
    char* tem_buffer = (char*)malloc(STRING_MAX * sizeof(char));

    while (1) {
        /* Control Function */
        InitMessage(0);

        /* Get Instruction and Handle it */
        fgets(tem_buffer, STRING_MAX, stdin);

        // If cannot read integer number, print error message and inquiry again.
        if (!sscanf(tem_buffer, "%d", &type_of_hw)) {
            type_of_hw = 0;
            printf("[Error] You have to give integer number.\n");

        // If range of integer if improper, print error message and inquiry again.
        } else if (type_of_hw > 3 ||  type_of_hw < 1) {
            type_of_hw = 0;
            printf("[Error] Improper number is read.\n");
            printf("        Your input : %d / proper input : 1 ~ 3 \n", type_of_hw);
        }

        /* Control Function to be given */
        // Case of choose MaxHeap.
        if (type_of_hw == 1) {
            MaxHeap();

        // Case of Binary search.
        } else if (type_of_hw == 2) {
            BinarySearch();

        // Case of Exit, ask again thoroughly , and exit if user truly want.
        } else if (type_of_hw == 3) {
            if (EndMessage()) {
                free(tem_buffer);
                return 0;
            }
        }
    }

    /* End of the main function */
    free(tem_buffer);
    return 0;
}


void MaxHeap() {
    pHeapArr *Heap_arr = (pHeapArr*)malloc((MAX_NUM_HEAP + 1) * sizeof(pHeapArr));
    LinkedStack *tem_HeapNode;
    pHeapArr *temp_Heap_arr = NULL;
    char *tem_buffer = (char*)malloc(STRING_MAX * sizeof(char));
    int num_node = 0;
    int len_num;
    char tem_char[24];
    int tem_num;
    int loop;

    /* Setting at first */
    // Initialize handling message
    int handle_msg = -1;

    // Get message until user want to exit(handle_msg == 0)
    while (handle_msg != 0) {
        /* Get message from user */
        // Print screen & get message
        InitMessage(1);
        fgets(tem_buffer, STRING_MAX, stdin);

        // Case of non_integer is read.
        if (!sscanf(tem_buffer, "%d", &handle_msg)) {
            printf("[Error] You have to give integer number.\n");
            continue;
        }

        /* Handling message */
        switch (handle_msg) {
        case 1:
            /// Insert Node
            // Check number of node exceed.
            if (num_node >= MAX_NUM_HEAP) {
                printf("[Error] Heap is too small to install all data.\n");
                printf("        You cannot exceed number of node [%d]", MAX_NUM_HEAP - 1);
                break;
            }

            // Increase number of node.
            num_node++;

            // Create new heap.
            Heap_arr[num_node] = (HeapNode*)malloc(sizeof(HeapNode));

            // Initialize heap.
            Heap_arr[num_node]->key = 0;
            Heap_arr[num_node]->pStack = NULL;

            // Require user to input number to node.
            printf("Give integer(s) (End for . )\n");
            printf("[Example : 35 24 .] [Wrong : 35 24.] : ");

            // Read all number or string until break.
            while(scanf("%s", tem_char)) {
                // get length of character
                len_num = strlen(tem_char);

                // set 0 after length.
                tem_char[len_num] = 0;

                // Check improper or end commend('.').
                if (!len_num) break;
                if (tem_char[0] == '.') break;

                // Change character onto number.
                tem_num = atoi(tem_char);

                // Choose user whether press 0 truly (or is just a mistake of atoi())
                if (tem_num == 0) {
                    printf("[Warning] Program found out 0 input.\n");
                    printf("          This also happen when you press non_integer.\n");
                    printf("          Do you want to include 0 onto Tree? [y / n] ");

                    fgets(tem_buffer, STRING_MAX, stdin);

                    if (tem_buffer[0] != 'y') {
                        break;
                    }
                }

                // Create new memory for node.
                tem_HeapNode = (LinkedStack*)malloc(sizeof(LinkedStack));

                // Set values of the memory.
                tem_HeapNode->value = tem_num;

                // Link stack with previous one.
                heap_insert(Heap_arr[num_node], tem_HeapNode);

                // Initialize string.
                for (loop = 0 ; loop < 24 ; loop ++) {
                    tem_char[loop] = 0;
                }
            }

            // Garbage newline character collect.
            fgets(tem_buffer, STRING_MAX, stdin);

            // Print result.
            printf("[Notice] New node [node %d] added with key %d.\n", num_node, Heap_arr[num_node]->key);

            // Create max heap properly.
            max_heapify(Heap_arr, num_node);

            break;

        case 2:
            /// Delete root Node
            // If node number is 0, print message and end.
            if (num_node < 1) {
                printf("[Warning] Current number of node is 0. Instruction cannot be done.\n");
                break;
            }

            // Or, send error message about current situation.
            printf("[Notice] Root node is erased. Modifying all heap again.\n");

            // Swap End of array and root.
            temp_Heap_arr = (pHeapArr*)Heap_arr[num_node];
            Heap_arr[num_node] = Heap_arr[1];
            Heap_arr[1] = (pHeapArr)temp_Heap_arr;

            // decrease number of the node.
            num_node--;

            // Rearrange Root node.
            max_heapify_ascend(Heap_arr, 1, num_node);

            // Erase end of the node.
            heap_delete(Heap_arr[num_node + 1]);

            // Free array of heap memory.
            free(Heap_arr[num_node + 1]);

            break;

        case 3:
            /// Push root heap
            // Case of no_node.
            if (num_node == 0) {
                printf("[Error] No node to push. Recommend to do [Insert node].\n");
                break;
            }

            // Get number of node to push
            printf("Enter push number to node : ");
            fgets(tem_buffer, STRING_MAX, stdin);

            // Check whether get integer properly
            if (!sscanf(tem_buffer, "%d", &tem_num)) {
                printf("[Error] You Have to enter integer.\n");
                break;
            }

            // push heap
            stack_push(Heap_arr[1], tem_num);

            // And rearrange
            max_heapify_ascend(Heap_arr, 1, num_node);

            // Send message
            printf("[Notice] successfully rearranged and insert %d.\n", tem_num);

            break;

        case 4:
            /// pop root heap
            // If there are no node , print error message and break;
            if (num_node == 0) {
                printf("[Error] No node to pop. Recommend to do [Insert Node].\n");
                break;
            }

            // If we cannot get any item from data, press error message.
            if(!stack_pop(Heap_arr[1], &tem_num)) {
                printf("[Error] Root seems to have no any other item. cannot execute the order.\n");
                break;
            }

            // rearrange if pop stack
            max_heapify_ascend(Heap_arr, 1, num_node);

            // Show result
            printf("[Notice] Pop number is %d.\n", tem_num);

            break;

        case 5:
            /// Show all nodes
            printf("*************************************************\n");
            printf("[Notice] Printing %d node(s).\n", num_node);

            for (loop = 1 ; loop < num_node + 1 ; loop++) {
                printf("[Node %3d] key value : %d\n" , loop, Heap_arr[loop]->key);
                printf("[ Items  ] : ");
                heap_print_status(Heap_arr[loop]);
                printf("\n");
            }

            break;

        case 0:
            /// End message
            // Check whether users truly want to exit again
            if (!EndMessage()) handle_msg = -1;

            break;

        default :
            /// Case of wrong
            printf("[Error] Improper number is read.\n");
            printf("        Your input : %d / proper input : 0 ~ 5 \n", handle_msg);
            break;
        }
    }

    /* End of function */
    // Notice user to go to main menu.
    printf("Go to the main menu.\n");

    // Free memory
    for (loop = 1 ; loop < num_node + 1 ; loop++) {
        free(Heap_arr[loop]);
    }
    free(tem_buffer);
    free(Heap_arr);

    // End of function
    return;
}

void BinarySearch() {
    pNode *LRNode = (pNode*)malloc((MAX_NUM_HEAP + 1) * sizeof(pNode));
    pNode tem_node;
    char *tem_buffer = (char*)malloc(STRING_MAX * sizeof(char));
    int num_of_node = 0;
    int add_node;
    char tem_char[24];
    int loop;
    int tem_num;
    int len_num;
    int isexist;
    int del_node;

    /* Initialize area */
    int handle_msg = -1;

    // Create header node
    LRNode[0] = (Node*)malloc(sizeof(Node));

    // Initialize header node
    LRNode[0]->pLlink = NULL;
    LRNode[0]->pRlink = NULL;
    LRNode[0]->key = INF;
    LRNode[0]->index = 0;

    /* Get & process message until user want to exit */
    while (handle_msg != 0) {
        /* Get & handle message */
        // Print screen & get message.
        InitMessage(2);
        fgets(tem_buffer, STRING_MAX, stdin);

        // Task error if cannot get integer.
        if (!sscanf(tem_buffer, "%d", &handle_msg)) {
            printf("[Error] You have to give integer number.\n");
            continue;
        }

        switch (handle_msg) {
        case 1:
            /// Insert binary tree
            // Initial number of added node.
            add_node = 0;

            // Get message onto data
            printf("Give integer(s) (End for . )\n");
            printf("[Example : 35 24 .] [Wrong : 35 24.] : ");

            while(scanf("%s", tem_char)) {
                // get length of character
                len_num = strlen(tem_char);

                // set 0 after length
                tem_char[len_num] = 0;

                // Check improper or end commend
                if (!len_num) break;
                if (tem_char[0] == '.') break;

                // Change character onto number
                tem_num = atoi(tem_char);

                // Choose user whether press 0 truly (or is just a mistake of atoi())
                if (tem_num == 0) {
                    printf("[Warning] Program found out 0 input.\n");
                    printf("          This also happen when you press non_integer.\n");
                    printf("          Do you want to include 0 onto Tree? [y / n] ");

                    fgets(tem_buffer, STRING_MAX, stdin);

                    if (tem_buffer[0] != 'y') {
                        break;
                    }
                }

                // Increase number of node
                num_of_node++;
                add_node++;

                // Create new memory to node
                LRNode[num_of_node] = (Node*)malloc(sizeof(Node));

                // Check whether node exist (only case of node's number is more than 1)
                if ((num_of_node > 1) && (BST_search(LRNode[1], tem_num))) {
                    printf("[Error] %d is already exist in the tree. Skip this number.\n", tem_num);

                    // decrease number of nodes
                    add_node--;
                    num_of_node--;

                    // skip current loop
                    continue;
                }

                // Insert and set Location of node
                BST_insert(LRNode[0], LRNode[num_of_node], tem_num, num_of_node);

                // Initialize tem_char
                for (loop = 0 ; loop < 24 ; loop ++) {
                    tem_char[loop] = 0;
                }
            }

            // Execute garbage collector
            fgets(tem_buffer, STRING_MAX, stdin);

            // Get result
            printf("%d node(s) saved onto array. ", add_node);
            printf("total current number of node is %d.\n", num_of_node);

            break;

        case 2:
            /// search with Binary tree.
            // If no node is inserted, print error and break
            if (!num_of_node) {
                printf("[Error] There are no node(s) to search with key.\n");
                break;
            }

            // Get message and get number
            printf("Insert number to be searched : ");
            fgets(tem_buffer, STRING_MAX, stdin);

            // If fail to read number, print error message
            if (!sscanf(tem_buffer, "%d", &tem_num)) {
                printf("[Error] You have to press integer number.\n");
                break;
            }

            // Check whether number exist
            isexist = BST_search(LRNode[0], tem_num);

            // Tell whether find value exist
            if (isexist) {
                printf("[Notice] %d exist in the binary tree.\n", tem_num);
            } else {
                printf("[Notice] %d does not exist.\n", tem_num);
            }

            break;

        case 3:
            /// Delete from Binary tree
            // If no node is inserted, print error and break
            if (!num_of_node) {
                printf("[Error] There are no node(s) for Delete.\n");
                break;
            }

            // Get message and get number
            printf("Insert number to delete : ");
            fgets(tem_buffer, STRING_MAX, stdin);

            // If not press number, print error message
            if (!sscanf(tem_buffer, "%d", &tem_num)) {
                printf("[Error] You have to press integer number.\n");
                break;
            }

            // Check whether number exist
            isexist = BST_search(LRNode[0], tem_num);

            // Erase if exist, and do not if not.
            if (isexist) {
                // Case of 1 node
                if (num_of_node == 1) {
                    // Set header does not point anything
                    LRNode[0]->pLlink = NULL;

                    // Decrease number of node
                    num_of_node--;

                    // Get back memory to system.
                    free(LRNode[1]);
                } else {
                    // Get number of node to delete
                    del_node = BST_delete(LRNode[0], tem_num);

                    // Exchange node with end and what will delete
                    tem_node = LRNode[num_of_node];
                    LRNode[num_of_node] = LRNode[del_node];
                    LRNode[del_node] = tem_node;

                    // decrease number of node
                    num_of_node--;

                    // Erase end of memory
                    free(LRNode[num_of_node + 1]);
                }

                // Send text
                printf("[Notice] %d erased successfully.\n", tem_num);
            } else {
                // Case of cannot find, return error message.
                printf("[Error] %d does not exist. Program cannot do instruction.\n", tem_num);
            }

            break;

        case 4:
            /// Preorder
            printf("*************************************************\n");

            // If no node is inserted
            if (!num_of_node) {
                printf("[Error] There is no node to express.\n");
                break;
            }

            // Print all nodes with preorder.
            printf("Preorder  : ");
            BST_preOrder(LRNode[0]);
            printf("\n");

            break;

        case 5:
            /// Inorder
            printf("*************************************************\n");

            // If no node is inserted
            if (!num_of_node) {
                printf("[Error] There is no node to express.\n");
                break;
            }

            // Print all nodes with inorder.
            printf("Inorder   : ");
            BST_inOrder(LRNode[0]);
            printf("\n");

            break;

        case 6:
            /// Postorder
            printf("*************************************************\n");

            // If no node is inserted
            if (!num_of_node) {
                printf("[Error] There is no node to express.\n");
                break;
            }

            // Print all nodes with postorder.
            printf("Postorder : ");
            BST_postOrder(LRNode[0]);
            printf("\n");

            break;

        case 7:
            /// Show all
            printf("*************************************************\n");

            // If no node is inserted
            if (!num_of_node) {
                printf("[Error] There is no node to express.\n");
                break;
            }

            // Print all nodes with preorder.
            printf("preorder  : ");
            BST_preOrder(LRNode[0]);
            printf("\n");

            // Print all nodes with inorder.
            printf ("inorder   : ");
            BST_inOrder(LRNode[0]);
            printf("\n");

            // Print all nodes with postorder.
            printf("postorder : ");
            BST_postOrder(LRNode[0]);
            printf("\n");

            break;

        case 0:
            /// End message
            if (!EndMessage()) handle_msg = -1;

            break;

        default :
            /// Error case
            printf("[Error] Improper number is read.\n");
            printf("        Your input : %d / proper input : 0 ~ 7 \n", handle_msg);
            break;
        }
    }

    /* End of function */

    // Send user to go out
    printf("Go to the main menu.\n");

    // Free memory
    for (loop = 0 ; loop <num_of_node + 1 ; loop++) {
        free(LRNode[loop]);
    }

    free(LRNode);
    free(tem_buffer);

    return;
}

void InitMessage(int type) {
    printf("*************************************************\n");

    switch (type) {
    // Main menu
    case 0 :
        printf("Welcome to Shin Hee Chan's homework 1 !! \n");
        printf("Select number to decide job what you do\n");
        printf("[1] Max heap with stack\n");
        printf("[2] Binary Search Tree\n");
        printf("[3] Exit\n");
        break;

    case 1:
        printf("Max Heap Function\n");
        printf("[1] Insert Heap\n");
        printf("[2] Delete Heap\n");
        printf("[3] Stack Push\n");
        printf("[4] Stack Pop\n");
        printf("[5] Print status\n");
        printf("[0] Goto Main menu\n");
        break;

    case 2:
        printf("Binary Search Function\n");
        printf("[1] Insert Heap\n");
        printf("[2] Search Heap \n");
        printf("[3] Delete Heap\n");
        printf("[4] Preorder\n");
        printf("[5] Inorder\n");
        printf("[6] Postorder\n");
        printf("[7] show all orders\n");
        printf("[0] Goto Main menu\n");
    }

    printf("*************************************************\n");
    printf("Please type number : ");


    return;
}

int EndMessage() {
    char yn[30];

    yn[0] = 0;

    printf("[Warning] Data might be deleted from this program or menu.\n");
    printf("          Do you want to exit anyway? [y/n] ");

    scanf("%s", yn);

    if (yn[0] == 'y') {
        printf("program or menu will be terminated.\n");

        // Collect garbage
        fgets(yn, 30 , stdin);

        return 1;
    }

    // Collect garbage
    fgets(yn, 30 , stdin);

    return 0;

}

void heap_insert(pHeapArr pHA, LinkedStack *stack) {
    LinkedStack *prestack;

    // Save previous stack
    prestack = pHA->pStack;

    // HeapArr will point new node
    pHA->pStack = stack;

    // Now, node stack will point previous stack pointer
    stack->next = prestack;

    // Increase heap's key
    pHA->key += stack->value;
}

void max_heapify(pHeapArr *pHA, int index) {
    int parentkey , mykey;
    pHeapArr temp_pHA = NULL;

    // Do not work in case of index < 2
    if (index < 2) return;

    // get parents key and current index' key
    parentkey = pHA[index / 2]->key;
    mykey = pHA[index]->key;

    // If my key is bigger than parents key, swap two Array
    if (mykey > parentkey) {
        temp_pHA = pHA[index];
        pHA[index] = pHA[index / 2];
        pHA[index / 2] = temp_pHA;
    }

    // Recall function for parents
    max_heapify(pHA, index / 2);

    // End of function
    return;
}

void max_heapify_ascend(pHeapArr *pHA, int index, int size_heap) {
    int mykey, lckey, rckey;
    pHeapArr temp_pHA = NULL;

    // Initialize all variable to minimum value
    mykey = lckey = rckey = -2147483647;

    // If index is larger than half size of the heap, break function
    if (index > size_heap / 2) return;

    // Get current index key
    mykey = pHA[index]->key;

    // Get child key if exist.
    if (size_heap >= (2 * index)) lckey = pHA[index * 2]->key;
    if (size_heap >= (2 * index) + 1) rckey = pHA[(index * 2) + 1]->key;

    // If left child is biggest
    if (lckey > mykey && lckey > rckey) {
        // swap array and recall function to left
        temp_pHA = pHA[index];
        pHA[index] = pHA[index * 2];
        pHA[index * 2] = temp_pHA;

        max_heapify_ascend(pHA, index * 2 , size_heap);

    // Else if right child is biggest
    } else if (rckey > mykey && rckey > lckey) {
            // swap array and recall function to right
        temp_pHA = pHA[index];
        pHA[index] = pHA[index * 2 + 1];
        pHA[index * 2 + 1] = temp_pHA;

        max_heapify_ascend(pHA, index * 2 + 1, size_heap);
    }

    // End of function
    return;
}

void heap_delete(pHeapArr pHA) {
    LinkedStack *prestack, *stack;

    // Save current
    stack = pHA->pStack;

    // Delete until every node is gone
    while(stack != NULL) {
        prestack = stack->next;

        free(stack);

        stack = prestack;
    }
}

void stack_push (pHeapArr pHA, int value) {
    LinkedStack *newstack;

    // create new node
    newstack = (LinkedStack*)malloc(sizeof(LinkedStack));

    // Save key onto new node
    newstack->value = value;

    // Created node will point previous node
    newstack->next = pHA->pStack;

    // Now, Array point new node.
    pHA->pStack = newstack;

    // Increase key value
    pHA->key += value;

    // End of function
    return;
}

int stack_pop(pHeapArr pHA, int *save_value) {
    LinkedStack *prestack, *stack;

    // If pointer is Null, return false(0)
    if (pHA->pStack == NULL) return 0;

    // Save current and stack
    stack = pHA->pStack;
    prestack = stack->next;

    // Collect data from top node
    *save_value = stack->value;

    // Delete header stack
    free(stack);

    // Now, pointer will point previous stack
    pHA->pStack = prestack;

    // Decrease key value
    pHA->key -= *save_value;

    // Return succeed
    return 1;
}


void heap_print_status(pHeapArr pHA) {
    LinkedStack *stack;

    // If current pointer is null, print "NULL"
    if (pHA->pStack == NULL) {
        printf("NULL");
        return;
    }

    // Save current
    stack = pHA->pStack;

    // Delete until every node is gone
    while(stack != NULL) {
        printf("%d ", stack->value);

        stack = stack->next;
    }

    // End of function
    return;
}

void BST_insert(pNode head, pNode pnode, int key, int index) {
    pNode cur_search = head->pLlink;

    // Insert key onto node
    pnode->key = key;
    pnode->index = index;

    // Insert link onto node
    pnode->pLlink = NULL;
    pnode->pRlink = NULL;

    // If inserted node is header node
    if ((!head->pLlink) && (!head->pRlink)) {
        head->pLlink = pnode;
        return;
    }

    // Search until reaching node which has no child
    while (1) {
        // In case of no child
        if ((!cur_search->pLlink) && (!cur_search->pRlink)) {
            // Hanging left if key is smaller than last node
            if (key < cur_search->key) {
                cur_search->pLlink = pnode;
            // Hanging right if not
            } else {
                cur_search->pRlink = pnode;
            }

            // break loop
            break;
        }

        // In case of have to go left
        if (key < cur_search->key) {
            // If we can go much deeper
            if (cur_search->pLlink) {
                cur_search = cur_search->pLlink;
                continue;
            // If not, hang it onto left array
            } else {
                cur_search->pLlink = pnode;

                // break loop
                break;
            }

        // In case of have to go right
        } else {
            if (cur_search->pRlink) {
                cur_search = cur_search->pRlink;
                continue;
            // If not, hang it onto left array
            } else {
                cur_search->pRlink = pnode;

                // break loop
                break;
            }
        }
    }

    // End of function
    return;
}

int BST_search(pNode pnode, int key) {
    // If node is header, go left once
    if (pnode->index == 0) pnode = pnode->pLlink;

    // Check whether current is right
    if (pnode->key == key) return 1;

    // Check left if exist
    if (pnode->pLlink && BST_search(pnode->pLlink, key)) return 1;

    // Check right if exist
    if (pnode->pRlink && BST_search(pnode->pRlink, key)) return 1;

    // If does not exist in all case, return 0
    return 0;

}

int BST_delete(pNode pnode, int key) {
    pNode parentnode;
    pNode minnode;
    int isleft = 1;

    // Search until find key
    while (pnode->key != key) {
        parentnode = pnode;

        // Go left if node's key is larger than key
        if (pnode->key > key) {
            isleft = 1;
            pnode = pnode->pLlink;

        // If not, go right
        } else {
            isleft = 0;
            pnode = pnode->pRlink;
        }
    }

    /* Case of no child */
    if (!(pnode->pLlink) && !(pnode->pRlink)) {
        // Set parent node point NULL
        if (isleft) {
            parentnode->pLlink = NULL;
        } else {
            parentnode->pRlink = NULL;
        }

        // Ready to delete it
        return pnode->index;
    }

    /* Case of left child only */
    if ((pnode->pLlink) && !(pnode->pRlink)) {
        // Parents node will point left child
        if (isleft || parentnode->index == 0) {
            parentnode->pLlink = pnode->pLlink;
        } else {
            parentnode->pRlink = pnode->pLlink;
        }

        return pnode->index;
    }

    /* Case of right child only */
    if (!(pnode->pLlink) && (pnode->pRlink)) {
        // Parents node will point right child
        // In case of header node, always set left child
        if (isleft || parentnode->index == 0) {
            parentnode->pLlink = pnode->pRlink;
        } else {
            parentnode->pRlink = pnode->pRlink;
        }

        return pnode->index;
    }

    /* Case of both children exist */
    minnode = pnode->pRlink;

    while(minnode->pLlink != NULL) {
        minnode = minnode->pLlink;
    }

    // Set parent node will point min_node
    if (isleft || parentnode->index == 0) {
        parentnode->pLlink = minnode;
    } else {
        parentnode->pRlink = minnode;
    }

    // min_node will point delete node's left child
    minnode->pLlink = pnode->pLlink;

    return pnode->index;
}

void BST_preOrder(pNode pnode) {
    /* preorder : Current -> Left -> Right */

    // If node is header, go left once
    if (pnode->index == 0) pnode = pnode->pLlink;

    // Current
    printf("[%d] ", pnode->key);

    // Left
    if (pnode->pLlink) {
        BST_preOrder(pnode->pLlink);
    }

    // Right
    if (pnode->pRlink) {
        BST_preOrder(pnode->pRlink);
    }

    // End of function
    return;
}

void BST_inOrder(pNode pnode) {
    /* inorder : Left -> Current -> Right */

    // If node is header, go left once
    if (pnode->index == 0) pnode = pnode->pLlink;

    // Left
    if (pnode->pLlink) {
        BST_inOrder(pnode->pLlink);
    }

    // Current
    printf("[%d] ", pnode->key);

    // Right
    if (pnode->pRlink) {
        BST_inOrder(pnode->pRlink);
    }

    // End of function
    return;
}

void BST_postOrder(pNode pnode) {
    /* postorder : Left -> Right -> Current */

    // If node is header, go left once
    if (pnode->index == 0) pnode = pnode->pLlink;

    // Left
    if (pnode->pLlink) {
        BST_postOrder(pnode->pLlink);
    }

    // Right
    if (pnode->pRlink) {
        BST_postOrder(pnode->pRlink);
    }

    // Current
    printf("[%d] ", pnode->key);


    // End of function
    return;
}
