#include "defs.h"

void initEvidenceList(EvidenceList* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    sem_init(&list->mutex, 0, 1);
}

void initEvidenceNode(EvidenceType data, HunterType* hunter) {
    EvidenceNode* newEvidenceNode = malloc(sizeof(EvidenceNode));
    newEvidenceNode->data = data;
    newEvidenceNode->next = hunter->evidence_list->head;
    hunter->evidence_list->head = newEvidenceNode;

    if (hunter->evidence_list->tail == NULL) {
        // if the list was empty, update the tail
        hunter->evidence_list->tail = newEvidenceNode;
    }

    hunter->evidence_list->size++; // increment list size
}

void initGhostEvidenceList(EvidenceList* evidence, GhostClass ghost) {
    initEvidenceList(evidence);

    switch(ghost) {
        case POLTERGEIST:
            addEvidence(evidence, EMF);
            addEvidence(evidence, TEMPERATURE);
            addEvidence(evidence, FINGERPRINTS);
            break;
        case BANSHEE:
            addEvidence(evidence, EMF);
            addEvidence(evidence, TEMPERATURE);
            addEvidence(evidence, SOUND);
            break;
        case BULLIES:
            addEvidence(evidence, EMF);
            addEvidence(evidence, FINGERPRINTS);
            addEvidence(evidence, SOUND);
            break;
        case PHANTOM:
            addEvidence(evidence, TEMPERATURE);
            addEvidence(evidence, FINGERPRINTS);
            addEvidence(evidence, SOUND);
            break;
        default:
            
            break;
    }
}

void addEvidence(EvidenceList* list, EvidenceType evidence) {
    EvidenceNode* node = malloc(sizeof(EvidenceNode));
    node->data = evidence;
    node->next = NULL;

    if (list->size == 0) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;
}

EvidenceType getRandomEvidence(EvidenceList* evidence_list) {
    int rand_index = randInt(0, evidence_list->size);
    EvidenceNode* curr_node = evidence_list->head;

    for (int i = 0; i < rand_index; i++) {
        curr_node = curr_node->next;
    }

    return curr_node->data;
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

void cleanupEvidenceList(EvidenceList* list) {
    EvidenceNode* curr = list->head;
    EvidenceNode* next = NULL;
    while(curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
}
