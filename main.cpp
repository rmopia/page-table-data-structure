/* Name: Robert Mopia
RedID: 817646343 */
#define BADFLAG 1
#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <string>
#include <sstream>
#include <cmath>
#include "pagetable.h"
#include "level.h"
#include "map.h"
#include "byu_tracereader.c"

using namespace std;

int frame = 0; /* global var stored in map, used for translation */

int main(int argc, char **argv){
    /* command line vars used after page initialization & insertion*/
    int Option, addr_limit = 0;
    bool p_bool = 0, t_bool = 0;
    char *res_filename;

    while ( (Option = getopt(argc, argv, "n:p:t")) != -1) {
        switch (Option) {
        case 'n': /* Number of addresses to process */
            addr_limit = atoi(optarg);
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
    int levels = 0, page_num_bits = 0;
    int level_bits[5];

    /* run while loop to obtain remaining arguments */
    while(argv[opt_idx] != nullptr){
        if(isdigit(*argv[opt_idx])){
            page_num_bits += atoi(argv[opt_idx]);
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
        if(PageInsert(pt, addrPtr->addr, frame) != 0){
            frame++;
        }
    }
    fclose(trfile);

    /* number of bits not used by page number declaration */
    int offset_bits = 32 - page_num_bits;
    /* page size determined by offset bits */
    int page_size = pow(2, offset_bits);
    /* bitmask used to find offset */
    int offset_bitmask = page_size - 1;

    /* logical to physical address translation flag */
    if(t_bool == 1){
        FILE *filePtr;
        /* reopens and rereads original file */
        filePtr = fopen(filename, "r");
        p2AddrTr *aPtr = (p2AddrTr*)malloc(sizeof(p2AddrTr));
        while(NextAddress(filePtr, aPtr)){
            /* processes virtual address */
            unsigned int addr = aPtr->addr;
            /* use map object to find acquainted frame */
            Map *mpPtr = new Map();
            mpPtr = PageLookUp(pt, addr);
            int fr = mpPtr->frame;
            /* convert address to hex for readability */
            string hex_addr = DecToHex(addr);
            cout << hex_addr << " -> ";
            /* offset obtained from logical addr and num of bits of offset */
            int offset = addr & offset_bitmask;
            /* virtual addr converted to physical addr */
            int result = (fr)*(page_size) + offset;
            cout << DecToHex(result) << endl;
        }
        fclose(filePtr);
    }
    /* write to specified file with ordered hex addresses and their frames */
    if(p_bool == 1){
        // do something w/ res file
    }

    return 0;
}
