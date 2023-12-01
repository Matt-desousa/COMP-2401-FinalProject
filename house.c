#include "defs.h"

void initHouse(HouseType* house) {
    initRoomList(&house->rooms);
    initEvidenceList(&house->evidence_list);
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

// helper function to replace underscores with spaces in a string
void replaceUnderscoreWithSpace(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '_') {
            str[i] = ' ';
        }
    }
}

void cleanupHouse(HouseType* house) {
    cleanupRoomList(&house->rooms);
    cleanupEvidenceList(&house->evidence_list);
}
