/* Name: Robert Mopia
RedID: 817646343 */
#include <string>
#ifndef PAGETABLE_HEADER
#define PAGETABLE_HEADER


class Pagetable{
    public:
        Pagetable(int level_count, int arguments[]);
        int LevelCount;
        std::string *BitmaskAry;
        int *ShiftAry;
        int *EntryCount;
};

class Map{
    public:
        Map(int entry_amount);
};

class Level{
    public:
        Level(Pagetable*, int current_depth, int array_size);
        Pagetable *PageTablePtr;
        int CurrentDepth;
        Level *nextLevel[256];
        Map *MapPtr;
};

unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);

// Frame starts at zero and then is incremented each time a page is inserted
void PageInsert(Pagetable *ptable, unsigned int LogicalAddress, unsigned int Frame);

Map *PageLookUp(Pagetable *ptable, unsigned int LogicalAddress);


#endif // PAGETABLE_HEADER
