#include <iostream>
#include "structures.h"
using namespace std;

Pagetable::Pagetable(int level_count){
    this->LevelCount = level_count;
    this->BitmaskAry = new int[3];
}

Level::Level(Pagetable *pt, int current_depth, int array_size){
    this->PageTablePtr = pt;
    this->CurrentDepth = current_depth;
    this->nextLevel[array_size];
    for(int i = 0; i < array_size; i++){
        cout << i << endl;
    }
}

Map::Map(int entry_amount){
    // loop stops at 2^n amount
}

unsigned int LogicalToPage(unsigned int LogicalAddress, unsigned int Mask, unsigned int Shift){

}
