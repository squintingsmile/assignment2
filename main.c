#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "parser.h"

// A dynamic container that can store changeable amout of lines
typedef struct node{
    char line[32];
    struct node* next;
} node;


// This function is used to addnode to the end of the link list
void addNode(node* head, char* line) {

    node* currentNode = head;
    while (currentNode->next != NULL) {
        currentNode = currentNode->next;
    }
    currentNode->next = (node*) malloc(sizeof(node));
    strcpy(currentNode->next->line, line);
    currentNode->next->next = NULL;
}

// This function is used to retrieve the node at specific position
node* accessNode(node* head, int position) {
    node* currentNode = head;
    for (int i = 0; i < position; i++) {
        currentNode = currentNode->next;
    }
    return currentNode;
}

int main() {
    FILE* fp;
    char path[256];
    printf("Please input the relative path of input file \n");
    scanf("%s",path);
    fp = fopen(path, "r");
    node* linesHead = NULL;

    // Storing the whole file in the linked list
    while (1) {
        size_t length = 32;
        char* newline;
        newline = (char *) malloc(32 * sizeof(char));
        if (getline(&newline, &length, fp) == -1) {
            break;
        }
        if (linesHead == NULL) {
            linesHead = (node*) malloc(sizeof(node));
            strcpy(linesHead->line, newline);
            linesHead->next = NULL;
        } else {
            addNode(linesHead, newline);
        }
    }

    while (1) {
        node* currentNode = accessNode(linesHead, programCounter);
        if (currentNode == NULL) {
            break;
        }
        parser(currentNode->line);
        programCounter++;
    }
}