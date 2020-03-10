#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include "pagetable.h"
#include "conversions.h"
#include "level.h"
#include "map.h"
using namespace std;

/* Pagetable constructor */
Pagetable::Pagetable(int level_count, int level_bits[]){
    this->LevelCount = level_count;
    /* initialize arrays dynamically of size level_count */
    this->BitmaskAry = new string[level_count];
    this->ShiftAry = new int[level_count];
    this->EntryCount = new int[level_count];
    /* records size of pagetable currently */
    this->StructureSize = sizeof(Pagetable);

    int bit_start = 0;
    for(int i = 0; i < level_count; i++){
        int bit_limit = bit_start + level_bits[i];
        string str_bit_mask;
        /* creates 32 binary bitmask string based on command line args */
        for(int i = 0; i < 32; i++){
            if(i >= bit_start && i < bit_limit){
                str_bit_mask += "1";
            }
            else{
                str_bit_mask += "0";
            }
        }
        bit_start = bit_limit;
        /* str binary converted to hex binary */
        /* stored in bitmask string array */
        this->BitmaskAry[i] = BinToHex(str_bit_mask);
    }

    int bit_amount = 32;
    /* find appropriate bit shift amount based on command line args */
    for(int i = 0; i < level_count; i++){
        bit_amount = bit_amount - level_bits[i];
        this->ShiftAry[i] = bit_amount;
    }

    /* find and store 2^n page sizes based on command line args */
    for(int i = 0; i < level_count; i++){
        this->EntryCount[i] = pow(2, level_bits[i]);
    }
}

/* page traversal */
Map *PageLookUp(Level *curr_level, unsigned int LogicalAddress){
    /* we find page number via logical address */
    unsigned int shift_amount =
        curr_level->PageTablePtr->ShiftAry[curr_level->CurrentDepth];
    unsigned int bit_mask = HexToDec
        (curr_level->PageTablePtr->BitmaskAry[curr_level->CurrentDepth]);
    unsigned int page_num =
        LogicalToPage(LogicalAddress, bit_mask, shift_amount);

    /* if we are not at leaf node */
    if(curr_level->CurrentDepth + 1 < curr_level->PageTablePtr->LevelCount){
        /* if page number index is null we return null */
        if(curr_level->NextLevelPtr[page_num].NextLevel == nullptr){
            return NULL;
        }
        /* if not null, we continue traversal */
        else{
            Level *nxt = curr_level->NextLevelPtr[page_num].NextLevel;
            return PageLookUp(nxt, LogicalAddress);
        }
    }
    /* if we are at leaf node */
    else{
        /* if validity bit is false, return null */
        if(curr_level->MapPtr[page_num].FrameIndex.first == false){
            return NULL;
        }
        /* if bit is true, return map object with indexed info */
        else{
            bool bit = curr_level->MapPtr[page_num].FrameIndex.first;
            int frame = curr_level->MapPtr[page_num].FrameIndex.second;
            Map *mp = new Map(bit, frame);

            return mp;
        }
    }
    return NULL;
}

/* beginning of page traversal function */
Map *PageLookUp(Pagetable *ptable, unsigned int LogicalAddress){
    return PageLookUp(ptable->RootNodePtr, LogicalAddress);
}

/* insert additional levels and ultimately the frame in map */
bool PageInsert(Level *curr_level, unsigned int LogicalAddress,
                unsigned int Frame){
    /* find current array index by finding the page number */
    unsigned int shift_amount =
        curr_level->PageTablePtr->ShiftAry[curr_level->CurrentDepth];
    unsigned int bit_mask = HexToDec
        (curr_level->PageTablePtr->BitmaskAry[curr_level->CurrentDepth]);
    unsigned int page_num =
        LogicalToPage(LogicalAddress, bit_mask, shift_amount);

    /* is leaf node so we process map structure here */
    if(curr_level->PageTablePtr->LevelCount == curr_level->CurrentDepth+1){
        /* if no frame stored, we add pair of values */
        /* pair of values include bool valid bit and int frame */
        if(curr_level->MapPtr[page_num].FrameIndex.first == false){
            curr_level->MapPtr[page_num].FrameIndex.first = true;
            curr_level->MapPtr[page_num].FrameIndex.second = Frame;
            return true;
        }
        /* if there is a frame already stored, we return false */
        else{
            return false;
        }
    }
    /* is not leaf node so we continue to create/move to next level */
    else{
        /* checks if next level is empty, if so, create next level and link */
        if(curr_level->NextLevelPtr[page_num].NextLevel == nullptr){
            unsigned int nextLevelDepth = curr_level->CurrentDepth + 1;
            unsigned int nextLevelSize = curr_level->
                PageTablePtr->EntryCount[curr_level->CurrentDepth + 1];
            Level *new_level = new Level
                (curr_level->PageTablePtr, nextLevelDepth, nextLevelSize);
            curr_level->NextLevelPtr[page_num].NextLevel = new_level;
            return PageInsert(new_level, LogicalAddress, Frame);
        }
        /* if next level is already there, just go there */
        else{
            Level *next = curr_level->NextLevelPtr[page_num].NextLevel;
            return PageInsert(next, LogicalAddress, Frame);
        }

    }
}

/* beginning of page insert function */
bool PageInsert(Pagetable *ptable, unsigned int LogicalAddress, unsigned int Frame){
    return PageInsert(ptable->RootNodePtr, LogicalAddress, Frame);
}
