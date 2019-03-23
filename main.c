#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "parser.h"

typedef struct node{
    char line[32];
    struct node* next;
} node;

void addNode(node* head, char* line) {

    node* currentNode = head;
    while (currentNode->next != NULL) {
        currentNode = currentNode->next;
    }
    currentNode->next = (node*) malloc(sizeof(node));
    strcpy(currentNode->next->line, line);
    currentNode->next->next = NULL;
}

node* accessNode(node* head, int position) {
    node* currentNode = head;
    for (int i = 0; i < position; i++) {
        currentNode = currentNode->next;
    }
    return currentNode;
}

int main() {
    FILE* fp;
    fp = fopen("./file.txt", "r");
    node* linesHead = NULL;
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

//    while (currentNode != NULL) {
//        printf("%s \n",currentNode->line);
//        currentNode = currentNode->next;
//    }

    while (1) {
        node* currentNode = accessNode(linesHead, programCounter);
        if (currentNode == NULL) {
            break;
        }
        parser(currentNode->line);
        programCounter++;
    }
}