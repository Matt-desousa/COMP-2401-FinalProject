#include "defs.h"
#include <pthread.h>

int main() {
    // Initialize the random number generator
    srand(time(NULL));

    // Initialize the house
    HouseType house;
    initHouse(&house);
    populateRooms(&house, "map.txt");

    // Initialize the ghost
    GhostType* ghost;
    initGhost(getRandomRoom(&(house.rooms), 1), &ghost, &house);

    // Initialize hunters
    for(int i = 0; i < NUM_HUNTERS; i++) {
        initHunter(house.rooms.head->data, i, &house.evidence_list, &house, &house.hunters[i]);
    }

    // Start the game
    pthread_create(&ghost->pid, NULL, ghostHandler, ghost);
    pthread_create(&house.hunters[0].tid, NULL, hunterHandler, &house.hunters[0]);
    pthread_create(&house.hunters[1].tid, NULL, hunterHandler, &house.hunters[1]);
    pthread_create(&house.hunters[2].tid, NULL, hunterHandler, &house.hunters[2]);
    pthread_create(&house.hunters[3].tid, NULL, hunterHandler, &house.hunters[3]);

    // End the game
    pthread_join(ghost->pid, NULL);

    for(int i = 0; i < NUM_HUNTERS; i++) {
        pthread_join(house.hunters[i].tid, NULL);
        
    }

    // Cleanup memory
    cleanupHouse(&house);
    cleanupGhost(ghost);

    return 0;
}
