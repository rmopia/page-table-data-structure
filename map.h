#include <utility>
#ifndef MAP_HEADER
#define MAP_HEADER

class Map{
    public:
        /* constructors */
        Map();
        Map(bool bit, int frame);
        /* validity bit: checks whether map entry is empty or not */
        bool bit;
        /* frame value used during logical to physical addr translation */
        int frame;
        /* pair index that stores validity bit and frame number */
        std::pair <bool, int> FrameIndex;
        /* specific index the page ends up at */
        unsigned int page_index;
};

#endif
