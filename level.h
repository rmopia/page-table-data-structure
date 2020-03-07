
#ifndef LEVEL_HEADER
#define LEVEL_HEADER
#include "structures.h"

class Level{
    public:
        Level();
        Level(Pagetable*, int current_depth, int array_size);
        Pagetable *PageTablePtr;
        int CurrentDepth;
        Level *NextLevelPtr;
        Level *NextLevel;
        Map *MapPtr;
};

#endif // LEVEL_HEADER
