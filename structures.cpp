#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include "structures.h"
using namespace std;

unsigned int HexToDec(string hex_num){
    unsigned int dec_num;
    stringstream ss;
    ss << hex << hex_num;
    ss >> dec_num;
    return dec_num;
}

string BinToHex(string bin_num){
    string hex_str = "0x";
    string byte;
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

Pagetable::Pagetable(int level_count, int level_bits[]){
    this->LevelCount = level_count;
    this->BitmaskAry = new string[level_count];
    this->ShiftAry = new int[level_count];
    this->EntryCount = new int[level_count];

    int bit_start = 0;
    for(int i = 0; i < level_count; i++){
        int bit_limit = bit_start + level_bits[i];
        string str_bit_mask;
        for(int i = 0; i < 32; i++){
            if(i >= bit_start && i < bit_limit){
                str_bit_mask.append("1");
            }
            else{
                str_bit_mask.append("0");
            }
        }
        bit_start = bit_limit;
        this->BitmaskAry[i] = BinToHex(str_bit_mask);
        cout << this->BitmaskAry[i] << endl;
    }

    int bit_amount = 32;
    for(int i = 0; i < level_count; i++){
        bit_amount = bit_amount - level_bits[i];
        this->ShiftAry[i] = bit_amount;
        cout << ShiftAry[i] << endl;
    }

    for(int i = 0; i < level_count; i++){
        this->EntryCount[i] = pow(2, level_bits[i]);
        cout << EntryCount[i] << endl;
    }
}

Map::Map(){

}

Level::Level(){

}

Level::Level(Pagetable *ptable, int current_depth, int array_size){
    this->PageTablePtr = ptable;
    this->CurrentDepth = current_depth;

    if(this->PageTablePtr->LevelCount != (current_depth + 1)){
        NextLevelPtr = new Level[array_size];
        for(int i = 0; i < array_size; i++){
            NextLevelPtr[i].NextLevel = nullptr;
        }
        cout << "level with next level array created" << endl;
    }
    else{
        MapPtr = new Map[array_size];
        for(int i = 0; i < array_size; i++){
            MapPtr[i].FrameIndex = make_pair(0,0);
        }
        /*
        for(int i = 0; i < array_size; i++){
            cout << MapPtr[i].FrameIndex.first;
            cout << " " << MapPtr[i].FrameIndex.second << endl;
        }*/
        cout << "level w/ map created" << endl;
    }
}

unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift){
    unsigned int bitwise_and_res;
    bitwise_and_res = LogicalAddress & Mask;
    unsigned int bitwise_shift_res = bitwise_and_res >> Shift;
    return bitwise_shift_res;
}

Map *PageLookUp(Pagetable *ptable, unsigned int LogicalAddress){

    return NULL;
}

void PageInsert(Level *curr_level, unsigned int LogicalAddress, unsigned int Frame){
    // find current array index
    unsigned int shift_amount = curr_level->PageTablePtr->ShiftAry[curr_level->CurrentDepth];
    unsigned int bit_mask = HexToDec(curr_level->PageTablePtr->BitmaskAry[curr_level->CurrentDepth]);

    unsigned int page_num = LogicalToPage(LogicalAddress, bit_mask, shift_amount);

    cout << "current index: " << page_num << endl;

    //cout << "over here: " << curr_level->PageTablePtr->LevelCount << endl;
    //cout << LogicalAddress << " and " << Frame << endl;

    if(curr_level->PageTablePtr->LevelCount == curr_level->CurrentDepth+1){
        // is leaf node so we process map structure here
        cout << "process map here" << endl;
        if(curr_level->MapPtr[page_num].FrameIndex.first == false){
            curr_level->MapPtr[page_num].FrameIndex.first = true;
            curr_level->MapPtr[page_num].FrameIndex.second = Frame;

            /*for(int i = 0; i < 256; i++){
                cout << curr_level->MapPtr[i].FrameIndex.first <<
                " " << curr_level->MapPtr[i].FrameIndex.second << endl;
            }*/

        }
        else{
            // do nothing?
        }
    }
    else{ // create new page and initialize it

        unsigned int nextLevelDepth = curr_level->CurrentDepth + 1;
        unsigned int nextLevelSize = curr_level->PageTablePtr->EntryCount[curr_level->CurrentDepth+1];
        Level *new_level = new Level(curr_level->PageTablePtr, nextLevelDepth, nextLevelSize);
        curr_level->NextLevelPtr[page_num].NextLevel = new_level;
        PageInsert(new_level, LogicalAddress, Frame);

        /*
        for(int i = 0; i < curr_level->PageTablePtr->EntryCount[curr_level->CurrentDepth]; i++){
            cout << curr_level->NextLevelPtr[i].NextLevel << endl;
        }*/

    }
}

void PageInsert(Pagetable *ptable, unsigned int LogicalAddress, unsigned int Frame){
    PageInsert(ptable->RootNodePtr, LogicalAddress, Frame);
}
