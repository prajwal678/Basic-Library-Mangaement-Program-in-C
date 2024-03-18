#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Library_Header.h"
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

int formTree(TREE *tree, FILE *fp);

int main() {
    TREE CS, ECE, EEE;
    TREE *subjects[4] = {NULL, &CS, &ECE, &EEE};
    initTree(&CS); 
    initTree(&ECE); 
    initTree(&EEE);

    FILE *fpCS = fopen("CS.csv", "r");
    if (fpCS == NULL) {
        printf("Error opening CS.csv\n");
        return 1;
    }
    int resultCS = formTree(&CS, fpCS);
    fclose(fpCS);

    FILE *fpECE = fopen("ECE.csv", "r");
    if (fpECE == NULL) {
        printf("Error opening ECE.csv\n");
        return 1;
    }
    int resultECE = formTree(&ECE, fpECE);
    fclose(fpECE);

    FILE *fpEEE = fopen("EEE.csv", "r");
    if (fpEEE == NULL) {
        printf("Error opening EEE.csv\n");
        return 1;
    }
    int resultEEE = formTree(&EEE, fpEEE);
    fclose(fpEEE);

    if (resultCS != 0) {
        printf("Error in CS.csv: ");
        if (resultCS == -1)
            printf("Memory allocation failed while creating node.\n");
        else if (resultCS == -2)
            printf("Error reading from file.\n");
        else if (resultCS == -3)
            printf("Unexpected data format in file.\n");
        return 1;
    }

    if (resultECE != 0) {
        printf("Error in ECE.csv: ");
        if (resultECE == -1)
            printf("Memory allocation failed while creating node.\n");
        else if (resultECE == -2)
            printf("Error reading from file.\n");
        else if (resultECE == -3)
            printf("Unexpected data format in file.\n");
        return 1;
    }

    if (resultEEE != 0) {
        printf("Error in EEE.csv: ");
        if (resultEEE == -1)
            printf("Memory allocation failed while creating node.\n");
        else if (resultEEE == -2)
            printf("Error reading from file.\n");
        else if (resultEEE == -3)
            printf("Unexpected data format in file.\n");
        return 1;
    }

    printf("Welcome To Library\n");

    STUDENT_LIST customer_list; 
    initCustomer(&customer_list);
    int studentOrAdmin;

    do {
        printf("\n1.Student 2.Admin\n");
        scanf("%d", &studentOrAdmin);
        char name[30], SRN[14], book[LEN], operation;
        int action, queueAction, id, subject, flag = 1;
        STUDENT *ptr;

        switch (studentOrAdmin) {
            case 1: 
                printf("\nEnter Name: "); 
                getchar(); 
                fgets(name, sizeof(name), stdin);
                printf("\nEnter SRN: "); 
                scanf("%s", SRN);

                if (!searchCustomer(&customer_list, SRN)) {
                    addCustomer(&customer_list, name, SRN);
                    ptr = customer_list.head;
                } else {
                    ptr = searchCustomer(&customer_list, SRN);
                }
                displayStudentDetails(ptr);

                printf("\n1.Borrow 2.Return 3.Request\t"); 
                scanf("%d", &action);
                do {
                    switch (action) {
                        //////////////////////////////////////////// BORROW //////////////////////////////////////////////////////////////////////////////////////////////
                        case 1: 
                            if (!isFullBooks(&ptr->book_list)) {
                                if (!isEmptyRequests(&(ptr->requests))) { 
                                    // check if request in null
                                    displayRequests(&ptr->requests);
                                    printf("Do you want to borrow from requested books? Y/N\t");
                                    scanf(" %c", &operation);
                                    if (operation == 'Y' || operation == 'y') {
                                        printf("\nEnter id of the book to be borrowed from requests\t");
                                        displayRequests(&ptr->requests);
                                        scanf("%d", &id);
                                        pushFromRequests(ptr, id, subject, subjects);
                                        displayBorrowedBooks(&ptr->book_list);
                                    } else {
                                        printf("Choose the subject you want\n1.CS 2.ECE 3.EEE\t"); 
                                        scanf("%d", &subject);
                                        switch(subject) {
                                            case 1: 
                                                displayTree(&CS); 
                                                break;
                                            case 2: 
                                                displayTree(&ECE); 
                                                break;
                                            case 3: 
                                                displayTree(&EEE); 
                                                break;
                                            default: 
                                                printf("Enter correct id\n"); 
                                                break;
                                        }
                                        printf("Enter the id of the book to be borrowed\t"); 
                                        scanf("%d", &id);
                                        if (insertBookToList(&(ptr->book_list), id, subject, subjects))
                                            printf("Book borrowed successfully\n");
                                        else
                                            printf("Book was NOT borrowed\n");
                                        displayBorrowedBooks(&ptr->book_list);
                                    }
                                } else {
                                    printf("Choose the subject you want\n1.CS 2.ECE 3.EEE\t"); 
                                    scanf("%d", &subject);
                                    switch(subject) {
                                        case 1: 
                                            displayTree(&CS); 
                                            break;
                                        case 2: 
                                            displayTree(&ECE); 
                                            break;
                                        case 3: 
                                            displayTree(&EEE); 
                                            break;
                                        default: 
                                            printf("Enter correct id\n"); 
                                            break;
                                    }
                                    printf("Enter the book id to be borrowed\t"); 
                                    scanf("%d", &id);
                                    if (insertBookToList(&(ptr->book_list), id, subject, subjects))
                                        printf("Book borrowed successfully\n");
                                    else
                                        printf("Book was NOT borrowed\n");
                                    displayBorrowedBooks(&ptr->book_list);
                                }
                            } else
                                printf("You have borrowed max\n");
                            break;
///////////////////////////////////////////////////////// RETURN ////////////////////////////////////////////////////////////////////////////////////////////////////////
                        case 2: 
                            if (isEmptyBooks(&ptr->book_list))  
                                printf("You have no books to return\n");
                            else {
                                displayBorrowedBooks(&ptr->book_list);
                                printf("Enter the id of the book to be returned\t");
                                scanf("%d", &id);
                                if (deleteBookFromList(&ptr->book_list, id))
                                    printf("Book returned!\n");
                                else
                                    printf("Couldn't remove book or wrong id provided\n");
                            }
                            break;
///////////////////////////////////////////////////////// REQUEST //////////////////////////////////////////////////////////////////////////////////////////                            
                        case 3: 
                            displayRequests(&ptr->requests);
                            printf("1.Make request 2.Delete a request\t\t"); 
                            scanf("%d", &queueAction);
                            switch(queueAction) {
                                case 1: 
                                    if (!isFullRequests(&ptr->requests)) {
                                        printf("Choose the subject you want\n1.CS 2.ECE 3.EEE\t"); 
                                        scanf("%d", &subject);
                                        switch(subject) {
                                            case 1: 
                                                displayTree(&CS); 
                                                break;
                                            case 2: 
                                                displayTree(&ECE); 
                                                break;
                                            case 3: 
                                                displayTree(&EEE); 
                                                break;
                                            default: 
                                                printf("Enter correct id\n"); 
                                                break;
                                        }
                                        printf("Enter the book id to be requested\n"); 
                                        scanf("%d", &id);
                                        printf("\n");
                                        if (enqueueRequest(&ptr->requests, id, subject, subjects))
                                            printf("Book added to requests\n");
                                        else
                                            printf("Book NOT added to requests");
                                        displayRequests(&ptr->requests);
                                    } else
                                        printf("Requests are full.");
                                    break;
                                case 2: 
                                    if (!isEmptyRequests(&ptr->requests)) {
                                        printf("Enter id of the book to be removed\t"); 
                                        scanf("%d", &id);
                                        if (dequeueRequest(&ptr->requests , id))
                                            printf("Dequeued sucessfully\n");
                                        else
                                            printf("Couldn't dequeue");
                                    } else
                                        printf("You have no requests\n");
                                    break;
                                default: 
                                    break;
                            }
                            break;
                        default: 
                            break;
                    }
                    printf("1.Borrow 2.Return 3.Request 4.Previosu Info\t");
                    scanf("%d", &action);
                } while (action < 4);
                break;
////////////////////////////////////////////////////// ADMIN ////////////////////////////////////////////////////////////////////////////////////////////////
            case 2: 
                printf("1.Display students 2.Delete Student Info\n"); 
                scanf("%d", &action);
                switch(action) {
                    case 1: 
                        displayStudentList(&customer_list);
                        break;
                    case 2: 
                        printf("Enter SRN to be removed\n"); 
                        scanf("%s", SRN);
                        if (removeCustomer(&customer_list, SRN))
                            printf("SRN: %s removed from student list\n", SRN);
                        else
                            printf("Student with SRN: %s was not found in the list\n", SRN);
                        break;
                    default: 
                        break;
                }
                break;
            default: 
                break;
        }
        free(ptr);
        ptr = NULL;
    } while(studentOrAdmin < 3);

    freeTree(&CS); 
    freeTree(&ECE); 
    freeTree(&EEE);
    freeCustomerList(&customer_list);
    return 0;
}

int formTree(TREE *tree, FILE *fp) {
    int row = 0, column, ID;
    char buffer[2*LEN], idstr[4], bookName[LEN];
    while (fgets(buffer, sizeof(buffer), fp)) {
        column = 0;
        row++;
        const char* tok = strtok(buffer, ",");
        while (tok) {
            if (column == 0) {
                strcpy(idstr, tok);
                tok = strtok(NULL, ",");
                column++;
            }
            if (column == 1) {
                strcpy(bookName, tok);
                tok = strtok(NULL, ",");
            }
            ID = atoi(idstr);
            TREE_NODE *newNode = createNode(ID, bookName);
            if (newNode == NULL) {
                return -1;
            }
            insertNode(tree, newNode);
            tok = strtok(NULL, ",");
        }
    }
    if (ferror(fp)) {
        return -2;
    }
    if (!feof(fp)) {
        return -3;
    }
    return 0;
}
