#include "defs.h"

void initRoomList(RoomList* list) {
    // Initialize the list
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void initRoom(char* name, RoomType** room) {
    *room = malloc(sizeof(RoomType)); // allocate memory for the room

    strcpy((*room)->name, name); // copy the name into the room

    // Initialize the connected rooms list
    (*room)->connected_rooms = malloc(sizeof(RoomList));
    initRoomList((*room)->connected_rooms);

    // Initialize the evidence list
    (*room)->evidence_in_room = malloc(sizeof(EvidenceList));
    initEvidenceList((*room)->evidence_in_room);

    // Initialize the number of hunters
    (*room)->num_hunters = 0;

    // Initialize the hunter pointers in the room
    for (int i = 0; i < NUM_HUNTERS; i++) {
        (*room)->hunters_in_room[i] = NULL; // clear the slot
    }

    // Initialize the mutex and unlock it
    sem_init(&(*room)->mutex, 0, 1);
    sem_post(&(*room)->mutex);

    // Initialize the ghost pointer in the room
    (*room)->ghost_in_room = NULL;
}

RoomType* createRoom(char* name) {
    // Allocate memory for the room
    RoomType* room;
    initRoom(name, &room); // initialize the room
    return room;
}

void addRoom (RoomList* list, RoomType* room) {
    // Create a new node
    RoomNode* node = malloc(sizeof(RoomNode));
    node->data = room;
    node->next = NULL;

    if (list->head == NULL) { // if the list is empty add the node to the head
        list->head = node;
        list->tail = node;
    } else { // if the list is not empty add the node to the tail
        list->tail->next = node;
        list->tail = node;
    }

    // Increment the size of the list
    list->size++;
}

void connectRooms(RoomType* room1, RoomType* room2) {
    // Add the rooms to each other's connected rooms list
    addRoom(room1->connected_rooms, room2);
    addRoom(room2->connected_rooms, room1);
}

RoomType* getRandomRoom(RoomList* rooms, int excludeVan) {
    // Generate a random index
    int rand_index = randInt(excludeVan, rooms->size);

    // Get the room at the random index
    RoomNode* curr_node = rooms->head;
    for (int i = 0; i < rand_index; i++) {
        curr_node = curr_node->next;
    }

    // Return the room
    return curr_node->data;
}

void cleanupRoom(RoomType* room) {
    // Call the cleanup functions for the connected rooms list and the evidence list
    cleanupConnectedRoomList(room->connected_rooms);
    cleanupEvidenceList(room->evidence_in_room);
    free(room->evidence_in_room);
    free(room);
}

void cleanupConnectedRoomList(RoomList* list) {
    // iterate through the list and free the nodes
    RoomNode* curr = list->head;
    RoomNode* next = NULL;
    while (curr != NULL) {
        next = curr->next;
        free(curr); // free the node
        curr = next;
    }
    free(list);
}

void cleanupRoomList(RoomList* list) {
    // iterate through the list and free the nodes
    RoomNode* curr = list->head;
    RoomNode* next = NULL;
    while (curr != NULL) {      
        next = curr->next;
        cleanupRoom(curr->data); // cleanup the room data
        free(curr); // free the node
        curr = next;
    }
}
