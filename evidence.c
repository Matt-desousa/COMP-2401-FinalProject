#include "defs.h"

void initEvidenceList(EvidenceList* list) {
    // Initialize the head and tail
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    sem_init(&list->mutex, 0, 1); // initialize the mutex
}

EvidenceNode* initEvidenceNode(EvidenceType data) {
    EvidenceNode* newEvidenceNode = malloc(sizeof(EvidenceNode)); // allocate memory for the new node
    newEvidenceNode->data = data; // set the data
}

void addEvidence(EvidenceList* list, EvidenceType data) {
    EvidenceNode* node = initEvidenceNode(data); // initialize the new node

    node->next = list->head; // set the next pointer to the current head
    list->head = node; // set the head to the new node

    if (list->tail == NULL) { // if the list was empty, update the tail
        list->tail = node;
    }

    list->size++; // increment list size
}

void initGhostEvidenceList(EvidenceList* evidence, GhostClass ghost) {
    initEvidenceList(evidence); // initialize the list

    switch(ghost) { // add the evidence based on the ghost type
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

EvidenceType getRandomEvidence(EvidenceList* evidence_list) {
    int rand_index = randInt(0, evidence_list->size); // get a random index
    
    EvidenceNode* curr_node = evidence_list->head;
    for (int i = 0; i < rand_index; i++) { // iterate to the random index
        curr_node = curr_node->next;
    }

    return curr_node->data; // return the data at the random index
}

void cleanupEvidenceList(EvidenceList* list) {
    EvidenceNode* curr = list->head;
    EvidenceNode* next = NULL;
    while(curr != NULL) { // iterate through the list and free each node
        next = curr->next;
        free(curr);
        curr = next;
    }
}
