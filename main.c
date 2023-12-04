#include "defs.h"
#include <pthread.h>
#include <semaphore.h>

int main() {
    // initialize the random number generator
    srand(time(NULL));
    
    // reset the text file because it will be opened again by the logging functions
    FILE* log_file = fopen(LOG_FILE, "w");
    fclose(log_file);
    
    // initialize the house
    HouseType house;
    initHouse(&house);
    populateRooms(&house, "map.txt");

    // initialize the ghost
    GhostType* ghost;
    initGhost(getRandomRoom(&(house.rooms), 1), &(house.active_hunters), &ghost);

    // initialize hunters
    for(int i = 0; i < NUM_HUNTERS; i++) {
        initHunter(house.rooms.head->data, i, &house.evidence_list, &(house.active_hunters), &(house.active_hunters_mutex), &house.hunters[i]);
    }

    // start the game
    pthread_create(&ghost->pid, NULL, ghostHandler, ghost);

    for(int i = 0; i < NUM_HUNTERS; i++) {
        pthread_create(&house.hunters[i].tid, NULL, hunterHandler, &house.hunters[i]);
    }

    pthread_join(ghost->pid, NULL);

    // end the game
    for(int i = 0; i < NUM_HUNTERS; i++) {
        pthread_join(house.hunters[i].tid, NULL);
    }
    
    printResults(house.hunters, ghost);

    // cleanup memory
    cleanupHouse(&house);
    cleanupGhost(ghost);

    return 0;
}
