#include <iostream>
#include <math.h>
#include <string>
#include <iomanip>
#include "structures.h"
using namespace std;

int frame = 0; // used when in map

string BinToHex(string bin_num){
    string rest("0x"),tmp,chr = "0000";
	int len = bin_num.length()/4;
	chr = chr.substr(0,len);
	bin_num = chr+bin_num;
	for (int i=0;i<bin_num.length();i+=4)
	{
		tmp = bin_num.substr(i,4);
		if (!tmp.compare("0000")){
			rest = rest + "0";
		}
		else if (!tmp.compare("0001")){
			rest = rest + "1";
		}
		else if (!tmp.compare("0010")){
			rest = rest + "2";
		}
		else if (!tmp.compare("0011")){
			rest = rest + "3";
		}
		else if (!tmp.compare("0100")){
			rest = rest + "4";
		}
		else if (!tmp.compare("0101")){
			rest = rest + "5";
		}
		else if (!tmp.compare("0110")){
			rest = rest + "6";
		}
		else if (!tmp.compare("0111")){
			rest = rest + "7";
		}
		else if (!tmp.compare("1000")){
			rest = rest + "8";
		}
		else if (!tmp.compare("1001")){
			rest = rest + "9";
		}
		else if (!tmp.compare("1010")){
			rest = rest + "A";
		}
		else if (!tmp.compare("1011")){
			rest = rest + "B";
		}
		else if (!tmp.compare("1100")){
			rest = rest + "C";
		}
		else if (!tmp.compare("1101")){
			rest = rest + "D";
		}
		else if (!tmp.compare("1110")){
			rest = rest + "E";
		}
		else if (!tmp.compare("1111")){
			rest = rest + "F";
		}
		else{
			continue;
		}
	}
	return rest;
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
        //cout << "bit start is: " << bit_start << endl;
        //cout << "bit limit is: " << bit_limit << endl;
        for(int i = 0; i < 32; i++){
            if(i >= bit_start && i < bit_limit){
                str_bit_mask.append("1");
            }
            else{
                str_bit_mask.append("0");
            }
        }
        bit_start = bit_limit;
        //cout << str_bit_mask << endl;
        //cout << BinToHex(str_bit_mask) << endl;
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

Level::Level(Pagetable *ptable, int current_depth, int array_size){
    this->PageTablePtr = ptable;
    this->CurrentDepth = current_depth;
    //this->nextLevel[array_size];
    // switch between nextLevel or map array if leaf node

    if(array_size != sizeof(nextLevel)/sizeof(*nextLevel)){
        cout << "do something" << endl; // resize
    }

    if(this->PageTablePtr->LevelCount != (current_depth + 1)){
        for(int i = 0; i < array_size; i++){
            this->nextLevel[i] = nullptr;
        }
    }
    else{
        cout << "create map and point to map" << endl;
    }
}

Map::Map(int entry_amount){
    // loop stops at 2^n amount
    for(int i = 0; i < entry_amount; i++){

    }
}

unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift){
    cout << LogicalAddress << endl;
    cout << Mask << endl;
    int bitwise_and_res;
    bitwise_and_res = LogicalAddress & Mask;
    cout << bitwise_and_res << endl;
    int bitwise_shift_res = bitwise_and_res >> Shift;
    return bitwise_shift_res;
}

Map *PageLookUp(Pagetable *ptable, unsigned int LogicalAddress){

}

void PageInsert(Pagetable *ptable, unsigned int LogicalAddress, unsigned int Frame){

}

