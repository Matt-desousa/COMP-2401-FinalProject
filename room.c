#include "defs.h"

void initRoomList(RoomList* list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void initRoom(RoomType** room, char* name) {
    *room = malloc(sizeof(RoomType));
    strcpy((*room)->name, name);

    (*room)->connected_rooms = malloc(sizeof(RoomList));
    initRoomList((*room)->connected_rooms);

    (*room)->evidence_in_room = malloc(sizeof(EvidenceList));
    initEvidenceList((*room)->evidence_in_room);

    (*room)->num_hunters = 0;

    (*room)->ghost_in_room = NULL;
}

RoomType* createRoom(char* name) {
    RoomType* room;
    initRoom(&room, name);
    return room;
}

void addRoom (RoomList* list, RoomType* room) {
    RoomNode* node = malloc(sizeof(RoomNode));
    node->data = room;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;
}

void connectRooms(RoomType* room1, RoomType* room2) {
    addRoom(room1->connected_rooms, room2);
    addRoom(room2->connected_rooms, room1);
}

RoomType* getRandomRoom(RoomList* rooms) {
    int rand_index = randInt(0, rooms->size);
    RoomNode* curr_node = rooms->head;

    for (int i = 0; i < rand_index; i++) {
        curr_node = curr_node->next;
    }

    return curr_node->data;
}

void cleanupRoom(RoomType* room) {
    cleanupConnectedRoomList(room->connected_rooms);
    free(room->connected_rooms);

    cleanupEvidenceList(room->evidence_in_room);
    free(room->evidence_in_room);

    free(room);
}

void cleanupConnectedRoomList(RoomList* list) {
    RoomNode* curr = list->head;
    RoomNode* next = NULL;
    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
}

void cleanupRoomList(RoomList* list) {
    RoomNode* curr = list->head;
    RoomNode* next = NULL;
    while (curr != NULL) {      
        next = curr->next;
        cleanupRoom(curr->data);
        free(curr);
        curr = next;
    }
}
