#include <string>
#include "level.h"
#include "map.h"
#ifndef PAGETABLE_HEADER
#define PAGETABLE_HEADER

class Level;
class Pagetable{
    public:
        Pagetable(int level_count, int bits_per_level[]);
        /* where total size of pagetable is stored */
        int StructureSize;
        /* total amount of levels to be constructed */
        int LevelCount;
        /* array of hex string bitmasks */
        std::string *BitmaskAry;
        /* array of right shift bit shift amounts */
        int *ShiftAry;
        /* array of page sizes (2^n) */
        int *EntryCount;
        /* pointer that points to base case level */
        Level *RootNodePtr;
};


/* recursive function that creates pages and stores frame */
bool PageInsert(Pagetable *ptable,
                unsigned int LogicalAddress, unsigned int Frame);

/* recursive function that validates logical address and returns map ptr */
Map *PageLookUp(Pagetable *ptable, unsigned int LogicalAddress);


#endif // PAGETABLE_HEADER

