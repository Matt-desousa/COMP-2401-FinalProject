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
    initGhost(&ghost, getRandomRoom(&(house.rooms), 1));

    // Initialize hunters
    for(int i = 0; i < NUM_HUNTERS; i++) {
        initHunter(house.rooms.head->data, i, &house.evidence_list, &house.hunters[i]);
    }

    // Start the game
    // ghostHandler(ghost);
    // pthread_create(ghost->pid, NULL, ghostHandler, ghost);

    if (sem_init(&mutex, 0, 1) < 0) {
        printf("semaphore initialization error\n");
        exit(1);
    }

    pthread_create(&ghost->pid, NULL, ghostHandler, ghost);
    pthread_create(&house.hunters[0].pid, NULL, hunterHandler, &house.hunters[0]);
    pthread_create(&house.hunters[1].pid, NULL, hunterHandler, &house.hunters[1]);
    pthread_create(&house.hunters[2].pid, NULL, hunterHandler, &house.hunters[2]);
    pthread_create(&house.hunters[3].pid, NULL, hunterHandler, &house.hunters[3]);

    // End the game
    pthread_join(ghost->pid, NULL);

    for(int i = 0; i < NUM_HUNTERS; i++) {
        pthread_join(house.hunters[i].pid, NULL);
    }

    // Cleanup memory
    cleanupHouse(&house);
    cleanupGhost(ghost);

    // printHouse(&house);
    return 0;
}
