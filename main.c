#include "defs.h"

int main() {
    printf("%d\n", sizeof(RoomNode));
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
    // for(int i = 0; i < NUM_HUNTERS; i++) {
    //     initHunter(house.rooms.head->data, i, &house.evidence_list, &house.hunters[i]);
    // }

    cleanupHouse(&house);
    // cleanupGhost(ghost);

    // printHouse(&house);
    return 0;
}