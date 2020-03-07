/* Name: Robert Mopia
RedID: 817646343 */
#include <string>
#include <iostream>
#include <utility>
#include "level.h"
#ifndef PAGETABLE_HEADER
#define PAGETABLE_HEADER

class Level;
class Pagetable{
    public:
        Pagetable(int level_count, int bits_per_level[]);
        int LevelCount;
        std::string *BitmaskAry;
        int *ShiftAry;
        int *EntryCount;
        Level *RootNodePtr;
};

class Map{
    public:
        Map();
        Map(bool bit, int frame);
        bool valid_bit;
        int frame;
        std::pair <bool, int> FrameIndex;
};
/*
class Level{
    public:
        Level();
        Level(Pagetable*, int current_depth, int array_size);
        Pagetable *PageTablePtr;
        int CurrentDepth;
        Level *NextLevelPtr;
        Level *NextLevel;
        Map *MapPtr;
};*/

unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);

// Frame starts at zero and then is incremented each time a page is inserted
void PageInsert(Pagetable *ptable, unsigned int LogicalAddress, unsigned int Frame);

Map *PageLookUp(Pagetable *ptable, unsigned int LogicalAddress);

unsigned int HexToDec(std::string hex_num);

#endif // PAGETABLE_HEADER
