/* Name: Robert Mopia
RedID: 817646343 */
#define BADFLAG 1
#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include "pagetable.h"
#include "level.h"
#include "map.h"
#include "byu_tracereader.c"

using namespace std;

int frame = 0; /* global var stored in map, used for translation */

int main(int argc, char **argv){
    /* command line vars used after page initialization & insertion*/
    int Option, addr_num = 0;
    bool p_bool = 0, t_bool = 0;
    char *res_filename;

    while ( (Option = getopt(argc, argv, "n:p:t")) != -1) {
        switch (Option) {
        case 'n': /* Number of addresses to process */
            addr_num = atoi(optarg);
        break;
        case 'p': /* produce map of pages */
            res_filename = optarg;
            p_bool = 1;
        break;
        case 't': /* Show address translation */
            t_bool = 1;
        break;
        default:
            /* invalid arguments throw user out */
            cout << "invalid argument found." << endl;
            exit(BADFLAG);
        }
    }

    /* first mandatory argument, optind is defined by getopt */
    int opt_idx = optind;
    int index = 0;
    char *filename;
    /* vars used to store misc. command line arguments */
    int levels = 0;
    int level_bits[5];

    /* run while loop to obtain remaining arguments */
    while(argv[opt_idx] != nullptr){
        if(isdigit(*argv[opt_idx])){
            level_bits[index] = atoi(argv[opt_idx]);
            index++;
            levels++;
        }
        else{
            filename = argv[opt_idx];
        }
        opt_idx++;
    }

    /* pagetable used by levels as a center of information */
    /* pagetable initialized with root node level */
    Pagetable *pt = new Pagetable(levels, level_bits);
    Level *level_zero = new Level(pt, 0, pt->EntryCount[0]);
    /* link pagetable to level with pointer */
    pt->RootNodePtr = level_zero;

    /* set file ptr and address array ptr for trace file */
    FILE *trfile;
    trfile = fopen(filename, "r");
    p2AddrTr *addrPtr = (p2AddrTr*)malloc(sizeof(p2AddrTr));

    /* reads values in trace file that are inserted into page table */
    while(NextAddress(trfile, addrPtr) != 0){
        //cout << PageInsert(pt, addrPtr->addr, frame) << endl;
        if(PageInsert(pt, addrPtr->addr, frame) != 0){
            frame++;
        }
    }
    fclose(trfile);

    /* logical to physical address translation flag */
    /* (frame)*(page size) + offset */
    if(t_bool == 1){
        FILE *pfile;
        pfile = fopen(filename, "r");
        p2AddrTr *aPtr = (p2AddrTr*)malloc(sizeof(p2AddrTr));
        while(NextAddress(pfile, aPtr)){
            Map *mpPtr = new Map();
            mpPtr = PageLookUp(pt, aPtr->addr);
            int fr = mpPtr->frame;
        }
    }
    if(p_bool == 1){
        // do something w/ res file
    }

    return 0;
}
