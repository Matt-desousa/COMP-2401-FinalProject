#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
// #define BOREDOM_MAX     10
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE

#define SUFFICIENT      1
#define INSUFFICIENT    0

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

typedef struct EvidenceNode EvidenceNode;
typedef struct EvidenceList EvidenceList;
typedef struct RoomNode RoomNode;
typedef struct RoomList RoomList;
typedef struct RoomType RoomType;
typedef struct GhostType GhostType;
typedef struct HunterType HunterType;
typedef struct HouseType HouseType;

// Structure Definitions

/*
  Struct:   Evidence Node
  Purpose:  Linked list node for evidence
  Variables: 
            EvidenceType data - type of evidence
            EvidenceNode* next - pointer to next node
*/
struct EvidenceNode {
  EvidenceType data;
  struct EvidenceNode* next;
};

/*
  Struct:   Evidence List
  Purpose:  Linked list of evidence
  Variables: 
            EvidenceNode* head - pointer to head node
            EvidenceNode* tail - pointer to tail node
            int size - size of list
*/
struct EvidenceList{
  EvidenceNode* head;
  EvidenceNode* tail;
  int size;
  sem_t mutex;
};

/*
  Struct:   Hunter Type
  Purpose:  Struct to hold hunter data
  Variables: 
            RoomType* curr_room - pointer to current room
            EvidenceType evidence_type - type of evidence the hunter can collect
            char name[MAX_STR] - name of the hunter
            EvidenceList* evidence_list - pointer to the list of evidence the hunters have collected
            int fear - fear level
            int boredom - boredom level
*/
struct HunterType{
  RoomType* curr_room;
  EvidenceType evidence_type;
  char name[MAX_STR];
  EvidenceList* evidence_list;
  HouseType* house;
  int fear;
  int boredom;
  pthread_t tid;
  char color[8];
};

/*
  Struct:   Room Node
  Purpose:  Linked list node for rooms
  Variables: 
            RoomType* data - pointer to the room data
            RoomNode* next - pointer to next node
*/
struct RoomNode{
  RoomType* data; // Pointer to the room data
  struct RoomNode* next; // Linked list of connected rooms
};

/*
  Struct:   Room List
  Purpose:  Linked list of rooms
  Variables: 
            RoomNode* head - pointer to head node
            RoomNode* tail - pointer to tail node
            int size - size of list
*/
struct RoomList{
  RoomNode* head;
  RoomNode* tail;
  int size;
};

/*
  Struct:   Room Type
  Purpose:  Struct to hold room data
  Variables: 
            char name[MAX_STR] - name of the room
            RoomList* connected_rooms - linked list of connected rooms
            EvidenceList* evidence_left - linked list of evidence in the room
            HunterType hunters_in_room - collection of hunters in the room
            GhostType* ghost_in_room - pointer to the ghost in the room or NULL
*/
struct RoomType{
  char name[MAX_STR];
  RoomList* connected_rooms;
  EvidenceList* evidence_in_room;
  HunterType *hunters_in_room[NUM_HUNTERS];
  int num_hunters;
  GhostType* ghost_in_room;
  sem_t mutex;
};

/*
  Struct:   Ghost Type
  Purpose:  Struct to hold ghost data
  Variables: 
            GhostClass ghost_type - enum type of ghost
            RoomType* current_room - pointer to current room
            int boredom_timer - boredom level
*/
struct GhostType{
  GhostClass ghost_class;
  RoomType* curr_room;
  EvidenceList evidence_list;
  HouseType* house;
  int boredom;
  pthread_t pid;
};

/*
  Struct:   House Type
  Purpose:  Struct to hold house data
  Variables: 
            HunterType hunters[NUM_HUNTERS] - array of hunters
            RoomList* rooms - pointer to the map/rooms list
            EvidenceList* evidence_list - pointer to the list of evidence the hunters have collected
*/
struct HouseType{
  HunterType hunters[NUM_HUNTERS];
  RoomList rooms;
  EvidenceList evidence_list;
  int active_hunters;
};

