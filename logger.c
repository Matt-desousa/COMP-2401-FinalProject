#include "defs.h"

void l_hunterInit(char* hunter, enum EvidenceType equipment, char* color) {
    FILE* log_file = fopen(LOG_FILE, "a"); // open game_log.txt

    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(equipment, ev_str);
    printf("%s[HUNTER INIT] [%s] is a [%s] hunter\n", color, hunter, ev_str); 

    fprintf(log_file, "[HUNTER INIT] [%s] is a [%s] hunter\n", hunter, ev_str); // append to game_log.txt
    fclose(log_file);  // close file 
}

void l_hunterMove(char* hunter, char* room, char* color) {
    FILE* log_file = fopen(LOG_FILE, "a"); // open game_log.txt

    if (!LOGGING) return;
    printf("%s[HUNTER MOVE] [%s] has moved into [%s]\n", color, hunter, room);

    fprintf(log_file, "[HUNTER MOVE] [%s] has moved into [%s]\n", hunter, room); // append to game_log.txt
    fclose(log_file); // close file 
}

void l_hunterExit(char* hunter, enum LoggerDetails reason, char* color) {
    FILE* log_file = fopen(LOG_FILE, "a"); // open game_log.txt

    if (!LOGGING) return;
    printf("%s[HUNTER EXIT] [%s] exited because ", color, hunter);
    fprintf(log_file, "[HUNTER EXIT] [%s] exited because ", hunter); // append to game_log.txt
    switch (reason) {
        case LOG_FEAR:
            printf("%s[FEAR]\n", color);
            fprintf(log_file, "[FEAR]\n"); // append to game_log.txt
            break;
        case LOG_BORED:
            printf("%s[BORED]\n", color);
            fprintf(log_file, "[BORED]\n"); // append to game_log.txt
            break;
        case LOG_EVIDENCE:
            printf("%s[EVIDENCE]\n", color);
            fprintf(log_file, "[EVIDENCE]\n"); // append to game_log.txt
            break;
        default:
            printf("%s[UNKNOWN]\n", color);
    }

    fclose(log_file);
}

void l_hunterReview(char* name, enum LoggerDetails result, char* color) {
    FILE* log_file = fopen(LOG_FILE, "a"); // open game_log.txt

    if (!LOGGING) return;
    printf("%s[HUNTER REVIEW] [%s] reviewed evidence and found ", color, name);
    fprintf(log_file, "[HUNTER REVIEW] [%s] reviewed evidence and found ", name); // append to game_log.txt
    switch (result) {
        case LOG_SUFFICIENT:
            printf("%s[SUFFICIENT]\n", color);
            fprintf(log_file, "[SUFFICIENT]\n"); // append to game_log.txt
            break;
        case LOG_INSUFFICIENT:
            printf("%s[INSUFFICIENT]\n", color);
            fprintf(log_file, "[INSUFFICIENT]\n"); // append to game_log.txt
            break;
        default:
            printf("%s[UNKNOWN]\n", color);
            fprintf(log_file, "[UKNOWN]\n"); // append to game_log.txt
    }

    fclose(log_file); // close file 
}

void l_hunterCollect(char* hunter, enum EvidenceType evidence, char* room, char* color) {
    FILE* log_file = fopen(LOG_FILE, "a"); // open game_log.txt

    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(evidence, ev_str);
    printf("%s[HUNTER EVIDENCE] [%s] found [%s] in [%s] and [COLLECTED]\n", color, hunter, ev_str, room);

    fprintf(log_file, "[HUNTER EVIDENCE] [%s] found [%s] in [%s] and [COLLECTED]\n", hunter, ev_str, room); // append to game_log.txt
    fclose(log_file); // close file 
}

void l_hunterChangeEvidence(char* hunter, enum EvidenceType evidence, char* color) {
    FILE* log_file = fopen(LOG_FILE, "a"); // open game_log.txt

    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(evidence, ev_str);
    printf("%s[HUNTER CHANGE EQUIPMENT] [%s] changed equipment to [%s]\n", color, hunter, ev_str);

    fprintf(log_file, "[HUNTER CHANGE EQUIPMENT] [%s] changed equipment to [%s]\n", hunter, ev_str); // append to game_log.txt
    fclose(log_file); // close file 
}

void l_ghostMove(char* room, int success) {
    FILE* log_file = fopen(LOG_FILE, "a"); // open game_log.txt

    if (!LOGGING) return;
    if (success){
        printf("\033[0m[GHOST MOVE] Ghost has moved into [%s]\n", room);
        fprintf(log_file, "[GHOST MOVE] Ghost has moved into [%s]\n", room); // append to game_log.txt
    }
    else{
        printf("\033[0m[GHOST MOVE] Ghost tried to move into [%s] but was blocked\n", room);
        fprintf(log_file, "[GHOST MOVE] Ghost tried to move into [%s] but was blocked\n", room); // append to game_log.txt
    }
    
    fclose(log_file); // close file 
}

void l_ghostExit(enum LoggerDetails reason) {
    FILE* log_file = fopen(LOG_FILE, "a"); // open game_log.txt

    if (!LOGGING) return;
    printf("\033[0m[GHOST EXIT] Exited because ");
    fprintf(log_file, "[GHOST EXIT] Exited because "); // append to game_log.txt
    switch (reason) {
        case LOG_FEAR:
            printf("\033[0m[FEAR]\n");
            fprintf(log_file, "[FEAR]\n"); // append to game_log.txt
            break;
        case LOG_BORED:
            printf("\033[0m[BORED]\n");
            fprintf(log_file, "[BORED]\n"); // append to game_log.txt
            break;
        case LOG_EVIDENCE:
            printf("\033[0m[EVIDENCE]\n");
            fprintf(log_file, "[EVIDENCE]\n"); // append to game_log.txt
            break;
        default:
            printf("\033[0m[UNKNOWN]\n");
            fprintf(log_file, "[UNKNOWN]\n"); // append to game_log.txt
    }

    fclose(log_file); // close file 
}

void l_ghostEvidence(enum EvidenceType evidence, char* room) {
    FILE* log_file = fopen(LOG_FILE, "a"); // open game_log.txt

    if (!LOGGING) return;
    char ev_str[MAX_STR];
    evidenceToString(evidence, ev_str);
    printf("\033[0m[GHOST EVIDENCE] Ghost left [%s] in [%s]\n", ev_str, room);

    fprintf(log_file, "[GHOST EVIDENCE] Ghost left [%s] in [%s]\n", ev_str, room); // append to game_log.txt
    fclose(log_file); // close file 
}

void l_ghostInit(enum GhostClass ghost, char* room) {
    FILE* log_file = fopen(LOG_FILE, "a"); // open game_log.txt

    if (!LOGGING) return;
    char ghost_str[MAX_STR];
    ghostToString(ghost, ghost_str);
    printf("\033[0m[GHOST INIT] Ghost is a [%s] in room [%s]\n", ghost_str, room);

    fprintf(log_file, "[GHOST INIT] Ghost is a [%s] in room [%s]\n", ghost_str, room); // append to game_log.txt
    fclose(log_file); // close file 
}

void l_ghostDoNothing() {
    FILE* log_file = fopen(LOG_FILE, "a"); // open game_log.txt

    if (!LOGGING) return;
    printf("\033[0m[GHOST DO NOTHING] Ghost did nothing\n");
    fprintf(log_file, "[GHOST DO NOTHING] Ghost did nothing\n"); // append to game_log.txt

    fclose(log_file); // close file 
}
