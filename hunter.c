#include "defs.h"
#include <pthread.h>
#include <semaphore.h>

void initHunter(RoomType* startingRoom, EvidenceType evidenceType, EvidenceList* sharedEvidenceList, int* active_hunters, sem_t* mutex, HunterType* newHunter) {
    // set the color of the hunter
    sprintf(newHunter->color, "\033[3%dm", evidenceType+1);

    // get the name of the hunter
    printf("%sEnter hunter%d's name: ", newHunter->color, evidenceType+1);
    fgets((newHunter)->name, MAX_STR, stdin);
    newHunter->name[strlen(newHunter->name) - 1] = 0;
    
    // initializing the hunter based on parameters
    newHunter->curr_room = startingRoom;
    newHunter->evidence_type = evidenceType;
    newHunter->evidence_list = sharedEvidenceList;

    // initialize the hunter's boredom and fear
    newHunter->fear = 0;
    newHunter->boredom = 0;

    // initialize the hunter's active hunters and mutex
    newHunter->active_hunters = active_hunters;
    *newHunter->active_hunters += 1;
    newHunter->active_hunters_mutex = mutex;

    // logging hunter initialization
    l_hunterInit(newHunter->name, newHunter->evidence_type, newHunter->color);
}

void *hunterHandler(void* arg){
    HunterType* hunter = (HunterType*) arg; // cast the void pointer to a hunter pointer

    int sufficient = 0; // flag to check if the hunter has collected sufficient evidence
    while (hunter->boredom < BOREDOM_MAX && hunter->fear < FEAR_MAX) { // while the hunter is not bored or scared
        if (hunter->curr_room->ghost_in_room != NULL) { // if the hunter is in the same room as the ghost increase fear and reset boredom
            hunter->fear++;
            hunter->boredom = 0;
        }
        else{ // otherwise increase boredom
            hunter->boredom++;
        }
        
        // randomly choose an action
        int choice = randInt(0, 3);
        switch (choice) {
            case 0:
                hunterMove(hunter); // move the hunter
                break;
            case 1:
                hunterCollect(hunter); // collect evidence
                break;
            case 2:
                sufficient = hunterReview(hunter); // review evidence
                break;
        }
        
        if (sufficient == SUFFICIENT) { // if the hunter has collected sufficient evidence then exit
            break;
        }

        sleep(1); // sleep for 1 second
    }

    hunterExit(hunter); // exit the hunter
    
    return NULL;
}

void hunterMove(HunterType* hunter) {
    // get the hunter's current room
    RoomType* current_room = hunter->curr_room;

    // randomly select a connected room
    RoomType* new_room = getRandomRoom(current_room->connected_rooms, 0);

    // update the hunter's current room
    hunter->curr_room = new_room;

    // lock the current room and the new room
    sem_wait(&current_room->mutex);
    sem_wait(&new_room->mutex);

    // update the hunter collection in the curr room
    removeHunterFromRoom(current_room, hunter);
    addHunterToRoom(new_room, hunter);

    // unlock the current room and the new room
    sem_post(&current_room->mutex);
    sem_post(&new_room->mutex);

    // logging hunter movement
    l_hunterMove(hunter->name, hunter->curr_room->name, hunter->color);
}

void addHunterToRoom(RoomType* room, HunterType* hunter) {
    // find an empty slot in the array and add the hunter
    for (int i = 0; i < NUM_HUNTERS; i++) {
        if (room->hunters_in_room[i] == NULL) { // if the slot is empty
            room->hunters_in_room[i] = hunter;
            room->num_hunters++;
            return;
        }
    }
}

void removeHunterFromRoom(RoomType* room, HunterType* hunter) {
    // find the hunter in the array and clear the slot
    for (int i = 0; i < NUM_HUNTERS; i++) {
        if (room->hunters_in_room[i] == hunter) {
            room->hunters_in_room[i] = NULL; // clear the slot
            room->num_hunters--;
            break;
        }
    }
}

