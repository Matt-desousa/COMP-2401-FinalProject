#include "defs.h"

void initHouse(HouseType* house) {
    // Initialize the rooms list
    initRoomList(&house->rooms);

    // Initialize the evidence list
    initEvidenceList(&house->evidence_list);

    // Initialize the active hunters
    house->active_hunters = 0;
    sem_init(&house->active_hunters_mutex, 0, 1);
}

void populateRooms(HouseType* house, const char* filename) {
    FILE* file = fopen(filename, "r");

    char room1[MAX_STR];
    char room2[MAX_STR];

    while (fscanf(file, "%s %s", room1, room2) == 2) {
        // Replace underscores with spaces
        replaceUnderscoreWithSpace(room1);
        replaceUnderscoreWithSpace(room2);

        RoomType* room1Ptr = createRoom(room1);
        RoomType* room2Ptr = createRoom(room2);

        connectRooms(room1Ptr, room2Ptr);

        addRoom(&house->rooms, room1Ptr);
        addRoom(&house->rooms, room2Ptr);
    }

    fclose(file);
}

void cleanupHouse(HouseType* house) {
    // Call cleanupRoomList and cleanupEvidenceList
    cleanupRoomList(&house->rooms);
    cleanupEvidenceList(&house->evidence_list);
}
