#include "defs.h"
#include <pthread.h>
#include <semaphore.h>

/*
    function for hunter initializing a hunter
    in: starting room
    in: evidence type that theyre assigned to collect
    in: shared evidence list
    in: the hunter themself 
*/
void initHunter(RoomType* startingRoom, EvidenceType evidenceType, EvidenceList* sharedEvidenceList, HunterType* newHunter) {
    sprintf(newHunter->color, "\033[3%dm", evidenceType+1);

    printf("%sEnter hunter%d's name: ", newHunter->color, evidenceType+1);
    fgets((newHunter)->name, MAX_STR, stdin);
    newHunter->name[strlen(newHunter->name) - 1] = 0;
    
    newHunter->curr_room = startingRoom;
    newHunter->evidence_type = evidenceType;
    newHunter->evidence_list = sharedEvidenceList;

    newHunter->fear = 0;
    newHunter->boredom = 0;

    l_hunterInit(newHunter->name, newHunter->evidence_type);
}

/*
    function for handling the actions of a hunter
    in: hunter thats either moving, collecting, or reviewing
*/
void *hunterHandler(void* arg){
    HunterType* hunter = (HunterType*) arg;
    while (hunter->boredom < BOREDOM_MAX && hunter->fear < FEAR_MAX) {
        sem_post(&mutex);

        if (hunter->curr_room->ghost_in_room != NULL) {
            hunter->fear++;
            hunter->boredom = 0;
        }
        else{
            hunter->boredom++;
        }
        
        int choice = randInt(0, 3); // not inclusive so max is 3
        switch (choice) {
            case 0:
                hunterMove(hunter, hunter->curr_room);
                break;
            case 1:
                hunterCollect(hunter, hunter->evidence_type);
                break;
            case 2:
                hunterReview(hunter);
                break;
        }
        sleep(1);
        sem_wait(&mutex);
    }
    hunterExit(hunter);
}

/*
    function for hunter movement 
    in: hunter thats moving
    in: current room (room data)
*/
void hunterMove(HunterType* hunter, RoomType* current_room) {
    RoomList* connected = current_room->connected_rooms; // get list of connected rooms

    // add a semaphore (since there are multiple hunters that will be using this function) here but i still havent figured that out...
    
    // if there are no connected rooms, the hunter stays in the curr room
    if (connected->size == 0) {
        l_hunterMove(hunter->name, current_room->name);
        return;
    }

    // randomly select a connected room
    RoomType* new_room = getRandomRoom(connected, 0);

    // update the hunter's current room
    hunter->curr_room = new_room;

    // update the hunter collection in the curr room
    removeHunterFromRoom(current_room, hunter);
    addHunterToRoom(hunter->curr_room, hunter);

    printf("%s", hunter->color);
    // logging hunter movement
    l_hunterMove(hunter->name, hunter->curr_room->name);
}

// helper function to add hunter (for hunterMove function)
void addHunterToRoom(RoomType* room, HunterType* hunter) {
    // find an empty slot in the array and add the hunter
    for (int i = 0; i < NUM_HUNTERS; i++) {
        if (room->hunters_in_room[i].curr_room == NULL) {
            room->hunters_in_room[i] = *hunter;
            room->num_hunters++;
            return;
        }
    }
}

// helper function to remove hunter (for hunterMove function)
void removeHunterFromRoom(RoomType* room, HunterType* hunter) {
    // find the hunter in the array and clear the slot
    for (int i = 0; i < NUM_HUNTERS; i++) {
        if (&room->hunters_in_room[i] == hunter) {
            room->hunters_in_room[i].curr_room = NULL;
            room->num_hunters--;
            return;
        }
    }
}

/*
    function for hunter collecting evidence
    in: hunter thats collecting evidence
    in: evidence type that hunter is assigned to collect
*/
void hunterCollect(HunterType* hunter, EvidenceType detectionType) {
    RoomType* current_room = hunter->curr_room;

    // check if the current room has evidence
    if (current_room->evidence_in_room->size > 0) {
        EvidenceNode* curr_evidence = current_room->evidence_in_room->head;
        EvidenceNode* prev_evidence = NULL;

        while (curr_evidence != NULL) {
            // check if the evidence matches the hunter's detection type
            if (curr_evidence->data == detectionType) {
                // remove the evidence from the room's evidence collection
                if (prev_evidence == NULL) {
                    // the evidence to be removed is at the head of the list
                    current_room->evidence_in_room->head = curr_evidence->next;
                    if (current_room->evidence_in_room->head == NULL) {
                        // if the list is now empty then update the tail
                        current_room->evidence_in_room->tail = NULL;
                    }
                } else {
                    // the evidence to be removed is in the middle or at the end of the list
                    prev_evidence->next = curr_evidence->next;
                    if (prev_evidence->next == NULL) {
                        // if the removed evidence was at the tail then update the tail
                        current_room->evidence_in_room->tail = prev_evidence;
                    }
                }

                // add the evidence to the shared evidence collection for all hunters
                initEvidenceNode(curr_evidence->data, hunter);

                free(curr_evidence);

                printf("%s", hunter->color);
                // log the event...
                l_hunterCollect(hunter->name, detectionType, current_room->name);

                return;
            }

            // move to the next evidence node
            prev_evidence = curr_evidence;
            curr_evidence = curr_evidence->next;
        }
    }

    // log the event even if no evidence is found
    // l_hunterCollect(hunter->name, EV_UNKNOWN, current_room->name);
}

/*
    function for hunter reviewing evidence
    in: hunter thats collecting evidence
*/
void hunterReview(HunterType* hunter) {
    // printEvidenceList(hunter->evidence_list);
    EvidenceList* sharedEvidenceList = hunter->evidence_list;
    EvidenceType uniqueEvidenceTypes[3] = {EV_UNKNOWN, EV_UNKNOWN, EV_UNKNOWN}; // store the unique evidence types here for checking
    int uniqueEvidenceCount = 0;

    // iterate through shared evidence list
    EvidenceNode* currentEvidence = sharedEvidenceList->head;
    while (currentEvidence != NULL && uniqueEvidenceCount < 3) {
        EvidenceType currentType = currentEvidence->data;

        // check if the evidence type is already in the uniqueEvidenceTypes array
        int alreadyExists = C_FALSE;
        for (int i = 0; i < uniqueEvidenceCount; i++) {
            if (uniqueEvidenceTypes[i] == currentType) {
                alreadyExists = C_TRUE;
                break;
            }
        }

        // if not then add it to the array
        if (!alreadyExists) {
            uniqueEvidenceTypes[uniqueEvidenceCount++] = currentType;
        }

        currentEvidence = currentEvidence->next;
    }

    // check if there are at least three unique evidence types
    printf("%s", hunter->color);
    if (uniqueEvidenceCount >= 3) {
        // log the event and exit...
        l_hunterReview(hunter->name, LOG_SUFFICIENT);
        l_hunterExit(hunter->name, LOG_SUFFICIENT);
    } else {
        // log the event...
        l_hunterReview(hunter->name, LOG_INSUFFICIENT);
    }
}

void hunterExit (HunterType* hunter) {
    removeHunterFromRoom(hunter->curr_room, hunter);
    printf("%s", hunter->color);
    if (hunter->fear >= FEAR_MAX) {
        l_hunterExit(hunter->name, LOG_FEAR);
    }
    else if (hunter->boredom >= BOREDOM_MAX) {
        l_hunterExit(hunter->name, LOG_BORED);
    }
    else {
        l_hunterExit(hunter->name, LOG_EVIDENCE);
    }
}