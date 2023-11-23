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
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

// OUR CHANGES
// structures
typedef struct Evidence {
    EvidenceType type;
    sem_t semaphore; 
} Evidence;

typedef struct RoomNode {
    struct Room* room;
    struct RoomNode* next;
} RoomNode;

typedef struct Room {
    char name[MAX_STR];
    RoomNode* room_connections; // linked list of connected rooms
    EvidenceNode* evidence_list; // linked list of evidence in the room
    struct Hunter* hunters_in_room[]; // collection of hunters in the room
    struct Ghost* ghost_in_room; // pointer to the ghost in the room or NULL
    sem_t semaphore; 
} Room;

typedef struct House {
    struct Hunter* hunters; // collection of all hunters
    RoomNode* all_rooms; // linked list of all rooms
    EvidenceNode* shared_evidence_list; // shared evidence list for all hunters
    int hunting; // 
} House;

typedef struct Ghost {
    GhostClass ghost_type;
    Room* current_room;
    int boredom_timer;
    sem_t semaphore; 
} Ghost;

typedef struct Hunter {
    Room* curr_room;
    EvidenceType evidence_type;
    char name[MAX_STR];
    EvidenceNode* shared_evidence_list;
    int fear;
    int boredom;
} Hunter;

typedef struct EvidenceNode {
    Evidence* evidence;
    struct EvidenceNode* next;
} EvidenceNode;

// function prototypes
void connectRooms(Room* room1, Room* room2);
void populateRooms(House* house);
// END OF OUR CHANGES (this is just here so that you know what we were given and what was added)

// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter

// Logging Utilities
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);
