#include <utility>
#include "level.h"
#include "map.h"

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
            MapPtr[i].FrameIndex = std::make_pair(0,0);
        }
    }
}
