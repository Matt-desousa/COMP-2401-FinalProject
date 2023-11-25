#include "defs.h"

// // function to initialize hunter
// void initHunter(char* name, EvidenceType evidenceType, RoomType* startingRoom, EvidenceNode* sharedEvidenceList) {
//     HunterType* newHunter = (HunterType*)malloc(sizeof(HunterType)); // allocate memory for the new hunter

//     // maybe here we can have some if statements to take care of cases when some parameters might be NULL but we can save that for the end when we have the basic stuff done

//     // initial values for the hunter
//     strncpy(newHunter->name, name, MAX_STR - 1);  // copy the name??
//     newHunter->evidence_type = evidenceType;
//     newHunter->curr_room = startingRoom; // placeholder because the startingRoom for a hunter should be the van
//     newHunter->evidence_list = sharedEvidenceList;
//     newHunter->fear = 0; // fear when they start hunting is initially 0
//     newHunter->boredom = 0; // boredom when they start hunting is initially 0

//     // log the initialization
//     l_hunterInit(newHunter->name, newHunter->evidence_type);
// }

void initHunter(RoomType* startingRoom, EvidenceType evidenceType, EvidenceList* sharedEvidenceList, HunterType* newHunter) {
    
    printf("Enter hunter%d's name: ", evidenceType+1);
    fgets(newHunter->name, MAX_STR, stdin);
    newHunter->name[strlen(newHunter->name) - 1] = 0;
    
    newHunter->curr_room = startingRoom;
    newHunter->evidence_type = evidenceType;
    newHunter->evidence_list = sharedEvidenceList;
    newHunter->fear = 0;
    newHunter->boredom = 0;

    // printf("Hunter %d initialized, welcome %s\n", evidenceType+1, newHunter->name);
}

void printHunter(HunterType* hunter) {
    printf("Hunter %s is in the %s and has equipment %d.\n", hunter->name, hunter->curr_room->name, hunter->evidence_type);
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

// helper function to add hunter to curr room (updating the hunters collection)
void addHunterToRoom(RoomType* room, HunterType* hunter) {
    HunterNode* new_hunter = malloc(sizeof(HunterNode)); // allocate memory for the new hunter 
    new_hunter->data = hunter;
    new_hunter->next = NULL;

    if (room->hunters_in_room->head == NULL) {
        room->hunters_in_room->head = new_hunter;
        room->hunters_in_room->tail = new_hunter;
    } else {
        room->hunters_in_room->tail->next = new_hunter;
        room->hunters_in_room->tail = new_hunter;
    }

    room->hunters_in_room->size++;
}

// helper function to remove hunter from curr room (updating the hunters collection)
void removeHunterFromRoom(RoomType* room, HunterType* hunter) {
    HunterNode* curr = room->hunters_in_room->head; // start (head) for traversing hunters collection
    HunterNode* prev = NULL; // set a previous node since there could be cases where we need to remove hunter nodes that are in the middle of the linked list

    // traverse collection and find hunter to be removed 
    while (curr != NULL && curr->data != hunter) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != NULL) {
        if (prev == NULL) {
            // hunter to remove is at the head of the list
            room->hunters_in_room->head = curr->next;
            if (room->hunters_in_room->head == NULL) {
                room->hunters_in_room->tail = NULL;
            }
        } else {
            // hunter to remove is in the middle or at the end of the list
            prev->next = curr->next;
            if (prev->next == NULL) {
                room->hunters_in_room->tail = prev;
            }
        }

        free(curr); // still not 100% sure if we only free the curr node or we also free the curr node data
        room->hunters_in_room->size--; // decrement size
    }
}
