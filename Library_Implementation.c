#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Library_Header.h"

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