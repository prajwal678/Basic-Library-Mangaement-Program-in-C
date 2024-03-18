#ifndef LIBRARY_HEADER_H
#define LIBRARY_HEADER_H

#define LEN 200
#define MAX 5

typedef struct book {
    char book_name[LEN];
    int id;
}BOOK;

typedef struct request_node {
    BOOK book;
    struct request_node *next;
}REQUEST_NODE;

typedef struct requests {
    REQUEST_NODE *front, *rear;
}REQUESTS;

typedef struct book_node {
    BOOK book;
    struct book_node *next;
}BOOK_NODE;

typedef struct owned_books {
    BOOK_NODE *head;
}OWNED_BOOKS;

typedef struct student {
    char student_name[30];
    char SRN[14];
    REQUESTS requests;
    OWNED_BOOKS book_list;
    struct student *next;
}STUDENT;

typedef struct student_list {
    STUDENT *head;
}STUDENT_LIST;

typedef struct tree_node {
    BOOK book;
    struct tree_node *left, *right;
    int height;
}TREE_NODE;

typedef struct tree {
    TREE_NODE *root;
}TREE;


// Initialising functions/methods
void initStudentList(STUDENT_LIST *student_list);
void initRequests(REQUESTS *requests);
void initOwnedBooks(OWNED_BOOKS *book_list);
void initTree(TREE *pt);

//  and Student List related functions/methods
int addStudentToList(STUDENT_LIST *student_list, char student_name[30], char SRN[14]);
int removeStudentFromList(STUDENT_LIST *student_list, char SRN[14]);
STUDENT *searchStudent(STUDENT_LIST *student_list, char SRN[14]);
void displayStudentList(STUDENT_LIST *student_list);
void freeStudentList(STUDENT_LIST *list);

// Student Info 
void displayStudentDetails(STUDENT *student);

// Request Queue functions/methods
int isFullRequests(REQUESTS *requests);
int isEmptyRequests(REQUESTS *requests);
int enqueueRequest(REQUESTS *requests , int id, int subject, TREE *subjects[4]);
int dequeueRequest(REQUESTS *requests, int id);
REQUEST_NODE *searchRequest(REQUESTS *requests, int id);
void displayRequests(REQUESTS *requests);
void freeRequests(REQUESTS *queue);

// Owned Books fucntions/methods
int isFullBooks(OWNED_BOOKS *book_list);
int isEmptyBooks(OWNED_BOOKS *book_list);
int insertBookToList(OWNED_BOOKS *book_list, int id, int subject, TREE *subjects[4]);
int deleteBookFromList(OWNED_BOOKS *book_list , int id);
BOOK_NODE* searchOwnedBook(OWNED_BOOKS *book_list, int id);
void displayBorrowedBooks(OWNED_BOOKS *book_list);
void freeOwnedBooks(OWNED_BOOKS *book_list);

// request queue to owned books lsit function/methods
void pushFromRequestsToBookList(STUDENT *student, int id, int subject, TREE *subjects[4]);

//AVL tree functions/methods
int max(int a, int b);
TREE_NODE* rightRotation(TREE_NODE* tree_node);
TREE_NODE* leftRotation(TREE_NODE* tree_node);
void insertNode(TREE *pt, TREE_NODE *newNode);
int deleteNode(TREE *pt, int id);
void displayTree(TREE *pt);
TREE_NODE* searchTreeName(TREE *pt, char book_name[LEN]);
TREE_NODE* searchTreeId(TREE *pt, int id);
void freeTree(TREE *pt);

#endif