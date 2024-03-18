#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Library_Header.h"
#define LEN 200
#define MAX 5


// Initialising functions/methods
void initStudentList(STUDENT_LIST *student_list) {
    student_list->head = NULL;
}

void initRequests(REQUESTS *requests) {
    requests->front = requests->rear = NULL;
}

void initOwnedBooks(OWNED_BOOKS *book_list) {
    book_list->head = NULL;
}

void initTree(TREE *pt) {
    pt->root = NULL;
}

// Student and Student List related functions/methods
STUDENT* createNewStudent(char student_name[30], char SRN[14]) {
    STUDENT *newStudent = (STUDENT*)malloc(sizeof(STUDENT));
    if (newStudent == NULL) {
        printf("Memory allocation for new student unsuccessful.\n");
        return NULL;
    }
    strcpy(newStudent->student_name, student_name);
    strcpy(newStudent->SRN, SRN);
    initRequests(&(newStudent->requests));
    initOwnedBooks(&(newStudent->book_list));
    newStudent->next = NULL;

    return newStudent;
}

int addStudentToList(STUDENT_LIST *student_list, char student_name[30], char SRN[14]) {
    if(searchStudent(student_list, SRN)) {
        printf("Student is already in the list\n");
        return 0;
    }
    STUDENT *newStudent = createNewStudent(student_name, SRN);
    if (newStudent == NULL) {
        printf("Student is already in the list\n");
        return 0;
    }
    if(student_list->head == NULL)
        student_list->head = newStudent;
    else {
        newStudent->next = student_list->head;
        student_list->head = newStudent;
    }
    return 1;
}

