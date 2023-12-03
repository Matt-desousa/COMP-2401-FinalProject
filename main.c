#include "defs.h"
#include <pthread.h>
#include <semaphore.h>

int main() {
    // Initialize the random number generator
    srand(time(NULL));
    
    FILE* log_file = fopen(LOG_FILE, "w");
    fclose(log_file); // resets the text file because it will be opened again by the logging functions
    
    // Initialize the house
    HouseType house;
    initHouse(&house);
    populateRooms(&house, "map.txt");

    // Initialize the ghost
    GhostType* ghost;
    initGhost(house.rooms.head->data, &(house.active_hunters), &ghost);
    // initGhost(getRandomRoom(&(house.rooms), 1), &(house.active_hunters), &ghost);

    // Initialize hunters
    for(int i = 0; i < NUM_HUNTERS; i++) {
        initHunter(house.rooms.head->data, i, &house.evidence_list, &(house.active_hunters), &(house.active_hunters_mutex), &house.hunters[i]);
    }

    // Start the game
    pthread_create(&ghost->pid, NULL, ghostHandler, ghost);
    pthread_create(&house.hunters[0].tid, NULL, hunterHandler, &house.hunters[0]);
    pthread_create(&house.hunters[1].tid, NULL, hunterHandler, &house.hunters[1]);
    pthread_create(&house.hunters[2].tid, NULL, hunterHandler, &house.hunters[2]);
    pthread_create(&house.hunters[3].tid, NULL, hunterHandler, &house.hunters[3]);

    pthread_join(ghost->pid, NULL);

    // End the game
    for(int i = 0; i < NUM_HUNTERS; i++) {
        pthread_join(house.hunters[i].tid, NULL);
    }
    
    printResults(house.hunters, ghost);

    // Cleanup memory
    cleanupHouse(&house);
    cleanupGhost(ghost);

    return 0;
}
