#include <utility>
#ifndef MAP_HEADER
#define MAP_HEADER

class Map{
    public:
        /* constructors */
        Map();
        Map(bool bit, int frame);
        bool bit;
        int frame;
        /* pair index that stores validity bit and frame number */
        std::pair <bool, int> FrameIndex;
};

#endif