// House Function Prototypes
/* 
  Function: Initialize House
  Purpose:  Initializes a house
  Params:   
    Input/Output: HouseType* house - pointer to the house to initialize
*/
void initHouse(HouseType* house);
/* 
  Function: Populate Rooms
  Purpose:  Dynamically allocates several rooms and populates the provided house.
  Params:   
    Input/Output: HouseType* house - pointer to the house to populate
*/
void populateRooms(HouseType* house, const char* filename);
/* 
  Function: Cleanup House
  Purpose:  Free all memory associated with a house
  Params:   
    Input:  HouseType* house - pointer to the house to free
*/
void cleanupHouse(HouseType* house);



// Room Function Prototypes
/* 
  Function: Initialize Room List
  Purpose:  Initializes a room list
  Params:   
    Input/Output: RoomList* list - pointer to the room list
*/
void initRoomList(RoomList* list);
/* 
  Function: Initialize Room
  Purpose:  Initializes a room
  Params:   
    Input:  char* name - name of the room
    Input/Output: RoomType* room - pointer to the room to initialize
*/
void initRoom(char* name, RoomType** room);
/* 
  Function: Create Room
  Purpose:  Initializes a room and returns a pointer to it
  Params:   
    Input:  char* name - name of the room
    Return: RoomType* - pointer to the room
*/
RoomType* createRoom(char* name);
/* 
  Function: Add Room
  Purpose:  Adds a room to the room list
  Params:   
    Input:  RoomType* room - pointer to the room to add
    Input/Output: RoomList* list - pointer to the room list
*/
void addRoom(RoomList* list, RoomType* room);
/* 
  Function: Connect Rooms
  Purpose:  Connects two rooms together by adding them to each other's connected room lists
  Params:   
    Input/Output: RoomType* room1 - pointer to room1, room connected to room2
                  RoomType* room2 - pointer to room2, room connected to room1
*/
void connectRooms(RoomType* room1, RoomType* room2);
/* 
  Function: Get Random Room
  Purpose:  Returns a random room from the room list
  Params:   
    Input:  RoomList* rooms - pointer to the room list
            int excludeVan - whether or not to exclude the van from the random selection
    Return: RoomType* - pointer to the random room
*/
RoomType* getRandomRoom(RoomList* rooms, int excludeVan);
/* 
  Function: Cleanup Room
  Purpose:  Free all memory associated with a room
  Params:   
    Input:  RoomType* room - pointer to the room to free
*/
void cleanupRoom(RoomType* room);
/* 
  Function: Cleanup Connected Room List
  Purpose:  Free all nodes of a room list, leaving the data alone.
  Params:   
    Input:  RoomList* list - pointer to the connected room list to free
*/
void cleanupConnectedRoomList(RoomList* list);
/* 
  Function: Cleanup Room List
  Purpose:  Free all memory associated with a room list
  Params:   
    Input:  RoomList* list - pointer to the room list to free
*/
void cleanupRoomList(RoomList* list);



