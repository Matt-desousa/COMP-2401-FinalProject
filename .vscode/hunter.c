#include "defs.h"

void initHunter(RoomType* startingRoom, EvidenceType evidenceType, EvidenceList* sharedEvidenceList, HunterType* newHunter) {
    newHunter = malloc(sizeof(HunterType));

    printf("Enter hunter%d's name: ", evidenceType+1);
    fgets(newHunter->name, MAX_STR, stdin);
    newHunter->name[strlen(newHunter->name) - 1] = 0;
    
    newHunter->curr_room = startingRoom;
    newHunter->evidence_type = evidenceType;
    newHunter->evidence_list = sharedEvidenceList;

    newHunter->fear = 0;
    newHunter->boredom = 0;

    l_hunterInit(newHunter->name, newHunter->evidence_type);
}

void hunterHandler(HunterType* hunter){
    while (hunter->boredom < BOREDOM_MAX || hunter->fear < FEAR_MAX) {
        if (hunter->curr_room->ghost_in_room != NULL) {
            hunter->fear++;
            hunter->boredom = 0;
        }
        else{
            hunter->boredom++;
        }
        
        int choice = randInt(0, 2);
        switch (choice) {
            case 0:
                hunterMove(hunter, hunter->curr_room);
                break;
            case 1:
                hunterCollect(hunter, hunter->evidence_type);
                break;
            case 2:
                printf("hunter review\n"); // Placeholder
                // hunterReview(hunter);
                break;
        }
    }

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
    int rand_index = randInt(0, connected->size); // from utils.c
    RoomNode* curr_node = connected->head; // first node in the room connections linked list (start from head)

    // traverse room connections list until you reach random index (the random room)
    for (int i = 0; i < rand_index; i++) {
        curr_node = curr_node->next;
    }

    // update the hunter's current room
    hunter->curr_room = curr_node->data;

    // update the hunter collection in the curr room
    removeHunterFromRoom(current_room, hunter);
    addHunterToRoom(hunter->curr_room, hunter);

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
                EvidenceNode* newEvidenceNode = malloc(sizeof(EvidenceNode));
                newEvidenceNode->data = curr_evidence->data;
                newEvidenceNode->next = hunter->evidence_list->head;
                hunter->evidence_list->head = newEvidenceNode;
                if (hunter->evidence_list->tail == NULL) {
                    // if the list was empty, update the tail
                    hunter->evidence_list->tail = newEvidenceNode;
                }

                hunter->evidence_list->size++; // increment list size

                free(curr_evidence);

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
    l_hunterCollect(hunter->name, EV_UNKNOWN, current_room->name);
}

void cleanupHunter(HunterType* hunter) {
    free(hunter);
}
