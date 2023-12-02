#include "defs.h"

void initEvidenceList(EvidenceList* list) {
    // Initialize the head and tail
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    // initialize the mutex
    sem_init(&list->mutex, 0, 1); 
}

EvidenceNode* initEvidenceNode(EvidenceType data) {
    // allocate memory for the new node
    EvidenceNode* newEvidenceNode = malloc(sizeof(EvidenceNode)); 
    newEvidenceNode->data = data;

    // return the new node
    return newEvidenceNode;
}

void addEvidence(EvidenceList* list, EvidenceType data) {
    // initialize the new node
    EvidenceNode* node = initEvidenceNode(data); 

    // add the node to the head of the list
    node->next = list->head;
    list->head = node;

    // update the tail if the list was empty
    if (list->tail == NULL) {
        list->tail = node;
    }

    // increment the size of the list
    list->size++;
}

void initGhostEvidenceList(EvidenceList* evidence, GhostClass ghost) {
    // initialize the list
    initEvidenceList(evidence);

    // add the evidence based on the ghost type
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

EvidenceType getRandomEvidence(EvidenceList* evidence_list) {
    // get a random index
    int rand_index = randInt(0, evidence_list->size);
    
    // iterate to the random index
    EvidenceNode* curr_node = evidence_list->head;
    for (int i = 0; i < rand_index; i++) {
        curr_node = curr_node->next;
    }

    // return the evidence type
    return curr_node->data;
}

void cleanupEvidenceList(EvidenceList* list) {
    // iterate through the list and free the nodes
    EvidenceNode* curr = list->head;
    EvidenceNode* next = NULL;
    while(curr != NULL) {
        next = curr->next;
        free(curr); // free the node
        curr = next;
    }
}
