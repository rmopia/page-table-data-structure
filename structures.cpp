/* Name: Robert Mopia
RedID: 817646343 */
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include "pagetable.h"
#include "level.h"
#include "map.h"
using namespace std;

/* convert int dec to hex string */
string DecToHex(unsigned int dec_num){
    string hex_str = "0x";
    string res;
    stringstream ss;
    ss << hex << dec_num;
    res = ss.str();
    hex_str.append(res);
    return hex_str;
}

/* convert hex string to dec */
unsigned int HexToDec(string hex_num){
    unsigned int dec_num;
    stringstream ss;
    ss << hex << hex_num;
    ss >> dec_num;
    return dec_num;
}

/* converts binary string to hex string */
string BinToHex(string bin_num){
    string hex_str = "0x";
    string byte;
    /* reads every 4 bits of binary and converts it to hex */
	for (int i = 0; i < bin_num.length(); i += 4){
		byte = bin_num.substr(i, 4);
		if (byte.compare("0000")==0){ hex_str += "0"; }
		else if (byte.compare("0001")==0){ hex_str += "1"; }
		else if (byte.compare("0010")==0){ hex_str += "2"; }
		else if (byte.compare("0011")==0){ hex_str += "3"; }
		else if (byte.compare("0100")==0){ hex_str += "4"; }
		else if (byte.compare("0101")==0){ hex_str += "5"; }
		else if (byte.compare("0110")==0){ hex_str += "6"; }
		else if (byte.compare("0111")==0){ hex_str += "7"; }
		else if (byte.compare("1000")==0){ hex_str += "8"; }
		else if (byte.compare("1001")==0){ hex_str += "9"; }
		else if (byte.compare("1010")==0){ hex_str += "A"; }
		else if (byte.compare("1011")==0){ hex_str += "B"; }
		else if (byte.compare("1100")==0){ hex_str += "C"; }
		else if (byte.compare("1101")==0){ hex_str += "D"; }
		else if (byte.compare("1110")==0){ hex_str += "E"; }
		else if (byte.compare("1111")==0){ hex_str += "F"; }
	}
	return hex_str;
}

/* Pagetable constructor */
Pagetable::Pagetable(int level_count, int level_bits[]){
    this->LevelCount = level_count;
    /* initialize arrays dynamically of size level_count */
    this->BitmaskAry = new string[level_count];
    this->ShiftAry = new int[level_count];
    this->EntryCount = new int[level_count];

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

    /* finds 2^n page sizes based on command line args */
    for(int i = 0; i < level_count; i++){
        this->EntryCount[i] = pow(2, level_bits[i]);
    }
}

/* Map object constructor with no arguments */
Map::Map(){}

/* Map object constructor with arguments */
Map::Map(bool bit, int fr){
    this->bit = bit;
    this->frame = fr;
    /* frame index pair */
    this->FrameIndex.first = bit;
    this->FrameIndex.second = fr;
}

/* Level object constructor with no arguments */
Level::Level(){}

/* Level object constructor with arguments */
Level::Level(Pagetable *ptable, int current_depth, int array_size){
    /* link level back to pagetable to find core info */
    this->PageTablePtr = ptable;
    this->CurrentDepth = current_depth;

    /* creates 2^n next level array of nullptrs if not a leaf node */
    if((current_depth + 1) < this->PageTablePtr->LevelCount){
        NextLevelPtr = new Level[array_size];
        for(int i = 0; i < array_size; i++){
            NextLevelPtr[i].NextLevel = nullptr;
        }
    }
    /* creates 2^n map array of empty pairs if leaf node */
    else{
        MapPtr = new Map[array_size];
        for(int i = 0; i < array_size; i++){
            MapPtr[i].FrameIndex = make_pair(0,0);
        }
    }
}

/* find page number function */
unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift){
    unsigned int bitwise_and_res;
    /* bitwise and to remove unnecessary bits */
    bitwise_and_res = LogicalAddress & Mask;
    /* bitwise shift to remove trailing zeros */
    unsigned int bitwise_shift_res = bitwise_and_res >> Shift;
    return bitwise_shift_res;
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
    unsigned int bit_mask =
        HexToDec(curr_level->PageTablePtr->BitmaskAry[curr_level->CurrentDepth]);
    unsigned int page_num =
        LogicalToPage(LogicalAddress, bit_mask, shift_amount);

    if(curr_level->PageTablePtr->LevelCount == curr_level->CurrentDepth+1){
        /* is leaf node so we process map structure here */
        /* if no frame stored, we add pair of values */
        if(curr_level->MapPtr[page_num].FrameIndex.first == false){
            curr_level->MapPtr[page_num].FrameIndex.first = true;
            curr_level->MapPtr[page_num].FrameIndex.second = Frame;
            return true;
        }
        /* if there is a frame already stored, we return false */
        else{
            //cout << "already has frame: " << curr_level->MapPtr[page_num].FrameIndex.second << endl;
            return false;
        }
    }
    else{
        /* is not leaf node so we continue to create/move to next level */
        /* checks if next level is empty, if so, create next level and link */
        if(curr_level->NextLevelPtr[page_num].NextLevel == nullptr){
            unsigned int nextLevelDepth = curr_level->CurrentDepth + 1;
            unsigned int nextLevelSize =
                curr_level->PageTablePtr->EntryCount[curr_level->CurrentDepth+1];
            Level *new_level =
                new Level(curr_level->PageTablePtr, nextLevelDepth, nextLevelSize);
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
