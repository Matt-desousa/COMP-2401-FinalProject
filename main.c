#include "defs.h"

int main() {
    // Initialize the random number generator
    srand(time(NULL));

    HouseType house;
    initHouse(&house);
    populateRooms(&house);
    for(int i = 0; i < NUM_HUNTERS; i++) {
        initHunter(house.rooms.head->data, i, &house.evidence_list, &house.hunters[i]);
        l_hunterInit(house.hunters[i].name, house.hunters[i].evidence_type);
    }

    // printHouse(&house);
    return 0;
}