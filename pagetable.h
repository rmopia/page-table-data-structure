/* Name: Robert Mopia
RedID: 817646343 */
#include <string>
#include "level.h"
#include "map.h"
#ifndef PAGETABLE_HEADER
#define PAGETABLE_HEADER

class Level;
class Pagetable{
    public:
        Pagetable(int level_count, int bits_per_level[]);
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

/* Logical address to page # converter */
unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift);

/* recursive function that creates pages and stores frame */
bool PageInsert(Pagetable *ptable, unsigned int LogicalAddress, unsigned int Frame);

/* recursive function that validates logical address and returns map ptr */
Map *PageLookUp(Pagetable *ptable, unsigned int LogicalAddress);

/* converts hex string to decimal int */
unsigned int HexToDec(std::string hex_num);

/* converts bin string to hex string */
std::string BinToHex(std::string bin_num);

#endif // PAGETABLE_HEADER

