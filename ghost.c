#include "defs.h"
#include <pthread.h>
#include <semaphore.h>

void initGhost(GhostType** ghost, RoomType* startingRoom) {
    *ghost = malloc(sizeof(GhostType));
    (*ghost)->ghost_class = randomGhost();
    (*ghost)->curr_room = startingRoom;
    (*ghost)->curr_room->ghost_in_room = *ghost;
    initGhostEvidenceList(&(*ghost)->evidence_list, (*ghost)->ghost_class);
    (*ghost)->boredom = 0;
    (*ghost)->pid = 0;

    l_ghostInit((*ghost)->ghost_class, (*ghost)->curr_room->name);
}

void *ghostHandler(void* arg) {
    GhostType* ghost = (GhostType*) arg;
    while (ghost->boredom < BOREDOM_MAX) {
        if (ghost->curr_room->num_hunters > 0) {
            ghost->boredom = 0;
        }
        else{
            ghost->boredom++;
        }
        
        int choice = randInt(0, 1);
        switch (choice) {
            case 0:
                ghostMove(ghost);
                break;
            case 1:
                leaveEvidence(ghost);
                break;
            case 2:
                // Do nothing.
                break;
        }
        sleep(1);
    }
    printf("\033[0m"); 
    l_ghostExit(LOG_BORED);
}

void ghostMove(GhostType* ghost) {

    RoomType* new_room = getRandomRoom(ghost->curr_room->connected_rooms, 0);

    if (new_room->num_hunters > 0) {
       // Do nothing.
       for (int i = 0; i < NUM_HUNTERS; i++) {
        printf("%p ", new_room->hunters_in_room[i]); 
       }
       printf("\n");
       l_ghostMove(new_room->name, 0);
       return;
    }

    RoomType* old_room = ghost->curr_room;

    sem_wait(&old_room->mutex);
    sem_wait(&new_room->mutex);

    ghost->curr_room->ghost_in_room = NULL;
    ghost->curr_room = new_room;
    ghost->curr_room->ghost_in_room = ghost;

    sem_post(&old_room->mutex);
    sem_post(&new_room->mutex);

    printf("\033[0m"); 
    l_ghostMove(ghost->curr_room->name, 1);
}

void leaveEvidence(GhostType* ghost) {
    EvidenceNode* new_evidence = malloc(sizeof(EvidenceNode));
    new_evidence->data = getRandomEvidence(&(ghost->evidence_list));
    new_evidence->next = NULL;

    sem_wait(&ghost->curr_room->evidence_in_room->mutex);

    if (ghost->curr_room->evidence_in_room->head == NULL) {
        ghost->curr_room->evidence_in_room->head = new_evidence;
        ghost->curr_room->evidence_in_room->tail = new_evidence;
    } else {
        ghost->curr_room->evidence_in_room->tail->next = new_evidence;
        ghost->curr_room->evidence_in_room->tail = new_evidence;
    }

    ghost->curr_room->evidence_in_room->size++;

    sem_post(&ghost->curr_room->evidence_in_room->mutex);

    printf("\033[0m");
    // printEvidenceList(ghost->curr_room->evidence_in_room);
    l_ghostEvidence(new_evidence->data, ghost->curr_room->name);
}

void cleanupGhost(GhostType* ghost) {
    cleanupEvidenceList(&(ghost->evidence_list));
    free(ghost);
}