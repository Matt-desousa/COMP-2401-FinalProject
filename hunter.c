#include "defs.h"

// // function to initialize hunter
// void initHunter(char* name, EvidenceType evidenceType, RoomType* startingRoom, EvidenceNode* sharedEvidenceList) {
//     HunterType* newHunter = (HunterType*)malloc(sizeof(HunterType)); // allocate memory for the new hunter

//     // maybe here we can have some if statements to take care of cases when some parameters might be NULL but we can save that for the end when we have the basic stuff done

//     // initial values for the hunter
//     strncpy(newHunter->name, name, MAX_STR - 1);  // copy the name??
//     newHunter->evidence_type = evidenceType;
//     newHunter->curr_room = startingRoom; // placeholder because the startingRoom for a hunter should be the van
//     newHunter->evidence_list = sharedEvidenceList;
//     newHunter->fear = 0; // fear when they start hunting is initially 0
//     newHunter->boredom = 0; // boredom when they start hunting is initially 0

//     // log the initialization
//     l_hunterInit(newHunter->name, newHunter->evidence_type);
// }