int removeStudentFromList(STUDENT_LIST *student_list, char SRN[14]) {
    STUDENT *current = student_list->head;
    STUDENT *previous = NULL;

    while(current != NULL) {
        if(strcmp(current->SRN, SRN) == 0) {
            if(previous == NULL)
                student_list->head = current->next;
            else
                previous->next = current->next;
                
            freeRequests(&current->requests);
            freeOwnedBooks(&current->book_list);
            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

STUDENT *searchStudent(STUDENT_LIST *student_list, char SRN[14]) {
    STUDENT *p = student_list->head;
    while(p != NULL) {
        if(strcmp(SRN, p->SRN) == 0)
            return p;
        p = p->next;
    }
    return NULL;
}

void displayStudentList(STUDENT_LIST *student_list) {
    if (student_list->head == NULL)
        printf("The student list is empty.\n");
    else {
        printf("Student List\n");
        STUDENT *p = student_list->head;
        while(p!=NULL) {
            printf("%s %s\n" , p->student_name, p->SRN);
            p = p->next;
        }
    }
}

void freeStudentList(STUDENT_LIST *list) {
    if (list->head == NULL)
        printf("The student list is already empty.\n");
    else {
        STUDENT *current = list->head;
        STUDENT *next = NULL;

        while (current != NULL) {
            next = current->next;
            freeRequests(&current->requests);
            freeOwnedBooks(&current->book_list);
            free(current);
            current = next;
        }
        list->head = NULL;
    }
}


// Student Info
void displayStudentDetails(STUDENT *student) {
    printf("\n\nStudent details\n\n");
    printf("Student name: %sSRN: %s", student->student_name, student->SRN);
    printf("\nYour requests are:\n");
    displayRequests(&student->requests);
    printf("\nYour books are:\n");
    displayBorrowedBooks(&student->book_list);
}

// Request Queue functions/methods
int countRequests(REQUESTS *requests) {
    REQUEST_NODE *p = requests->front;
    int count = 0;
    while(p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}

int isFullRequests(REQUESTS *requests) {
    int n = countRequests(requests);
    if(n == MAX)
        return 1;
    else
        return 0;
}

int isEmptyRequests(REQUESTS *requests) {
    return requests->rear == NULL;
}

int enqueueRequest(REQUESTS *requests, int id, int subject, TREE *subjects[4]) {
    int result = 0;
    TREE_NODE *p = searchTreeId(subjects[subject], id);

    if (p != NULL) {
        REQUEST_NODE *newNode = (REQUEST_NODE*)malloc(sizeof(REQUEST_NODE));
        newNode->book.id = p->book.id;
        strcpy(newNode->book.book_name, p->book.book_name);
        newNode->next = NULL;

        if(isEmptyRequests(requests)) {
            requests->front = newNode;
            requests->rear = newNode;
            result = 1;
        }
        else if(!searchRequest(requests, id)) {
            requests->rear->next = newNode;
            requests->rear = newNode;
            result = 1;
        }
        else {
            printf("Already in your requests\n");
            free(newNode);
        }
    }
    else {
        printf("Book id not found in the subject tree\n");
    }

    return result;
}

int dequeueRequest(REQUESTS *requests, int id) {
    if (isEmptyRequests(requests)) {
        printf("The queue is empty.\n");
        return 0;
    }

    REQUEST_NODE *current = requests->front;
    REQUEST_NODE *previous = NULL;
    while (current != NULL) {
        if (current->book.id == id) {
            if (previous == NULL)
                requests->front = current->next;
            else
                previous->next = current->next;

            if (current == requests->rear)
                requests->rear = previous;

            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }

    printf("A request with id %d was not found in the queue.\n", id);
    return 0;
}


REQUEST_NODE *searchRequest(REQUESTS *requests, int id) {
    if (requests->front == NULL) {
        printf("request queue is empty\n");
        return NULL; // or appropriate error code
    }

    REQUEST_NODE *p = requests->front;
    while(p != NULL) {
        if(p->book.id == id)
            return p;
        p = p->next;
    }
    printf("A request with id %d was not found in the queue.\n", id);
    return NULL;
}

void displayRequests(REQUESTS *requests) {
    if (requests->front == NULL)
        printf("The queue is empty.\n");
    else {
        REQUEST_NODE *p = requests->front;
        while(p != NULL) {
            printf("%d. %s\n" , p->book.id , p->book.book_name);
            p = p->next;
        }
    }
}

void freeRequests(REQUESTS *queue) {
    if (queue->front == NULL)
        printf("The queue is already empty.\n");
    else {
        REQUEST_NODE *current = queue->front;
        REQUEST_NODE *next;

        while (current != NULL) {
            next = current->next;
            free(current);
            current = next;
        }
        queue->front = NULL;
        queue->rear = NULL;
    }
}

// Owned Books fucntions/methods
int countOwnedBooks(OWNED_BOOKS *book_list) {
    if (book_list->head == NULL)
        return 0;

    BOOK_NODE *p = book_list->head;
    int count = 0;
    while(p!=NULL) {
        count++;
        p = p->next;
    }
    return count;
}

int isFullBooks(OWNED_BOOKS *book_list) {
    if (book_list->head == NULL)
        return 0;

    int c = countOwnedBooks(book_list);
    if(c == MAX)
        return 1;
    return 0;
}

int isEmptyBooks(OWNED_BOOKS *book_list) {
    if(book_list->head == NULL)
        return 1;
    return 0;
}

int insertBookToList(OWNED_BOOKS *book_list, int id, int subject, TREE *subjects[4]) {
    if(searchOwnedBook(book_list, id)) {
        printf("You've already borrowed this book\n");
        return 0;
    }
    BOOK_NODE *newNode = malloc(sizeof(BOOK_NODE));
    if (newNode == NULL) {
        printf("Memory allocation for new node unsuccesful\n");
        return 0;
    }
    TREE_NODE *p = searchTreeId(subjects[subject], id);
    if (p == NULL) {
        printf("Error: Book not found in tree\n");
        free(newNode);
        return 0;
    }
    newNode->book.id = p->book.id;
    strcpy(newNode->book.book_name, p->book.book_name);
    
    newNode->next = book_list->head;
    book_list->head = newNode;

    return 1;
}

int deleteBookFromList(OWNED_BOOKS *book_list , int id) {
    if (book_list == NULL) {
        printf("Error: Null pointer passed to function\n");
        return 0;
    }
    BOOK_NODE *current = book_list->head;
    BOOK_NODE *previous = NULL;

    while(current != NULL) {
        if(current->book.id == id) {
            if(previous == NULL)
                book_list->head = current->next;
            else
                previous->next = current->next;

            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

BOOK_NODE* searchOwnedBook(OWNED_BOOKS *book_list, int id) {
    if (book_list->head == NULL)
        return NULL;

    BOOK_NODE *p = book_list->head;
    while(p!=NULL) {
        if(p->book.id == id)
            return p;
        p = p->next;
    }
    return NULL;
}

void displayBorrowedBooks(OWNED_BOOKS *book_list) {
    if (book_list->head == NULL) {
        printf("Booklist is empty\n");
        return;
    }

    BOOK_NODE *p = book_list->head;
    while(p!=NULL) {
        printf("%d. %s\n" ,p->book.id , p->book.book_name);
        p = p->next;
    }
}

void freeOwnedBooks(OWNED_BOOKS *book_list) {
    if (book_list->head == NULL) {
        printf("Book list is empty\n");
        return;
    }
    BOOK_NODE *current = book_list->head;
    BOOK_NODE *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    book_list->head = NULL;
}

// request queue to owned books list function
void pushFromRequestsToBookList(STUDENT *student, int id, int subject, TREE *subjects[4]) {
    REQUEST_NODE *p = searchRequest(&student->requests , id);
    if(p!=NULL) {
        insertBookToList(&(student->book_list) , id, subject, subjects);
        if(dequeueRequest(&student->requests , id))
            printf("Request to borrow successful\n");
        else
            printf("Couldn't transfer\n");
    }
    else
        printf("Book NOT found in request\n"); 
}

// AVL tree functions/methods
int max(int a, int b) {
    return (a>b)?a:b;
}

TREE_NODE* createNode(int id, char book_name[LEN]) {
    TREE_NODE* newNode = (TREE_NODE*)malloc(sizeof(TREE_NODE));
    if(newNode == NULL) {
        printf("Memory allocation failed for tree node\n");
        return NULL;
    }
    strcpy(newNode->book.book_name, book_name);
    newNode->book.id = id;
    newNode->left = newNode->right = NULL;
    newNode->height = 1;

    return newNode;
}

int height(TREE_NODE *tree_node) {
    if(tree_node == NULL)
        return 0;
    else
        return max(height(tree_node->left), height(tree_node->right)) + 1;
}

int balanceFactor(TREE_NODE *tree_node) { 
    if(tree_node == NULL)
        return 0;
    else
        return height(tree_node->left) - height(tree_node->right);
}

TREE_NODE* rightRotation(TREE_NODE* tree_node) { 
    TREE_NODE *leftChild = tree_node->left;
    tree_node->left = leftChild->right;
    leftChild->right = tree_node;

    tree_node->height = height(tree_node);
    leftChild->height = height(leftChild);

    return leftChild;
}

TREE_NODE* leftRotation(TREE_NODE* tree_node) {
    TREE_NODE *rightChild = tree_node->right;
    tree_node->right = rightChild->left;
    rightChild->left = tree_node;

    tree_node->height = height(tree_node);
    rightChild->height = height(rightChild);

    return rightChild;
}

//////////// INSERTION ////////////////////////////////////////////////////////////
TREE_NODE* insertNewNode(TREE_NODE *root, TREE_NODE *newNode) {
    if(root == NULL)
        return newNode;

    if(strcmp(newNode->book.book_name, root->book.book_name)>0) {
        root->right = insertNewNode(root->right, newNode);
    }
    else if(strcmp(newNode->book.book_name, root->book.book_name)<0) {
        root->left = insertNewNode(root->left, newNode);
    }
    else
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int bf = balanceFactor(root);
    if(bf > 1) {                                        // left heavy
        if(balanceFactor(root->left) < 0)               // left-right case
            root->left = leftRotation(root->left);
        return rightRotation(root);                     // left-left case
    }
    else if(bf < -1) {                                  // right heavy
        if(balanceFactor(root->right) > 0)              // right-left case
            root->right = rightRotation(root->right);
        return leftRotation(root);                      // right-right case
    }

    return root;    
}

void insertNode(TREE *pt, TREE_NODE *newNode) {
    pt->root = insertNewNode(pt->root, newNode);
}

///////////////// DELETION ////////////////////////////////////////////////////////
TREE_NODE* leftMostNode(TREE_NODE* node) {
    TREE_NODE* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

TREE_NODE* deleteNodeRecursion(TREE_NODE* root, int id) {
    if (root == NULL)
        return root;

    if (id < root->book.id)
        root->left = deleteNodeRecursion(root->left, id);
    else if(id > root->book.id)
        root->right = deleteNodeRecursion(root->right, id);
    else {
        if( (root->left == NULL) || (root->right == NULL) ) {
            TREE_NODE *temp = root->left ? root->left : root->right;
            if(temp == NULL) {
                temp = root;
                root = NULL;
            }
            else {
                // Copy only the book data, not the entire node
                root->book = temp->book;
                // Delete the temp node
                free(temp);
            }
        }
        else {
            TREE_NODE* temp = leftMostNode(root->right);
            root->book = temp->book;
            root->right = deleteNodeRecursion(root->right, temp->book.id);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int bf = balanceFactor(root); // bf is the balance factor of the root
    if(bf > 1) {
        if(balanceFactor(root->left) < 0)
            root->left = leftRotation(root->left);
        root = rightRotation(root);
    }
    else if(bf < -1) {
        if(balanceFactor(root->right) > 0)
            root->right = rightRotation(root->right);
        root = leftRotation(root);
    }

    return root;
}

int deleteNode(TREE *pt, int id) {
    if(pt->root == NULL)
        return 0;
    pt->root = deleteNodeRecursion(pt->root, id);
    return 1;
}

void displayTreeNodes(TREE_NODE *root) {
    if(root == NULL)
        return;
    
    displayTreeNodes(root->left);
    printf("id: %d name: %s\n", root->book.id, root->book.book_name);
    displayTreeNodes(root->right);
}

void displayTree(TREE *pt) {
    displayTreeNodes(pt->root);
}

TREE_NODE* searchNodeName(TREE_NODE *root, char book_name[LEN]) {
    if(root == NULL || (strcasecmp(book_name, root->book.book_name) == 0))
        return root;

    if(strcasecmp(book_name, root->book.book_name) > 0)
        return searchNodeName(root->right, book_name);
    else
        return searchNodeName(root->left, book_name);
}

TREE_NODE* searchTreeName(TREE *pt, char book_name[LEN]) {
    TREE_NODE *p = searchNodeName(pt->root, book_name);
    if(p)
        return p;
    return NULL;
}

TREE_NODE* searchNodeId(TREE_NODE *root, int id) {
    if(root == NULL || (root->book.id == id))
        return root;

    if(id > root->book.id)
        return searchNodeId(root->right, id);
    else
        return searchNodeId(root->left, id);
}

TREE_NODE* searchTreeId(TREE *pt, int id) {
    TREE_NODE *p = searchNodeId(pt->root, id);
    if(p)
        return p;
    return NULL;
}

void freeTreeRecursive(TREE_NODE *root) {
    if (root == NULL)
        return;

    freeTreeRecursive(root->left);
    freeTreeRecursive(root->right);
    free(root);
}

void freeTree(TREE *pt) {
    freeTreeRecursive(pt->root);
    pt->root = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void set(TREE *CS, TREE *ECE, TREE *EEE) {
    TREE_NODE *cs1 = createNode(11, "CS Book 1");
    TREE_NODE *cs2 = createNode(12, "CS Book 2");
    TREE_NODE *cs3 = createNode(13, "CS Book 3");
    TREE_NODE *cs4 = createNode(14, "CS Book 4");
    TREE_NODE *cs5 = createNode(15, "CS Book 5");
    insertNode(CS, cs1);
    insertNode(CS, cs2);
    insertNode(CS, cs3);
    insertNode(CS, cs4);
    insertNode(CS, cs5);

    TREE_NODE *ece1 = createNode(21, "ECE Book 1");
    TREE_NODE *ece2 = createNode(22, "ECE Book 2");
    TREE_NODE *ece3 = createNode(23, "ECE Book 3");
    TREE_NODE *ece4 = createNode(24, "ECE Book 4");
    TREE_NODE *ece5 = createNode(25, "ECE Book 5");
    insertNode(ECE, ece1);
    insertNode(ECE, ece2);
    insertNode(ECE, ece3);
    insertNode(ECE, ece4);
    insertNode(ECE, ece5);

    TREE_NODE *eee1 = createNode(31, "EEE Book 1");
    TREE_NODE *eee2 = createNode(32, "EEE Book 2");
    TREE_NODE *eee3 = createNode(33, "EEE Book 3");
    TREE_NODE *eee4 = createNode(34, "EEE Book 4");
    TREE_NODE *eee5 = createNode(35, "EEE Book 5");
    insertNode(EEE, eee1);
    insertNode(EEE, eee2);
    insertNode(EEE, eee3);
    insertNode(EEE, eee4);
    insertNode(EEE, eee5);
}

int main()
{
    TREE CS, ECE, EEE;
    TREE *subjects[4] = {NULL, &CS, &ECE, &EEE};
    initTree(&CS); initTree(&ECE); initTree(&EEE);

    set(&CS, &ECE, &EEE);

    printf("\nWelcome To Library\n");

    STUDENT_LIST student_list; initStudentList(&student_list);
    int studentOrAdmin;

    do { 
        printf("\n1.Student 2.Admin\n");
        scanf("%d",&studentOrAdmin); getchar();
        char name[30], SRN[14], book[LEN], operation;
        int action, queueAction, id, subject;
        STUDENT *ptr;

        switch (studentOrAdmin) {
            case 1: printf("\nEnter Name: "); fgets(name, sizeof(name), stdin);
                    printf("\nEnter SRN: "); fgets(SRN, sizeof(SRN), stdin);

                    if(searchStudent(&student_list, SRN))
                        ptr = searchStudent(&student_list, SRN);
                    else {
                        addStudentToList(&student_list, name, SRN);
                        ptr = student_list.head; // ptr = searchStudent(&student_list, SRN);     
                    }                       

                    printf("\n1.Borrow 2.Return 3.Request\t"); scanf("%d",&action);
                    do {
                        switch (action) {
//////////////////////////////////////////// BORROW //////////////////////////////////////////////////////////////////////////////////////////////                            
                            case 1: if(!isFullBooks(&ptr->book_list)) {
                                        if(!isEmptyRequests(&(ptr->requests))) { //  check if request in null
                                            displayRequests(&ptr->requests);
                                            printf("\nDo you want to borrow from requested books? Y/N\t");
                                    
                                            scanf(" %c", &operation);
                                            if(operation == 'Y' | operation == 'y') {
                                                displayRequests(&ptr->requests);
                                                printf("\nEnter id of the book to be borrowed from requests\t");
                                                scanf("%d", &id);
                                                pushFromRequestsToBookList(ptr, id, subject, subjects);
                                                displayBorrowedBooks(&ptr->book_list);
                                            }
                                            else {
                                                printf("\nChoose the subject you want\n1.CS 2.ECE 3.EEE\t");
                                                scanf("%d", &subject);
                                                switch(subject) {
                                                    case 1: displayTree(&CS); break;
                                                    case 2: displayTree(&ECE); break;
                                                    case 3: displayTree(&EEE); break;
                                                    default: printf("\nEnter correct id\n"); break;
                                                }
                                                printf("\nEnter the id of the book to be borrowed\t");
                                                scanf("%d", &id);
                                                if(insertBookToList(&(ptr->book_list), id, subject, subjects))
                                                    printf("\nBook borrowed successfully\n");
                                                else
                                                    printf("\nBook was NOT borrowed\n");
                                                printf("rsegsth\n");
                                                displayBorrowedBooks(&ptr->book_list);
                                            }
                                        }  
                                        else {
                                            printf("\nChoose the subject you want\n1.CS 2.ECE 3.EEE\t"); scanf("%d", &subject);
                                            switch(subject) {
                                                case 1: displayTree(&CS); break;
                                                case 2: displayTree(&ECE); break;
                                                case 3: displayTree(&EEE); break;
                                                default: printf("\nEnter correct id\n"); break;
                                            }
                                            printf("\nEnter the book id to be borrowed\t");
                                            scanf("%d", &id);
                                            if(insertBookToList(&(ptr->book_list), id, subject, subjects))
                                                printf("\nBook borrowed successfully\n");
                                            else
                                                printf("\nBook was NOT borrowed\n");
                                            displayBorrowedBooks(&ptr->book_list);
                                        }
                                    }
                                    else
                                        printf("\nYou have borrowed max\n");
                                    
                                    break;
////////////////////////////////////////////////// RETURN ////////////////////////////////////////////////////////////////////////////////////////////////////////
                            case 2: if(isEmptyBooks(&ptr->book_list))  //check if bookList is empty
                                        printf("\nYou have no books to return\n");
                                    else {
                                        displayBorrowedBooks(&ptr->book_list);
                                        printf("\nEnter the id of the book to be returned\t");
                                        scanf("%d", &id);
                                        if(deleteBookFromList(&ptr->book_list, id))
                                            printf("\nBook returned!\n");
                                        else
                                            printf("\nCouldn't remove book or wrong id provided\n");
                                    }
                                    break;
//////////////////////////////////////////////////// REQUEST //////////////////////////////////////////////////////////////////////////////////////////                                
                            case 3: printf("\n1.Make request 2.Delete a request\t"); scanf("%d",&queueAction);
                                    switch(queueAction) {
                                        case 1: if(!isFullRequests(&ptr->requests)) {
                                                    printf("\nChoose the subject you want\n1.CS 2.ECE 3.EEE\t"); scanf("%d", &subject);
                                                    switch(subject) {
                                                        case 1: displayTree(&CS); break;
                                                        case 2: displayTree(&ECE); break;
                                                        case 3: displayTree(&EEE); break;
                                                        default: printf("\nEnter correct id\n"); break;
                                                    }
                                                    printf("\nEnter the book id to be requested\t");
                                                    scanf("%d", &id);
                                                    if(enqueueRequest(&ptr->requests, id, subject, subjects))
                                                        printf("\nBook added to requests\n");
                                                    else
                                                        printf("\nBook NOT added to requests");

                                                    displayRequests(&ptr->requests);
                                                }
                                                else
                                                    printf("\nRequests are full.\n");
                                                break;

                                        case 2: if(!isEmptyRequests(&ptr->requests)) {
                                                    displayRequests(&ptr->requests);
                                                    printf("\nEnter id of the book to be removed\t");
                                                    scanf("%d", &id);
                                                    if(dequeueRequest(&ptr->requests , id))
                                                        printf("\nDequeued sucessfully\n");
                                                    else
                                                        printf("\nCouldn't dequeue");
                                                }
                                                else
                                                    printf("\nYou have no requests\n");
                                                break;
                                            
                                        default: break;
                                    }
                                    break;

                            default: break;

                            case 4: displayStudentDetails(ptr); break;
                        }
                        printf("\n1.Borrow 2.Return 3.Request 4.Display Info 5.Previous Menu\t");
                        scanf("%d",&action);
                    } while (action<5);
                    break;
///////////////////////////////////////////// ADMIN ////////////////////////////////////////////////////////////////////////////////////////////////     
            case 2: printf("1.Display students 2.Delete Student Info\n"); scanf("%d", &action);
                    switch(action) {
                        case 1: displayStudentList(&student_list);
                                break;
                        case 2: printf("Enter SRN to be removed\n"); scanf("%s", &SRN);
                                if(removeStudentFromList(&student_list, SRN))
                                    printf("SRN: %s removed from student list\n", SRN);
                                else
                                    printf("Student with SRN: %s was not found in the list\n");
                        default: break;
                    }
                    break;
                
            default : break;
            printf("1.Student 2.Admin");
            scanf("%d",&studentOrAdmin);       
        }
        free(ptr);
        ptr = NULL;
    }while(studentOrAdmin<3);

    freeTree(&CS); 
    freeTree(&ECE); 
    freeTree(&EEE);
    freeStudentList(&student_list);
    return 0;
}