// Hunter Function Prototypes //
/* 
  Function: Initialize Hunter
  Purpose:  Initializes a hunter
  Params:   
    Input:  RoomType* startingRoom - pointer to the room the hunter starts in (the van)
            EvidenceType evidenceType - type of evidence the hunter can collect
            EvidenceList* sharedEvidenceList - pointer to the shared evidence list
    Input/Output: HunterType* newHunter - pointer to the hunter to initialize
*/
void initHunter(RoomType* startingRoom, EvidenceType evidenceType, EvidenceList* sharedEvidenceList, HouseType* newHouse, HunterType* newHunter);
/* 
  Function: Hunter Handler
  Purpose:  Handles the hunter's actions
  Params:   
    Input:  void* arg - pointer to the hunter whos actions are being handled
*/
void *hunterHandler(void*);
/* 
  Function: Hunter Move
  Purpose:  Moves a hunter to a random connected room
  Params:
    Input:  HunterType* hunter - pointer to the hunter to move
*/
void hunterMove(HunterType* hunter);
/* 
  Function: Add Hunter To Room
  Purpose:  Adds a hunter to a room. Helper function for hunterMove.
  Params:   
    Input:  RoomType* room - pointer to the room to add the hunter to
            HunterType* hunter - pointer to the hunter to add to the room
*/
void addHunterToRoom(RoomType* room, HunterType* hunter);
/* 
  Function: Remove Hunter From Room
  Purpose:  Removes a hunter from a room. Helper function for hunterMove.
  Params:   
    Input:  RoomType* room - pointer to the room to remove the hunter from
            HunterType* hunter - pointer to the hunter to remove from the room
*/
void removeHunterFromRoom(RoomType* room, HunterType* hunter);
/* 
  Function: Hunter Collect
  Purpose:  Hunter collects evidence from the room they are in
  Params:   
    Input:  HunterType* hunter - pointer to the hunter thats collecting evidence
*/
void hunterCollect(HunterType* hunter);
/* 
  Function: Hunter Review
  Purpose:  Hunter reviews the evidence they have all collected
  Params:   
    Input:  HunterType* hunter - pointer to the hunter thats reviewing the evidence
*/
int hunterReview(HunterType* hunter);
/* 
  Function: Hunter Exit
  Purpose:  Removes the hunter from their current room and logs their exit
  Params:   
    Input:  HunterType* hunter - pointer to the hunter thats exiting
*/
void hunterExit (HunterType* hunter, HouseType* house);



// Ghost Function Prototypes //
/* 
  Function: Initialize Ghost
  Purpose:  Initializes a ghost
  Params:   
    Input:  RoomType* startingRoom - pointer to the room the ghost starts in
    Input/Output: GhostType** ghost - pointer to the ghost to initialize
*/
void initGhost(RoomType* startingRoom, GhostType** ghost);
/* 
  Function: Ghost Handler
  Purpose:  Handles the ghost's actions
  Params:   
    Input:  void* arg - pointer to the ghost whos actions are being handled
*/
void *ghostHandler(void* arg);
/* 
  Function: Ghost Move
  Purpose:  Moves a ghost to a random connected room
  Params:   
    Input/Output:  GhostType* ghost - pointer to the ghost to move
*/
void ghostMove(GhostType* ghost);
/* 
  Function: Leave Evidence
  Purpose:  Leaves evidence in the current room
  Params:   
    Input:  GhostType* ghost - pointer to the ghost to leave evidence
*/
void leaveEvidence(GhostType* ghost);
/* 
  Function: Cleanup Ghost
  Purpose:  Free all memory associated with a ghost
  Params:   
    Input:  GhostType* ghost - pointer to the ghost to free
*/
void cleanupGhost(GhostType* ghost);



// Evidence Function Prototypes //
/* 
  Function: Initialize Evidence List
  Purpose:  Initializes an evidence list
  Params:   
    Input/Output: EvidenceList* list - pointer to the evidence list to initialize
*/
void initEvidenceList(EvidenceList* list);
/*
    Function: Initialize Evidence Node
    Purpose:  Initializes an evidence node.
    Params:
        Input:  EvidenceType data - type of evidence
        Retrun: EvidenceNode* - pointer to the new evidence node
*/
EvidenceNode* initEvidenceNode(EvidenceType data);
/* 
  Function: Add Evidence
  Purpose:  Adds an evidence to an evidence list
  Params:   
    Input:  EvidenceList* list - pointer to the evidence list to add to
            EvidenceType data - type of evidence to add
*/
void addEvidence(EvidenceList* list, EvidenceType data);
/* 
  Function: Initialize Ghost Evidence List
  Purpose:  Initializes an evidence list for a ghost based on the ghost type
  Params:   
    Input:  enum GhostClass ghost - the type of ghost
    Output: EvidenceList* evidence - pointer to the evidence list to initialize
*/
void initGhostEvidenceList(EvidenceList* evidence, GhostClass ghost);
/* 
  Function: Get Random Evidence
  Purpose:  Returns a random evidence type from the ghost's evidence list. Helper function for leaveEvidence.
  Params:   
    Input:  EvidenceList* evidence_list - pointer to the ghost's evidence list
    Return: EvidenceType - random evidence type
*/
EvidenceType getRandomEvidence(EvidenceList* evidence_list);
/* 
  Function: Cleanup Evidence List
  Purpose:  Free all memory associated with an evidence list
  Params:   
    Input:  EvidenceList* list - pointer to the evidence list to free
*/
void cleanupEvidenceList(EvidenceList* list);


