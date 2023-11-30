#include "defs.h"

/* 
    Logs the hunter being created.
    in: hunter - the hunter name to log
    in: equipment - the hunter's equipment
*/
void l_hunterInit(char* hunter, enum EvidenceType equipment, char* color) {
    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(equipment, ev_str);
    printf("%s[HUNTER INIT] [%s] is a [%s] hunter\n", color, hunter, ev_str);    
}

/*
    Logs the hunter moving into a new room.
    in: hunter - the hunter name to log
    in: room - the room name to log
*/
void l_hunterMove(char* hunter, char* room, char* color) {
    if (!LOGGING) return;
    printf("%s[HUNTER MOVE] [%s] has moved into [%s]\n", color, hunter, room);
}

/*
    Logs the hunter exiting the house.
    in: hunter - the hunter name to log
    in: reason - the reason for exiting, either LOG_FEAR, LOG_BORED, or LOG_EVIDENCE
*/
void l_hunterExit(char* hunter, enum LoggerDetails reason, char* color) {
    if (!LOGGING) return;
    printf("%s[HUNTER EXIT] [%s] exited because ", color, hunter);
    switch (reason) {
        case LOG_FEAR:
            printf("%s[FEAR]\n", color);
            break;
        case LOG_BORED:
            printf("%s[BORED]\n", color);
            break;
        case LOG_EVIDENCE:
            printf("%s[EVIDENCE]\n", color);
            break;
        default:
            printf("%s[UNKNOWN]\n", color);
    }
}

/*
    Logs the hunter reviewing evidence.
    in: hunter - the hunter name to log
    in: result - the result of the review, either LOG_SUFFICIENT or LOG_INSUFFICIENT
*/
void l_hunterReview(char* hunter, enum LoggerDetails result, char* color) {
    if (!LOGGING) return;
    printf("%s[HUNTER REVIEW] [%s] reviewed evidence and found ", color, hunter);
    switch (result) {
        case LOG_SUFFICIENT:
            printf("%s[SUFFICIENT]\n", color);
            break;
        case LOG_INSUFFICIENT:
            printf("%s[INSUFFICIENT]\n", color);
            break;
        default:
            printf("%s[UNKNOWN]\n", color);
    }
}

/*
    Logs the hunter collecting evidence.
    in: hunter - the hunter name to log
    in: evidence - the evidence type to log
    in: room - the room name to log
*/
void l_hunterCollect(char* hunter, enum EvidenceType evidence, char* room, char* color) {
    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(evidence, ev_str);
    printf("%s[HUNTER EVIDENCE] [%s] found [%s] in [%s] and [COLLECTED]\n", color, hunter, ev_str, room);
}

/*
    Logs the hunter NOT finding/collecting evidence.
    in: hunter - the hunter name to log
    in: evidence - the evidence type to log
    in: room - the room name to log
*/
void l_hunterCollectNone(char* hunter, char* room, char* color) {
    if (!LOGGING) return;
    printf("%s[HUNTER EVIDENCE] [%s] found [NONE] in [%s]\n", color, hunter, room);
}

/*
    Logs the ghost moving into a new room.
    in: room - the room name to log
*/
void l_ghostMove(char* room, int success) {
    if (!LOGGING) return;
    if (success){
        printf("\033[0m[GHOST MOVE] Ghost has moved into [%s]\n", room);
    }
    else{
        printf("\033[0m[GHOST MOVE] Ghost tried to move into [%s] but was blocked\n", room);
    }
    
}

/*
    Logs the ghost exiting the house.
    in: reason - the reason for exiting, either LOG_FEAR, LOG_BORED, or LOG_EVIDENCE
*/
void l_ghostExit(enum LoggerDetails reason) {
    if (!LOGGING) return;
    printf("\033[0m[GHOST EXIT] Exited because ");
    switch (reason) {
        case LOG_FEAR:
            printf("\033[0m[FEAR]\n");
            break;
        case LOG_BORED:
            printf("\033[0m[BORED]\n");
            break;
        case LOG_EVIDENCE:
            printf("\033[0m[EVIDENCE]\n");
            break;
        default:
            printf("\033[0m[UNKNOWN]\n");
    }
}

/*
    Logs the ghost leaving evidence in a room.
    in: evidence - the evidence type to log
    in: room - the room name to log
*/
void l_ghostEvidence(enum EvidenceType evidence, char* room) {
    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(evidence, ev_str);
    printf("\033[0m[GHOST EVIDENCE] Ghost left [%s] in [%s]\n", ev_str, room);
}

/*
    Logs the ghost being created.
    in: ghost - the ghost type to log
    in: room - the room name that the ghost is starting in
*/
void l_ghostInit(enum GhostClass ghost, char* room) {
    if (!LOGGING) return;
    char ghost_str[MAX_STR];
    ghostToString(ghost, ghost_str);
    printf("\033[0m[GHOST INIT] Ghost is a [%s] in room [%s]\n", ghost_str, room);
}
