#include "defs.h"

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

void ghostHandler(GhostType* ghost) {
    while (ghost->boredom < BOREDOM_MAX) {
        if (ghost->curr_room->num_hunters > 0) {
            ghost->boredom = 0;
        }
        else{
            ghost->boredom++;
        }
        
        int choice = randInt(0, 2);
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
    }
    l_ghostExit(LOG_BORED);
}

void ghostMove(GhostType* ghost) {
    RoomList* connected = ghost->curr_room->connected_rooms;

    int rand_index = randInt(0, connected->size);
    RoomNode* curr_node = connected->head;

    for (int i = 0; i < rand_index; i++) {
        curr_node = curr_node->next;
    }

    if (curr_node->data->num_hunters > 0) {
       // Do nothing.
       return;
    }

    ghost->curr_room->ghost_in_room = NULL;
    ghost->curr_room = curr_node->data;
    ghost->curr_room->ghost_in_room = ghost;

    l_ghostMove(ghost->curr_room->name);
}

void leaveEvidence(GhostType* ghost) {
    EvidenceNode* new_evidence = malloc(sizeof(EvidenceNode));
    new_evidence->data = getRandomEvidence(&(ghost->evidence_list));
    new_evidence->next = NULL;

    if (ghost->curr_room->evidence_in_room->head == NULL) {
        ghost->curr_room->evidence_in_room->head = new_evidence;
        ghost->curr_room->evidence_in_room->tail = new_evidence;
    } else {
        ghost->curr_room->evidence_in_room->tail->next = new_evidence;
        ghost->curr_room->evidence_in_room->tail = new_evidence;
    }

    ghost->curr_room->evidence_in_room->size++;

    l_ghostEvidence(new_evidence->data, ghost->curr_room->name);
}

void cleanupGhost(GhostType* ghost) {
    cleanupEvidenceList(&(ghost->evidence_list));
    free(ghost);
}