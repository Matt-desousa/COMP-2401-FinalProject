#include "defs.h"

void initGhost(GhostType* ghost, RoomType* startingRoom) {
    ghost->ghost_type = randInt(0, 3);
    ghost->curr_room = startingRoom;
    ghost->boredom = 0;
}
