#include "defs.h"

int randInt(int min, int max)
{
    return (int) randFloat(min, max);
}

float randFloat(float min, float max) {
    static __thread unsigned int seed = 0;
    if (seed == 0) {
        seed = (unsigned int)time(NULL) ^ (unsigned int)pthread_self();
    }

    float random = ((float) rand_r(&seed)) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

enum GhostClass randomGhost() {
    return (enum GhostClass) randInt(0, GHOST_COUNT);
}

void evidenceToString(enum EvidenceType type, char* str) {
    switch (type) {
        case EMF:
            strcpy(str, "EMF");
            break;
        case TEMPERATURE:
            strcpy(str, "TEMPERATURE");
            break;
        case FINGERPRINTS:
            strcpy(str, "FINGERPRINTS");
            break;
        case SOUND:
            strcpy(str, "SOUND");
            break;
        default:
            strcpy(str, "UNKNOWN");
            break;
    }
}

void ghostToString(enum GhostClass ghost, char* buffer) {
    switch(ghost) {
        case BANSHEE:
            strcpy(buffer, "Banshee");
            break;
        case BULLIES:
            strcpy(buffer, "Bullies");
            break;
        case PHANTOM:
            strcpy(buffer, "Phantom");
            break;
        case POLTERGEIST:
            strcpy(buffer, "Poltergeist");
            break;
        default:
            strcpy(buffer, "Unknown");
            break;
        
    }
}

void replaceUnderscoreWithSpace(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '_') {
            str[i] = ' ';
        }
    }
}

RoomType* findRoomByName(RoomList* list, const char* name) {
    RoomNode* curr = list->head;
    while (curr != NULL) {
        if (strcmp(curr->data->name, name) == 0) {
            return curr->data;
        }
        curr = curr->next;
    }
    return NULL;
}

void printResults(HouseType* house, GhostType* ghost) {
    HunterType* hunters = house->hunters;

    printf("\033[0m");
    printf("====================================\n");
    printf("All done! Here are the results\n");
    printf("====================================\n");

    char ghost_type[20];
    ghostToString(ghost->ghost_class, ghost_type);

    int ran_in_fear = 0;
    int got_bored = 0;
    int found_ghost = 0;

    for (int i = 0; i < NUM_HUNTERS; i++){
        if (hunters[i].fear >= FEAR_MAX) {
            printf("%s%4s ran away in fear.\n", hunters[i].color, hunters[i].name);
            ran_in_fear++;
        }
        else if (hunters[i].boredom >= BOREDOM_MAX) {
            printf("%s%4s got bored and left.\n", hunters[i].color, hunters[i].name);
            got_bored++;
        }
        else {
            printf("%s%4s found the ghost!\n", hunters[i].color, hunters[i].name);
            found_ghost++;
        }
    }

    printf("\033[0m");
    printf("------------------------------------\n");

    if (found_ghost == NUM_HUNTERS){
        printf("The hunters all found the ghost!\nThe hunters have won!\n");
    }
    else { // if ran_in_fear or got_bored > 0
        printf("The hunters failed to all find the ghost and left.\n");
        printf("The ghost has won!\nThe hunters have failed!\n");
    }

    if (found_ghost > 0) {
        printf("Using their evidence, %d hunter(s) correctly identified the ghost as a %s.\n", found_ghost, ghost_type);
    }
    else{
        printf("Using their evidence, the hunters incorrectly identified the ghost as a Unkown.\n");
        printf("The ghost was a %s.\n", ghost_type);
    }

    printf("------------------------------------\n");
    
    printf("The hunters collected the following evidence:\n");
    
    EvidenceNode* curr = hunters[0].evidence_list->head;
    EvidenceType evidence[3] = {EV_UNKNOWN, EV_UNKNOWN, EV_UNKNOWN};
    int curr_index = 0;
    while (curr != NULL) {
        if (evidence[0] != curr->data && evidence[1] != curr->data && evidence[2] != curr->data) {
            evidence[curr_index] = curr->data;
            curr_index++;
        }
        curr = curr->next;
    }

    for (int i = 0; i < 3; i++) {
        char evidence_str[20];
        evidenceToString(evidence[i], evidence_str);
        printf("%s\n", evidence_str);
    }
}