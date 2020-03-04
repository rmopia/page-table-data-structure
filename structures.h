/* Name: Robert Mopia
RedID: 817646343 */
#ifndef PAGETABLE_HEADER
#define PAGETABLE_HEADER

class Pagetable{
    public:
        Pagetable(int level_count);
        int LevelCount;
        int *BitmaskAry;
        int *ShiftAry;
        int *EntryCount;
        //static int BitmaskAry[];
        //static int ShiftAry[];
        //static int EntryCount[];
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
        Level *nextLevel[]; // consider removing brackets just in case
        Map *MapPtr;
};


unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);


#endif // PAGETABLE_HEADER
