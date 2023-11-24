#include "defs.h"

void initEvidenceList(EvidenceList* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void printEvidenceList(EvidenceList* list) {
    if (list->size == 0) {
        printf("No evidence has been found.\n");
        return;
    }
    EvidenceNode* curr = list->head;
    while(curr != NULL) {
        printf("%d\n", curr->data);
        curr = curr->next;
    }
}