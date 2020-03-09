#include "map.h"

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
