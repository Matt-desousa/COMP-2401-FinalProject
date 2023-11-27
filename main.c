#include "defs.h"
#include <pthread.h>

int main() {
    // printf("%d\n", sizeof(HunterType));
    // Initialize the random number generator
    srand(time(NULL));

    // Initialize the house
    HouseType house;
    initHouse(&house);
    populateRooms(&house);

    // Initialize the ghost
    GhostType* ghost;
    initGhost(&ghost, getRandomRoom(&(house.rooms)));

    // Initialize hunters
    for(int i = 0; i < NUM_HUNTERS; i++) {
        initHunter(house.rooms.head->data, i, &house.evidence_list, &house.hunters[i]);
    }

    // Start the game
    ghostHandler(ghost);
    // pthread_create(ghost->pid, NULL, ghostHandler, ghost);

    for (int i = 0; i < NUM_HUNTERS; i ++) {
        hunterHandler(&house.hunters[i]);
    } 

    // End the game
    // pthread_join(ghost->pid, NULL);

    // Cleanup memory
    cleanupHouse(&house);
    cleanupGhost(ghost);

    // printHouse(&house);
    return 0;
}
