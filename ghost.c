#include "defs.h"
#include <pthread.h>
#include <semaphore.h>

void initGhost(RoomType* startingRoom, int* active_hunters, GhostType** ghost) {
    *ghost = malloc(sizeof(GhostType)); // allocate memory for the ghost

    (*ghost)->ghost_class = randomGhost(); // set the ghost class

    // initialize the ghost's current room
    (*ghost)->curr_room = startingRoom;
    (*ghost)->curr_room->ghost_in_room = *ghost;

    // initialize the ghost's evidence list
    initGhostEvidenceList(&(*ghost)->evidence_list, (*ghost)->ghost_class);
    
    // initialize the ghost's boredom
    (*ghost)->boredom = 0;

    // initialize the ghost's active hunters
    (*ghost)->active_hunters = active_hunters;

    // logging ghost initialization
    l_ghostInit((*ghost)->ghost_class, (*ghost)->curr_room->name);
}

void *ghostHandler(void* arg) {
    GhostType* ghost = (GhostType*) arg; // cast the void pointer to a ghost pointer

    while (ghost->boredom < BOREDOM_MAX && *ghost->active_hunters > 0) { // while the ghost is not bored or there are still active hunters
        if (ghost->curr_room->num_hunters > 0) { // if the ghost is in the same room as a hunter reset boredom
            ghost->boredom = 0;
        }
        else{ // otherwise increase boredom
            ghost->boredom++;
        }
        
        // randomly choose an action
        int choice = randInt(0, 3);
        switch (choice) {
            case 0:
                ghostMove(ghost);
                break;
            case 1:
                leaveEvidence(ghost);
                break;
            case 2:
                // Do nothing.
                l_ghostDoNothing();
                break;
        }

        sleep(1); // sleep for 1 second
    }
    
    // logging ghost exit
    l_ghostExit(LOG_BORED);

    return NULL;
}

void ghostMove(GhostType* ghost) {
    // get a random room that is not the current room
    RoomType* new_room = getRandomRoom(ghost->curr_room->connected_rooms, 0);

    // if the new room has a hunter in it then do nothing
    if (new_room->num_hunters > 0) {
        // Do nothing.
        l_ghostDoNothing();
        return;
    }

    // otherwise move the ghost to the new room
    RoomType* old_room = ghost->curr_room;

    // lock the old and new rooms
    sem_wait(&old_room->mutex);
    sem_wait(&new_room->mutex);

    // update the ghost's current room
    ghost->curr_room->ghost_in_room = NULL;
    ghost->curr_room = new_room;
    ghost->curr_room->ghost_in_room = ghost;

    // unlock the old and new rooms
    sem_post(&old_room->mutex);
    sem_post(&new_room->mutex);

    // logging ghost movement
    l_ghostMove(ghost->curr_room->name, 1);
}

void leaveEvidence(GhostType* ghost) {
    // get a random evidence type
    EvidenceType new_evidence = getRandomEvidence(&(ghost->evidence_list));

    // lock the current room
    sem_wait(&ghost->curr_room->evidence_in_room->mutex);

    // check if the evidence is already in the room
    EvidenceNode* curr_node = ghost->curr_room->evidence_in_room->head;
    while(curr_node != NULL) {
        if (curr_node->data == new_evidence) { // if the evidence is already in the room then do nothing.
            sem_post(&ghost->curr_room->evidence_in_room->mutex); // unlock the current room
            l_ghostDoNothing(); // logging ghost doing nothing
            return;
        }
        curr_node = curr_node->next;   
    }

    // add the evidence to the room
    EvidenceNode* new_evidence_node = malloc(sizeof(EvidenceNode));
    new_evidence_node->data = new_evidence;
    new_evidence_node->next = NULL;

    if (ghost->curr_room->evidence_in_room->head == NULL) { // if the list is empty
        ghost->curr_room->evidence_in_room->head = new_evidence_node;
        ghost->curr_room->evidence_in_room->tail = new_evidence_node;
    } else { // otherwise add the evidence to the end of the list
        ghost->curr_room->evidence_in_room->tail->next = new_evidence_node;
        ghost->curr_room->evidence_in_room->tail = new_evidence_node;
    }

    // increment the size of the list
    ghost->curr_room->evidence_in_room->size++;

    // unlock the current room
    sem_post(&ghost->curr_room->evidence_in_room->mutex);

    // printEvidenceList(ghost->curr_room->evidence_in_room);
    l_ghostEvidence(new_evidence_node->data, ghost->curr_room->name);
}

void cleanupGhost(GhostType* ghost) {
    cleanupEvidenceList(&(ghost->evidence_list)); // cleanup the ghost's evidence list
    free(ghost); // free the memory of the ghost
}
