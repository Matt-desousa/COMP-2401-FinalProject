 #include <pthread.h>
#include <semaphore.h>

#define MAX_STRING 50

#define FEAR_MAX 10
#define BOREDOM_MAX 5

// enumerated Data Types
typedef enum {
    EMF,
    TEMPERATURE,
    FINGERPRINTS,
    SOUND
} EvidenceType;

typedef enum {
    POLTERGEIST,
    BANSHEE,
    BULLIES,
    PHANTOM
} GhostClass;

// structures
typedef struct Evidence {
    EvidenceType type;
    sem_t semaphore; // semaphore to prevent concurrent modification
} Evidence;

typedef struct RoomNode {
    struct Room* room;
    struct RoomNode* next;
} RoomNode;

typedef struct Room {
    char name[MAX_STRING];
    RoomNode* room_connections; // linked list of connected rooms
    EvidenceNode* evidence_list; // linked list of evidence in the room
    struct Hunter* hunters_in_room; // collection of hunters in the room
    struct Ghost* ghost_in_room; // pointer to the ghost in the room or NULL
    sem_t semaphore; 
} Room;

typedef struct House {
    struct Hunter* hunters; // collection of all hunters
    RoomNode* all_rooms; // linked list of all rooms
    EvidenceNode* shared_evidence_list; // shared evidence list for all hunters
    int hunting;
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
    char name[MAX_STRING];
    EvidenceNode* shared_evidence_list;
    int fear;
    int boredom;
} Hunter;

typedef struct EvidenceNode {
    Evidence* evidence;
    struct EvidenceNode* next;
} EvidenceNode;

// Function prototypes
void connectRooms(Room* room1, Room* room2);
void populateRooms(House* house);