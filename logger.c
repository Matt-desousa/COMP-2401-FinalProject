#include "defs.h"

void l_hunterInit(char* hunter, enum EvidenceType equipment, char* color) {
    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(equipment, ev_str);
    printf("%s[HUNTER INIT] [%s] is a [%s] hunter\n", color, hunter, ev_str);    
}

void l_hunterMove(char* hunter, char* room, char* color) {
    if (!LOGGING) return;
    printf("%s[HUNTER MOVE] [%s] has moved into [%s]\n", color, hunter, room);
}

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

void l_hunterReview(char* name, enum LoggerDetails result, char* color) {
    if (!LOGGING) return;
    printf("%s[HUNTER REVIEW] [%s] reviewed evidence and found ", color, name);
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

void l_hunterCollect(char* hunter, enum EvidenceType evidence, char* room, char* color) {
    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(evidence, ev_str);
    printf("%s[HUNTER EVIDENCE] [%s] found [%s] in [%s] and [COLLECTED]\n", color, hunter, ev_str, room);
}

void l_ghostMove(char* room, int success) {
    if (!LOGGING) return;
    if (success){
        printf("\033[0m[GHOST MOVE] Ghost has moved into [%s]\n", room);
    }
    else{
        printf("\033[0m[GHOST MOVE] Ghost tried to move into [%s] but was blocked\n", room);
    }
    
}

void l_ghostDoNothing() {
    if (!LOGGING) return;
    printf("\033[0m[GHOST DO NOTHING] Ghost did nothing\n");
}

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

void l_ghostEvidence(enum EvidenceType evidence, char* room) {
    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(evidence, ev_str);
    printf("\033[0m[GHOST EVIDENCE] Ghost left [%s] in [%s]\n", ev_str, room);
}

void l_ghostInit(enum GhostClass ghost, char* room) {
    if (!LOGGING) return;
    char ghost_str[MAX_STR];
    ghostToString(ghost, ghost_str);
    printf("\033[0m[GHOST INIT] Ghost is a [%s] in room [%s]\n", ghost_str, room);
}