// Helper Utilies
int randInt(int,int); // Pseudo-random number generator function
float randFloat(float, float); // Pseudo-random float generator function
enum GhostClass randomGhost(); // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter

// Logging Utilities
/* 
  Function: Log Hunter Initialization
  Purpose:  Logs a hunter's initialization
  Params:   
    Input:  char* name - name of the hunter
            enum EvidenceType equipment - type of evidence the hunter can collect
            char* color - color of the hunter`s output
*/
void l_hunterInit(char* name, enum EvidenceType equipment, char* color);
/* 
  Function: Log Hunter Move
  Purpose:  Logs a hunter's move
  Params:   
    Input:  char* name - name of the hunter
            char* room - name of the room the hunter moved to
            char* color - color of the hunter`s output
*/
void l_hunterMove(char* name, char* room, char* color);
/* 
  Function: Log Hunter Review
  Purpose:  Logs a hunter's review
  Params:   
    Input:  char* name - name of the hunter
            enum LoggerDetails reviewResult - whether or not the hunter found sufficient evidence
            char* color - color of the hunter`s output
*/
void l_hunterReview(char* name, enum LoggerDetails reviewResult, char* color);
/* 
  Function: Log Hunter Collect
  Purpose:  Logs a hunter's collect
  Params:   
    Input:  char* name - name of the hunter
            enum EvidenceType evidence - type of evidence the hunter collected
            char* room - name of the room the hunter collected evidence from
            char* color - color of the hunter`s output
*/
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room, char* color);
/* 
  Function: Log Hunter Exit
  Purpose:  Logs a hunter's exit
  Params:   
    Input:  char* name - name of the hunter
            enum LoggerDetails reason - whether or not the hunter found sufficient evidence
            char* color - color of the hunter`s output
*/

void l_hunterExit(char* name, enum LoggerDetails reason, char* color);
void l_hunterCollectNone(char* hunter, char* room, char* color);
/* 
  Function: Log Ghost Initialization
  Purpose:  Logs a ghost's initialization
  Params:   
    Input:  enum GhostClass type - type of ghost
            char* room - name of the room the ghost starts in
*/
void l_ghostInit(enum GhostClass type, char* room);
/* 
  Function: Log Ghost Move
  Purpose:  Logs a ghost's move
  Params:   
    Input:  char* room - name of the room the ghost moved to
            int success - whether or not the ghost was able to move to the room
*/
void l_ghostMove(char* room, int success);
/* 
  Function: Log Ghost Evidence
  Purpose:  Logs a ghost's evidence
  Params:   
    Input:  enum EvidenceType evidence - type of evidence the ghost left
            char* room - name of the room the ghost left evidence in
*/
void l_ghostEvidence(enum EvidenceType evidence, char* room);
/* 
  Function: Log Ghost Exit
  Purpose:  Logs a ghost's exit
  Params:   
    Input:  enum LoggerDetails reason - whether or not the ghost was bored
*/
void l_ghostExit(enum LoggerDetails reason);
