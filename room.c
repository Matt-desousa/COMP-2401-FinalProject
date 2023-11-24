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
    (*room)->evidence_left = malloc(sizeof(EvidenceList));
    (*room)->hunters_in_room = malloc(sizeof(HunterList));
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

void printRoom(RoomType* room) {
    printf("Room: %s\n", room->name);
    printf("Connected Rooms: ");
    RoomNode* curr = room->connected_rooms->head;
    while (curr != NULL) {
        printf("%s, ", curr->data->name);
        curr = curr->next;
    }
    printf("\n");
}