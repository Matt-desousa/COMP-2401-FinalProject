#include "defs.h"

int main()
{
    // Initialize the random number generator
    srand(time(NULL));

    HouseType house;
    initHouse(&house);
    populateRooms(&house);

    printHouse(&house);
    return 0;
}

