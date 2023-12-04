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

void populateRooms(HouseType* house, const char* file_name) {
    FILE* file = fopen(file_name, "r");
    
    char room1_name[MAX_STR];
    char room2_name[MAX_STR];

    while (fscanf(file, "%s %s", room1_name, room2_name) == 2) {
        replaceUnderscoreWithSpace(room1_name);
        replaceUnderscoreWithSpace(room2_name);

        // Check if the rooms are already added
        RoomType* room1 = findRoomByName(&house->rooms, room1_name);
        RoomType* room2 = findRoomByName(&house->rooms, room2_name);

        // If not, create and add them to the house
        if (room1 == NULL) {
            room1 = createRoom(room1_name);
            addRoom(&house->rooms, room1);
        }

        if (room2 == NULL) {
            room2 = createRoom(room2_name);
            addRoom(&house->rooms, room2);
        }

        // Connect the rooms
        connectRooms(room1, room2);
        connectRooms(room2, room1);
    }

    fclose(file);
}

void cleanupHouse(HouseType* house) {
    // Call cleanupRoomList and cleanupEvidenceList
    cleanupRoomList(&house->rooms);
    cleanupEvidenceList(&house->evidence_list);
}
