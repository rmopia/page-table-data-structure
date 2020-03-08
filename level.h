#ifndef LEVEL_HEADER
#define LEVEL_HEADER
#include "pagetable.h"

class Level{
    public:
        /* constructors */
        Level();
        Level(Pagetable*, int current_depth, int array_size);
        /* ptr that points back to page table */
        Pagetable *PageTablePtr;
        int CurrentDepth;
        /* ptr that points to 2^n array of pointers */
        Level *NextLevelPtr;
        /* ptr that points to next level structure */
        Level *NextLevel;
        /* ptr that points to map array structure */
        Map *MapPtr;
};

#endif // LEVEL_HEADER