void hunterCollect(HunterType* hunter) {
    // get the hunter's detection type
    EvidenceType detectionType = hunter->evidence_type;

    // get the hunter's current room
    RoomType* current_room = hunter->curr_room;

    // get the head of the evidence list
    EvidenceNode* curr_evidence = current_room->evidence_in_room->head;
    EvidenceNode* prev_evidence = NULL;

    // lock the current room and the shared evidence list
    sem_wait(&current_room->evidence_in_room->mutex);
    sem_wait(&hunter->evidence_list->mutex);

    // iterate through the evidence in the room
    while (curr_evidence != NULL) {
        if (curr_evidence->data == detectionType) { // if the evidence type matches the hunter's detection type
            if (prev_evidence == NULL) { // if the evidence to be removed is at the head of the list
                current_room->evidence_in_room->head = curr_evidence->next; // update the head
                if (current_room->evidence_in_room->head == NULL) { // if the removed evidence was the only evidence in the room
                    current_room->evidence_in_room->tail = NULL; // update the tail
                }
            } else {
                prev_evidence->next = curr_evidence->next; // update the previous evidence node
                if (prev_evidence->next == NULL) { // if the removed evidence was the tail of the list
                    current_room->evidence_in_room->tail = prev_evidence; // update the tail
                }
            }

            addEvidence(hunter->evidence_list, curr_evidence->data); // add the evidence to the shared evidence collection for all hunters

            free(curr_evidence); // free the memory of the removed evidence node

            // logging hunter collecting evidence
            l_hunterCollect(hunter->name, detectionType, current_room->name, hunter->color);

            break; // break out of the loop
        }

        // move to the next evidence node
        prev_evidence = curr_evidence;
        curr_evidence = curr_evidence->next;
    }

    // unlock the current room and the shared evidence list
    sem_post(&current_room->evidence_in_room->mutex);
    sem_post(&hunter->evidence_list->mutex);
}

int hunterReview(HunterType* hunter) {
    sem_wait(&hunter->evidence_list->mutex); // lock the shared evidence list

    // get the head of the shared evidence list
    EvidenceList* sharedEvidenceList = hunter->evidence_list;

    // create an array to store the unique evidence types
    EvidenceType uniqueEvidenceTypes[3];
    int uniqueEvidenceCount = 0;

    // iterate through shared evidence list
    EvidenceNode* currentEvidence = sharedEvidenceList->head;
    while (currentEvidence != NULL && uniqueEvidenceCount < 3) {
        // check if the evidence type already exists in the array if it does then set the flag to true
        int alreadyExists = C_FALSE;
        for (int i = 0; i < uniqueEvidenceCount; i++) { 
            if (uniqueEvidenceTypes[i] == currentEvidence->data) {
                alreadyExists = C_TRUE;
                break;
            }
        }

        // if not then add it to the array
        if (!alreadyExists) {
            uniqueEvidenceTypes[uniqueEvidenceCount++] = currentEvidence->data;
        }

        currentEvidence = currentEvidence->next;
    }
    
    sem_post(&hunter->evidence_list->mutex); // unlock the shared evidence list

    // check if there are at least three unique evidence types and log the events.
    if (uniqueEvidenceCount >= 3) {
        l_hunterReview(hunter->name, LOG_SUFFICIENT, hunter->color);
        return SUFFICIENT;
    } else {
        l_hunterReview(hunter->name, LOG_INSUFFICIENT, hunter->color);
        return INSUFFICIENT;
    }
}

void hunterExit(HunterType* hunter) {
    removeHunterFromRoom(hunter->curr_room, hunter); // remove the hunter from the room

    // logging hunter exiting
    if (hunter->fear >= FEAR_MAX) {
        l_hunterExit(hunter->name, LOG_FEAR, hunter->color);
    } else if (hunter->boredom >= BOREDOM_MAX) {
        l_hunterExit(hunter->name, LOG_BORED, hunter->color);
    } else {
        l_hunterExit(hunter->name, LOG_EVIDENCE, hunter->color);
    }

    // decrement the number of active hunters
    // sem_wait(hunter->active_hunters_mutex);
    (*hunter->active_hunters)--;
    // sem_post(hunter->active_hunters_mutex);
}
